#include <stdio.h>
#include <string.h>

#define MAX_NUM_BUSES 200
#define MAX_NUM_STOPS 10000
#define MAX_NUM_LINKS 30000
#define MAX_NAME_BUS 21
#define MAX_NAME_STOP 51
#define MAX_NAME_INVERSE 8
#define TRUE 1
#define FALSE 0


typedef struct
{
    char name[MAX_NAME_STOP];
    int buses_of_this_stop; /* number of buses that stop in Stop */
    int buses[MAX_NUM_BUSES]; /* store buses that stop in Stop */
    double latitude;
    double longitude;
} Stop;

typedef struct
{
    char name[MAX_NAME_BUS];
    int origin_stop_index;
    int destination_stop_index;
    int totalStops;
    int route[MAX_NUM_LINKS]; /* store stops defining the bus route */
    double totalCost;
    double totalDuration;
} Bus;


int number_of_buses = 0;
Bus allBuses[MAX_NUM_BUSES];

int number_of_stops = 0;
Stop allStops[MAX_NUM_STOPS];


/* reads till the end of line */
void reads()  {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}

/* reads next word from input */
int readWord(char word[]) {
    char c = getchar();
    int i = 0;
    while (c == ' ' || c == '\t')
        c = getchar();

    if (c == '"') {
        c = getchar();
        while (c != '"') {
            word[i++] = c;
            c = getchar();
        }
        word[i] = '\0';
    }
    else {
        while (c != ' ' && c != '\t' && c != '\n') {
            word[i++] = c;
            c = getchar();
        }
        word[i] = '\0';
    }
    return (c == '\n');
}

/* checks if a stop/bus name already exists */
int nameChecker(char str[], int is_stop_name) {
    int i;

    if (is_stop_name) {
        for (i = 0; i < number_of_stops; i++) {
            if (strcmp(allStops[i].name, str) == 0)
                return TRUE;
        }
        return FALSE;
    }
    else {
        for (i = 0; i < number_of_buses; i++) {
            if (strcmp(allBuses[i].name, str) == 0)
                return TRUE;
        }
        return FALSE;
    }
}

/* finds and returns the index of the stop/bus */
int finder(char str[], int is_stop_name) {
    int i;

    if (is_stop_name) {
        for (i = 0; i < number_of_stops; i++) {
            if (strcmp(allStops[i].name, str) == 0)
                return i;
        }
    }
    else {
        for (i = 0; number_of_buses; i++) {
            if (strcmp(allBuses[i].name, str) == 0)
                return i;
        }
    }
    return -1;
}

void stopFunction()  {
    char stop_name[MAX_NAME_STOP];
    int i, state;
    double latitude, longitude;
    state = readWord(stop_name);

    if (strlen(stop_name) == 0) {
        for (i = 0; i < number_of_stops; i++)
            printf("%s: %16.12f %16.12f %d\n", allStops[i].name,
            allStops[i].latitude, allStops[i].longitude,
            allStops[i].buses_of_this_stop);
    }
    else {
        if (state) { 
            if (nameChecker(stop_name, TRUE)) { 
                i = finder(stop_name, TRUE);
                printf("%16.12f %16.12f\n", allStops[i].latitude,
                allStops[i].longitude);
            }    
            else
                printf("%s: no such stop.\n", stop_name);
        }
        else {
            if (nameChecker(stop_name, TRUE)) {
                printf("%s: stop already exists.\n", stop_name);
                reads();
            }
            else {
                scanf("%lf %lf", &latitude, &longitude);
                reads();
                strcpy(allStops[number_of_stops].name, stop_name);
                allStops[number_of_stops].latitude = latitude;
                allStops[number_of_stops].longitude = longitude;
                allStops[number_of_stops].buses_of_this_stop = 0;
                number_of_stops++;
            }
        }
    }
}

/* prints all buses of the system */
void listAllBuses() {
    int i, j, k;
    for (i = 0; i < number_of_buses; i++) {
        j = allBuses[i].origin_stop_index;
        k = allBuses[i].destination_stop_index;
        if (allBuses[i].totalStops == 0)
            printf("%s %d %.2f %.2f\n", allBuses[i].name,
            allBuses[i].totalStops, allBuses[i].totalCost,
            allBuses[i].totalDuration);
        else
            printf("%s %s %s %d %.2f %.2f\n", allBuses[i].name,
            allStops[j].name, allStops[k].name, allBuses[i].totalStops,
            allBuses[i].totalCost, allBuses[i].totalDuration);
    }
}

/* prints the route of the bus, normally or inverted */
void showRouteOfBus(int bus_index, int inverted) {
    int i, j, n;
    n = allBuses[bus_index].totalStops;
    if (!inverted) {
        for (i = 0; i < n; i++) {
            j = allBuses[bus_index].route[i];
            if (i != n - 1)
                printf("%s, ", allStops[j].name);
            else
                printf("%s\n", allStops[j].name);
        }
    }
    else {
        for (i = n - 1; i >= 0; i--) {
            j = allBuses[bus_index].route[i]; /* get index of stop */
            if (i == 0)
                printf("%s\n", allStops[j].name);
            else
                printf("%s, ", allStops[j].name);
        }
    }
}

void busFunction() {
    char bus_name[MAX_NAME_BUS], inverse[MAX_NAME_INVERSE];
    int state; 
    state = readWord(bus_name);

    if (strlen(bus_name) == 0)
        listAllBuses();
    else {
        if (state) {
            if (!nameChecker(bus_name, FALSE)) {
                strcpy(allBuses[number_of_buses].name, bus_name);
                allBuses[number_of_buses].totalStops = 0;
                number_of_buses++;
            }
            else
                showRouteOfBus(finder(bus_name, FALSE), FALSE);
        }
        else {
            readWord(inverse);
            if (strcmp(inverse, "inverso") == 0 || strcmp(inverse, "inv") == 0
            || strcmp(inverse, "inve") == 0 || strcmp(inverse, "inver") == 0
            || strcmp(inverse, "invers") == 0) 
                showRouteOfBus(finder(bus_name, FALSE), TRUE);
            else 
                printf("incorrect sort option.\n");
        }
    }
}

/* update route of bus by adding one stop in the beginning or end of route */
void updateRoute(int bus, int oStop, int dStop, double c, double d) {
    int i, n, temp;

    /* if the bus destination matches the new origin of the link */
    if (allBuses[bus].destination_stop_index == oStop) { 
        i = allBuses[bus].totalStops; 
        allBuses[bus].route[i] = dStop; 
        allBuses[bus].destination_stop_index = dStop; 
        allBuses[bus].totalStops++; 
        allBuses[bus].totalCost += c; 
        allBuses[bus].totalDuration += d; 
    }
    else {
        /* shift all stops to the right one position */
        n = allBuses[bus].totalStops;
        temp = allBuses[bus].route[n - 1];
        for (i = n - 1; i > 0; i--) 
            allBuses[bus].route[i] = allBuses[bus].route[i - 1];
        allBuses[bus].route[0] = oStop; /* add stop in the beginning */
        allBuses[bus].route[n] = temp;

        allBuses[bus].origin_stop_index = oStop;
        allBuses[bus].totalStops++;
        allBuses[bus].totalCost += c;
        allBuses[bus].totalDuration += d;
    }
}

/* checks if bus is already in the list of buses that stop in Stop */
int busInStopList(int busIndex, int stopIndex) {
    int i;
    for (i = 0; i < allStops[stopIndex].buses_of_this_stop; i++) {
        if (allStops[stopIndex].buses[i] == busIndex)
            return TRUE;
    }
    return FALSE;
}

void addLink(char busName[], char oStop[], char dStop[], double c, double d) {
    int origin, destination, bus; /* indexes */
    int i;
    origin = finder(oStop, TRUE);
    destination = finder(dStop, TRUE);
    bus = finder(busName, FALSE);

    if (allBuses[bus].totalStops == 0) { /* if bus doesnt have route yet */
        allBuses[bus].route[0] = origin; 
        allBuses[bus].route[1] = destination;
        allBuses[bus].totalStops = 2; 
        allBuses[bus].origin_stop_index = origin; 
        allBuses[bus].destination_stop_index = destination;
        allBuses[bus].totalCost = c; 
        allBuses[bus].totalDuration = d;
    }
    else 
        updateRoute(bus, origin, destination, c, d);

    /* if bus is not in the list of buses that stop in Stop, add the bus! */
    if (!busInStopList(bus, origin)) { 
        i = allStops[origin].buses_of_this_stop;
        allStops[origin].buses[i] = bus;
        allStops[origin].buses_of_this_stop++;

    }
    if (!busInStopList(bus, destination)) {
        i = allStops[destination].buses_of_this_stop;
        allStops[destination].buses[i] = bus;
        allStops[destination].buses_of_this_stop++;
    }
}

/* checks if new destination or new origin are the extremes of route */
int notValidLink(char busName[], char oStop[], char dStop[]) {
    int bus, origin, destination;
    bus = finder(busName, FALSE);
    if (allBuses[bus].totalStops == 0)
        return FALSE;
    
    origin = finder(oStop, TRUE);
    destination = finder(dStop, TRUE);

    if (allBuses[bus].destination_stop_index == origin || 
    allBuses[bus].origin_stop_index == destination)
        return FALSE;
    return TRUE;
}

void linkFunction() {
    char busName[MAX_NAME_BUS]; 
    char originStopName[MAX_NAME_STOP], destinationStopName[MAX_NAME_STOP];
    double cost, duration;

    readWord(busName);
    readWord(originStopName);
    readWord(destinationStopName);
    scanf("%lf %lf", &cost, &duration);
    reads();

    if (!nameChecker(busName, FALSE))
        printf("%s: no such line.\n", busName);
    else if (!nameChecker(originStopName, TRUE))
        printf("%s: no such stop.\n", originStopName);
    else if (!nameChecker(destinationStopName, TRUE))
        printf("%s: no such stop.\n", destinationStopName);
    else if (notValidLink(busName, originStopName, destinationStopName))
        printf("link cannot be associated with bus line.\n");
    else if (cost < 0 || duration < 0)
        printf("negative cost or duration.\n");
    else 
        addLink(busName, originStopName, destinationStopName, cost, duration);
}

/* sorts the indexes of buses that stop in Stop */
void bubbleSort(int stop) {
    int i, j, n, index, indexPlusOne, temp, done;
    n = allStops[stop].buses_of_this_stop;

    for (i = 0; i < n; i++) {
        done = 1;
        for (j = 0; j < n - 1 - i; j++) {

            index = allStops[stop].buses[j];
            indexPlusOne = allStops[stop].buses[j + 1];

            if (strcmp(allBuses[index].name, allBuses[indexPlusOne].name) > 0){
                temp = allStops[stop].buses[j];
                allStops[stop].buses[j] = allStops[stop].buses[j + 1];
                allStops[stop].buses[j + 1] = temp;
                done = 0;
            }
        }
        if (done)
            break;
    }
}

void intersectionFunction() {
    int i, j, k;

    for (i = 0; i < number_of_stops; i++) {
        if (allStops[i].buses_of_this_stop >= 2) {

            bubbleSort(i);
            printf("%s %d: ", allStops[i].name, allStops[i].buses_of_this_stop);

            for (j = 0; j < allStops[i].buses_of_this_stop; j++) {

                k = allStops[i].buses[j];

                if (j == allStops[i].buses_of_this_stop - 1) 
                    printf("%s\n", allBuses[k].name);
                else 
                    printf("%s ", allBuses[k].name);
            }
        }
    }
    reads();
}

int main() {
    int c;

    while ((c = getchar()) != EOF) {
        switch (c) {
        case 'q':
            return 0;
        case 'c':
            busFunction();
            break;
        case 'p':
            stopFunction();
            break;
        case 'l':
            linkFunction();
            break;
        case 'i':
            intersectionFunction();
            break;
        default:
            printf("Invalid command: %c\n", c);
        }
    }
    return 0;
}
