/*
Two Passes
Tsega Tsewameskel
CS 325
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>

struct Node{
    //int info;
    int* memoryLocation;
    char* symbol;
    //char* info;
    struct Node* next;
};

struct LinkedList{
    struct Node* head;


};

void printList(struct LinkedList* list);
void insertAtEnd(struct LinkedList* list,char* pos, char* item);
void deleteList(struct LinkedList* list);
char* searchNode(struct LinkedList* list, char* item);

void firstPass(char* inputFileName, struct LinkedList* list);
char* noSpace(char*);
int stringToInt(char*);
char* intToBinary(int);
char* breakBefore(char[], char);
char* breakAfter(char[], char);
char* juDe(char []);
char aBit(char []);
char* compT(char []);
char* destT(char []);
char* jumpT(char []);
char* defaultSymbols(char []);
char* labelsCheck(struct LinkedList* list,char []);

void secondPass(struct LinkedList* list);


int main()
{
    struct LinkedList symbolTable;
    symbolTable.head = NULL;

    /*
    insertAtEnd(&myList, 42);
    insertAtEnd(&myList, 43);
    insertAtEnd(&myList, 45);
    insertAtEnd(&myList, 44);

    */
    secondPass(&symbolTable);

    //printList(&myList);
    deleteList(&symbolTable);

    return 0;
}




void firstPass(char* inputFileName, struct LinkedList* list){
  FILE* inputFile = NULL;
  //FILE* outputFile = NULL;

  const int bufferSize = 256; //const fixes whining about variable length array
  //char inputFileName[bufferSize];

  //printf("Please enter an input filename: ");
  //fgets(inputFileName, bufferSize, stdin);

  inputFile = fopen(inputFileName, "r");
  //outputFile = fopen("out.txt", "w");

  if(inputFile  == NULL){
    puts("Error opening input file");
  }

  else{
    char buffer[bufferSize];
    int i = 0;
    char *line = (char*)malloc(sizeof(char) * bufferSize);

    int lineCount = -1;
    while(fgets(buffer, bufferSize, inputFile) != NULL){
        buffer[strcspn(buffer, "\r\n")] = 0;

        line = noSpace(buffer);

        if(line[i] == '('){
            char* newLine = breakBefore(line, ')') + 1;
            char* stringCount=(char*)malloc(10*sizeof(char));
            char sstringCount[20];

            sprintf(sstringCount, "%d", lineCount + 1);
            strcpy(stringCount, sstringCount);
            insertAtEnd(list, stringCount, newLine);


            free(stringCount);
        }
        else if(line[i] == '@' ||
                line[i] == 'A' ||
                line[i] == 'M'||
                line[i] == 'D'||
                line[i] == '0'){
                  lineCount++;
        }
        else{
          lineCount++;
        }

    }
    //free(line);
    fclose(inputFile);
    //fclose(outputFile);
  }

}

void secondPass(struct LinkedList* list){
  FILE* inputFile = NULL;
  FILE* outputFile = NULL;

  const int bufferSize = 256; //256 is an arbitrary value
  char inputFileName[bufferSize];

  //Get file name from keyboard
  printf("Please enter an input filename: ");
  fgets(inputFileName, bufferSize, stdin);
  inputFileName[strcspn(inputFileName, "\n")] = 0; //removes the \n

  firstPass(inputFileName, list);
  printList(list);

  //Open files with fopen
  inputFile = fopen(inputFileName, "r"); //read only
  outputFile = fopen("out.txt", "w");

  if(inputFile == NULL){
      puts("Error opening input file");
  }
  else if(outputFile == NULL){
      puts("Error opening output file");
  }
  else{ //Don't proceed unless you know the files are open

      //Create a buffer to read into
      char buffer[bufferSize];
      int i = 0;
      char *line = (char*)malloc(sizeof(char) * bufferSize);

      char *binaryFormC = (char*)malloc(17*sizeof(char));
      char* binaryForm = NULL;
      char* jumpOutput= NULL;
      char* jumpBinaryForm= NULL;
      char* compOutput= NULL;
      char* compBinaryForm= NULL;
      char* destOutput= NULL;
      char* compOutput1= NULL;
      char* destBinaryForm= NULL;
      char* compBinaryForm1= NULL;
      char* checkSym=NULL;
      char* checkLabel=NULL;
      int varPosition = 16;

      //Use fgets to read from the file, null when end of file
      while(fgets(buffer, bufferSize, inputFile) != NULL){
          buffer[strcspn(buffer, "\r\n")] = 0;

          line = noSpace(buffer);
          if (line[i] == '@'){
              char* newLine = line + 1;
              int num = -1;
              char* checksAll = " ";

              if(isdigit(newLine[0])==0){
                  if(newLine[0] >= 'a' && newLine[0] <= 'z'){
                      char* pos=(char*)malloc(10*sizeof(char));
                      char stringPos[10];


                      sprintf(stringPos, "%d", varPosition);
                      strcpy(pos, stringPos);
                      insertAtEnd(list, pos, newLine);

                      checksAll = pos;
                      varPosition++;
                      free(pos);
                  }

                  assert(strcmp(checksAll,"") != 0);
                  num = stringToInt(checksAll);
              }
              else if(newLine[0] >= 'A' && newLine[0] <= 'Z'){
                  checkSym = defaultSymbols(newLine);
                  checkLabel = labelsCheck(list,checkSym);
                  checksAll = checkLabel;
              }


              else{
                  num = stringToInt(newLine);
              }

              assert(num != -1);
              binaryForm = intToBinary(num);
              printf("%s\n", binaryForm);
              fputs(binaryForm,outputFile);
              fputs("\n",outputFile);
          }
          else if(line[i] == '/'){
              puts("Comment");
          }
          else if(line[i] == 'A' ||
                  line[i] == 'M' ||
                  line[i] == 'D' ||
                  line[i] == '0' ||
                  line[i] == '1' ||
                  line[i] == '-' ||
                  line[i] == '!'
                  ){
              binaryFormC[16] = '\0';
              binaryFormC[15] = '1';
              binaryFormC[14] = '1';
              binaryFormC[13] = '1';

              char compare[5];
              strcpy(compare, juDe(line));

              if (strcmp(compare,"Dest\0") == 0){
                  destOutput = breakBefore(line, '=');
                  compOutput = breakAfter(line, '=');

                  destBinaryForm = destT(destOutput);
                  compBinaryForm = compT(compOutput);

                  int compCounter = 0;
                  for (int position = 11; position>5; position--){
                      binaryFormC[position] = compBinaryForm[compCounter];
                      compCounter++;
                  }

                  int destCounter = 0;
                  for (int position = 5; position>2; position--){
                      binaryFormC[position] = destBinaryForm[destCounter];
                      destCounter++;
                  }

                  binaryFormC[12] = aBit(compOutput);
                  binaryFormC[2] = '0';
                  binaryFormC[1] = '0';
                  binaryFormC[0] = '0';

                  puts(binaryFormC);
                  fputs(binaryFormC,outputFile);
                  fputs("\n",outputFile);

              }

              else if(strcmp(compare,"Jump\0") == 0){

                  jumpOutput = breakAfter(line, ';');
                  jumpBinaryForm = jumpT(jumpOutput);

                  int jumpCounter = 0;
                  for (int position = 2; position>-1; position--){
                      binaryFormC[position] = jumpBinaryForm[jumpCounter];
                      jumpCounter++;
                  }
                  compOutput1 = breakBefore(line, ';');
                  compBinaryForm1 = compT(compOutput1);

                  int compCounter = 0;
                  for (int position = 11; position>5; position--){
                      binaryFormC[position] = compBinaryForm1[compCounter];
                      compCounter++;
                  }

                  binaryFormC[12] = aBit(compOutput);
                  binaryFormC[5] = '0';
                  binaryFormC[4] = '0';
                  binaryFormC[3] = '0';

                  puts(binaryFormC);
                  fputs(binaryFormC,outputFile);
                  fputs("\n",outputFile);

              }

              else{
                  puts("Error");
                  puts(compare);
              }
          }
          else{
              puts("Error");
          }
      }
      //Close the file
      fclose(inputFile);
      fclose(outputFile);

      free(binaryFormC);
      free(binaryForm);
      free(line);
      free(checkSym);

      free(jumpOutput);
      free(compOutput);
      free(destOutput);
      free(compOutput1);

      free(compBinaryForm);
      free(jumpBinaryForm);
      free(destBinaryForm);
      free(compBinaryForm1);
  }

}

char* defaultSymbols(char string[]){
    char* value = (char*)malloc(7*sizeof(char));

    if(strcmp(string, "R0\0") == 0 || strcmp(string, "SP\0") == 0){
        strcpy(value, "0");
    }
    else if(strcmp(string, "R1\0") == 0 || strcmp(string, "LCL\0") == 0){
        strcpy(value, "1");
    }
    else if(strcmp(string, "R2\0") == 0 || strcmp(string, "ARG\0") == 0){
        strcpy(value, "2");
    }
    else if(strcmp(string, "R3\0") == 0 || strcmp(string, "THIS\0") == 0 ){
        strcpy(value, "3");
    }
    else if(strcmp(string, "R4\0") == 0 || strcmp(string, "THAT\0") == 0){
        strcpy(value, "4");
    }
    else if(strcmp(string, "R5\0") == 0){
        strcpy(value, "5");
    }
    else if(strcmp(string, "R6\0") == 0){
        strcpy(value, "6");
    }
    else if(strcmp(string, "R7\0") == 0){
        strcpy(value, "7");
    }
    else if(strcmp(string, "R8\0") == 0){
        strcpy(value, "8");
    }
    else if(strcmp(string, "R9\0") == 0){
        strcpy(value, "9");
    }
    else if(strcmp(string, "R10\0") == 0){
        strcpy(value, "10");
    }
    else if(strcmp(string, "R11\0") == 0){
        strcpy(value, "11");
    }
    else if(strcmp(string, "R12\0") == 0){
        strcpy(value, "12");
    }
    else if(strcmp(string, "R13\0") == 0){
        strcpy(value, "13");
    }
    else if(strcmp(string, "R14\0") == 0){
        strcpy(value, "14");
    }
    else if(strcmp(string, "R15\0") == 0){
        strcpy(value, "15");
    }
    else{
        strcpy(value, string);
    }

    assert(value != NULL);
    return value;
}

char* labelsCheck(struct LinkedList* list, char symbol[]){
  char* labelT = "none";
  if(strcmp(searchNode(list,symbol), "none") == 0){
      return symbol;
  }
  else{
      labelT = searchNode(list,symbol);
      return labelT;
  }
}

char* destT(char string[]){

  char* binaryForm = (char*)malloc(3*sizeof(char));


  if(strcmp(string, "M\0") == 0){
    strcpy(binaryForm, "001");
  }
  else if(strcmp(string, "D\0") == 0){
    strcpy(binaryForm, "010");
  }
  else if(strcmp(string, "MD\0") == 0){
    strcpy(binaryForm, "011");
  }
  else if(strcmp(string, "A\0") == 0){
    strcpy(binaryForm, "100");
  }
  else if(strcmp(string, "AM\0") == 0){
    strcpy(binaryForm, "101");
  }
  else if(strcmp(string, "AD\0") == 0){
    strcpy(binaryForm, "110");
  }
  else if(strcmp(string, "AMD\0") == 0){
    strcpy(binaryForm, "111");
  }
  else{
    strcpy(binaryForm, "000");
  }
  return binaryForm;
}

char* jumpT(char string[]){

  char *binaryForm = (char*)malloc(3*sizeof(char));

  if(strcmp(string, "JGT\0") == 0){
    strcpy(binaryForm, "001");
  }
  else if(strcmp(string, "JEG\0") == 0){
    strcpy(binaryForm, "010");
  }
  else if(strcmp(string, "JLT\0") == 0){
    strcpy(binaryForm, "011");
  }
  else if(strcmp(string, "JNE\0") == 0){
    strcpy(binaryForm, "100");
  }
  else if(strcmp(string, "JLE\0") == 0){
    strcpy(binaryForm, "110");
  }
  else if(strcmp(string, "JMP\0") == 0){
    strcpy(binaryForm, "111");
  }
  else{
    strcpy(binaryForm, "000");
  }
  return binaryForm;
}



char* compT(char string[]){

  char *binaryForm = (char*)malloc(6*sizeof(char));

  if(strcmp(string, "0\0") == 0){
    strcpy(binaryForm, "101010");
  }
  else if(strcmp(string, "1\0") == 0){
    strcpy(binaryForm, "111111");
  }
  else if(strcmp(string, "1\0") == 0){
    strcpy(binaryForm, "111111");
  }
  else if(strcmp(string, "-1\0") == 0){
    strcpy(binaryForm, "111010");
  }
  else if(strcmp(string, "D\0") == 0){
    strcpy(binaryForm, "001100");
  }
  else if(strcmp(string, "A\0") == 0 || strcmp(string, "M\0") == 0){
    strcpy(binaryForm, "110000");
  }
  else if(strcmp(string, "!D\0" == 0)){
    strcpy(binaryForm, "001101");
  }
  else if(strcmp(string, "!A\0") == 0 || strcmp(string, "!M\0") == 0){
    strcpy(binaryForm, "110001");
  }
  else if(strcmp(string, "-D\0")){
    strcpy(binaryForm, "001111");
  }
  else if(strcmp(string, "-A\0") == 0 || strcmp(string, "-M\0") == 0){
    strcpy(binaryForm, "110111");
  }
  else if(strcmp(string, "D+1") == 0){
    strcpy(binaryForm, "001111");
  }
  else if(strcmp(string, "D-1\0") == 0){
      strcpy(binaryForm, "001110");
  }
  else if(strcmp(string, "A+1\0") == 0 || strcmp(string, "M+1\0") == 0){
      strcpy(binaryForm, "110111");
  }
  else if(strcmp(string, "A-1\0") == 0 || strcmp(string, "M-1\0") == 0){
      strcpy(binaryForm, "110010");
  }
  else if(strcmp(string, "D+A\0") == 0 || strcmp(string, "D+M\0") == 0){
      strcpy(binaryForm, "000010");
  }
  else if (strcmp(string, "D-A\0") == 0 || strcmp(string, "D-M\0") == 0){
      strcpy(binaryForm, "010011");
  }
  else if(strcmp(string, "A-D\0") == 0 || strcmp(string, "M-D\0") == 0){
      strcpy(binaryForm, "000111");
  }
  else if(strcmp(string, "D&A\0") == 0 || strcmp(string, "D&M\0") == 0){
      strcpy(binaryForm, "000000");
  }
  else if(strcmp(string, "D|A\0") == 0 || strcmp(string, "D|M\0") == 0){
      strcpy(binaryForm, "010101");
  }
  else{
      puts("Error");
  }
  return binaryForm;

}


char aBit(char string[]){
    char bit = '\0';
    int found = 0;
    int len = strlen(string);

    if(len > 0){
        for (unsigned int i =0; i<len; i++){
           if (string[i] == 'M'){
                bit = '1';
                found = 1;
            }
            else if(string[i] == 'A'){
                bit = '0';
                found = 1;
            }
        }
      if (found == 0){
            bit = '0';
        }
    }
    return bit;
}

char* breakBefore(char string[], char breakChar){
    int position = -1;
    int len = strlen(string) - 1;


    for(int i = 0; i < len; i++){
      if(string[i] == breakChar){
          position = i;
          break;
      }
    }
    if(position > -1){
      int newLen = len + position +1;
      char* newStr = malloc(sizeof(char) * newLen);
      for(int begin = 0; begin < position; begin++){
        newStr[begin] = string[begin];
      }
       newStr[newLen] = '\0';
       return newStr;
    }

    return NULL;
}

char* breakAfter(char string[], char breakChar){
    int position =-1;
    int len =  strlen(string)-1;

    for (int i = 0 ; i < len; i++ ){

      if(string[i] == breakChar){
          position = i;
          break;
      }
    }
    if (position > -1){
        int newLen = len - position + 1;
        char *newStr = malloc(sizeof(char) * newLen);
        int begin = 0 ;
        for (int oldBegin = position + 1; oldBegin < len; oldBegin++){
            newStr[begin] = string[oldBegin];
            begin++;
        }

        newStr[newLen] = '\0';
        return newStr;
    }
    return NULL;

}


char* juDe(char string[]){
    char *value = (char*)malloc(sizeof(char) * 5);
    int len = strlen(string);

    if(len > 0){
        for(unsigned int i=0; i<len; i++){
          if(string[i] == '='){
            strcpy(value, "Dest");
          }
          else if(string[i] == ';'){
            strcpy(value, "Jump");
          }
        }
    }
    else{
        strcpy(value, "l137");
    }

    int int1 = strlen(value)+1;
    value[int1] = '\0';
    return value;
}

char* noSpace(char *string){

    int len = strlen(string) + 1;

    int end = 0;
    int start = 0;
    char compare[2];
    compare[1] = '\0';
    char comment[] = "/\0";
    char *noSpaceLine = (char*)malloc(len*sizeof(char));

    if (string == NULL){
        return NULL;
    }

    for(int i = 0; i < len; i++){
            compare[0] = string[i];
            if(strcmp(compare,comment) == 0 && start == 0){
                noSpaceLine[start] = string[i];
                start++;
            }
            else if(end == 0 && string[i] != ' '){
                noSpaceLine[start] = string[i];
                start++;
            }
            else if(strcmp(compare,comment) == 0){
                end = 1;
                i=len;
            }
    }

    int int1 = start ;
    noSpaceLine[int1] = '\0';
    return noSpaceLine;
}

int charToDigit(char digit){
    return digit - '0';
}

int stringToInt(char* string){
    unsigned int len = strlen(string) - 1;

    int sum = 0;
    int power = 0;

    for (int i= len; i>-1; i--){
        sum += (charToDigit(string[i]) * floor(pow(10,power++)));
    }
    return sum;
}


char digitToChar(int num){
    return (char)num + '0';
}

char* intToBinary(int num){
      char *result = (char*)malloc(16*sizeof(char));
      for(int position = 14; position>-1; position--){
        if(num != 0){
          if(num % 2 == 1){
            result[position] = '1';
            num = num/2;
          }
          else{
            result[position] = '0';
            num = num/2;
          }
        }
        else{
          result[position] = '0';
        }
      }
      result[15] = '\0';
      return result;
}
void printList(struct LinkedList* list)
{
    struct Node* temp = NULL;
    temp = list->head;
    while( temp != NULL )
    {
        printf("Label -> %s, Symbol -> %s\n", temp->memoryLocation, temp->symbol);
        temp = temp -> next;
    }
}

void insertAtEnd(struct LinkedList* list, char* pos, char* item)
{
    if(list->head == NULL)
    {
        list->head = malloc(sizeof(struct Node*));
        list->head->memoryLocation = pos;

        list->head->symbol = item;
        list->head->next = NULL;
    }
    else{

      if(strcmp(searchNode(list, item), "none") == 0){
        struct Node* position = list->head;


          while(position->next != NULL){
              position = position->next;
          }

          struct Node* newNode = malloc(sizeof(struct Node));
          newNode->memoryLocation = pos;
          newNode->symbol = item;
          newNode->next = NULL;
          position->next = newNode;
      }
    }
}


void deleteList(struct LinkedList* list)
{
    struct Node* temp = list->head;
    while(temp != NULL){
        list->head = temp->next;
        free(temp);
        temp = list->head;
    }
}

char* searchNode(struct LinkedList* list,char* item){
    char* found = "none";
    struct Node* pos = list->head;

    while(pos != NULL){
         if(strcmp(pos->symbol, item) == 0){
             found = pos->memoryLocation;
         }
         pos = pos->next;
    }
    return found;
}
