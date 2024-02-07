/** @file music_manager.c
 *  @brief A small program to analyze songs data, pulled from several csv files of unknown origin. For SENG 265, Summer 2023 -- Dr. Hausi Muller
 *  @author Konrad Jasman
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

#define MAX_LINE_LEN 256
typedef struct {
    char *fileNames;
    char *criteria;
    int numHead;
}Parameters;

//------------------Function Prototypes
/**
 * @brief Free memory from list, starting at myNode
 * @param  *myNode The first node in the list
 */
void freeMemory(node_t *head);
/**
 * @brief Parse Command Line Argument
 * @param input The array of command line input
 * @returns A struct, sorted into types of input
*/
Parameters getParameters(char *argv[], int argc);
/**
 * @brief Parse csv formatted line into string
 * @param lineIn Contains csv-format comma-delimited string
 * @returns parsed string
*/
char** formatLine (char* lineIn);
/**
 * @brief Find relevant index for searched criteria
 * @param firstLine Contains first line of file as a string. Column headers of csv 
 * @param criteria Criteria to match against column header
 * @returns integer value of index. -1 if not found
*/
int findIndex (char* firstLine, char* criteria);
/**
 * @brief Iterate through file, generating new lines to parse with helper functions. Store the correct indices in nodes.
 * @param fp File pointer to file
 * @param criteria string containing parameter data, pulled from struct
 * @param head node at the head of the list
*/
void parseFile (FILE* fp, char* criteria, node_t** head);
/**
 * @brief Output list into csv file. Each line will be a comma delimited string.
 * @param printHead integer number of top songs to enter into file
 * @param head head node of the list
*/
void toFile (node_t* head, int printHead, char *param);
/**
 * @brief Print from the final csv file: output.csv
*/
void printFile();

//------------------Function Definitions
void freeMemory(node_t *head){
    node_t *temp = NULL;

    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }    
}

Parameters getParameters(char *argv[], int argc){
    Parameters myPar;
    
    char* criteriaPref = "--sortBy=";
    char* displayPref = "--display=";
    char* filePref = "--files=";

    for (int ii = 0; ii<argc; ii++){
        if (strncasecmp(argv[ii], criteriaPref, strlen(criteriaPref))==0){
           myPar.criteria = argv[ii]+strlen(criteriaPref);
        }
        else if(strncasecmp(argv[ii], displayPref, strlen(displayPref))==0){
            myPar.numHead = atoi(argv[ii]+strlen(displayPref));
        }
        else if(strncasecmp(argv[ii], filePref, strlen(filePref))==0){
            myPar.fileNames = argv[ii]+strlen(filePref);
            strtok(myPar.fileNames, ",");
        }
    }
    return myPar;
}

char** formatLine(char* lineIn){
    int countTokens = 0;
    char* tempString = strdup(lineIn);

    char* token = strtok(tempString, ",");
    //Count number of tokens in the input line
    while(token!=NULL){
        countTokens++;
        token = strtok(NULL, ",");
    }

    char** formatted = (char**)malloc((countTokens+1)*sizeof(char*));

    strcpy(tempString, lineIn);

    countTokens = 0;
    token = strtok(tempString, ",");
    while(token!=NULL){
        formatted[countTokens] = strdup(token);
        countTokens++;
        token = strtok(NULL, ",");
    }
    free(tempString);
    return formatted;
}

int findIndex(char* firstLine, char* criteria){
    char** formattedLine = formatLine(firstLine);

    for(int ii = 0; ii<strlen(*formattedLine); ii++){
        if(strcasecmp(formattedLine[ii], criteria)==0){
            return ii;
        }
    }
    return -1;
}

void parseFile (FILE* fp, char* criteria, node_t** head){
    char inLine[MAX_LINE_LEN];
    fgets(inLine, MAX_LINE_LEN, fp); //Header line

    int myIndex = findIndex(inLine, criteria); //acquired column number value

    while(fgets(inLine, MAX_LINE_LEN, fp)!=NULL){
        char** myLine = formatLine(inLine);
        node_t* temp = new_node(myLine[0], myLine[1], atoi(myLine[4]), myLine[myIndex]); //Artist - Song - Year - Score

        *head = add_inorder(*head, temp);
    }
}

void toFile (node_t* head, int printHead, char* param){
    //Put list into a file. 

    FILE *fout = fopen("output.csv", "w");
    if(fout == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    fprintf(fout, "artist, song, year, %s\n", param);

    node_t* curr = head;
    int entries = 0;
    while(curr != NULL && entries < printHead){
        fprintf(fout, "%s,%s,%d,%s\n", curr->artist, curr->song, curr->year, curr->score);
        curr=curr->next;
        entries++;
    }
    fclose(fout);
}

void printFile(){
    //Print head number of rows from the file
    FILE *fin = fopen("output.csv", "r");
    char line[MAX_LINE_LEN];
    if(fin == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    if(fgets(line, sizeof(line), fin)!=NULL){
        printf("%s", line); //Col headers
    }

    while(fgets(line, sizeof(line), fin)!=NULL){
        printf("%s", line);
    }
    fclose(fin);
}

/**
 * @brief The main function and entry point of the program.
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 */
int main(int argc, char *argv[]){


    char *line = NULL;
    node_t *head = NULL;
    line = (char *)malloc(sizeof(char) * MAX_LINE_LEN);

    Parameters myParams = getParameters(argv, argc); //Acquired Parameters from CLI
    FILE* fp = fopen(myParams.fileNames, "r"); //opened file
    parseFile(fp, myParams.criteria, &head);

    toFile(head, myParams.numHead, myParams.criteria);
    free(line);
    fclose(fp); // close file
    freeMemory(head);
    return 0;
}
