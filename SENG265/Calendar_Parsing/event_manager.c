/** @brief Assignment for SENG265, Summer 2023 under Dr. Hausi Mueller. Takes .ics files in the format: './event_manager --start=2023/4/20 --end=2023/4/30 --file=<example.ics>'
 * Outputs a simplified event format w/ details
 *  @author Konrad J.
 */

/*
To fix in the future (note added on 2023/06/21):
-Overload operator for comparing dates. 
-Logic for checking if event is within date range is incorrect (does not account for months with 31 days, etc.).
-Better utilize pointers & references instead of throwing around structs of myCal.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


#define MAX_LINE_LEN 132
/*
Takes as parameters a string and a struct Calendar.
 *Returns a struct calendar containing parsed data from string
 *String generated from file line
 */
struct CalendarEvent parseInput(char *str, struct CalendarEvent myCal);
/*
Print event to output, return nothing.
parameters: calendar struct, integer representing boolean whether the date is the same as the previous call, and integer representing the number of calls to printEvent. 
*/
void printEvent(struct CalendarEvent myCal, int sameDate, int numCalls);
/*
Process time and date into correct printable values. Return calendarEvent processed for printing.
Take as parameters: calendarEvent with raw values.
*/
struct CalendarEvent processDate(struct CalendarEvent myCal);
/*
 Return 1 if these structs are the same. 0 if not.
 Takes as parameters:  two structs of CalendarEvent,"oldCal" and "MyCal". oldCal is the previous event, and myCal is the current.
*/
int isNew(struct CalendarEvent myCal, struct CalendarEvent oldCal);
/*
Take a struct of type CalendarEvent, set all values to 0. Return the struct.
*/
struct CalendarEvent emptyEvent(struct CalendarEvent myCal);
/*
Take a struct of type tm, set all values to 0. Return the struct.
*/
struct tm emptyTime(struct tm myTime);
/*
Parse the string into struct myInputs, based off defined prefixes. Return a struct of myInputs with fields: (start and end) year, month and day 
Take as parameters:
-int argc, length of input
-string argv, containing the input given by user
 */
struct myInputs getInput(int argc, char* argv[]);
/*
Contains logic to print to output. Of note, use myEvent as a buffer until reaching "END:VEVENT", then analyze logic to print event in desired format.
Return a struct of CalendarEvent, containing the updated myEvent.
Takes as parameters: 
-struct Inputs containing input from user, parsed from CLI
-char inLine, a string of defined length containing a line from the file. 
-struct myEvent, containing a live-updated version of the calendarEvent. Updated by parsing the file.
FILE* fp, a pointer to the file. 
*/
struct CalendarEvent printLogic(struct myInputs Inputs, char inLine[MAX_LINE_LEN], struct CalendarEvent myEvent, FILE* fp);
//Calendar Struct
struct CalendarEvent{
    char eventDate[MAX_LINE_LEN];
    char endSMin[MAX_LINE_LEN];
    char endSHour[MAX_LINE_LEN];
    char endEHour[MAX_LINE_LEN];
    char endEMin[MAX_LINE_LEN];
    char location[MAX_LINE_LEN];
    char summary[MAX_LINE_LEN];
    int repeat;

    char* sToD;
    char* eToD;

    struct tm mySTime;
    struct tm myETime;
};
//Inputs Struct
struct myInputs{
    char* fileName;

    int startYear, startMonth, startDay;
    int endYear, endMonth, endDay;
};

struct CalendarEvent parseInput(char *strIn, struct CalendarEvent myCal){

    if(strstr(strIn, "DTSTART:")!=NULL){
        myCal.mySTime.tm_year=0;
        //Parse into numbers for comparison
        sscanf((strIn+strlen("DTSTART:")), "%4d%2d%2dT%2d%2d", &myCal.mySTime.tm_year, &myCal.mySTime.tm_mon, &myCal.mySTime.tm_mday, &myCal.mySTime.tm_hour, &myCal.mySTime.tm_min);
    }
    else if(strstr(strIn, "DTEND:")!=NULL){
        sscanf((strIn+strlen("DTEND:")), "%4d%2d%2dT%2d%2d", &myCal.myETime.tm_year, &myCal.myETime.tm_mon, &myCal.myETime.tm_mday, &myCal.myETime.tm_hour, &myCal.myETime.tm_min);
    }
    else if(strstr(strIn, "LOCATION:")!=NULL){
        strcpy(myCal.location, strIn+strlen("LOCATION:"));
        myCal.location[strlen(myCal.location)-1]='\0';
    }
    else if(strstr(strIn, "SUMMARY:")!=NULL){
        strcpy(myCal.summary, strIn+strlen("SUMMARY:"));
        myCal.summary[strlen(myCal.summary)-1]='\0';
    }//repeat rule handling
    else if(strstr(strIn, "UNTIL=")!=NULL){
        char day[3];
        strncpy(day, strstr(strIn, "UNTIL=")+12, 2);   
        myCal.repeat = atoi(day);
        myCal.repeat = abs((myCal.mySTime.tm_mday - myCal.repeat))/7;
    }

    return myCal;
}
struct CalendarEvent processDate(struct CalendarEvent myCal){
    myCal.eToD = "AM";
    myCal.sToD = "AM";

    myCal.mySTime.tm_year-=1900;
    myCal.mySTime.tm_mon-=1;
    //Adjust to 12-hr format
    if(myCal.myETime.tm_hour>12){
        myCal.eToD = "PM";
        myCal.myETime.tm_hour-=12;
    }
    if(myCal.myETime.tm_hour==12){
        myCal.eToD="PM";
    }
    if(myCal.mySTime.tm_hour>12){
        myCal.sToD="PM";
        myCal.mySTime.tm_hour-=12;
    }
    if(myCal.mySTime.tm_hour==12){
        myCal.sToD="PM";
    }

    strftime(myCal.eventDate, MAX_LINE_LEN, "%B %d, %Y", &myCal.mySTime); 
    strftime(myCal.endSMin, MAX_LINE_LEN, "%M", &myCal.mySTime);
    strftime(myCal.endEMin, MAX_LINE_LEN, "%M", &myCal.myETime);

    return myCal;
}
void printEvent(struct CalendarEvent myCal, int sameDate, int numCalls){ 
    
    myCal=processDate(myCal);

    if(sameDate!=1){
        if(numCalls>0) {printf("\n");}
        printf("%s\n", myCal.eventDate);
        for(int ii = 0; ii<=strlen(myCal.eventDate)-1; ii++){
        printf("-");
        }
        printf("\n");
    }
    if(myCal.mySTime.tm_hour<10) printf(" ");
    printf("%d:%s %s to ", myCal.mySTime.tm_hour, myCal.endSMin, myCal.sToD);
    if(myCal.myETime.tm_hour<10) printf(" ");
    printf("%d:%s %s: ", myCal.myETime.tm_hour, myCal.endEMin, myCal.eToD);

    printf("%s {{%s}}\n", myCal.summary, myCal.location);
} 
int isNew(struct CalendarEvent MyCal, struct CalendarEvent oldCal){
    if(MyCal.mySTime.tm_year == oldCal.mySTime.tm_year&&MyCal.mySTime.tm_mon==oldCal.mySTime.tm_mon && MyCal.mySTime.tm_mday==oldCal.mySTime.tm_mday){
        return 1;
    }
    else return 0;
}
struct tm emptyTime(struct tm myTime){
    myTime.tm_gmtoff=0;
    myTime.tm_hour=0;
    myTime.tm_isdst=0;
    myTime.tm_mday=0;
    myTime.tm_min=0;
    myTime.tm_mon=0;
    myTime.tm_sec=0;
    myTime.tm_wday=0;
    myTime.tm_yday=0;
    myTime.tm_year=0;
    myTime.tm_zone=0;

    return myTime;
}
struct CalendarEvent emptyEvent(struct CalendarEvent myCal){
    myCal.eventDate[0] = '\0';
    myCal.endEMin[0] = '\0';
    myCal.endSHour[0] = '\0';
    myCal.endEHour[0] = '\0';
    myCal.endSMin[0] = '\0';
    myCal.location[0] = '\0';
    myCal.summary[0] = '\0';
    myCal.repeat = 0;
    
    myCal.myETime=emptyTime(myCal.myETime);
    myCal.mySTime=myCal.myETime;

    return myCal;
}
struct myInputs getInput(int argc, char* argv[]){
    struct myInputs Inputs;

    char* filePrefix = "--file=";
    char* dateSPrefix = "--start=";
    char* dateEPrefix = "--end=";

    for(int ii = 0; ii<argc; ii++){
        if(strncasecmp(argv[ii], filePrefix, strlen(filePrefix))==0){
            Inputs.fileName = argv[ii]+strlen(filePrefix);
        }
        if(strncasecmp(argv[ii], dateSPrefix, strlen(dateSPrefix))==0){
            Inputs.startYear = atoi(strtok(argv[ii]+strlen(dateSPrefix), "/"));
            Inputs.startMonth = atoi(strtok(NULL, "/"));
            Inputs.startDay = atoi(strtok(NULL, "/"));
        }
        if(strncasecmp(argv[ii], dateEPrefix, strlen(dateEPrefix))==0){
            Inputs.endYear = atoi(strtok(argv[ii]+strlen(dateEPrefix), "/"));
            Inputs.endMonth = atoi(strtok(NULL, "/"));
            Inputs.endDay = atoi(strtok(NULL, "/"));
        }
    }
    return Inputs;
}
struct CalendarEvent printLogic(struct myInputs Inputs, char inLine[MAX_LINE_LEN], struct CalendarEvent myEvent, FILE* fp){
    struct CalendarEvent oldCal;
    int firstCall = 0; 
    int sameDate = 0;
    while((fgets(inLine, MAX_LINE_LEN, fp)!=NULL)){    
        myEvent = parseInput(inLine, myEvent);
        if(strstr(inLine, "END:VEVENT")!=0){
            //If within date range, print when hit buffer
            if(myEvent.mySTime.tm_mon*30+myEvent.mySTime.tm_mday>=Inputs.startDay+30*Inputs.startMonth&&
            myEvent.myETime.tm_mon*30+myEvent.myETime.tm_mday<=Inputs.endDay+30*Inputs.endMonth){
                if(myEvent.repeat != 0){
                    for(int ii = 0; ii<=myEvent.repeat; ii++){
                        printEvent(myEvent, 0, firstCall);
                        firstCall++;
                        myEvent.mySTime.tm_mday+=7;
                        oldCal=myEvent;
                    }
                    myEvent.repeat=0;  
                }
                else if(firstCall==0){
                    printEvent(myEvent, sameDate, firstCall);
                    oldCal=myEvent;
                    firstCall++;                  
                }
                else if (firstCall!=0){
                    sameDate=isNew(myEvent, oldCal);
                    oldCal=myEvent;
                    printEvent(myEvent, sameDate, firstCall);
                }
            }
            myEvent=emptyEvent(myEvent);
        }
    }
    return myEvent;
}
//Open file, iterate through file and print events.
int main(int argc, char *argv[]){
    struct myInputs Inputs = getInput(argc, argv);
    struct CalendarEvent myEvent;
    myEvent.repeat=0;
    //Attempt to open file for reading. Terminate if fail.
    FILE* fp = fopen(Inputs.fileName, "r"); 
    if (fp == NULL){
        printf("File Cannot be Opened\n");
        return 0;
    }
    char inLine[MAX_LINE_LEN];
    while((fgets(inLine, MAX_LINE_LEN, fp)!=NULL)){       
       myEvent=printLogic(Inputs, inLine, myEvent, fp);
    }
    fclose(fp);
    return 0;
}
