/*
        iaed-23 - Gabriel Silva - ist1102637 - project2
    File: system1.c
    Description: dynamic program equivalent of project1:
                    -> management system of buses, stops and routes
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "system.h"



/* - - - - - - - - - - reading functions - - - - - - - - - - */

/* reads till the end of line */
void reads()  {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}

/* reads a word from input and indicates if next character is '\n' */
int readWord(char word[], int *len) {
    char c = getchar();
    int i = 0;
    while (c == ' ' || c == '\t')
        c = getchar();

    if (c == '"') {
        c = getchar();
        while (c != '"') {
            word[i++] = c;
            c = getchar();
            (*len)++;
        }
        word[i] = '\0';
    }
    else {
        while (c != ' ' && c != '\t' && c != '\n') {
            word[i++] = c;
            c = getchar();
            (*len)++;
        }
        word[i] = '\0';
    }
    return (c == '\n');
}


/* - - - - - - - - - - general auxiliary function - - - - - - - - - - */

/* checks if the name of a stop or a bus already exists 
if so, returns the stop or bus ID, else returns FALSE */
int nameChecker(char str[], Stop_Node **sHead, Bus_Node **bHead) {
    if (sHead != NULL) {
        Stop_Node *aux = *sHead;
        while (aux != NULL) {
            if (strcmp((*aux).stop.name, str) == 0)
                return (*aux).stop.id;
            aux = (*aux).next;
        }
    }
    else {
        Bus_Node *aux = *bHead;
        while (aux != NULL) {
            if (strcmp((*aux).bus.name, str) == 0)
                return (*aux).bus.id;
            aux = (*aux).next;
        }
    }
    return FALSE;
}


/* - - - - - - - - - - linked list functions - - - - - - - - - - */

/* creates a Stop node and inserts it at the END of linked list */
void insertStop(char name[], int len, int id, double latit, double longit,
                    Stop_Node **sH, Stop_Node **sT) {
    char *name_pointer = (char *)malloc(sizeof(char) * (len + 1));
    Stop_Node *aux = (Stop_Node *)malloc(sizeof(Stop_Node));

    strcpy(name_pointer, name);

    (*aux).stop.name = name_pointer;
    (*aux).stop.id = id;
    (*aux).stop.latitude = latit;
    (*aux).stop.longitude = longit;
    (*aux).stop.number_of_buses = 0;

    (*aux).next = NULL;

    if (*sH == NULL) { /* if linked list is empty */
        *sH = aux;
        *sT = aux;
    }
    else {
        (**sT).next = aux;
        *sT = aux;
    }
}

/* creates a Bus node and inserts it at the END of linked list */
void insertBus(char name[], int len, int id, Bus_Node **bH, Bus_Node **bT) {
    char *name_pointer = (char *)malloc(sizeof(char) * (len + 1));
    Bus_Node *aux = (Bus_Node *)malloc(sizeof(Bus_Node));

    strcpy(name_pointer, name);

    (*aux).bus.name = name_pointer;
    (*aux).bus.id = id;
    (*aux).bus.totalStops = 0;
    (*aux).bus.totalCost = 0.0;
    (*aux).bus.totalDuration = 0.0;

    (*aux).next = NULL;

    if (*bH == NULL) { /* if linked list is empty */
        *bH = aux;
        *bT = aux;
    }
    else {
        (**bT).next = aux;
        *bT = aux;
    }
}

/* creates a Link node and inserts it at the END of linked list */
void insertLinkEnd(int busID, int stopID, int isOrigin, int isDestination,
                    double c, double d, Link_Node **lH, Link_Node **lT) {
    Link_Node *aux = (Link_Node *)malloc(sizeof(Link_Node));

    (*aux).link.busID = busID;
    (*aux).link.stopID = stopID;
    (*aux).link.isOrigin = isOrigin;
    (*aux).link.isDestination = isDestination;
    (*aux).link.cost = c;
    (*aux).link.duration = d;

    (*aux).next = NULL;
    (*aux).previous = *lT;

    if (*lH == NULL) {
        *lH = aux;
        *lT = aux;
    }
    else {
        (**lT).next = aux;
        *lT = aux;
    }
}

/* creates a Link node and inserts it at the BEGINNING of linked list */
void insertLinkBeginning(int busID, int stopID, Link_Node **lH) {
    Link_Node *aux = (Link_Node *)malloc(sizeof(Link_Node));

    (*aux).link.busID = busID;
    (*aux).link.stopID = stopID;
    (*aux).link.cost = 0.0;
    (*aux).link.duration = 0.0;
    (*aux).link.isOrigin = TRUE;
    (*aux).link.isDestination = FALSE;

    (*aux).next = *lH;
    (*aux).previous = NULL;

    if (*lH != NULL)
        (**lH).previous = aux;

    *lH = aux;
}

/* creates an Int node and inserts it at the BEGINNING of linked list */
void insertIntersection(char bus[], int len,
                            int stopID, int busID, Int_Node **iH) {
    char *name_pointer = (char *)malloc(sizeof(char) * (len + 1));
    Int_Node *aux = (Int_Node *)malloc(sizeof(Int_Node));

    strcpy(name_pointer, bus);

    (*aux).intersection.bus_name = name_pointer;
    (*aux).intersection.stopID = stopID;
    (*aux).intersection.busID = busID;

    (*aux).next = *iH;
    *iH = aux;
}


/* - - - - - - - - - - stop auxiliary functions - - - - - - - - - - */

void printAllStops(Stop_Node **sH) {
    Stop_Node *aux = *sH;
    while (aux != NULL) {
        printf("%s: %16.12f %16.12f %d\n", (*aux).stop.name,
                (*aux).stop.latitude, (*aux).stop.longitude,
                (*aux).stop.number_of_buses);
        aux = (*aux).next;
    }
}

void printOneStop(int id, Stop_Node **sH) {
    Stop_Node *aux = *sH;
    while (aux != NULL) {
        if ((*aux).stop.id == id) {
            printf("%16.12f %16.12f\n",
                    (*aux).stop.latitude, (*aux).stop.longitude);
            break;
        }
        aux = (*aux).next;
    }
}


/* - - - - - - - - - - bus auxiliary functions - - - - - - - - - - */

/* gets the name of a specific stop */
char *getName(int stopID, Stop_Node **sH) {
    Stop_Node *aux = *sH;
    while (aux != NULL) {
        if ((*aux).stop.id == stopID)
            return (*aux).stop.name;
        aux = (*aux).next;
    }
    return NULL;
}

/* checks if the string is valid as the 'inverso' parameter */
int inverseChecker(char string[], int len) {
    if (len == 3) 
        return strcmp(string, "inv");
    else if (len == 4)
        return strcmp(string, "inve");
    else if (len == 5)
        return strcmp(string, "inver");
    else if (len == 6)
        return strcmp(string, "invers");
    else if (len == 7)
        return strcmp(string, "inverso");
    else
        return -1;
}

void printAllBuses(Stop_Node **sH, Bus_Node **bH) {
    Bus_Node *aux = *bH;
    while (aux != NULL) {
        if ((*aux).bus.totalStops == 0)
            printf("%s %d %.2f %.2f\n", (*aux).bus.name, 0, 0.0, 0.0);
        else
            printf("%s %s %s %d %.2f %.2f\n", (*aux).bus.name,
                    getName((*aux).bus.originID, sH),
                    getName((*aux).bus.destinationID, sH),
                    (*aux).bus.totalStops, (*aux).bus.totalCost,
                    (*aux).bus.totalDuration);
        aux = (*aux).next;
    }
}

/* prints the normal route of a specific bus.*/
void printOneBus(int busID, Stop_Node **sH, Link_Node **lH) {
    Link_Node *aux = *lH;
    while (aux != NULL) {
        if ((*aux).link.busID == busID) {
            if ((*aux).link.isDestination) {
                printf("%s\n", getName((*aux).link.stopID, sH));
                break;
            }
            printf("%s, ", getName((*aux).link.stopID, sH));
        }
        aux = (*aux).next;
    }
}

/* prints the route, in reversed order, of a specific bus */
void printOneBusReverse(int busID, Stop_Node **sH, Link_Node **lT) {
    Link_Node *aux = *lT;
    while (aux != NULL) {
        if ((*aux).link.busID == busID) {
            if ((*aux).link.isOrigin) {
                printf("%s\n", getName((*aux).link.stopID, sH));
                break;
            }
            printf("%s, ", getName((*aux).link.stopID, sH));
        }
        aux = (*aux).previous;
    }
}


/* - - - - - - - - - - link auxiliary functions - - - - - - - - - - */

/* returns the total number of stops of a specific bus */
int firstLink(int busID, Bus_Node **bH) {
    Bus_Node *aux = *bH;
    while (aux != NULL) {
        if ((*aux).bus.id == busID)
            return (*aux).bus.totalStops;
        aux = (*aux).next;
    }
    return TRUE;
}

/* checks if new link makes the route circular */ 
int circularRoute(int busID, int newOrigin, int newDest, Bus_Node **bH) {
    Bus_Node *aux = *bH;
    while (aux != NULL) {
        if ((*aux).bus.id == busID) 
            return ((*aux).bus.destinationID == newOrigin &&
                        (*aux).bus.originID == newDest);
        aux = (*aux).next;
    }
    return FALSE;
}

/* update bus information according to new link */
void updateBusAfterNewLink(int bus, int origin, int dest, int updateInfo,
                            double c, double d, Bus_Node **bH) {
    Bus_Node *aux = *bH;
    while (aux != NULL) {
        if ((*aux).bus.id == bus) {

            if ((*aux).bus.totalStops == 0) 
                (*aux).bus.totalStops++;
            
            (*aux).bus.totalStops++;
            (*aux).bus.totalCost += c;
            (*aux).bus.totalDuration += d;

            if (updateInfo == ORIGIN_DESTINATION) {
                (*aux).bus.originID = origin;
                (*aux).bus.destinationID = dest;
            }
            else if (updateInfo == ORIGIN)
                (*aux).bus.originID = origin;
            else if (updateInfo == DESTINATION)
                (*aux).bus.destinationID = dest;
            break;
        }
        aux = (*aux).next;
    }
}

/* update stop information according to new link */
void updateStopAfterNewLink(int stop, Stop_Node **sH) {
    Stop_Node *aux = *sH;
    while (aux != NULL) {
        if ((*aux).stop.id == stop) {
            (*aux).stop.number_of_buses++;
            break;
        }
        aux = (*aux).next;
    }
}

/* checks if there is already an intersection of stop A with bus C */
int alreadyHasIntersection(int busID, int stopID, Int_Node **iH) {
    Int_Node *aux = *iH;
    while (aux != NULL) {
        if ((*aux).intersection.stopID == stopID &&
                (*aux).intersection.busID == busID)
            return TRUE;
        aux = (*aux).next;
    }
    return FALSE;
}

/* checks if the new origin and new destination make a valid link or not */
int notValidLink(int busID, int newOrigin, int newDestination, Bus_Node **bH) {
    Bus_Node *aux = *bH;
    while (aux != NULL) {
        if ((*aux).bus.id == busID) {
            if ((*aux).bus.totalStops == 0)
                return FALSE;
            else if ((*aux).bus.destinationID == newOrigin ||
                        (*aux).bus.originID == newDestination)
                return FALSE;
            else
                return TRUE;
        }
        aux = (*aux).next;
    }
    return TRUE;
}

/* checks wether to insert at the end or beginning of linked list */
void carefullyInsertLink(char bus_name[], int len, int busID, int newOrigin,
                        int newDestination, double c, double d,
                        Stop_Node **sH, Bus_Node **bH, Link_Node **lH,
                        Link_Node **lT, Int_Node **iH) {
    int state = 1;
    Link_Node *aux = *lH;
    while (aux != NULL) {
        if ((*aux).link.busID == busID) {
            if ((*aux).link.isOrigin && (*aux).link.stopID == newDestination) {
                if (circularRoute(busID, newOrigin, newDestination, bH))
                    state = 0;
                else {
                    (*aux).link.cost = c;
                    (*aux).link.duration = d;
                    (*aux).link.isOrigin = FALSE;
                    memoryChecker(sizeof(Link_Node), 0, sH, bH, lH, iH);
                    insertLinkBeginning(busID, newOrigin, lH);
                    updateBusAfterNewLink(busID, newOrigin,
                                            FALSE, ORIGIN, c, d, bH);
                    if (!alreadyHasIntersection(busID, newOrigin, iH)) {
                        updateStopAfterNewLink(newOrigin, sH);
                        memoryChecker((sizeof(char) * (len + 1)),
                                        sizeof(Int_Node), sH, bH, lH, iH);
                        insertIntersection(bus_name, len,
                                                newOrigin, busID, iH);
                    }
                    break;
                }
            }
            else if ((*aux).link.isDestination) {
                (*aux).link.isDestination = FALSE;
                memoryChecker(sizeof(Link_Node), 0, sH, bH, lH, iH);
                insertLinkEnd(busID, newDestination,
                                FALSE, TRUE, c, d, lH, lT);
                updateBusAfterNewLink(busID, FALSE, newDestination,
                                        DESTINATION, c, d, bH);
                if (state) { /* if its not circular */
                    if (!alreadyHasIntersection(busID, newDestination, iH)) {
                        updateStopAfterNewLink(newDestination, sH);
                        memoryChecker((sizeof(char) * (len + 1)),
                                        sizeof(Int_Node), sH, bH, lH, iH);
                        insertIntersection(bus_name,  len,
                                            newDestination, busID, iH);
                    }
                }
                break;
            }
        }
        aux = (*aux).next;
    }
}


/* - - - - - - - - - - intersection auxiliary functions - - - - - - - - - - */

/* swaps 2 nodes information */
void swap(Int_Node **node1, Int_Node **node2) {
    char *tempName = (**node1).intersection.bus_name;
    int tempID = (**node1).intersection.stopID;

    (**node1).intersection.bus_name = (**node2).intersection.bus_name;
    (**node1).intersection.stopID = (**node2).intersection.stopID;

    (**node2).intersection.bus_name = tempName;
    (**node2).intersection.stopID = tempID;
}

/* sorts a linked list alphabetically */
void bubbleSort(Int_Node **iH) {
    int done;

    Int_Node *aux, *last = NULL;

    do {
        done = 0;
        aux = *iH;

        while ((*aux).next != last) {
            if (strcmp((*aux).intersection.bus_name,
                    (*(*aux).next).intersection.bus_name) > 0) {
                swap(&aux, &(*aux).next);
                done = 1;
            }
            aux = (*aux).next;
        }
        last = aux;
    } while (done);
}


/* - - - - - - - - - - system functions - - - - - - - - - - */

void stopFunction(Stop_Node **sH, Stop_Node **sT,
                    Bus_Node **bH, Link_Node **lH, Int_Node **iH) {
    char stop_name[MAX_LINE_SIZE];
    int state, stopID, name_lenght = 0;
    static int id = 1;
    double latitude, longitude;
    state = readWord(stop_name, &name_lenght);
    if (name_lenght == 0) {
        printAllStops(sH);
    }
    else {
        stopID = nameChecker(stop_name, sH, NULL);
        if (state) {
            if (stopID)
                printOneStop(stopID, sH);
            else
                printf("%s: %s", stop_name, NO_SUCH_STOP_ERROR);
        }
        else {
            if (stopID)
                printf("%s: %s", stop_name, STOP_ALREADY_EXISTS_ERROR);
            else {
                scanf("%lf %lf", &latitude, &longitude);
                memoryChecker((sizeof(char) * (name_lenght + 1)),
                                sizeof(Stop_Node), sH, bH, lH, iH);
                insertStop(stop_name, name_lenght, id,
                                latitude, longitude, sH, sT);
                id++;
                reads();
            }
        }
    }
}

void busFunction(Stop_Node **sH, Bus_Node **bH, Bus_Node **bT,
                    Link_Node **lH, Link_Node **lT, Int_Node **iH) {
    char bus_name[MAX_LINE_SIZE];
    int state, busID, name_lenght = 0;
    static int id = 1;
    state = readWord(bus_name, &name_lenght);
    if (name_lenght == 0) 
        printAllBuses(sH, bH);

    else {
        busID = nameChecker(bus_name, NULL, bH);
        if (state) {
            if (busID) 
                printOneBus(busID, sH, lH);
            else {
                memoryChecker((sizeof(char) * (name_lenght + 1)),
                                sizeof(Bus_Node), sH, bH, lH, iH);
                insertBus(bus_name, name_lenght, id, bH, bT);
                id++;
            }
        }
        else {
            char str[MAX_INV_SIZE];
            name_lenght = 0;
            readWord(str, &name_lenght);

            if (inverseChecker(str, name_lenght) == 0)
                printOneBusReverse(busID, sH, lT);
            else
                printf(INCORRECT_SORT_OPTION_ERROR);
        }
    }
}

void linkFunction(Stop_Node **sH, Bus_Node **bH,
                    Link_Node **lH,Link_Node **lT, Int_Node **iH) {
    char bus_name[MAX_LINE_SIZE];
    char origin_name[MAX_LINE_SIZE], destination_name[MAX_LINE_SIZE];
    int busID, originID, destinationID, busLen = 0, otherLen = 0;
    double cost, duration;
    readWord(bus_name, &busLen);
    readWord(origin_name, &otherLen);
    readWord(destination_name, &otherLen);
    scanf("%lf %lf", &cost, &duration);
    reads();
    busID = nameChecker(bus_name, NULL, bH);
    originID = nameChecker(origin_name, sH, NULL);
    destinationID = nameChecker(destination_name, sH, NULL);
    if (!busID)
        printf("%s: %s", bus_name, NO_SUCH_LINE_ERROR);
    else if (!originID)
        printf("%s: %s", origin_name, NO_SUCH_STOP_ERROR);
    else if (!destinationID)
        printf("%s: %s", destination_name, NO_SUCH_STOP_ERROR);
    else if (notValidLink(busID, originID, destinationID, bH))
        printf(INVALID_LINK_ERROR);
    else if (cost < 0 || duration < 0)
        printf(NEGATIVE_COST_DURATION_ERROR);
    else {
        if (firstLink(busID, bH) == 0) { /* if it is the first link of bus */
            memoryChecker(sizeof(Link_Node), sizeof(Link_Node),
                            sH, bH, lH, iH);
            insertLinkEnd(busID, originID, TRUE, FALSE, 0.0, 0.0, lH, lT);
            insertLinkEnd(busID, destinationID, FALSE, TRUE,
                            cost, duration, lH, lT);
            updateBusAfterNewLink(busID, originID, destinationID,
                            ORIGIN_DESTINATION, cost, duration, bH);
            updateStopAfterNewLink(originID, sH);
            memoryChecker((sizeof(char) * (busLen + 1)), sizeof(Int_Node),
                            sH, bH, lH, iH);
            insertIntersection(bus_name, busLen, originID, busID, iH);
            if (originID != destinationID) {
                updateStopAfterNewLink(destinationID, sH);
                memoryChecker((sizeof(char) * (busLen + 1)), sizeof(Int_Node),
                                sH, bH, lH, iH);
                insertIntersection(bus_name, busLen, destinationID, busID, iH);
            }
        }
        else 
            carefullyInsertLink(bus_name, busLen, busID, originID,
                            destinationID, cost, duration, sH, bH, lH, lT, iH);
    }
}

void intersectionFunction(Stop_Node **sH, Int_Node **iH) {
    Stop_Node *stopAux = *sH;
    bubbleSort(iH);

    while (stopAux != NULL) {

        if ((*stopAux).stop.number_of_buses > 1) {
            Int_Node *intAux = *iH;
            int n = 1;

            printf("%s %d: ", (*stopAux).stop.name,
                    (*stopAux).stop.number_of_buses);

            while (intAux != NULL) {
                if ((*intAux).intersection.stopID == (*stopAux).stop.id) {

                    if (n == (*stopAux).stop.number_of_buses) {
                        printf("%s\n", (*intAux).intersection.bus_name);
                        break;
                    }
                    else 
                        printf("%s ", (*intAux).intersection.bus_name);
                    n++;
                }
                intAux = (*intAux).next;
            }
        }
        stopAux = (*stopAux).next;
    }
}