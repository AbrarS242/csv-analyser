## Project Overview

CSV Analyser allows you to read CSV files, perform various operations on the data, and generate insightful reports. It leverages core C programming concepts such as arrays, strings, and functions to provide a robust solution for working with CSV data. Plus, it's entirely file-operation and `malloc()` free, ensuring simplicity and efficiency in your coding process.

## Key Features

### Data Processing

CSV Analyser reads CSV data from files and processes it into an internal format for manipulation. It provides a user-friendly command-line interface for interacting with the data.

### Interactive Commands

The command-line interface supports various commands for manipulating and analysing the data. From calculating statistics to sorting and plotting, CSV Analyser offers a range of functionalities to meet your data analysis needs.

### Statistical Analysis

You can perform statistical analysis on selected columns of data, including computing maximum, minimum, average, and median values. This feature allows for quick insights into your dataset's characteristics.

### Sorting

CSV Analyser enables sorting of CSV data based on specified columns. It maintains stability in sorting to preserve the original order of rows where ties occur.

### Data Visualization

Generate frequency histograms of selected columns as sideways bar charts for visualizing data distributions. CSV Analyser provides flexible options for customizing the appearance of your plots.

## Getting Started
1. **Clone the Repository**: Begin by cloning the CSV Analyser repository to your local machine.
2. **Compile the Program**: Navigate to the project directory and compile the CSV Analyser program using a C compiler such as `gcc`.

`gcc -o csv_analyser main_program.c`

3. **Run the Program**: Execute the compiled program, providing the path to your CSV file as an argument.
   
`./csv_analyser data.csv`

4. **Interact with the Data**: Once the program is launched, you can enter commands at the prompt to interact with the data and perform various operations. Refer to the list of available commands below for details.

### Commands

- **a**: Perform statistical analysis on selected columns.

Example: `a 1 4`

!['a' command](https://github.com/AbrarS242/csv-analyser/blob/main/example_output/a_command.PNG)

- **d**: Display data from specified columns.

Example: `d 2 0 1`

!['d' command](https://github.com/AbrarS242/csv-analyser/blob/main/example_output/d_command.PNG)

- **s**: Sort the data based on specified columns.

Example: `s 3 0 1 2`

!['s' command](https://github.com/AbrarS242/csv-analyser/blob/main/example_output/s_command.PNG)

- **p**: Generate a frequency histogram of selected columns.

Example: `p 2 3`

!['p' command](https://github.com/AbrarS242/csv-analyser/blob/main/example_output/p_command.PNG)

- **i**: Display index information.

!['i' command](https://github.com/AbrarS242/csv-analyser/blob/main/example_output/i_command.PNG)

## Note ##
This program was written as an assignment at the University of Melbourne. 
The skeleton code has been written by Alistair Moffat (ammoffat@unimelb.edu.au).
