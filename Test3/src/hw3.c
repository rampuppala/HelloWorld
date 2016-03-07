/*
 * hw3.c
 *
 *  Created on: Mar 5, 2016
 *      Author: e567174
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct courseinfo
{
	char *courseId;
	double assignment;
	char *title;
	struct courseinfo *next;
}courseinfo;

typedef struct catalog_list
{
	struct courseinfo *start;

}courseCatalog;

typedef struct studentTranscript {
	char *courseId;
	char *grade;
	char *semester;
	struct studentTranscript *next;
}studentTranscript;

typedef struct transcriptList {
	struct studentTranscript *start;
}transcriptList;

int createCatalogFromFile();
char* parseCourseDetails(const char* str, int begin, int len);
void printCourseCatalog(struct catalog_list *sList);
void insertIntoCatalogList (struct catalog_list *sCatalogList, char inputInfo[]);


//creating global instance of courseCatalog
courseCatalog myCourseCatalog;

//creating global instance of studentTranscrip
transcriptList myTranscriptList;

// initialize the catalog list
void initCatalogList (courseCatalog *sCatalogList)
{
	sCatalogList->start = NULL;
}

// initialize the transcript list
void initStudentTranscript(transcriptList *sTranscriptList)
{
	sTranscriptList->start = NULL;
}

// create course catalog from input file
int createCatalogFromFile() {
	char inputinfo[50];


	//initiatize the catalog list
	initCatalogList(&myCourseCatalog);

	FILE *input = fopen("cat1.txt", "r");

	if (input == NULL) {
		printf("Error Opening File \n");
		exit(1);
	} else {
		// read the one line at a time from file
		while (fgets(inputinfo, 50, input) != NULL) {
			//parse the string, build struct and add to list
			insertIntoCatalogList (&myCourseCatalog, inputinfo);
		}
	}
	return 0;
}


char* parseCourseDetails(const char* str, int begin, int len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;
  return strndup(str + begin, len);
}

/* inserts the value to the front of the catalog*/
void insertIntoCatalogList (struct catalog_list *sCatalogList, char inputInfo[])
{
	courseinfo *p;

	p = malloc(sizeof(courseinfo)); //allocate memory for courseinfo structure

	p->courseId = parseCourseDetails(inputInfo, 0,10);

	// convert credits from string to double
	sscanf(parseCourseDetails(inputInfo, 11, 3), "%lf", &p->assignment);
	p->title = parseCourseDetails(inputInfo, 15, strlen(inputInfo)-15);

	p->next = sCatalogList->start;
	sCatalogList->start = p;
}

void insertIntoTranscriptList(transcriptList *sTranscriptList, char inputInfo[])
{
	studentTranscript *sPtr;


	sPtr = malloc(sizeof(studentTranscript));

	sPtr->courseId = parseCourseDetails(inputInfo, 2, 10);
	sPtr->semester = parseCourseDetails(inputInfo, 13, 6);
	sPtr->grade = parseCourseDetails(inputInfo, 20, 2);

	sPtr->next = sTranscriptList->start;
	sTranscriptList->start = sPtr;
}


/* find the course in catalog using courseId xx.yyy.nn */
courseinfo * findCourseInfoInCatalog(courseCatalog *sList, char *courseId) {
	if (sList == NULL) {
		printf("Error: Catalog is empty\n");
		exit(1);
	}
	courseinfo *p = sList->start;
	while (p != NULL) {

		int retCode = strcmp(p->courseId, courseId);
		if (retCode == 0) // found the match
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

/* print the course catalog*/
void printCourseCatalog(struct catalog_list *sList)
{
	printf("Printing Catalog\n");
	if (sList == NULL)
	{
		printf("Error: Catalog is empty\n");
		exit(1);
	}
	courseinfo *p = sList->start;
    while(p != NULL) {
         printf("%s %.2f %s\n", p->courseId, p->assignment, p->title);
         p = p->next;
    }
}

/*print the student transcript */
void printTranscriptList(transcriptList *sTranscriptList) {
	printf("Printing Transcript\n");

	if (sTranscriptList == NULL) {
		printf("Error: Transcript is empty\n");
		return;
	}

	studentTranscript *p = sTranscriptList->start;
	while (p != NULL) {
		printf("%s %s %s\n", p->courseId, p->semester, p->grade);
		p = p->next;
	}
}

/* Search if the course is already added to the transcript list */
studentTranscript * findTranscriptForCourse(transcriptList *sTranscriptList, char *courseId, char *semester)
{
	if (sTranscriptList == NULL) {
		printf("Error: Transcript is empty\n");
		return NULL;
	}

	studentTranscript *p = sTranscriptList->start;

	while (p != NULL) {
		if (strcmp(p->courseId, courseId) == 0 && strcmp(p->semester, semester) == 0)
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

void deleteStudentTranscript(transcriptList *sTranscriptList, char *courseId, char *semester)
{
	if (sTranscriptList == NULL) {
			printf("Error: Transcript is empty\n");
			return;
		}

	studentTranscript *p = sTranscriptList->start; // current transcript
	studentTranscript *temp;

	while (p != NULL) {
		printf("deleting %s %s\n", courseId, semester);

		if (strcmp(p->courseId, courseId) == 0 && strcmp(p->semester, semester) == 0)
		{
			if (!temp)
			{
				printf("First one in list");
				sTranscriptList->start = p->next;
			} else
			{
				temp->next = p->next;
			}

			free(p);
			break;
		} else
		{
			temp = p;
			p = p->next;
		}


	}
	return;

}


int main() {


		char input [50];
		char *courseId;
		char *semester;

		courseinfo *sCourseInfo;

		char *newTitle;

		int continueInput=1;

		//create course catalog from file
		createCatalogFromFile();

		char command;
		do
		{
			puts("Please enter your choice:");

			//scanf("%49[^\n]", &input);
			fgets(input, 50, stdin);

			if (isalpha(input[0])) // make sure the first character is alphabet
			{
				command = tolower(input[0]);

				switch (command)
				{
					case 'q': // break out of loop
						continueInput = 0;
						break;
					case 'p' : // display catalog
						printCourseCatalog(&myCourseCatalog);
						break;
					case 'c' : // display information specific catalog
						courseId = parseCourseDetails(input, 2,10 ); // parse courseId from input command
						sCourseInfo = findCourseInfoInCatalog(&myCourseCatalog, courseId);
						if (sCourseInfo != NULL)
						{
							printf("%s %.2f %s\n",sCourseInfo->courseId, sCourseInfo->assignment, sCourseInfo->title);
						} else {
							printf ("%s absent.\n", courseId);
						}

						break;
					case 't' :
						courseId = parseCourseDetails(input, 2,10 ); // parse courseId from input command
						newTitle = parseCourseDetails(input, 13, strlen(input) - 13 ); // parse the title from input command

						//check if entered course id is in catalog
						sCourseInfo = findCourseInfoInCatalog(&myCourseCatalog, courseId);
						if (sCourseInfo != NULL)
						{
							//free the memory of old title
							free(sCourseInfo->title);
							//course is in catalog, now update the title
							sCourseInfo->title = newTitle;
						} else {
							printf ("%s absent.\n", courseId);
						}
						break;
					case 'a' : // add course to the transcript
						courseId = parseCourseDetails(input, 2,10 );
						semester = parseCourseDetails(input, 13, 6);

						//check if entered course id is in catalog
						sCourseInfo = findCourseInfoInCatalog(&myCourseCatalog, courseId);


						if (sCourseInfo != NULL)
						{
							//now insert the studentTranscript into transcriptList
							printf("found course in catalog.\n");
							if (findTranscriptForCourse(&myTranscriptList, courseId, semester) == NULL)
							{
								insertIntoTranscriptList(&myTranscriptList, input);
							}
							else
							{
								printf("Course is already exits in transcript list.\n");
							}



						} else
						{
							printf ("%s absent.\n", courseId);
						}
						break;
					case 'i' : //display current transcript
						printTranscriptList(&myTranscriptList);
						break;

					case 'd' : //display current transcript
						courseId = parseCourseDetails(input, 2,10 );
						semester = parseCourseDetails(input, 13, 6);
						deleteStudentTranscript(&myTranscriptList, courseId, semester);

						break;

				}
			}

		} while (continueInput != 0);


		printf("input:%s\n",input);

		return 0;
}
