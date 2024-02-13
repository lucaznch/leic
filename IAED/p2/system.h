/*
        iaed-23 - Gabriel Silva - ist1102637 - project2
    File: system.h
    Description: constants, structs and prototypes used in the system
*/

#include <stddef.h>

#define MAX_LINE_SIZE 65535
#define MAX_INV_SIZE 8

#define ORIGIN_DESTINATION 10   /* update origin and destination of bus */
#define DESTINATION 20          /* update destination of bus */
#define ORIGIN 30               /* update origin of bus */

#define SUBTRACT 60    /* update totalStops, totalCost, totalDuration of bus */
#define RESET 50       /* reset totalStops, totalCost, totalDuration of bus */

#define INVALID_LINK_ERROR "link cannot be associated with bus line.\n"
#define NEGATIVE_COST_DURATION_ERROR "negative cost or duration.\n"
#define INCORRECT_SORT_OPTION_ERROR "incorrect sort option.\n"
#define STOP_ALREADY_EXISTS_ERROR "stop already exists.\n"
#define NO_SUCH_STOP_ERROR "no such stop.\n"
#define NO_SUCH_LINE_ERROR "no such line.\n"
#define NO_MEMORY_ERROR "No memory.\n"

#define TRUE 1
#define FALSE 0



/* - - - - - - - - - - data types - - - - - - - - - - */

typedef struct {
    char *name;
    int id, number_of_buses;
    double latitude, longitude;
} Stop;

typedef struct {
    char *name;
    int id, originID, destinationID; 
    int totalStops;
    double totalCost, totalDuration;
} Bus;

typedef struct {
    int busID, stopID; 
    int isOrigin, isDestination;
    double cost, duration;
} Link;

typedef struct {
    char *bus_name;
    int stopID, busID;
} Intersection;



/* - - - - - - - - - - linked lists - - - - - - - - - - */

typedef struct Bus_Node {
    Bus bus;
    struct Bus_Node *next;
} Bus_Node;


typedef struct Stop_Node {
    Stop stop;
    struct Stop_Node *next;
} Stop_Node;


typedef struct Link_Node {
    Link link;
    struct Link_Node *previous;
    struct Link_Node *next;
} Link_Node;


typedef struct Int_Node {
    Intersection intersection;
    struct Int_Node *next;
} Int_Node;



/* - - - - - - - - - - reading functions - - - - - - - - - - */

void reads(void);
int readWord(char string[], int *lenght);



/* - - - - - - - - - -  memory functions - - - - - - - - - - */

void clearData(Stop_Node **sH, Bus_Node **bH, Link_Node **lH, Int_Node **iH);
void memoryChecker(size_t n1, size_t n2, Stop_Node **sH, Bus_Node **bH,
                    Link_Node **lH, Int_Node **iH);



/* - - - - - - - - - - general auxiliary functions - - - - - - - - - - */

int nameChecker(char string[], Stop_Node **sH, Bus_Node **bH);



/* - - - - - - - - - - system functions - - - - - - - - - - */

void stopFunction(Stop_Node **sH, Stop_Node **sT, Bus_Node **bH,
                    Link_Node **lH, Int_Node **iH);

void busFunction(Stop_Node **sH, Bus_Node **bH, Bus_Node **bT, Link_Node **lH,
                    Link_Node **lT, Int_Node **iH);

void linkFunction(Stop_Node **sH, Bus_Node **bH, Link_Node **lH,
                    Link_Node **lT, Int_Node **iH);

void intersectionFunction(Stop_Node **sH, Int_Node **iH);

void deleteBusFromSystem(Stop_Node **sH, Bus_Node **bH,
                            Link_Node **lH, Int_Node **iH);

void deleteStopFromSystem(Stop_Node **sH, Bus_Node **bH,
                            Link_Node **lH, Int_Node **iH);

void resetVariables(Stop_Node **sT, Bus_Node **bT, Link_Node **lT);