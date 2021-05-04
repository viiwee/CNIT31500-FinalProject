// Project Title: CNIT 31500 Final Project
// Description: This program will create tickets, and store them in a local database
// Created by Matthew Fisher, Renee Forfa, Alex Booher, Kenall Birchfield, and Rishabh Kukean on 5/4/2021.
// Email: fishe257@purdue.edu
// CNIT 31500 - Friday 0730 (Kim)
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#define DBFILE "tickets.dat"
#define DBFILE2 "tickets2.dat"
#define PAGEFILENAME "page.out"
#define TIMESITE "http://google.com"
const char *MthStr[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

struct TicketStruct {
    time_t dateTime;
    int ticketID;
    char name[30];
    char email[30];
    char description[100];
    char roomNumber[10];
    char machineID[10];
    int priority;
    struct TicketStruct *nextTicket;
};

time_t curTime;

struct TicketStruct *createListPointer() { //CreateListNoNodes: Function to create the start pointer
    struct TicketStruct *newStruct; //Initialize structure pointer

    //Create the structure pointer and allocate space. This only needs to be the size of a single value since it will only store a single memory location.
    newStruct = (struct TicketStruct*) malloc(sizeof(newStruct));
    if (newStruct == NULL) //Check to make sure address was allocated
    {
        printf("\nCould not allocate space.\n");
        exit(1);
    }
    newStruct = NULL; //Set to NULL so our indexer works correctly.
    return newStruct; //Return the pointer to the new structure
}

int indexList(struct TicketStruct *curStruct) {
    //traverse the linked list and count how many entries there are
    int count = 0;
    while (curStruct != NULL) { //Checking to see if the address is NULL or not
        count++;
        curStruct = curStruct->nextTicket; //Set to the next address
    }
    return count; //Return the number of entries
}

void printStack(struct TicketStruct *curTicket) { //curStudent will simply be an address to the stack
    //Function to Display the List
    printf("\nPrinting list of students:\n");
    int indexMax = indexList(curTicket); //Set this number so we can display X out of X entries, and check to see if it is empty.
    if (curTicket != NULL) { //Check if the list is empty
        for (int index = 1; curTicket != NULL; index++) { //Checking to see if the address is NULL or not
            printf("############%d of %d############\n", index, indexMax);
            printf("Ticket ID: %d\n", curTicket->ticketID);
            printf("Date Submitted: %s", ctime(&curTicket->dateTime));
            printf("Name: %s\n", curTicket->name);
            printf("Email: %s\n", curTicket->email);
            printf("Description: %s\n", curTicket->description);
            printf("Room #: %s\n", curTicket->roomNumber);
            printf("MachineID: %s\n", curTicket->machineID);
            printf("Priority: %i\n", curTicket->priority);
            //printf("Next Address: %p\n", curTicket->nextTicket);
            curTicket = curTicket->nextTicket;
        }
        printf("\n#################\n");
    } else {
        printf("\nThere are currently no tickets.\n");
    }
    printf("Press Any Key to Continue\n");
    getchar();
    getchar();
}

int addToEnd(struct TicketStruct *inputStruct, struct TicketStruct **pTicketStruct) { //*newStruct is the structure that is being added. **curStruct is the pointer to the structure that it is being added to
    struct TicketStruct *newStruct, *curStruct;
    curStruct = *pTicketStruct; //Set the current structure to the one that the structure pointer points to
    newStruct = (struct TicketStruct *) malloc(sizeof(struct TicketStruct)); //Create a new, memory allocated, structure

    if (newStruct == NULL) //Check to make sure address was allocated
    {
        printf("\nCould not allocate space.\n");
        exit(1);
    }

    if (curStruct == NULL) { //If we were handed an empty struct, edit the pointer to the struct
        *pTicketStruct = newStruct; //Set the stack pointer to the new struct
    } else {
        //Find end (find null)
        while (curStruct->nextTicket != NULL) { //Continue through the list until we reach the last student.
            curStruct = curStruct->nextTicket; //Move to the next student
        }
        //At this time curStruct is the last node in the list
        curStruct->nextTicket = newStruct; //Add the new struct onto the end of the last structure
    }
    *newStruct = *inputStruct;
    return 0;
}

void query(struct TicketStruct *curTicket, char passedEmail[])
{
    if(curTicket == NULL) {
        printf("No tickets to search through.\n");
        return;
    }
    bool resultFound = false;
    printf("Searching for tickets with the following email: %s\n", passedEmail);
    int indexMax = indexList(curTicket); //Set this number so we can display X out of X entries, and check to see if it is empty.
    for (int index = 1; curTicket != NULL; index++) { //Checking to see if the address is NULL or not
        if (strcmp(passedEmail, curTicket->email) == 0) { //If the email is found
            printf("############%d of %d############\n", index, indexMax);
            printf("Ticket ID: %d\n", curTicket->ticketID);
            printf("Date Submitted: %s", ctime(&curTicket->dateTime));
            printf("Name: %s\n", curTicket->name);
            printf("Email: %s\n", curTicket->email);
            printf("Description: %s\n", curTicket->description);
            printf("Room #: %s\n", curTicket->roomNumber);
            printf("MachineID: %s\n", curTicket->machineID);
            printf("Priority: %i\n", curTicket->priority);
            printf("Next Address: %p\n", curTicket->nextTicket);
            resultFound = true;
        }
        curTicket = curTicket->nextTicket;
    }
    if (! resultFound)
    { //If no results were found, print out a statement so the user knows the program tried.
        printf("No tickets were found with the email: %s\n", passedEmail);
    }
    printf("\n#################\n");
    printf("Press Any Key to Continue\n");
    getchar();
}

void emptyStack(struct TicketStruct **stackPointer) {
    //Traverse the linked list. After finding the next address, clear the memory space for the current node and move on.
    struct TicketStruct *curTicket, *nextTicket;
    curTicket = *stackPointer;
    stackPointer = NULL;
    while (curTicket != NULL) { //Checking to see if the address is NULL or not
        //printf("\nFreeing '%s %s' from the top of the stack\n", curTicket->name, curTicket->description);
        nextTicket = curTicket->nextTicket; //Set to the next address
        free(curTicket);
        curTicket = nextTicket;
    }
}

void sortByDate(struct TicketStruct *pTicketStruct, bool ascending) {
    if(pTicketStruct == NULL) {
        printf("No tickets to sort by\n");
        return;
    }

    //Create new structure pointer
    struct TicketStruct *pTempTicketStruct = createListPointer(); //Permanent structure pointer

    //Create temporary nodes: curTicket (Used to hold the address of the
    struct TicketStruct *curTicket, *prevTicket, *tempCurTicket, *pInsertAfter;
    curTicket = (struct TicketStruct *) malloc(sizeof(struct TicketStruct));
    *curTicket = *pTicketStruct;
    //strcpy(curTicket->name, "Test");
    //printf("Name of original: %s\n", pTicketStruct->name);
    //printf("Name of new: %s\n", curTicket->name);

    pTempTicketStruct = curTicket; //Send one ticket over to the new structure
    prevTicket = curTicket; //Set a pointer to the ticket that is now in the new structure
    curTicket = curTicket->nextTicket; //Set the next ticket in line
    prevTicket->nextTicket = NULL; //Set this to NULL so we don't cause a loop

    while (curTicket != NULL) { //Sorting through the inputted ticket struct
        //Using the current struct, compare it to all of the structures within the tempStruct
        tempCurTicket = pTempTicketStruct;
        bool isFirst = true;
        while (tempCurTicket != NULL) {
            if (ascending) {
                if (difftime(curTicket->dateTime, tempCurTicket->dateTime) > 0) {
                    // Check if the priority is lower than the current priority of the temp ticket
                    // Set variable = address of the ticket that this one should go after.
                    pInsertAfter = tempCurTicket; //Set this so we know where to insert the ticket once we search them all
                    isFirst = false;
                }
            } else { //If descending order
                if (difftime(curTicket->dateTime, tempCurTicket->dateTime) <= 0) {
                    // Check if the priority is lower than the current priority of the temp ticket
                    // Set variable = address of the ticket that this one should go after.
                    pInsertAfter = tempCurTicket; //Set this so we know where to insert the ticket once we search them all
                    isFirst = false;
                }
            }

            tempCurTicket = tempCurTicket->nextTicket;
        }
        //Create a whole new node, then add it to the struct
        struct TicketStruct *newTicket = (struct TicketStruct *) malloc(sizeof(struct TicketStruct));
        *newTicket = *curTicket; //Set the new struct to the current one we are comparing

        if (isFirst) {
            newTicket->nextTicket = pTempTicketStruct;
            pTempTicketStruct = newTicket; //If this is the new first ticket in the struct, set the struct pointer to this ticket instead
        } else {
            newTicket->nextTicket = pInsertAfter->nextTicket; //Set the nextAddress of the new student to the next student.
            pInsertAfter->nextTicket = newTicket; //If this is not the first ticket in the struct, Set the nextAddress of the student before this one to this new student's address
        }
        // If curTicket->priority   > tempCurTicket: add to the list using the address set in the previous while loop
        curTicket = curTicket->nextTicket;
    }
    printStack(pTempTicketStruct);
    emptyStack(&pTempTicketStruct);
}

void sortByPriority(struct TicketStruct *pTicketStruct, bool ascending) {
    if(pTicketStruct == NULL) {
        printf("No tickets to sort by\n");
        return;
    }

    //Create new structure pointer
    struct TicketStruct *pTempTicketStruct = createListPointer(); //Permanent structure pointer

    //Create temporary nodes: curTicket (Used to hold the address of the
    struct TicketStruct *curTicket, *prevTicket, *tempCurTicket, *pInsertAfter;
    curTicket = (struct TicketStruct *) malloc(sizeof(struct TicketStruct));
    *curTicket = *pTicketStruct;
    //strcpy(curTicket->name, "Test");
    //printf("Name of original: %s\n", pTicketStruct->name);
    //printf("Name of new: %s\n", curTicket->name);

    pTempTicketStruct = curTicket; //Send one ticket over to the new structure
    prevTicket = curTicket; //Set a pointer to the ticket that is now in the new structure
    curTicket = curTicket->nextTicket; //Set the next ticket in line
    prevTicket->nextTicket = NULL; //Set this to NULL so we don't cause a loop

    while (curTicket != NULL) { //Sorting through the inputted ticket struct
        //Using the current struct, compare it to all of the structures within the tempStruct
        tempCurTicket = pTempTicketStruct;
        bool isFirst = true;
        while (tempCurTicket != NULL) {
            if (ascending) {
                if (curTicket->priority >= tempCurTicket->priority) {
                    // Check if the priority is lower than the current priority of the temp ticket
                    // Set variable = address of the ticket that this one should go after.
                    pInsertAfter = tempCurTicket; //Set this so we know where to insert the ticket once we search them all
                    isFirst = false;
                }
            } else { //If descending order
                if (curTicket->priority < tempCurTicket->priority) {
                    // Check if the priority is lower than the current priority of the temp ticket
                    // Set variable = address of the ticket that this one should go after.
                    pInsertAfter = tempCurTicket; //Set this so we know where to insert the ticket once we search them all
                    isFirst = false;
                }
            }

            tempCurTicket = tempCurTicket->nextTicket;
        }
        //Create a whole new node, then add it to the struct
        struct TicketStruct *newTicket = (struct TicketStruct *) malloc(sizeof(struct TicketStruct));
        *newTicket = *curTicket; //Set the new struct to the current one we are comparing

        if (isFirst) {
            newTicket->nextTicket = pTempTicketStruct;
            pTempTicketStruct = newTicket; //If this is the new first ticket in the struct, set the struct pointer to this ticket instead
        } else {
            newTicket->nextTicket = pInsertAfter->nextTicket; //Set the nextAddress of the new student to the next student.
            pInsertAfter->nextTicket = newTicket; //If this is not the first ticket in the struct, Set the nextAddress of the student before this one to this new student's address
        }
        // If curTicket->priority   > tempCurTicket: add to the list using the address set in the previous while loop
        curTicket = curTicket->nextTicket;
    }
    printStack(pTempTicketStruct);
    //Go through the structure
    //Check priority of the
    emptyStack(&pTempTicketStruct);
}

int importData(char fileName[], struct TicketStruct **curStruct) {
    FILE *dbFile;
    struct TicketStruct temp;
    dbFile = fopen(fileName, "r");
    if (dbFile== NULL) {
        printf("\nNo database file found. Data will not be imported.\n");
        return 1;
    }

    //Empty the stack so that we can read everything back in
    emptyStack(curStruct);
    while(fread(&temp, sizeof(struct TicketStruct), 1, dbFile)) {
        temp.nextTicket = NULL;
        addToEnd(&temp, curStruct); //Add this struct to the correct list
        printf("Imported the following ticket ID: %d\n", temp.ticketID);
    }
    fclose(dbFile);
    return 0;
}

int exportData(char fileName[], struct TicketStruct *curStruct) {
    FILE *dbFile;
    //Open for writing
    dbFile = fopen(fileName, "w");
    if (dbFile == NULL) {
        printf("\nError opening file.\n");
        return 1;
    }
    while (curStruct != NULL) { //Checking to see if the address is NULL or not
        fwrite(curStruct, sizeof(struct TicketStruct), 1, dbFile);
        curStruct = (curStruct)->nextTicket; //Set to the next address
    }
    fclose(dbFile);
    printf("Successfully exported tickets to database.\n");
    return 0;
}

int nextTicketID(struct TicketStruct *pActiveTickets, struct TicketStruct *pInactiveTickets) {
    int newID = 1;
    while (pActiveTickets != NULL) { //Check the active ticket list for the highest ticketID
        if (pActiveTickets->ticketID >= newID) {
            newID = pActiveTickets->ticketID + 1;
        }
        pActiveTickets = pActiveTickets->nextTicket; //Set to the next address
    }

    while (pInactiveTickets != NULL) { //Check the inactive ticket list for the highest ticketID
        if (pInactiveTickets->ticketID >= newID) {
            newID = pInactiveTickets->ticketID + 1;
        }
        pInactiveTickets = pInactiveTickets->nextTicket; //Set to the next address
    }
    return newID; //Return the new ID for a ticket to be generated
}

void solveTicket(struct TicketStruct **pTicketStruct, struct TicketStruct **pSolvedTicketStruct, int ticketID) {
    struct TicketStruct *curTicket, *solvedTicket, *prevTicket;
    curTicket = *pTicketStruct;
    solvedTicket=NULL;
    //Find address of solved ticket, and the ticket/structure pointer before it
    while (curTicket != NULL) {
        if (curTicket->ticketID == ticketID) {
            solvedTicket = curTicket;
            curTicket=NULL;
            continue;
        }
        prevTicket=curTicket;
        curTicket=curTicket->nextTicket;
    }

    if(solvedTicket == NULL) {//If solvedTicket was never set, it means we couldn't find it.
        printf("\nTicket not found.\n");
        return;
    }

    //Update address of previous ticket to the next ticket
    //If the address is the same as the initial pointer, update the structure pointer instead
    if(solvedTicket == *pTicketStruct) {
        *pTicketStruct = solvedTicket->nextTicket;
        solvedTicket->nextTicket=NULL;
    } else { //If we are modifying a normal ticket
        prevTicket->nextTicket = solvedTicket->nextTicket;
        solvedTicket->nextTicket=NULL;
        addToEnd(solvedTicket, pSolvedTicketStruct);
    }
}

//Begin time functions using CURL

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

void download_time() {
    CURL *curl_handle;
    static const char *pagefilename = "page.out";
    FILE *pagefile;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* set URL to get here */
    curl_easy_setopt(curl_handle, CURLOPT_URL, TIMESITE);

    /* Switch on full protocol/debug output while testing */
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    /* disable progress meter, set to 0L to enable it */
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    /* open the file */
    pagefile = fopen(pagefilename, "wb");
    if(pagefile) {

        /* write the page body to this file handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEHEADER, pagefile);
        //curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

        /* get it! */
        curl_easy_perform(curl_handle);

        /* close the header file */
        fclose(pagefile);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    curl_global_cleanup();
}

void update_time(time_t *out_tm) {
    static const char *pagefilename = PAGEFILENAME;
    struct tm tm;
    time_t output_time;
    unsigned short day, year, hour, min, sec, mon;
    char char_day[26], char_month[26];
    char line[256];

    download_time(); //First, download the most recent time from online

    FILE *pagefile = fopen(pagefilename, "r");

    if (pagefile != NULL) {
        while (fgets(line, sizeof line, pagefile) != NULL) /* read a line */
        {
            if (strncmp((line), "Date:", 5) == 0) { //Check if this is the date line
                sscanf(line, "Date: %s %hu %s %hu %hu:%hu:%hu", char_day, &day, char_month, &year, &hour, &min,
                       &sec); //Convert the date line to the correct variables
                for (int i = 1; i <= 12; i++) { //Scan through the list of months, setting the correct one.
                    if (strcmp(MthStr[i], char_month) == 0) {
                        mon = i; //Set the numerical month
                    }
                }
                tm.tm_hour = hour;
                tm.tm_min = min;
                tm.tm_sec = sec;
                tm.tm_mday = day;
                tm.tm_mon = mon;
                tm.tm_year = year - 1900; //Offset by epoch

                output_time = mktime(&tm); //Set an output time equal to the computed time created by the structure we made
                *out_tm = output_time; //Set the value of the time variable inputted into this function
                //printf("Date: %s %hu %s %hu %hu:%hu:%hu GMT\n", char_day, day, char_month, year, hour, min, sec);
            }
        }
        fclose(pagefile); //Close the file for reading
    }
}

//This needs to be after the update_time function.
struct TicketStruct *createTicket (char name[], char email[], char description[], char roomNumber[], char machineID[], int priority, struct TicketStruct *pActiveTickets, struct TicketStruct *pInactiveTickets) {
    struct TicketStruct *newTicket;
    //Allocate space (malloc) for the new structure
    newTicket = (struct TicketStruct *) malloc(sizeof(struct TicketStruct));
    if (newTicket == NULL) //Check to make sure address was allocated
    {
        printf("\nCould not allocate space.\n");
        exit(1);
    }
    //Set the values of the new struct
    update_time(&curTime);
    newTicket->ticketID = nextTicketID(pActiveTickets, pInactiveTickets);
    newTicket->dateTime = curTime;
    strcpy(newTicket->name, name);
    strcpy(newTicket->email, email);
    strcpy(newTicket->description, description);
    strcpy(newTicket->roomNumber, roomNumber);
    strcpy(newTicket->machineID, machineID);
    newTicket->priority = priority;
    newTicket->nextTicket = NULL; //Copy the address stored in the structure pointer into this new structure. This will point this one to what used to be the first structure. This may be NULL, that is okay.
    return newTicket;
}
int main() {

    struct TicketStruct *activeTickets = createListPointer(); //Initialize this variable for later use.
    struct TicketStruct *inactiveTickets = createListPointer(); //Initialize this variable for later use.

    //Automatically import on startup
    importData(DBFILE, &activeTickets);
    importData(DBFILE2, &inactiveTickets);

    //User interface
    int response;
    int ticketResponse;
    int ticketID;
    char name[30];
    char email[30];
    char description[100];
    char roomNumber[10];
    char machineID[10];
    int priority;
    do
    {
        //Setting up the interactive menu
        printf("\nSelect an option below to begin: \n");
        printf("0. Exit the program\n");
        printf("1. Create ticket\n");
        printf("2. Query unsolved tickets\n");
        printf("3. Query solved tickets\n");
        printf("4. View unsolved tickets\n");
        printf("5. View solved tickets\n");
        printf("6. Solve a ticket\n");
        printf("7. Export to database\n");
        printf("8. Reload from database\n");

        //Read user input
        scanf("%d", &response);

        switch (response) {
            //Create ticket
            case 1:
                printf("Enter the first and last name: ");
                getchar();
                gets(name);

                printf("Enter the email: ");
                gets(email);

                printf("Enter the description of the problem: ");
                gets(description);

                printf("Enter the room number: ");
                gets(roomNumber);

                printf("Enter the Machine ID: ");
                gets(machineID);

                printf("Enter the priority of the problem (A number between 1-5): ");
                scanf("%d", &priority);

                addToEnd(createTicket(name, email, description, roomNumber, machineID, priority, activeTickets,inactiveTickets),
                         &activeTickets);
                break;

                //Query unsolved tickets
            case 2:
                printf("Enter the email you would like to query by: \n");
                scanf("%s", &email);

                query(activeTickets, email);
                break;

                //Query solved tickets
            case 3:
                printf("Enter the email you would like to query by: \n");
                scanf("%s", &email);

                query(inactiveTickets, email);
                break;

                //View unsolved tickets (opens the tickets menu)
            case 4:
                do
                {
                    printf("Select an option below: \n");
                    printf("0. Return to main menu\n");
                    printf("1. View unsolved tickets (Oldest first)\n");
                    printf("2. View unsolved tickets (Newest first)\n");
                    printf("3. View unsolved tickets (Sort by highest priority first)\n");
                    printf("4. View unsolved tickets (Sort by lowest priority first)\n");

                    //Read user input
                    scanf("%d", &ticketResponse);

                    switch (ticketResponse) {
                        //View tickets (Oldest first)
                        case 1:
                            sortByDate(activeTickets, true);
                            break;

                            //View tickets (Newest first)
                        case 2:
                            sortByDate(activeTickets, false);
                            break;

                            //View tickets (Sort by priority)
                        case 3:
                            sortByPriority(activeTickets, true);
                            break;

                        case 4:
                            sortByPriority(activeTickets, false);
                            break;
                    }
                }

                while (ticketResponse != 0);

                break;

                //View solved tickets (opens the tickets menu)
            case 5:
                do
                {
                    printf("Select an option below: \n");
                    printf("0. Return to main menu\n");
                    printf("1. View solved tickets (Oldest first)\n");
                    printf("2. View solved tickets (Newest first)\n");
                    printf("3. View solved tickets (Sort by highest priority first)\n");
                    printf("4. View solved tickets (Sort by lowest priority first)\n");

                    //Read user input
                    scanf("%d", &ticketResponse);

                    switch (ticketResponse) {
                        //View tickets (Oldest first)
                        case 1:
                            sortByDate(inactiveTickets, true);
                            break;

                            //View tickets (Newest first)
                        case 2:
                            sortByDate(inactiveTickets, false);
                            break;

                            //View tickets (Sort by priority)
                        case 3:
                            sortByPriority(inactiveTickets, true);
                            break;

                        case 4:
                            sortByPriority(inactiveTickets, false);
                            break;
                    }
                }

                while (ticketResponse != 0);

                break;

                //Solve ticket
            case 6:
                printf("Enter the ticket ID: \n");
                scanf("%d", &ticketID);

                solveTicket(&activeTickets, &inactiveTickets, ticketID);
                break;

                //Export to database
            case 7:
                exportData(DBFILE, activeTickets);
                exportData(DBFILE2, inactiveTickets);
                break;

                //Reload from database
            case 8:
                importData(DBFILE, &activeTickets);
                importData(DBFILE2, &inactiveTickets);
                break;
        }
    } while (response != 0);
    return 0;
}