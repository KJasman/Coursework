# Music Manager

This is a C program that analyzes song data, pulled from several CSV files. It's designed for SENG 265, Summer 2023, under the guidance of Dr. Hausi Muller.

## Overview

The program takes command line arguments to specify criteria for sorting, the number of top songs to display, and the file names to read from. It then reads the files, parses the songs, and prints out the top songs based on the specified criteria in a CSV format.

## Main Functions

- `freeMemory`: Frees memory from list, starting at the head node.
- `getParameters`: Parses command line arguments into a struct.
- `formatLine`: Parses a CSV formatted line into a string.
- `findIndex`: Finds the relevant index for the searched criteria.
- `parseFile`: Iterates through file, generating new lines to parse with helper functions. Stores the correct indices in nodes.
- `toFile`: Outputs list into a CSV file. Each line will be a comma-delimited string.
- `printFile`: Prints from the final CSV file: output.csv.
- `main`: Gets the command line inputs, opens the file, and calls `parseFile` to process the file. It also handles file opening errors.

## Built With

- C
- list.h library
- emalloc.h library

## Authors

- Konrad Jasman
