// Project Title: CNIT 31500 Lab 4 - Queue and Stack
// Description: This program will create and edit both a queue and stack
// Created by Matthew Fisher on 4/18/2021.
// Email: fishe257@purdue.edu
// CNIT 31500 - Friday 0730 (Kim)
//
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXNAME 30
//Create Structure for the list. Data: First Name, Last Name, PUID, Age
struct PurdueStudent {
    char firstName[MAXNAME];
    char lastName[MAXNAME];
    long PUID;
    int age;
    struct PurdueStudent *nextAddr;
};

struct PurdueStudent *stackStruct = NULL; //Initialize this variable for later use.
struct PurdueStudent *queueStruct_in = NULL; //Initialize this variable for later use.
struct PurdueStudent *queueStruct_out = NULL; //Initialize this variable for later use.

struct PurdueStudent *createListNoNodes() { //CreateListNoNodes: Function to create the start pointer
    struct PurdueStudent *newFirstStudent; //Initialize structure pointer

    //Create the structure pointer and allocate space. This only needs to be the size of a single value since it will only store a single memory location.
    newFirstStudent = (struct PurdueStudent*) malloc(sizeof(newFirstStudent));
    if (newFirstStudent == NULL) //Check to make sure address was allocated
    {
        printf("\nCould not allocate space.\n");
        exit(1);
    }
    newFirstStudent = NULL; //Set to NULL so our indexer works correctly.
    return newFirstStudent; //Return the pointer to the new structure
}

int indexList(struct PurdueStudent *curStudent) {
    //traverse the linked list and count how many entries there are
    int count = 0;
    while (curStudent != NULL) { //Checking to see if the address is NULL or not
        count++;
        curStudent = curStudent->nextAddr; //Set to the next address
    }
    return count; //Return the number of entries
}
bool empty(struct PurdueStudent *curStudent) {
    int size = indexList(curStudent);
    if (size == 0) return true;
    return false;
}
void printStack(struct PurdueStudent *curStudent) { //curStudent will simply be an address to the stack
    //Function to Display the List
    printf("\nPrinting list of students:\n");
    int indexMax = indexList(curStudent); //Set this number so we can display X out of X entries, and check to see if it is empty.
    if (! empty(curStudent)) { //Check if the list is empty
        for (int index = 1; curStudent != NULL; index++) { //Checking to see if the address is NULL or not
            printf("####%d of %d####\n", index, indexMax);
            printf("Current Address: %p\n", curStudent);
            printf("Name: %s %s\n", curStudent->firstName, curStudent->lastName);
            printf("PUID: %ld\n", curStudent->PUID);
            printf("Age: %i\n", curStudent->age);
            printf("Next Address: %p\n", curStudent->nextAddr);
            curStudent = curStudent->nextAddr;
        }
    } else {
        printf("\nList is empty.\n");
    }
}
//PUSH Operation for placing new structure on stack
//Create the new structure
//Point towards current last node
//Fill in fields for new structure
//Change top of stack pointer
void pushStack(struct PurdueStudent **curStruct, char firstName[], char lastName[], long PUID, int age){ //Insert a value at the front of the list
    struct PurdueStudent *newStudent;
    //Allocate space (malloc) for the new structure
    newStudent = (struct PurdueStudent *) malloc(sizeof(struct PurdueStudent));
    if (newStudent == NULL) //Check to make sure address was allocated
    {
        printf("\nCould not allocate space.\n");
        exit(1);
    }
    //Set the values of the new struct
    strcpy(newStudent->firstName, firstName);
    strcpy(newStudent->lastName, lastName);
    newStudent->PUID = PUID;
    newStudent->age = age;
    newStudent->nextAddr = stackStruct; //Copy the address stored in the structure pointer into this new structure. This will point this one to what used to be the first structure. This may be NULL, that is okay.
    *curStruct = newStudent; //Set the value of the structure pointer to this new structure's address.
}

void popStack(struct PurdueStudent **curStruct) { //Use of double pointer in order to send and edit the actual initial pointer
    struct PurdueStudent *delStudent, *nextStudent;
    delStudent = *curStruct; //Set a temp variable equal to the address of the deleted student (1st in list)
    if ( empty(*curStruct) ) { //Check if the stack is empty. If it is, return.
        printf("This stack is empty.\n");
        return;
    }
    printf("\nPopping '%s %s' from the front of the stack\n", delStudent->firstName, delStudent->lastName);
    nextStudent = (*curStruct)->nextAddr; //Set the nextStudent to the address of the listed student that will be deleted
    *curStruct = nextStudent; //Set the value of curStruct to the next address listed.

    //Free memory for the first struct
    free(delStudent);
}

void emptyStack(struct PurdueStudent **stackPointer) {
    //Traverse the linked list. After finding the next address, clear the memory space for the current node and move on.
    struct PurdueStudent *nextStudent, *curStudent;
    curStudent = *stackPointer;
    if ( empty(*stackPointer) ) { //Check if the stack is empty. If it is, return.
        printf("This stack is empty.\n");
        return;
    }
    *stackPointer = NULL; //Set the stack pointer to NULL, so that the stack resets.
    while (curStudent != NULL) { //Checking to see if the address is NULL or not
        printf("\nFreeing '%s %s' from the top of the stack\n", curStudent->firstName, curStudent->lastName);
        nextStudent = curStudent->nextAddr; //Set to the next address
        free(curStudent);
        curStudent = nextStudent;
    }
}
void enqueue(struct PurdueStudent **queueIn, struct PurdueStudent **queueOut, char firstName[], char lastName[], long PUID, int age) {
    struct PurdueStudent *newStudent;
    //Allocate space (malloc) for the new structure
    newStudent = (struct PurdueStudent *) malloc(sizeof(struct PurdueStudent));
    if (newStudent == NULL) //Check to make sure address was allocated
    {
        printf("\nCould not allocate space.\n");
        exit(1);
    }
    //Set the values of the new struct
    strcpy(newStudent->firstName, firstName);
    strcpy(newStudent->lastName, lastName);
    newStudent->PUID = PUID;
    newStudent->age = age;
    newStudent->nextAddr = NULL;
    if (*queueIn != NULL) { //Check if there is nothing in the queue. If there is nothing, do not attempt to edit the next node's address.
        (*queueIn)->nextAddr = newStudent; //Set the prior last structures address field to this new structure.
    }
    *queueIn = newStudent; //Update the address of queueIn
    if (*queueOut == NULL) *queueOut = newStudent; //If the in node has nothing set, set its address to this.
}

void dequeue(struct PurdueStudent **queueOut, struct PurdueStudent **queueIn) { //Use of double pointer in order to send and edit the actual initial pointer
    struct PurdueStudent *delStudent, *nextStudent;
    delStudent = *queueOut; //Set a temp variable equal to the address of the deleted student (1st in list)
    //printf("\ndelStudent Pointer: %p", delStudent);
    //printf("\nQueueOut Pointer: %p", *queueOut);
    //printf("\nQueueIn Pointer: %p", *queueIn);

    if ( empty(delStudent) ) { //Check if the queue is empty. If it is, return.
        printf("This queue is empty.\n");
        return;
    }

    printf("\nServing '%s %s' from the front of the queue\n", delStudent->firstName, delStudent->lastName);
    nextStudent = delStudent->nextAddr; //Set the nextStudent to the address of the listed student that will be deleted
    *queueOut = nextStudent; //Set the value of queueOut to the next address listed.
    if (nextStudent == NULL) *queueIn = nextStudent; //If there are no more nodes remaining, set the queueIn pointer to NULL
    //Free memory for the first struct
    free(delStudent);

}

void emptyQueue(struct PurdueStudent **queueIn, struct PurdueStudent **queueOut) {
    //Traverse the linked list. After finding the next address, clear the memory space for the current node and move on.
    struct PurdueStudent *nextStudent, *curStudent;
    curStudent = *queueOut;
    if ( empty(*queueOut) ) { //Check if the queue is empty. If it is, return.
        printf("This queue is already empty.\n");
        return;
    }
    *queueOut = NULL; //Set the top fo queue pointer to NULL
    *queueIn = NULL; //Set the bottom of queue pointer to NULL
    while (curStudent != NULL) { //Checking to see if the address is NULL or not
        printf("\nFreeing '%s %s' from the front of the queue\n", curStudent->firstName, curStudent->lastName);
        nextStudent = curStudent->nextAddr; //Set to the next address
        free(curStudent);
        curStudent = nextStudent;
    }
}
void userInitialization() {
    char input[MAXNAME]; //Define this for the user input within the cases
    char firstName[MAXNAME];
    char lastName[MAXNAME];
    long PUID;
    int age;
    printf("\nPlease enter a First Name: ");
    scanf("%s", input);
    strcpy(firstName, input);

    printf("\nPlease enter a Last Name: ");
    scanf("%s", input);
    strcpy(lastName, input);

    printf("\nPlease enter a PUID: ");
    scanf("%s", input);
    PUID = strtol(input, NULL, 10);

    printf("\nPlease enter an age: ");
    scanf("%s", input);
    age = (int) strtol(input, NULL, 10);

    //Add to the stack
    pushStack(&stackStruct, firstName, lastName, PUID, age);

    //Add to a queue
    enqueue(&queueStruct_in, &queueStruct_out, firstName, lastName, PUID, age);
}

int main ()
{
    int done = 0;

    //Method of designating current top stack structure
    //Initialize the stack
    stackStruct = createListNoNodes(); //Create the pointer for the stack

    //Initialize the queue
    queueStruct_in = createListNoNodes(); //Create the in pointer for the queue
    queueStruct_out = createListNoNodes(); //Create the out pointer for the queue

    // Ask to user to enter the initial values
    userInitialization();
    do {
        printf("\nWelcome to the Stack & Queue System - By Matthew Fisher");
        printf("\n1.  Pop - Delete a node from the stack");
        printf("\n2.  Push - Add a node to the stack");
        printf("\n3.  Empty Stack - Remove all nodes from the stack");
        printf("\n4.  Print Stack - Print the nodes of the stack");
        printf("\n5.  Enqueue - Add a node to the queue");
        printf("\n6.  Dequeue - Delete a node from the queue");
        printf("\n7.  Empty Queue - Remove all the nodes from the queue");
        printf("\n8.  Print Queue - Print all nodes of the queue");
        printf("\n9.  Exit the program.");
        char userInput[2];
        printf("\nSelection: ");
        scanf("%s", userInput);
        long userSelection = strtol(userInput, NULL, 10);

        //Define vars for use within the switch statement
        char input[MAXNAME]; //Define this for the user input within the cases
        char firstName[MAXNAME];
        char lastName[MAXNAME];
        long PUID;
        int age;
        switch(userSelection) { //Switch statement to run the code for the specific user selection
            case 1:
                popStack(&stackStruct);
                printf("Press Any Key to Continue\n");
                getchar(); //Doubled because C is stupid sometimes and won't stop without it. idk, something to do with a leftover return
                getchar();
                break;
            case 2:
                printf("\nPlease enter a First Name: ");
                scanf("%s", input);
                strcpy(firstName, input);

                printf("\nPlease enter a Last Name: ");
                scanf("%s", input);
                strcpy(lastName, input);

                printf("\nPlease enter a PUID: ");
                scanf("%s", input);
                PUID = strtol(input, NULL, 10);

                printf("\nPlease enter an age: ");
                scanf("%s", input);
                age = (int) strtol(input, NULL, 10);

                pushStack(&stackStruct, firstName, lastName, PUID, age);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 3:
                emptyStack(&stackStruct);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 4:
                printStack(stackStruct);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 5:
                printf("\nPlease enter a First Name: ");
                scanf("%s", input);
                strcpy(firstName, input);

                printf("\nPlease enter a Last Name: ");
                scanf("%s", input);
                strcpy(lastName, input);

                printf("\nPlease enter a PUID: ");
                scanf("%s", input);
                PUID = strtol(input, NULL, 10);

                printf("\nPlease enter an age: ");
                scanf("%s", input);
                age = (int) strtol(input, NULL, 10);

                enqueue(&queueStruct_in, &queueStruct_out, firstName, lastName, PUID, age);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 6:
                dequeue(&queueStruct_out, &queueStruct_in);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 7:
                emptyQueue(&queueStruct_in, &queueStruct_out);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 8:
                printStack(queueStruct_out);
                printf("Press Any Key to Continue\n");
                getchar();
                getchar();
                break;
            case 9:
                done = 1; //Exit the program
                break;
            default:
                printf("\nPlease enter a valid option.\n");
        }
    } while (done == 0);
    //And we're done!
}

