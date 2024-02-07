# Calendar Parsing -- event_manager.c

This is a C program that reads and processes .ics calendar files.

## Overview

The program takes command line arguments to specify a date range and a file to read from. It then reads the file, parses the events, and prints out the events that fall within the specified date range in a simplified format.

## Main Functions

- `parseInput`: Parses a string for event details and updates a CalendarEvent struct with these details.
- `printEvent`: Prints the event details in a specific format.
- `isNew`: Compares two CalendarEvent structs to check if they represent the same date.
- `emptyEvent` and `emptyTime`: Reset all values of a CalendarEvent struct and a tm struct to 0, respectively.
- `getInput`: Parses the command line arguments to get the start and end dates and the file name.
- `printLogic`: Contains the logic for reading the file line by line, parsing the events, and deciding whether to print an event based on the date range.
- `main`: Gets the command line inputs, opens the file, and calls `printLogic` to process the file. It also handles file opening errors.

## Known Issues

The program has some known issues, such as incorrect logic for checking if an event is within the date range and inefficient use of structs.

## Built With

- C
- time.h library

## Authors

- Konrad Jasman