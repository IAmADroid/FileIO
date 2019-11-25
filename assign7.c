#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>


//TODO: move definitions to header file.

#define ERR_COURSE_NOT_FOUND "ERROR: course not found"
#define ERR_COURSE_EXISTS "ERROR: course already exists"
#define ERR_INVALID_OPTION "ERROR: invalid option"

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

//String editing methods
char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

//Console input methods
int readNum(char* prompt){
  char buffer[128];
  while(1){
    printf("%s", prompt);

    fgets(buffer, 128, stdin);
    char* endptr;
    
    errno = 0;

    long num = strtol(buffer, &endptr, 10);

    int noNumberRead = 0;
    //From strtol's documentation:
    // If the subject sequence is empty or does not have the expected form,
    // no conversion is performed;
    // the value of nptr shall be stored in the object pointed to by endptr,
    // provided that endptr is not a null pointer.
    if(buffer == endptr){
      noNumberRead = 1;
    }


    if(errno == 0 && !noNumberRead){
      return (int) num;
    }

    printf("Invalid number.\n");
  }
}

//Global variables (Sorry!)
FILE* a7datafile;


int main(void){
  start();

  while(1){
    char option = menuSelect();

    if(feof(stdin)){
      end();
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
    case 'x':
      printf("xkcd, Elements of Java Style, Clinic of Horrors (webtoon), Tomboys (girls that do cool stuff like dirtbike racing), Hackers (movie).\nC is terrible for console input, but it feels very rewarding for some reason?\n");
    default:
      fprintf(stderr, "%s\n", ERR_INVALID_OPTION );
      break;
    }

    printf("\n");
  }

  end();
}
/**
 * Initializes data file.
 * Must be called before findEntry
 * and writeEntry.
 */
void start(){
  FILE* file = fopen("./courses.dat", "r+");
  if(file == NULL){
    fprintf(stderr, "Could not open file.");
  }

  a7datafile = file;//set global variable
}
/**
 * closes data file
 */
void end(){
  printf("Exiting...");
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

  fflush(file);
}

void printCourse(COURSE* c){
  printf("Course name: %s\n", c->courseName);
  printf("Scheduled days: %s\n", c->courseSched);
  printf("Credit hours: %d\n", c->courseHours);
  printf("Enrolled Students: %d\n", c->courseSize);
}

void createCourse(){
  printf("Please enter course details.\n");

  int courseNum = readNum("Course number: ");

  COURSE* c = findEntry(courseNum);
  if(c != NULL){
    fprintf(stderr, "%s\n", ERR_COURSE_EXISTS);
    return;
  }

  COURSE course;


  printf("Name of the course: ");
  char courseName[128];
  fgets(courseName, 128, stdin);

  if(courseName[strlen(courseName) - 1] == '\n'){
    courseName[strlen(courseName) - 1] = '\0';
  }

  memcpy(course.courseName, courseName, 63);
  course.courseName[63] = '\0';

  printf("Course schedule: ");
  char buffer[128];
  fgets(buffer, 128, stdin);

  if(buffer[strlen(buffer) - 1] == '\n'){
    buffer[strlen(buffer) - 1] = '\0';
  }

  memcpy(course.courseSched, buffer, 3);
  course.courseSched[3] = '\0';

  int creditHours = readNum("Credit hours: ");
  course.courseHours = creditHours;

  int enrollment = readNum("Enrollment (students already in the course): ");
  course.courseSize = enrollment;
  
  printCourse(&course);
  writeEntry(courseNum, &course);

}
void readCourse(){
  int courseNum = readNum("Enter a CS course number: ");
  COURSE* course = findEntry(courseNum);
  if(course == NULL){
    fprintf(stderr, "%s\n", ERR_COURSE_NOT_FOUND );
    return;
  }

  printf("Course number: %d\n", courseNum);
  printCourse(course);
  free(course);
}
void updateCourse(){
  int courseNum = readNum("Enter a CS course number: ");
  COURSE* course = findEntry(courseNum);
  if(course == NULL){
    fprintf(stderr, "%s\n", ERR_COURSE_NOT_FOUND );
    return;
  }

  printf("Type a new value, or leave the new value blank ");
  printf("to maintain original value.\n");
  
  printf("Course name: \"%s\"\n", course->courseName);
  printf("type new value: ");

  char buffer[128];
  char* trimmed;

  fgets(buffer, 128, stdin);
  trimmed = trim(buffer);
  if(strlen(trimmed) != 0){
    memcpy(course->courseName, trimmed, 63);
    course->courseName[63] = '\0';
    printf("Edited.\n"); 
  } else {
    printf("Retaining original value.\n");
  }

  printf("Scheduled days: \"%s\"\n", course->courseSched);
  printf("type new value: ");

  fgets(buffer, 128, stdin);
  trimmed = trim(buffer);
  if(strlen(trimmed) != 0){
    memcpy(course->courseSched, trimmed, 3);
    course->courseSched[3] = '\0';
    printf("Edited.\n");
  } else {
    printf("Retaining original value.\n");
  }

  printf("Credit hours: \"%d\"\n", course->courseHours);
  printf("type new value: ");

  fgets(buffer, 128, stdin);
  trimmed = trim(buffer);
  if(strlen(trimmed) != 0){
    errno = 0;
    char* endptr;
    long num = strtol(trimmed, &endptr, 10);

    if(errno == 0 && trimmed != endptr){
      course->courseHours = num;
    }

    printf("Edited.\n");
  } else {
    printf("Retaining original value.\n");
  }

  printf("Enrolled Students: \"%d\"\n", course->courseSize);
  printf("type new value: ");

  fgets(buffer, 128, stdin);
  trimmed = trim(buffer);
  if(strlen(trimmed) != 0){
    errno = 0;
    char* endptr;
    long num = strtol(trimmed, &endptr, 10);

    if(errno == 0 && trimmed != endptr){
      course->courseSize = num;
    }


    printf("Edited.\n");
  } else {
    printf("Retaining original value.\n");
  }

  writeEntry(courseNum, course);

}
void deleteCourse(){
  //Prompt user for selection
  int courseNum = readNum("Enter a course number: ");

  //Find selection
  COURSE* course = findEntry(courseNum);
  if(course == NULL){
    fprintf(stderr, "%s\n", ERR_COURSE_NOT_FOUND);
    return;
  }

  //Delete selection
  course->courseHours = 0;  //No course has 0 credit hours
                            //So this is interpreted as a
                            //deleted course.            
  writeEntry(courseNum, course);
  printf("%d was successfully deleted.\n", courseNum);
}


char menuSelect(){
  //Prompt User
  printf("Enter one of the following actions or press CTRL-D to exit.\n");
  printf("C - create a new course record\n");
  printf("R - read an existing course record\n");
  printf("U - update an existing course record\n");
  printf("D - delete an existing course record\n");

  //Read character from user
  char buffer[128];
  fgets(buffer, 128, stdin);

  if(strcmp(buffer, "best\n") == 0){
    return 'x';
  }

  if(buffer[0] == 'x'){
    return 'y';
  }

  //Return character
  buffer[0] = tolower(buffer[0]);
  return buffer[0];
}
