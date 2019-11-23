#include <stdio.h>
#include <ctype.h>

//TODO: move definitions to header file.
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


//Console input methods
int readNum(char* prompt){
  char buffer[128];
  while(1){
    fgets(buffer, 128, stdin);
    //TODO: convert String to number
  }
}


int main(void){
  while(1){
    char option = menuSelect();

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
}

void createCourse(){
  //TODO: CreateCourse method.

}
void readCourse(){}
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
