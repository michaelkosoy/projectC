#include <stdio.h>
#pragma warning(disable : 4996)
#include <stdlib.h>
#include <string.h>
#include "objects.h"
#define INITIAL_ID 1
#define _CRT_SECURE_NO_WARNINGS
#define BOOL int
#define TRUE 1
#define FALSE 0

///List function declaration
NodesList makeEmptyNodeList();
List makeEmptyList();
void insertDataToEndList(NodesList *lst, char* ch);
int isEmptyList(List *lst);
void printApartmentList(List lst);
void checkMemoryAllocation(void *ptr);
ListNode *createNewListNode(char* ch, ListNode *next);
void insertDataToEndList(NodesList *lst, char* ch);
void addToList(List *lst, Apartment *apt);
void insertNodeToEndList(NodesList *lst, ListNode *newTail);
int isEmptyNodeList(NodesList *lst);
//Apartment function declaration
void swapApt(ApartmentNode* first, ApartmentNode* second);
void sortApartmentList(List *apartmentList);
ApartmentNode *createApartmentNode(Apartment *apt, ApartmentNode *prev, ApartmentNode*next);
void removeApartment(ApartmentNode *apt);
void buyApartment(unsigned int apt_id);
void findApartmentsByDate(unsigned short day, unsigned short month, unsigned short year);
void addApartment(char *address, int price, short numOfRooms, unsigned short day, unsigned short month, unsigned int year);
//Parcing related function declaration
char* requestType(char* userRequest);
NodesList requestTypeFind(char* input);	
char *timeToString(time_t rawtime);




List apartmentList; // This list is sorted by ID in ascending order - the head of the list points to the first apartment added
static unsigned int current_id = INITIAL_ID;


int main() {
	apartmentList = makeEmptyList();
	addApartment("Aza 25, Tel Aviv", 5000000, 5, 1, 12, 2020);
	addApartment("37 Tavistock, Saint Pancras, London", 2000000, 40, 16, 12, 2020);
	addApartment("HaBrosh 49, Kfar Daniel", 1000000, 7, 1, 2, 2021);
	printApartmentList(apartmentList);
	printf("Apartments should be sorted by price \n");
	sortApartmentList(&apartmentList);
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

	sortApartmentList(&apartmentList);
	printApartmentList(apartmentList);

	
	/*printf("Enter a request");
	char str[100];
	gets(str);
	char str2[100];
	strcpy(str2, str);
	char* result = requestType(str);
	printf("%s", result);
	NodesList res = makeEmptyNodeList();
	res = requestTypeFind(str2);
	ListNode* curr = res.head;
	while (curr->data != NULL) {
		printf("%s", curr->data);
		curr = curr->next;
	}
	*/
	return 0;
}
void sortApartmentList(List *start) {
	ApartmentNode* curr = start->head;
	ApartmentNode* lptr = NULL;
	if (curr == NULL)
		return;
	int swapped;
	ApartmentNode* ptr1;
	do
	{
		swapped = 0;
		ptr1 = curr;
		while (ptr1->next != lptr) {
			if (ptr1->apt->price > ptr1->next->apt->price) {
				swapApt(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
	while (swapped);
}
void swapApt(ApartmentNode* first, ApartmentNode* second) {	//swap apt values
	ApartmentNode* temp = (ApartmentNode*)malloc(sizeof(ApartmentNode*));
	temp->apt = first->apt;
	first->apt = second->apt;
	second->apt = temp->apt;
}
NodesList requestTypeFind(char* input) {	//Returns list of strings: MinNumRooms 3 MaxNumRooms 5 
	NodesList requests = makeEmptyNodeList();
	char *found;
	found = strtok(input, "-");
	found = strtok(NULL, "-");
	found = strtok(NULL, " ");
	while (found)
	{
		insertDataToEndList(&requests, found);
		found = strtok(NULL, "-");
		insertDataToEndList(&requests, found);
		found = strtok(NULL, " ");
	}
	return requests;
}
int requestTypeBuy(char* input) {	//Returns number of apt to remove from db 
	char *found;
	found = strtok(input, " ");	// returns buy-apt
	found = strtok(NULL, "-");	//contains apt number as a string
	int aptNum;
	sscanf(found, "%d", &aptNum);	//convert to int
	return aptNum;
}

char* requestType(char* userRequest) {	// Return requestType add/buy/find
	char *found;
	found = strtok(userRequest, "-");
	if (found == NULL)return(NULL);
	return found;
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
void insertDataToEndList(NodesList *lst, char* ch)
{
	ListNode *newTail;
	newTail = createNewListNode(ch, NULL);
	insertNodeToEndList(lst, newTail);
}
void insertNodeToEndList(NodesList *lst, ListNode *newTail)
{
	if (isEmptyNodeList(lst) == TRUE)
	{
		lst->head = newTail;
		lst->tail = newTail;
	}
	else
	{
		lst->tail->next = newTail;
		lst->tail = newTail;
	}
}
ListNode *createNewListNode(char* ch, ListNode *next)	//WIP
{
	ListNode *curr;
	curr = (ListNode *)malloc(sizeof(ListNode));
	checkMemoryAllocation(curr);
	int size = strlen(ch);
	curr->data = (char *)malloc(sizeof(char)*size+1);
	checkMemoryAllocation(curr->data);
	strcpy(curr->data, ch);
	curr->data[size] = '\0';
	curr->next = next;
	return curr;
}
void checkMemoryAllocation(void *ptr)
{
	if (ptr == NULL)
	{
		printf("Not enough memory");
		exit(1);
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
NodesList makeEmptyNodeList() {
	NodesList lst;
	lst.head = NULL;
	lst.tail = NULL;
	return lst;
}
int isEmptyList(List *lst) {
	return lst->head == NULL || lst->tail == NULL;
}
int isEmptyNodeList(NodesList *lst) {
	return lst->head == NULL || lst->tail == NULL;
}



