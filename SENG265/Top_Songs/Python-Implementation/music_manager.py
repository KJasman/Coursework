"""
!/usr/bin/env python
 -*- coding: utf-8 -*-
Assignment 2 for SENG 265, Software Development Methonds at the University of Victoria; taught by Dr. Hausi Muller.
Data based on: https://www.kaggle.com/datasets/arbazmohammad/world-airports-and-airlines-datasets
Sample input: --AIRLINES="airlines.yaml" --AIRPORTS="airports.yaml" --ROUTES="routes.yaml" --QUESTION="q1" --GRAPH_TYPE="bar"
@author: Konrad Jasman

Note: Issues. See Git repo for up to date file 
"""
import pandas as pd
import typing 
import sys

def parse_args(args: list) -> list:
    """Parses the arguments passed to the program.
            Parameters
            ----------
                args : list, required
                    list of arguments passed to the program.
            Returns
            -------
                list of arguments to be used in the program, formatted.
    """
    myArgs: list[str] = []
    myFiles: list[str] = []
    for arg in args:
        if arg.startswith('--'):
            myArgs.append(arg.split('=')[1])
    
    myFiles=myArgs[2].split(',')
    myArgs[2] = myFiles
    return myArgs

def read_In(fileNames: list) -> pd.DataFrame:
    """Reads a csv file and returns a pandas dataframe. If multiple files are passed, they are concatenated.
            Parameters
            ----------
                fileNames : list, required
                    The list of files to be read.
            Returns
            -------
                pd.DataFrame
                    The dataframe returned.
    """
    toConcat: list = []
    myDF: pd.DataFrame = pd.DataFrame()
    for Name in fileNames:
        toConcat.append(pd.read_csv(Name))
        myDF=pd.concat(toConcat)
        
    return myDF

def output_csv(myDF: pd.DataFrame) -> None:
    """Outputs a csv file.
            Parameters
            ----------
                myDF : pd.DataFrame, required
                    The dataframe to be outputted.
                fileName : str, required
                    The name of the file to be outputted.
    """
    myDF.to_csv('output.csv', index=False)

def sort_DF(myDF: pd.DataFrame, filter: str, head: int) -> pd.DataFrame:
    """Outputs a csv file.
            Parameters
            ----------
                myDF : pd.DataFrame, required
                    The dataframe to be outputted.
                filter : str, required
                    The name of the element to filter by. 0th element of argument list.
                head : int, required
                    The number of elements to print. 1st element of argument list.    
                fileName : str, required
                    The name of the file to be outputted.
    """
    return myDF.filter(items=['artist', 'song', 'year', filter]).sort_values(by=[filter, 'song'], ascending=[False, False]).head(head)

def main(): 
    listArgs: list = parse_args(args=sys.argv)
    listArgs: list = parse_args(args=sys.argv) 
    topRated: pd.DataFrame = read_In(listArgs[2])
    topRated=sort_DF(topRated, listArgs[0], int(listArgs[1]))
    output_csv(topRated)    

if __name__ == '__main__':
    main()
