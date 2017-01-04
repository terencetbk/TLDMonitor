// AP3 Accessed Exercise 1
// Done by: Terence Tan Boon Kiat
// Matriculation ID: 15AC083B
// This is my own work as defined in the Academic Ethics agreement I have signed.

// date.c

#include "date.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct date{
	int day;
	int month;
	int year;
};

/*
* date_create creates a Date structure from `datestr`
* `datestr' is expected to be of the form "dd/mm/yyyy"
* returns pointer to Date structure if successful,
*         NULL if not (syntax error)
*/
Date *date_create(char *datestr) {
	//create new date struct and malloc to pointer
	Date *newDate = malloc(sizeof(Date));
	//define a string to store text from split
	char *delimText;
	
	if (newDate != NULL)
	{
		// get first token
		delimText = strtok(datestr, "/");
		// save as day
		newDate->day = atoi(delimText);
		// get second token
		delimText = strtok(NULL, "/");
		// save as month
		newDate->month = atoi(delimText);
		// get third token
		delimText = strtok(NULL, "/");
		// save as year
		newDate->year = atoi(delimText);
	}
	else {
		free(newDate);
		printf("syntax error");
	}
	return newDate;
}

/*
* date_duplicate creates a duplicate of `d'
* returns pointer to new Date structure if successful,
*         NULL if not (memory allocation failure)
*/
Date *date_duplicate(Date *d) {
	Date *newDate = malloc(sizeof(Date));
	if (newDate != NULL) {
		// copy all input date values to new date
		newDate->day = d->day;
		newDate->month = d->month;
		newDate->year = d->year;

		return newDate;
	}
	else {
		free(newDate);
		printf("memory allocation failure");
		return NULL;
	}
}

/*
* date_compare compares two dates, returning <0, 0, >0 if
* date1<date2, date1==date2, date1>date2, respectively
*/
int date_compare(Date *date1, Date *date2) {
	//compare year first
	if (date1->year < date2->year)
		return -1;
	else if (date1->year > date2->year)
		return 1;
	else {
		//if year same, compare month
		if (date1->month < date2->month)
			return -1;
		else if (date1->month > date2->month)
			return 1;
		else {
			//if month same, compare day
			if (date1->day < date2->day)
				return -1;
			else if (date1->day > date2->day)
				return 1;
			else
				//if all same, return 0
				return 0;
		}
	}
}

/*
* date_destroy returns any storage associated with `d' to the system
*/
void date_destroy(Date *d) {
	if (d != NULL)
		free(d);
	else
		printf("date is NULL");
}
