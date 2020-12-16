#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Header.h"
#define INITIAL_ID 1

List makeEmptyList();
int isEmptyList(List *lst);
void printApartmentList(List lst);
char *timeToString(time_t rawtime);
void addApartment(char *address, int price, short numOfRooms, unsigned short day, unsigned short month, unsigned int year);
void addToList(List *lst, Apartment *apt);
ApartmentNode *createApartmentNode(Apartment *apt, ApartmentNode *prev, ApartmentNode*next);
void removeApartment(ApartmentNode *apt);
void buyApartment(unsigned int apt_id);
void findApartmentsByDate(unsigned short day, unsigned short month, unsigned short year);



List apartmentList; // This list is sorted by ID in ascending order - the head of the list points to the first apartment added
static unsigned int current_id = INITIAL_ID;


int main() {
	apartmentList = makeEmptyList();
	addApartment("Aza 25, Tel Aviv", 100000, 5, 1, 12, 2020);
	addApartment("37 Tavistock, Saint Pancras, London", 200000, 40, 16, 12, 2020);
	addApartment("HaBrosh 49, Kfar Daniel", 4000000, 7, 1, 2, 2021);
	printApartmentList(apartmentList);
	findApartmentsByDate(16, 12, 2020);
	buyApartment(2);
	printf("Apartment with id #2 should be removed from the list\n");
	printApartmentList(apartmentList);
	addApartment("Rotschild Bvd 11, Tel Aviv", 20000000, 7, 1, 5, 2021);
	printf("Apartment with id #4 should be added\n");
	printApartmentList(apartmentList);
	buyApartment(1);
	printf("Apartment with id #1 should be removed from the list\n");
	printApartmentList(apartmentList);
	buyApartment(10);
	printf("There should be no change in the list\n");
	printApartmentList(apartmentList);
	return 0;
}

void addApartment(char *address, int price, short numOfRooms, unsigned short day, unsigned short month, unsigned int year) {
	Apartment *apt = (Apartment *)malloc(sizeof(Apartment));
	apt->id = current_id++;
	strcpy(apt->address, address);
	apt->price = price;
	apt->numOfRooms = numOfRooms;
	apt->dayOfEntrance = day;
	apt->monthOfEntrance = month;
	apt->yearOfEntrance = year;
	apt->systemTime = time(NULL);
	addToList(&apartmentList, apt);
}

void addToList(List *lst, Apartment *apt) {
	ApartmentNode * aptNode = createApartmentNode(apt, NULL, NULL);
	if (isEmptyList(lst)) {
		lst->head = lst->tail = aptNode;
	}
	else {
		lst->tail->next = aptNode;
		aptNode->prev = lst->tail;
		lst->tail = aptNode;
	}
}

ApartmentNode *createApartmentNode(Apartment *apt, ApartmentNode *prev, ApartmentNode*next) {
	ApartmentNode *aptNode = (ApartmentNode *)malloc(sizeof(ApartmentNode));
	aptNode->apt = apt;
	aptNode->prev = prev;
	aptNode->next = next;
	return aptNode;
}

void buyApartment(unsigned int apt_id) {
	ApartmentNode *aptNode = apartmentList.head;
	while (aptNode != NULL && aptNode->apt->id < apt_id) {
		aptNode = aptNode->next;
	}
	if (!aptNode || aptNode->apt->id > apt_id) {
		printf("We are sorry, we couldn't find apartment with id #%d\n", apt_id);
	}
	else {
		removeApartment(aptNode);
	}
}

void removeApartment(ApartmentNode *apt) {
	if (!apt) {
		fprintf(stderr, "removeApartment received NULL\n");
		exit(1);
	}
	if (apt == apartmentList.head) {
		apartmentList.head = apt->next;
		if (isEmptyList(&apartmentList)) {
			apartmentList.tail = NULL;
		}
		else {
			apartmentList.head->prev = NULL;
		}
	}
	else if (apt == apartmentList.tail) {
		apartmentList.tail = apt->prev;
		apartmentList.tail->next = NULL;
	}
	else {
		ApartmentNode *prev = apt->prev;
		ApartmentNode *next = apt->next;
		prev->next = next;
		next->prev = prev;
	}
	free(apt);
}

void findApartmentsByDate(unsigned short day, unsigned short month, unsigned short year) {
	List res = makeEmptyList();
	ApartmentNode *aptNode = apartmentList.head;
	while (aptNode != NULL) {
		Apartment *apt = aptNode->apt;
		if (apt->yearOfEntrance < year) {
			addToList(&res, apt);
		}
		else if (apt->yearOfEntrance == year) {
			if (apt->monthOfEntrance < month) {
				addToList(&res, apt);
			}
			else if (apt->monthOfEntrance == month && apt->dayOfEntrance <= day) {
				addToList(&res, apt);
			}
		}
		aptNode = aptNode->next;
	}
	printf("Printing apartments with entry date before %02d.%02d.%d \n", day, month, year);
	printApartmentList(res);
}


void printApartmentList(List lst) {
	ApartmentNode *aptNode = lst.head;
	printf("----------------------------------------------------------\n"); // DELETE LATER
	while (aptNode != NULL) {
		Apartment *apt = aptNode->apt;
		printf("Apt details:\n");
		printf("Code: %d\n", apt->id);
		printf("Address: %s\n", apt->address);
		printf("Number of rooms: %d\n", apt->numOfRooms);
		printf("Price: %d\n", apt->price);
		printf("Entry date:%02d.%02d.%d\n", apt->dayOfEntrance, apt->monthOfEntrance, apt->yearOfEntrance);
		char *entry_date = timeToString(apt->systemTime);
		printf("Database entry date: %s\n", entry_date);
		free(entry_date);
		aptNode = aptNode->next;
	}
	printf("----------------------------------------------------------\n"); // DELETE LATER
}

char *timeToString(time_t rawtime) {
	struct tm *time_info;
	int buffer_size = 20;
	char *buffer = (char *)malloc(buffer_size);
	if (!buffer) {
		fprintf(stderr, "Allocation Error at timeToString\n");
		exit(1);
	}

	time_info = localtime(&rawtime);
	strftime(buffer, buffer_size, "%d.%m.%Y\n", time_info);
	return buffer;
}



List makeEmptyList() {
	List lst;
	lst.head = NULL;
	lst.tail = NULL;
	return lst;
}

int isEmptyList(List *lst) {
	return lst->head == NULL || lst->tail == NULL;
}



