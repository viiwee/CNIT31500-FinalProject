//
// Created by Matthew on 4/25/2021.
//

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

struct url_data {
    size_t size;
    char* data;
};

struct TicketStruct {
    time_t dateTime;
    char *name;
    char *email;
    char *description;
    char *roomNumber;
    char *machineID;
    int priority;
    struct TicketStruct *kendallGetYourAssInHere;
};

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

int importData(char fileName[], struct TicketStruct **curStruct) {
    FILE *dbFile;
    struct TicketStruct temp;
    dbFile = fopen(fileName, "r");
    if (dbFile== NULL) {
        printf("\nError opening files.\n");
        return 1;
    }
    while(fread(&temp, sizeof(struct TicketStruct), 1, dbFile)) {
        printf("desc: %s, name: %s\n", temp.description, temp.name);
    }
    fclose(dbFile);
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
        //Update struct pointer if need be
        //Create new allocated struct and assign val
        //Add to end
        *curStruct = (*curStruct)->kendallGetYourAssInHere; //Set to the next address
    }
    fclose(dbFile);
    return 0;
}

time_t curTime() {
    //return time();
}
int main() {
    struct TicketStruct *activeTickets = createListPointer(); //Initialize this variable for later use.
    struct TicketStruct *inactiveTickets = createListPointer(); //Initialize this variable for later use.
    struct TicketStruct *ticket1;
    ticket1->description = "testing";
    ticket1->name = "Matt";
    struct TicketStruct *ticket2;
    ticket2->description = "Testing2";
    ticket2->name = "Alex";

    ticket1->kendallGetYourAssInHere = ticket2;
    activeTickets=ticket1;
    exportData("test.dat", &activeTickets);
    importData("test.dat", &activeTickets);
    exit(0);
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}