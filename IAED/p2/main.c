/*
        iaed-23 - Gabriel Silva - ist1102637 - project2
    File: main.c
    Description: main function of system
*/

#include <stdio.h>
#include "system.h"


int main() {
    int c;
    Stop_Node *stopHead = NULL, *stopTail = NULL;
    Bus_Node *busHead = NULL, *busTail = NULL;
    Link_Node *linkHead = NULL, *linkTail = NULL;
    Int_Node *intHead = NULL;
    while ((c = getchar()) != EOF) {
        switch (c) {
        case 'q':
            clearData(&stopHead, &busHead, &linkHead, &intHead);
            return 0;
        case 'p':
            stopFunction(&stopHead, &stopTail, &busHead, &linkHead, &intHead);
            break;
        case 'c':
            busFunction(&stopHead, &busHead, &busTail,
                            &linkHead, &linkTail, &intHead);
            break;
        case 'l':
            linkFunction(&stopHead, &busHead, &linkHead, &linkTail, &intHead);
            break;
        case 'i':
            intersectionFunction(&stopHead, &intHead);
            break;
        case 'r':
            deleteBusFromSystem(&stopHead, &busHead, &linkHead, &intHead);
            break;
        case 'e':
            deleteStopFromSystem(&stopHead, &busHead, &linkHead, &intHead);
            break;
        case 'a':
            clearData(&stopHead, &busHead, &linkHead, &intHead);
            resetVariables(&stopTail, &busTail, &linkTail);
            reads();
            break;
        }
    }
    return 0;
}