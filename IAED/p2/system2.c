/*
        iaed-23 - Gabriel Silva - ist1102637 - project2
    File: system2.c
    Description: extension to project1 allowing the system to:
                    -> remove Buses
                    -> remove Stops
                    -> free all allocated memory of the system
*/

#include <stdio.h>
#include <stdlib.h>
#include "system.h"



/* - - - - - - - - - -  memory functions - - - - - - - - - - */

/* free all allcated memory */
void clearData(Stop_Node **sH, Bus_Node **bH, Link_Node **lH, Int_Node **iH) {
    Stop_Node *auxStop;
    Bus_Node *auxBus;
    Link_Node *auxLink;
    Int_Node *auxInt;
    while (*sH != NULL) {
        auxStop = *sH;
        *sH = (**sH).next;
        free((*auxStop).stop.name);
        free(auxStop);
    }
    while (*bH != NULL) {
        auxBus = *bH;
        *bH = (**bH).next;
        free((*auxBus).bus.name);
        free(auxBus);
    }
    while (*lH != NULL) {
        auxLink = *lH;
        *lH = (**lH).next;
        free(auxLink);
    }
    while (*iH != NULL) {
        auxInt = *iH;
        *iH = (**iH).next;
        free((*auxInt).intersection.bus_name);
        free(auxInt);
    }
}

/* checks if there is still memory to allocate, if not the program ends */
void memoryChecker(size_t n1, size_t n2, Stop_Node **sH,
                    Bus_Node **bH, Link_Node **lH, Int_Node **iH) {
    void *pointer1 = malloc(n1);
    void *pointer2;

    if (n2 != 0) {
        pointer2 = malloc(n2);
        if (pointer1 == NULL || pointer2 == NULL) {
            clearData(sH, bH, lH, iH);
            printf(NO_MEMORY_ERROR);
            exit(EXIT_FAILURE);
        }
        else {
            free(pointer1);
            free(pointer2);
        }
    }
    else {
        if (pointer1 == NULL) {
            clearData(sH, bH, lH, iH);
            printf(NO_MEMORY_ERROR);
            exit(EXIT_FAILURE);
        }
        else
            free(pointer1);
    }
}


/* - - - - - - - - - - linked list functions - - - - - - - - - - */

/* removes one specific bus from the linked list */
void removeBus(int busID, Bus_Node **bH) {
    Bus_Node *current = *bH, *previous = NULL;

    while (current != NULL) {
        if ((*current).bus.id == busID) {
            if (previous == NULL) 
                *bH = (*current).next;
            else
                (*previous).next = (*current).next;
            free((*current).bus.name);
            free(current);
            break;
        }
        previous = current;
        current = (*current).next;
    }
}

/* removes one specific stop from the linked list */
void removeStop(int stopID, Stop_Node **sH) {
    Stop_Node *current = *sH, *previous = NULL;
    while (current != NULL) {
        if ((*current).stop.id == stopID) {
            if (previous == NULL)
                *sH = (*current).next;
            else
                (*previous).next = (*current).next;
            free((*current).stop.name);
            free(current);
            break;
        }
        previous = current;
        current = (*current).next;
    }
}

/* remove all links associated with a specific bus */
void removeLinksOfBus(int busID, Link_Node **lH) {
    Link_Node *current = *lH, *temp = NULL;
    while (current != NULL) {
        if ((*current).link.busID == busID) {
            
            if (current == *lH) 
                *lH = (*current).next;
            else
                (*(*current).previous).next = (*current).next;


            if ((*current).next != NULL) 
                (*(*current).next).previous = (*current).previous;
            
            temp = current;
            current = (*current).next;
            free(temp);
        }
        else
            current = (*current).next;
    }
}

/* removes all intersections associated with a specific bus */
void removeIntersectionsOfBus(int busID, Int_Node **iH) {
    Int_Node *current = *iH, *previous = NULL;
    while (current != NULL) {
        if ((*current).intersection.busID == busID) {
            if (previous == NULL) { 
                *iH = (*current).next;
                free((*current).intersection.bus_name);
                free(current);
                current = *iH;
            }
            else {
                (*previous).next = (*current).next;
                free((*current).intersection.bus_name);
                free(current);
                current = (*previous).next;
            }
        }
        else {
            previous = current;
            current = (*current).next;
        }
    }
}

/* removes all intersections associated with a specific stop */
void removeIntersectionsOfStop(int stopID, Int_Node **iH) {
    Int_Node *current = *iH, *previous = NULL;
    while (current != NULL) {
        if ((*current).intersection.stopID == stopID) {
            if (previous == NULL) {
                *iH = (*current).next;
                free((*current).intersection.bus_name);
                free(current);
                current = *iH;
            }
            else {
                (*previous).next = (*current).next;
                free((*current).intersection.bus_name);
                free(current);
                current = (*previous).next;
            }
        }
        else {
            previous = current;
            current = (*current).next;
        }
    }
}


/* - - - - - - - - - - remove bus auxiliary functions - - - - - - - - - - */

/* update all stops 'number_of_buses' that had the specific bus */
void updateStopAfterBusRemoval(int busID, Stop_Node **sH, Link_Node **lH) {
    Stop_Node *stopAux = *sH;
    while (stopAux != NULL) {
        Link_Node *linkAux = *lH;
        while (linkAux != NULL) {
            if ((*linkAux).link.stopID == (*stopAux).stop.id &&
                    (*linkAux).link.busID == busID) {
                (*stopAux).stop.number_of_buses--;
                break;
            }
            linkAux = (*linkAux).next;
        }
        stopAux = (*stopAux).next;
    }
}


/* - - - - - - - - - - remove stop auxiliary functions - - - - - - - - - - */

/* updates a specific bus information after removing links */
void updateBusAfterStopRemoval(int busID, int updateInfo, int originOrDest,
                                int stopID, double c, double d, Bus_Node **bH){
    Bus_Node *aux = *bH;
    while (aux != NULL) {
        if ((*aux).bus.id == busID) {
            if (updateInfo == RESET) {
                (*aux).bus.totalStops = 0;
                (*aux).bus.totalCost = 0.0;
                (*aux).bus.totalDuration = 0.0;
            }
            else if (updateInfo == SUBTRACT) {
                (*aux).bus.totalStops--;
                (*aux).bus.totalCost -= c;
                (*aux).bus.totalDuration -= d;

                if (originOrDest == ORIGIN)
                    (*aux).bus.originID = stopID;
                else if (originOrDest == DESTINATION)
                    (*aux).bus.destinationID = stopID;
            }
            else
                (*aux).bus.totalStops--;
            break;
        }
        aux = (*aux).next;
    }
}

/* frees allocated memory of give node and updates pointer */
void removeCurrent(Link_Node **lNode, Link_Node **lH) {
    Link_Node *temp;
    if (*lNode == *lH)
        *lH = (**lNode).next;
    else
        (*(**lNode).previous).next = (**lNode).next;
    
    if ((**lNode).next != NULL)
        (*(**lNode).next).previous = (**lNode).previous;
    
    temp = *lNode;
    *lNode = (**lNode).next;
    free(temp);
}

/* removes all links associated with a specific stop, updating its buses */
void carefullyRemoveLinksOfStop(int stopID, Bus_Node **bH, Link_Node **lH) {
    int state = FALSE;
    Link_Node *current = *lH;
    while (current != NULL) {
        if (state) {
            removeCurrent(&current, lH);
            state = FALSE;
        }
        /*  */
        else if ((*current).link.isOrigin &&
                    (*(*current).next).link.isDestination &&
                    ((*current).link.stopID == stopID ||
                    (*(*current).next).link.stopID == stopID)) {
            updateBusAfterStopRemoval((*current).link.busID, RESET,
                                        0, 0, 0.0, 0.0, bH);
            removeCurrent(&current, lH);
            state = TRUE;
        }
        else if ((*current).link.stopID == stopID && (*current).link.isOrigin){
            Link_Node *aux = (*current).next;
            while (aux != NULL) {
                if ((*aux).link.busID == (*current).link.busID) {
                    (*aux).link.isOrigin = TRUE;
                    updateBusAfterStopRemoval((*current).link.busID, SUBTRACT,
                                                ORIGIN, (*aux).link.stopID,
                                                (*aux).link.cost, 
                                                (*aux).link.duration, bH);
                    (*aux).link.cost = 0.0;
                    (*aux).link.duration = 0.0;
                    break;
                }
                aux = (*aux).next;
            }
            removeCurrent(&current, lH);
        }
        else if ((*current).link.stopID == stopID &&
                    (*current).link.isDestination) {
            Link_Node *aux = (*current).previous;
            while (aux != NULL) {
                if ((*aux).link.busID == (*current).link.busID) {
                    (*aux).link.isDestination = TRUE;
                    updateBusAfterStopRemoval((*current).link.busID, SUBTRACT,
                                            DESTINATION, (*aux).link.stopID, 
                                            (*current).link.cost,
                                            (*current).link.duration, bH);
                    break;
                }
                aux = (*aux).previous;
            }
            removeCurrent(&current, lH);
        }
        else if ((*current).link.stopID == stopID) {
            Link_Node *aux = (*current).next;
            updateBusAfterStopRemoval((*current).link.busID, FALSE, FALSE,
                                        FALSE, 0.0, 0.0, bH);
            while (aux != NULL) {
                if ((*aux).link.busID == (*current).link.busID) {
                    (*aux).link.cost += (*current).link.cost;
                    (*aux).link.duration += (*current).link.duration;
                    break;
                }
                aux = (*aux).next;
            }
            removeCurrent(&current, lH);
        }
        else
            current = (*current).next;
    }
}


/* - - - - - - - - - - system functions - - - - - - - - - - */

void deleteBusFromSystem(Stop_Node **sH, Bus_Node **bH,
                            Link_Node **lH, Int_Node **iH) {
    char bus_name[MAX_LINE_SIZE];
    int len, busID;

    readWord(bus_name, &len);
    busID = nameChecker(bus_name, NULL, bH);

    if (busID) {
        updateStopAfterBusRemoval(busID, sH, lH);
        removeBus(busID, bH);
        removeLinksOfBus(busID, lH);
        removeIntersectionsOfBus(busID, iH);
    }
    else 
        printf("%s: %s", bus_name, NO_SUCH_LINE_ERROR);
}

void deleteStopFromSystem(Stop_Node **sH, Bus_Node **bH,
                            Link_Node **lH, Int_Node **iH) {
    char stop_name[MAX_LINE_SIZE];
    int len, stopID;

    readWord(stop_name, &len);
    stopID = nameChecker(stop_name, sH, NULL);

    if (stopID) {
        removeStop(stopID, sH);
        carefullyRemoveLinksOfStop(stopID,  bH, lH);
        removeIntersectionsOfStop(stopID, iH);
    }
    else
        printf("%s: %s", stop_name, NO_SUCH_STOP_ERROR);
}

/* reset system variables */
void resetVariables(Stop_Node **sT, Bus_Node **bT, Link_Node **lT) {
    *sT = NULL;
    *bT = NULL;
    *lT = NULL;
}