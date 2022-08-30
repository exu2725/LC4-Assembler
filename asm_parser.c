/***************************************************************************
 * file name   : asm_parser.c                                              *
 * author      : tjf & Eric Xu                                             *
 * description : the functions are declared in asm_parser.h                *
 *               The intention of this library is to parse a .ASM file     *
 *                                                                         * 
 *                                                                         *
 ***************************************************************************
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm_parser.h"

/* to do - implement all the functions in asm_parser.h */
int read_asm_file(char * filename, char program[ROWS][COLS]) {
  FILE * fp;
  int column;
  int row = 0;

  // array to keep track of characters in current row, and a pointer to the position of a comment if it exists
  char inputRow[COLS], * commentPos; 

  fp = fopen(filename, "r"); //open the file specified

  if (fp == NULL) {
    perror("error2: read_asm_file() failed"); //print error message if file doesn't exist
    return 2; //return from function
  }

  while (fgets(inputRow, COLS, fp) != NULL) { // read in the current row to the correct position in the arrayfprint
    trimLeadingSpaces(inputRow);  //call helper method to eliminate empty space in front of a row
    if(strlen(inputRow) > 0) {  //if the row is not blank, read it in
      commentPos = strchr(inputRow, ';');  //find position of comment in row (if it exists)
      if (commentPos == NULL) { // if there is no comment, read in as normal
        strcpy(program[row], inputRow);
        program[row][strlen(program[row]) - 1] = '\0'; // get rid of newline character at end of row
        row++; //increment to next row
      } else if (commentPos != NULL && commentPos != inputRow) {  // if comment is at the end of the line, ignore it
        strncpy(program[row], inputRow, commentPos - inputRow);
        program[row][commentPos - inputRow] = '\0';
        row++;
      } // do nothing if entire line is a comment
    }
  }
  return 0; // return 0 if executed correctly
}

// Remove leading whitespace characters from string
trimLeadingSpaces(char * str) {
    int index, i;
    index = 0;

    // skip leading spaces
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n') {
        index++;
    }

    if(index != 0) {
        //Shift non-space characters left
        i = 0;
        while(str[i + index] != '\0') {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; // Make sure that string is NULL terminated
    }
}

int parse_instruction(char * instr, char * instr_bin_str) {
  const char s[3] = " ,";
  char * token;
  char instrCopy[COLS];

  //create copy of instr to change
  strcpy(instrCopy, instr);

  /* get the first token */
  token = strtok(instrCopy, s);

  //return if invalid OPCODE
  if (token == NULL) {
    perror("error3: parse_instruction() failed.");
    return 3; //return from function
  } else if (strcmp(token, "ADD") == 0) {
    strcpy(instr_bin_str, "0001"); //add proper opcode to binary

    if (parse_add(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else if (strcmp(token, "MUL") == 0) {
    strcpy(instr_bin_str, "0001"); //add proper opcode to binary

    if (parse_mul(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else if (strcmp(token, "SUB") == 0) {
    strcpy(instr_bin_str, "0001"); //add proper opcode to binary

    if (parse_sub(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else if (strcmp(token, "DIV") == 0) {
    strcpy(instr_bin_str, "0001"); //add proper opcode to binary

    if (parse_div(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else if (strcmp(token, "AND") == 0) {
    strcpy(instr_bin_str, "0101"); //add proper opcode to binary

    if (parse_add(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else if (strcmp(token, "OR") == 0) {
    strcpy(instr_bin_str, "0101"); //add proper opcode to binary

    if (parse_sub(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else if (strcmp(token, "XOR") == 0) {
    strcpy(instr_bin_str, "0101"); //add proper opcode to binary

    if (parse_div(instr, instr_bin_str) != 0) { //parse rest of the string according to the operation
      perror("error3: parse_instruction() failed.");
      return 3; //return from function if this fails
    }
  } else { //return if invalid token
    perror("error3: parse_instruction() failed.");
    return 3;
  }

  return 0;
}

int parse_add(char * instr, char * instr_bin_str) { //handles last 12 digits of binary for ADD, AND opcode
  return parse_op(instr, instr_bin_str, "000");
}
int parse_mul(char * instr, char * instr_bin_str) { //handles last 12 digits of binary for MUL opcode
  return parse_op(instr, instr_bin_str, "001");
}
int parse_sub(char * instr, char * instr_bin_str) { //handles last 12 digits of binary for SUB, OR opcode
  return parse_op(instr, instr_bin_str, "010");
}
int parse_div(char * instr, char * instr_bin_str) { //handles last 12 digits of binary for MUL, XOR opcode
  return parse_op(instr, instr_bin_str, "011");
}
int parse_add_immediate(char * instr, char * instr_bin_str) { //handles last 12 digits of binary for ADD, AND opcode
  return parse_op(instr, instr_bin_str, "1");
}

unsigned short int str_to_bin(char * instr_bin_str) {
  long ret;

  ret = strtol(instr_bin_str, NULL, 2); //convert the binary string to its hex equivalent

  if (ret == 0) {
    perror("error6: str_to_bin() failed."); //return if error
    return 6;
  } else {
    return ret; //else, return the hex equivalent
  }
}

int write_obj_file(char * filename, unsigned short int program_bin[ROWS]) {
  FILE * fp;
  unsigned short int rows = 0;
  char p[4];
  unsigned short int value;
  int i;
  int j;

  // 1) Take the filename passed in the argument “filename”, change the last 3 letters to “obj”
  filename[strlen(filename) - 3] = 'o';
  filename[strlen(filename) - 2] = 'b';
  filename[strlen(filename) - 1] = 'j';

  // 2) Open up that file for writing and in “BINARY” format.
  fp = fopen(filename, "wb");

  if (fp == NULL) /* If an error occurs during the file creation */ {
    perror("error7: write_obj_file() failed.");
    return 7;
  }

  value = (unsigned short) strtol("CADE", NULL, 16);  // 3) Write out the first word in the header: 0xCADE
  fwrite( & value, 1, 2, fp);

  value = (unsigned short) strtol("0000", NULL, 16); // 4) Write out the address your program should be loaded at: 0x0000 is the default we’ll use
  fwrite( & value, 1, 2, fp);

  while (program_bin[rows] != 0) {  //count number of commands ("<n>")
    rows++;
  }
  fwrite( & rows, 1, 2, fp); // write out <n>

  // 6) Now that the header is complete, write out the <n> rows of data in program_bin[]
  for (j = 0; j < rows; j++) {
    fwrite( & program_bin[j], 1, 2, fp); 
  }

  fclose(fp); // 7) Close the file using fclose().
}

int parse_op(char * instr, char * instr_bin_str, char * op) { //helper function for common aspects of converting to binary_str for all operations
  const char s[3] = " ,"; //delimiter
  char * token, *t; //current segment
  char instrCopy[COLS]; //used to preserve instr
  char errorMsg[COLS] = "error4: parse_";
  char imm5String[5];
  unsigned int ret, imm5;

  //create copy of instr to change
  strcpy(instrCopy, instr);

  token = strtok(instrCopy, s); //skip over opcode
  strcat(errorMsg, instrCopy);  // save the error message (in case error happens, it will be displayed)
  strcat(errorMsg, " failed.");
  token = strtok(NULL, s); //retrieve first register
  
  if (token[0] != 'R' || parse_reg(token[1], instr_bin_str) != 0) { //converts the register to binary, returns if invalid
    perror(errorMsg);
    return 4;
  }

  token = strtok(NULL, s);
  if (token[0] != 'R' || parse_reg(token[1], instr_bin_str) != 0) { //converts the register to binary, returns if invalid
    perror(errorMsg);
    return 4;
  }

  token = strtok(NULL, s);     
  if (token[0] == 'R') { // normal arithmatic operations
    strcat(instr_bin_str, op); //this is the only difference between all the operations' binary strings in the last 12 digits
    if (parse_reg(token[1], instr_bin_str) != 0) { //converts the register to binary, returns if invalid
      perror(errorMsg);
      return 4;
    }
  } else {   // ADD or AND immediate operations
    strcat(instr_bin_str, "1"); // for immediate operations, op code is 1
    ret = strtol(token, &t, 10); 
    if (ret >= 32 || ret <= 0) {  // must be IMM5 number 
      perror(errorMsg);
      return 4;
    }   
    imm5 = (unsigned short) decToBinary(ret);    // get binary format of imm5
    sprintf(imm5String, "%d", imm5);      // convert to string
    shift_imm5(imm5String);               // add leading 0s for small binary numbers
    strcat(instr_bin_str, imm5String);    
  }

  return 0; //return 0 if successful
}

int decToBinary(int decNo){   // convert decimal to binary
   long binNo=0, reminder, f=1;
   while(decNo != 0){
      reminder = decNo % 2;
      binNo = binNo + reminder * f;
      f = f * 10;
      decNo = decNo / 2;
   }
   return binNo;
}

shift_imm5(char * imm5String) { // if the length of string is less than 5, shift to the right, and fill 0s on left side
  int i;
  int j = 4;
  for (i = strlen(imm5String) - 1; i >= 0; i--) { // shift string
    imm5String[j] = imm5String[i];
    j--;
  }
  for (; j >= 0; j--) { // fill leading empty slots with '0'
    imm5String[j] = '0';
  }
} 

int parse_reg(char reg_num, char * instr_bin_str) { //helper function for converting registers to binary

  //converts integers (i.e. register number) to its 3 digit binary equivalent
  if (reg_num == '0') {
    strcat(instr_bin_str, "000");
  } else if (reg_num == '1') {
    strcat(instr_bin_str, "001");
  } else if (reg_num == '2') {
    strcat(instr_bin_str, "010");
  } else if (reg_num == '3') {
    strcat(instr_bin_str, "011");
  } else if (reg_num == '4') {
    strcat(instr_bin_str, "100");
  } else if (reg_num == '5') {
    strcat(instr_bin_str, "101");
  } else if (reg_num == '6') {
    strcat(instr_bin_str, "110");
  } else if (reg_num == '7') {
    strcat(instr_bin_str, "111");
  } //if the number is not between 0 and 7, return an error message
  else {
    perror("error5: parse_reg() failed.");
    return 5;
  }

  return 0; //return 0 if successful
}