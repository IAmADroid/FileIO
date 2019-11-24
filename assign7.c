#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

//TODO: move definitions to header file.

#define ERR_COURSE_NOT_FOUND "ERROR: course not found"

typedef struct
{
  char courseName [64];
  char courseSched [4];
  unsigned int courseHours;
  unsigned int courseSize;
} COURSE;


//Method Headers
char menuSelect();
void createCourse();
void readCourse();
void updateCourse();
void deleteCourse();
void start();
void end();
COURSE* findEntry(int courseNum);
void writeEntry(int courseNum, COURSE* course);
void printCourse(COURSE* c);


//Console input methods
int readNum(char* prompt){
  char buffer[128];
  while(1){
    printf("%s", prompt);

    fgets(buffer, 128, stdin);
    char* endptr;
    
    errno = 0;

    long num = strtol(buffer, &endptr, 10);
    //TODO; handle case where a user types "0"
    //maybe with sscanf?
    if(errno == 0 && num != 0){
      return (int) num;
    }

    printf("Num: %ld, errno=%d, end?\"%s\"\n", num, errno, endptr);
  }
}

//Global variables (Sorry!)
FILE* a7datafile;


int main(void){
  start();

  while(1){
    char option = menuSelect();

    if(feof(stdin)){
      printf("Exiting...");
      return(0);
    }

    switch(option){
    case 'c':
      createCourse();
      break;
    case 'r':
      readCourse();
      break;
    case 'u':
      updateCourse();
      break;
    case 'd':
      deleteCourse();
      break;
    default:
      printf("Unexpected selection: \"%c\"\n", option);
      break;
    }


  }

  end();
}
/**
 * Initializes data file.
 * Must be called before findEntry
 * and writeEntry.
 */
void start(){
  FILE* file = fopen("./courses.dat", "a+");
  if(file == NULL){
    fprintf(stderr, "Could not open file.");
  }

  a7datafile = file;//set global variable
}
/**
 * closes data file
 */
void end(){
  FILE* file = a7datafile; //retrieve global variable
  fclose(file);
}

COURSE* findEntry(int courseNum){
  FILE* file = a7datafile;

  long location = courseNum * sizeof(COURSE);

  fseek(file, location, SEEK_SET);

  COURSE* pCourse = malloc(sizeof(COURSE));

  int numRead = fread(pCourse, sizeof(COURSE), 1L, file);

  if(numRead != 1 || pCourse->courseHours == 0){
    free(pCourse);
    return NULL;
  }

  return pCourse;
}
void writeEntry(int courseNum, COURSE* course){
  FILE* file = a7datafile;

  long location = courseNum * sizeof(COURSE);

  fseek(file, location, SEEK_SET);
  
  int numWrite = fwrite(course, sizeof(COURSE), 1L, file);

  if(numWrite != 1){
    fprintf(stderr, "ERROR: Could not write record.");
  }
}

void printCourse(COURSE* c){
  printf("Course name: %s\n", c->courseName);
  printf("Scheduled days: %s\n", c->courseSched);
  printf("Credit hours: %d\n", c->courseHours);
  printf("Enrolled Students: %d\n", c->courseSize);
}

void createCourse(){
  //TODO: CreateCourse method.

  printf("Please enter course details.\n");

  int courseNum = readNum("Course number: ");
  //TODO: check if course already exists

  printf("Name of the course: ");
  char courseName[128];
  fgets(courseName, 128, stdin);

  printf("Course schedule: ");
  char buffer[128];
  fgets(buffer, 128, stdin);
  char courseSchedule[4];

  int creditHours = readNum("Credit hours: ");

  int enrollment = readNum("Enrollment (students already in the course): ");


  //TODO: write course to file.

}
void readCourse(){
  int courseNum = readNum("Enter a CS course number: ");
  COURSE* course = findEntry(courseNum);
  if(course == NULL){
    printf("%s\n", ERR_COURSE_NOT_FOUND );
    return;
  }

  printf("Course number: %d\n", courseNum);
  printCourse(course);
  free(course);
}
void updateCourse(){}
void deleteCourse(){}


char menuSelect(){
  //TODO: Exit program if CTRL-D is entered
  //TODO: Loop input selection untill valid input is selected.
  
  //Prompt User
  printf("Enter one of the following actions or press CTRL-D to exit.\n");
  printf("C - create a new course record\n");
  printf("R - read an existing course record\n");
  printf("U - update an existing course record\n");
  printf("D - delete an existing course record\n");

  //Read character from user
  char buffer[128];
  fgets(buffer, 128, stdin);

  //TODO: Verify that a valid menu option was chosen

  //Return character
  buffer[0] = tolower(buffer[0]);
  return buffer[0];
}
