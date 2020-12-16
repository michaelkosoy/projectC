#pragma once


#ifndef PROJECT_OBJECTS_H
#define PROJECT_OBJECTS_H
#include <time.h>
#define SHORT_TERM_SIZE 7
#define ADDRESS_SIZE 100
/*
 * Apartment contains all the fields of apartment and represents a doubly-linked node
 */
typedef struct apartment {
	unsigned int id; // id of apartment
	char address[ADDRESS_SIZE]; // the address of the apartment
	int price;
	short int numOfRooms;
	short int dayOfEntrance;
	short int monthOfEntrance;
	short int yearOfEntrance;
	time_t systemTime;
} Apartment;

typedef struct apartmentNode {
	Apartment *apt;
	struct apartmentNode *next;
	struct apartmentNode *prev;
} ApartmentNode;

typedef struct list {
	ApartmentNode *head;
	ApartmentNode *tail;
}List;

typedef struct command {	// for maagar B
	char *command;
	struct command *next;
} Command;
char *short_term_history[SHORT_TERM_SIZE];	//maagar A

#endif //PROJECT_OBJECTS_H
