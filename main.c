//
// Created by Matthew on 4/25/2021.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

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
            printf("####%d of %d####\n", index, indexMax);
            printf("Ticket ID: %d\n", curTicket->ticketID);
            printf("Date Submitted: %s", ctime(&curTicket->dateTime));
            printf("Name: %s\n", curTicket->name);
            printf("Email: %s\n", curTicket->email);
            printf("Description: %s\n", curTicket->description);
            printf("Room #: %s\n", curTicket->roomNumber);
            printf("MachineID: %s\n", curTicket->machineID);
            printf("Priority: %i\n", curTicket->priority);
            printf("Next Address: %p\n", curTicket->nextTicket);
            curTicket = curTicket->nextTicket;
        }
    } else {
        printf("\nList is empty.\n");
    }

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

void emptyStack(struct TicketStruct **stackPointer) {
    //Traverse the linked list. After finding the next address, clear the memory space for the current node and move on.
    struct TicketStruct *curTicket, *nextTicket;
    curTicket = *stackPointer;
    stackPointer = NULL;
    while (curTicket != NULL) { //Checking to see if the address is NULL or not
        printf("\nFreeing '%s %s' from the top of the stack\n", curTicket->name, curTicket->description);
        nextTicket = curTicket->nextTicket; //Set to the next address
        free(curTicket);
        curTicket = nextTicket;
    }
}

int importData(char fileName[], struct TicketStruct **curStruct) {
    FILE *dbFile;
    struct TicketStruct temp;
    dbFile = fopen(fileName, "r");
    if (dbFile== NULL) {
        printf("\nError opening files.\n");
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

int exportData(char fileName[], struct TicketStruct **curStruct) {
    FILE *dbFile;
    //Open for writing
    dbFile = fopen(fileName, "w");
    if (dbFile == NULL) {
        printf("\nError opening file.\n");
        return 1;
    }
    while (*curStruct != NULL) { //Checking to see if the address is NULL or not
        fwrite(*curStruct, sizeof(struct TicketStruct), 1, dbFile);
        *curStruct = (*curStruct)->nextTicket; //Set to the next address
    }
    fclose(dbFile);
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
    time(&curTime);
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

    //Testing: Set structure 1
    addToEnd(createTicket(
            "Matt Fisher",
            "viiwee@live.com",
            "Broken screen",
            "G10",
            "B220",
            1, activeTickets, inactiveTickets),
             &activeTickets);
    addToEnd(
    createTicket(
            "Alex Booher",
            "booher@live.com",
            "Shit broke",
            "223",
            "2AA",
            3, activeTickets, inactiveTickets),
             &activeTickets);
    //printStack(activeTickets);
    exportData("test.dat", &activeTickets);
    importData("test.dat", &inactiveTickets);
    printStack(inactiveTickets);
    return 0;
}