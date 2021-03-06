/* Program to do "calculations" on numeric CSV data files. */

/* Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   September 2020, with the intention that it be modified by students
   to add functionality, as required by the assignment specification. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/* These #defines provided as part of the initial skeleton */

#define MAXCOLS	20	/* maximum number of columns to be handled */
#define MAXROWS	999	/* maximum number of rows to be handled */
#define LABLEN  20	/* maximum length of each column header */
#define LINELEN 100	/* maximum length of command lines */

#define ERROR	(-1)	/* error return value from some functions */

#define O_NOC	'-'	/* the "do nothing" command */
#define O_IND	'i'	/* the "index" command */
#define O_ANA 	'a'	/* the "analyze" command */
#define O_DPY 	'd'	/* the "display" command */
#define O_PLT 	'p'	/* the "plot" command */
#define O_SRT 	's'	/* the "sort" command */

#define CH_COMMA ','	/* comma character */
#define CH_CR    '\r'	/* CR character in DOS-format files */
#define CH_NL    '\n'	/* newline character */

/* Further #defines */

#define DISPLAYLEN 8         /* the length of a column header in the display 
                                command */
#define BOUNDARY pow(10, -6) /* numbers falling within less than this value of 
                                 each other are regarded as being equal */
#define GRAPH_BANDS 10       /* number of equal length bands required for the
                                frequency histogram */
#define MAX_BAR_LEN 60       /* maximum length of a bar in the frequency
                                histogram */


/* These types are provided as part of the skeelton  */

typedef char head_t[LABLEN+1];
typedef double csv_t[MAXROWS][MAXCOLS];

/****************************************************************/

/* function prototypes (Author: Alistair Moffat) */

void get_csv_data(csv_t D, head_t H[],  int *dr,  int *dc, int argc,
	char *argv[]);
void error_and_exit(char *msg);
void print_prompt(void);
int  get_command(int dc, int *command, int ccols[], int *nccols);
void handle_command(int command, int ccols[], int nccols,
			csv_t D, head_t H[], int dr, int dc);
void do_index(csv_t D, head_t H[], int dr, int dc, int ccols[], int nccols);

/* Further function prototypes */

void do_analyze(csv_t D, head_t H[], int dr, int dc, int ccols[], int nccols);
void do_display(csv_t D, head_t H[], int dr, int dc, int ccols[], int nccols);
void do_sort(csv_t D, head_t H[], int dr, int dc, int ccols[], int nccols);
void do_plot(csv_t D, head_t H[], int dr, int dc, int ccols[], int nccols);
void val_swap(double *p1, double *p2);
double find_col_max(csv_t D, int dr, int cur_col);
double find_col_min(csv_t D, int dr, int cur_col);
int find_freq(csv_t D, int dr, int cur_col, double lbound, double ubound);
int find_scale(csv_t D, int dr, int ccols[], int ncols, double band_width,
		double graph_min);


/****************************************************************
*****************************************************************

The following section is the skeleton program.
Author: Alistair Moffat

*****************************************************************
****************************************************************/

/* main program controls all the action
*/
int
main(int argc, char *argv[]) {

	head_t H[MAXCOLS];	/* labels from the first row in csv file */
	csv_t D;		/* the csv data stored in a 2d matrix */
	int dr=0, dc=0;		/* number of rows and columns in csv file */
	int ccols[MAXCOLS];
	int nccols;
	int command;

	/* this next is a bit of magic code that you can ignore for
	   now, it reads csv data from a file named on the
	   commandline and saves it to D, H, dr, and dc
	   */
	get_csv_data(D, H, &dr, &dc, argc, argv);
	
	/* ok, all the input data has been read, safe now to start
	   processing commands against it */

	print_prompt();
	while (get_command(dc, &command, ccols, &nccols) != EOF) {
		handle_command(command, ccols, nccols,
			D, H, dr, dc);
		print_prompt();
	}

	/* all done, so pack up bat and ball and head home */
	printf("\nTa daa!!!\n");
	return 0;
}

/****************************************************************/

/* prints the prompt indicating ready for input
*/
void
print_prompt(void) {
	printf("> ");
}

/****************************************************************/

/* read a line of input into the array passed as argument
   returns false if there is no input available
   all whitespace characters are removed
   all arguments are checked for validity
   if no argumnets, the numbers 0..dc-1 are put into the array
*/
int
get_command(int dc, int *command, int columns[], int *nccols) {
	int i=0, c, col=0;
	char line[LINELEN];
	/* comand is in first character position */
	if ((*command=getchar()) == EOF) {
		return EOF;
	}
	/* and now collect the rest of the line, integer by integer */
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (isdigit(c)) {
			/* digit contributes to a number */
			line[i++] = c;
		} else if (i!=0)  {
			/* reached end of a number */
			line[i] = '\0';
			columns[col++] = atoi(line);
			/* reset, to collect next number */
			i = 0;
		} else {
			/* just discard it */
		}
	}
	if (i>0) {
		/* reached end of the final number in input line */
		line[i] = '\0';
		columns[col++] = atoi(line);
	}

	if (col==0) {
		/* no column numbers were provided, so generate them */
		for (i=0; i<dc; i++) {
			columns[i] = i;
		}
		*nccols = dc;
		return !EOF;
	}

	/* otherwise, check the ones that were typed against dc,
	   the number of cols in the CSV data that was read */
	for (i=0; i<col; i++) {
		if (columns[i]<0 || columns[i]>=dc) {
			printf("%d is not between 0 and %d\n",
				columns[i], dc);
			/* and change to "do nothing" command */
			*command = O_NOC;
		}
	}
	/* all good */
	*nccols = col;
	return !EOF;
}

/****************************************************************/

/* this next is a bit of code reads the
   input csv data from a file named on the commandline and saves
   it into an array of character strings (first line), and into a
   matrix of doubles (all other lines)
*/

void
get_csv_data(csv_t D, head_t H[],  int *dr,  int *dc, int argc,
		char *argv[]) {
	FILE *fp;
	int rows=0, cols=0, c, len;
	double num;

	if (argc<2) {
		/* no filename specified */
		error_and_exit("no CSV file named on commandline");
	}
	if (argc>2) {
		/* confusion on command line */
		error_and_exit("too many arguments supplied");
	}
	if ((fp=fopen(argv[1], "r")) == NULL) {
		error_and_exit("cannot open CSV file");
	}

	/* ok, file exists and can be read, next up, first input
	   line will be all the headings, need to read them as
	   characters and build up the corresponding strings */
	len = 0;
	while ((c=fgetc(fp))!=EOF && (c!=CH_CR) && (c!=CH_NL)) {
		/* process one input character at a time */
		if (c==CH_COMMA) {
			/* previous heading is ended, close it off */
			H[cols][len] = '\0';
			/* and start a new heading */
			cols += 1;
			len = 0;
		} else {
			/* store the character */
			if (len==LABLEN) {
				error_and_exit("a csv heading is too long");
			}
			H[cols][len] = c;
			len++;
		}
	}
	/* Close off the last string */
	H[cols][len] = '\0';
	*dc = cols+1;

	/* now to read all of the numbers in, assumption is that the input
	   data is properly formatted and error-free, and that every row
	   of data has a numeric value provided for every column */
	rows = cols = 0;
	while (fscanf(fp, "%lf", &num) == 1) {
		/* read a number, put it into the matrix */
		if (cols==*dc) {
			/* but first need to start a new row */
			cols = 0;
			rows += 1;
		}
		/* now ok to do the actual assignment... */
		D[rows][cols] = num;
		cols++;
		/* and consume the comma (or newline) that comes straight
		   after the number that was just read */
		fgetc(fp);
	}
	/* should be at last column of a row */
	if (cols != *dc) {
		error_and_exit("missing values in input");
	}
	/* Just a bit of tidying up required now  */
	*dr = rows+1;
	fclose(fp);
	printf("    csv data loaded from %s", argv[1]);
	printf(" (%d rows by %d cols)\n", *dr, *dc);
	return;
}
 
/****************************************************************/

void
error_and_exit(char *msg) {
	printf("Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

/****************************************************************/

/* the 'i' index command
*/
void
do_index(csv_t D, head_t H[], int dr, int dc,
		int ccols[], int nccols) {
	int i, c;
	printf("\n");
	for (i=0; i<nccols; i++) {
		c = ccols[i];
		printf("    column %2d: %s\n", c, H[c]);
	}
}


/*****************************************************************
******************************************************************

This concludes the skeleton program. The following code has been
written by Abrar Yusuf.

******************************************************************
*****************************************************************/

/* this function examines each incoming command and decides what
   to do with it, kind of traffic control, deciding what gets
   called for each command, and which of the arguments it gets
*/
void
handle_command(int command, int ccols[], int nccols,
		csv_t D, head_t H[], int dr, int dc) { 
	if (command==O_NOC) {
		/* the null command, just do nothing */
	} else if (command==O_IND) {
		do_index(D, H, dr, dc, ccols, nccols);
	} else if (command==O_ANA) {
		do_analyze(D, H, dr, dc, ccols, nccols);
	} else if (command==O_DPY) {
		do_display(D, H, dr, dc, ccols, nccols);
	} else if (command==O_SRT){
		do_sort(D, H, dr, dc, ccols, nccols);	
	} else if (command==O_PLT){
		do_plot(D, H, dr, dc, ccols, nccols);
	/* and now a last option for things that aren't known */
	} else {
		printf("command '%c' is not recognized"
			" or not implemented yet\n", command);
	}
	return;
}
 
/* the 'a' analyze command
*/
void 
do_analyze(csv_t D, head_t H[], int dr, int dc, 
		int ccols[], int ncols){
	int i, j, sorted, cur_col;
	double total, max, min, avg, median;
	/* analyze each specified column */
	for (i=0; i<ncols; i++){
		sorted = 1;
		cur_col = ccols[i];
		max = find_col_max(D, dr, cur_col);
		min = find_col_min(D, dr, cur_col);
		total = D[0][cur_col];
		/* calculate the mean value of the column */
		for (j=1; j<dr; j++){
			/* check if column values are sorted */
			if (D[j][cur_col] < D[j-1][cur_col] ){
				sorted = 0;
			}
			total += D[j][cur_col];
		}
		avg = total / dr; 
		/* print the appropriate heading for each set of statistics */
		printf("%c%17s", CH_NL, H[cur_col]);
		if (sorted == 1){
			printf(" (sorted)");
		}
		/* print the maximum, minimum and mean for each column */
		printf("%c    max = %7.1f%c", CH_NL, max, CH_NL);
		printf("    min = %7.1f%c", min, CH_NL);
		printf("    avg = %7.1f%c", avg, CH_NL);
		/* calculate and print the median value of the column
		   if the values are sorted */
		if (sorted == 1){
			if ((dr + 1) % 2 == 0){
				median = D[((dr + 1) / 2) - 1][cur_col];
			} else {
				median = (D[(dr / 2) - 1][cur_col] + D[dr / 2][cur_col]) / 2;
			}
			printf("    med = %7.1f%c", median, CH_NL);
		}
	}
	return;
}

/* the 'd' display command
*/
void
do_display(csv_t D, head_t H[], int dr, int dc,
	    int ccols[], int ncols){
	int i, j, repeats=1, next_same, cur_col, spacing;
	double cur_val, next_val;
	/* print out the row headers */
	spacing = DISPLAYLEN * ncols;
	for (j=ncols-1; j>=0; j--){
		cur_col = ccols[j];
		printf("%c%*s", CH_NL, spacing, H[cur_col]);
		spacing -= DISPLAYLEN;
	}
	printf("%c", CH_NL);
    /* check each row and account for any consecutive duplicates */
	for (i=0; i<dr; i++){
		next_same = 1;
		/* check if the next row is exactly the same */
		for (j=0; j<ncols; j++){
			cur_col = ccols[j];
			cur_val = D[i][cur_col];
			next_val = D[i+1][cur_col];
			if ((i == dr - 1) || (cur_val <= next_val - BOUNDARY) ||
				 (cur_val >= next_val + BOUNDARY)){
				next_same = 0;
			}
		}	
		/* print out the given row if it is not a duplicate */
		if (next_same == 0){
			for (j=0; j<ncols; j++){
				cur_col = ccols[j];
				printf(" %7.1f", D[i][cur_col]);
			}
			if (repeats == 1){
				printf("    ( 1 instance)%c", CH_NL);
			} else {
				printf("    (%2d instances)%c", repeats, CH_NL);
				repeats = 1;
			}
		} else {
			repeats += 1;
		}				
	}
	return;
}

/* the 's' sort command
*/
void 
do_sort(csv_t D, head_t H[], int dr, int dc, 
		int ccols[], int ncols){
	int i, j, k, a, col_key;
	/* print out the sorting keys */
	printf("%c    sorted by:", CH_NL);
	for (k=0; k<ncols-1; k++){
		col_key = ccols[k];
		printf(" %s,", H[col_key]);
	}
	printf(" %s%c", H[ccols[ncols-1]], CH_NL);
	/* use insertion sort to sort and swap the table rows */
	/***********************************************************
	The following insertion sort code has been adapted from:
	
	Source: Programming, Problem Solving and Abstraction with C
	        (Textbook (p. 104))
	Author: Alistair Moffat
	Date: 19 December 2012
	***********************************************************/
	for (i=1; i<dr; i++){
		for (j=i-1; j>=0; j--){
			/* compare the primary key values of the two rows and if they
			   are equal, check the values of the two rows corresponding
			   to the next provided key */
			for (k=0; k<ncols; k++){
				col_key = ccols[k];
				if (D[j+1][col_key] < D[j][col_key]){
					for (a=0; a<dc; a++){
						val_swap(&D[j][a], &D[j+1][a]);
					}
					break;
				} else if (D[j+1][col_key] > D[j][col_key]){
					break;
				}
			}		
		}
	}
	return;
}

/* the 'p' plot command
*/
void
do_plot(csv_t D, head_t H[], int dr, int dc, 
		int ccols[], int ncols){
	int i, j, k, cur_col, all_same=1, freq, scale, bars;
	double col_max, col_min, graph_max, graph_min, check_val,
		cur_band, next_band, band_width, range;
	/* find the maximum and minimum values across all specified columns */
	graph_max = D[0][ccols[0]];
	graph_min = D[0][ccols[0]];
	for (i=0; i<ncols; i++){
	    cur_col = ccols[i];	
	    col_max = find_col_max(D, dr, cur_col);
	    if (col_max > graph_max){
	    	graph_max = col_max;
	    }
	    col_min = find_col_min(D, dr, cur_col);
	    if (col_min < graph_min){
	    	graph_min = col_min;
	    }	
	}
	/* check if all selected elements are the same */
	check_val = D[0][ccols[0]];
	for (i=0; i<ncols; i++){
		cur_col = ccols[i];
		for (j=0; j<dr; j++){
			if (D[j][cur_col] != check_val){
				all_same = 0;
				break;
			}	
		}	
	}
	if (all_same == 1){
		printf("%call selected elements are %.1f%c", CH_NL, check_val, CH_NL);
		return;
	}
	/* compute bands and the scaling factor of the graph */
	range = (graph_max + BOUNDARY) - (graph_min - BOUNDARY);
	band_width = range / GRAPH_BANDS;
	scale = find_scale(D, dr, ccols, ncols, band_width, graph_min);
	/* print out the frequency histogram */
	printf("%c    %7.1f +", CH_NL, graph_min - BOUNDARY);
	cur_band = graph_min - BOUNDARY;
	next_band = graph_min - BOUNDARY;
	for (i=0; i<GRAPH_BANDS; i++){
		/* print out the bars for each specified column for every band */
		next_band += band_width;
		for (j=0; j<ncols; j++){
			cur_col = ccols[j];
			freq = find_freq(D, dr, cur_col, cur_band, next_band);
			printf("%c%11d |", CH_NL, cur_col);
			bars = ceil(freq * 1.0 / scale);
			for (k=0; k<bars; k++){
				printf("]");
			}
		}
		printf("%c    %7.1f +", CH_NL, next_band);
		cur_band += band_width;
	}
	printf("%c    scale = %d%c", CH_NL, scale, CH_NL);
	return;
}

/* swap two given values of the double type
*/
void
val_swap(double *p1, double *p2){	
	double tmp;
	tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
	return;
}

/* find the maximum value of a given column 
*/
double
find_col_max(csv_t D, int dr, int cur_col){
	int i;
	double max;
	max = D[0][cur_col];
    for (i=0; i<dr; i++){
		if (D[i][cur_col] > max){
			max = D[i][cur_col];
		}	
	}
	return max;
}

/* find the minimum value of a given column 
*/
double
find_col_min(csv_t D, int dr, int cur_col){
	int i;
	double min;
	min = D[0][cur_col];
    for (i=0; i<dr; i++){
		if (D[i][cur_col] < min){
			min = D[i][cur_col];
		}	
	}
	return min;
}

/* find the frequency of a given bar in the frequency histogram 
*/
int
find_freq (csv_t D, int dr, int cur_col, double lbound, double ubound){
	int i, freq=0;
	for (i=0; i<dr; i++){
		if ((D[i][cur_col] >= lbound) && (D[i][cur_col] < ubound)){
			freq += 1;
		}
	}
	return freq;
}

/* calculate the scale of the frequency histogram 
*/
int
find_scale(csv_t D, int dr, int ccols[], int ncols, double band_width,
		double graph_min){
	int i, j, freq, max_freq=0, cur_col, scale=1;
	double cur_band, next_band;
	/* find the highest frequency across all columns */
	cur_band = graph_min - BOUNDARY;
	next_band = graph_min - BOUNDARY;
	for (i=0; i<GRAPH_BANDS; i++){
		next_band += band_width;
		for (j=0; j<ncols; j++){
			cur_col = ccols[j];
			freq = find_freq(D, dr, cur_col, cur_band, next_band);
			if (freq > max_freq){
				max_freq = freq;	
			}
		}
		cur_band += band_width;
	}
	while ((max_freq / scale) > MAX_BAR_LEN){
		scale += 1;
	} 
	return scale;
} 
