/***************************************************************************
 * file name   : assembler.c                                               *
 * author      : tjf & Eric Xu                                             *
 * description : This program will assemble a .ASM file into a .OBJ file   *
 *               This program will use the "asm_parser" library to achieve *
 *			     its functionality.										   * 
 *                                                                         *
 ***************************************************************************
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "asm_parser.h"

int main(int argc, char** argv) {

	char* filename = NULL ;					// name of ASM file
	char  program [ROWS][COLS] ; 			// ASM file line-by-line
	char  program_bin_str [ROWS][17] ; 		// instructions converted to a binary string
	unsigned short int program_bin [ROWS] ; // instructions in binary (HEX)
	int i;
	int j = 0;
	int ret = 0;

   	if (argc != 2) { //return error message if assembler is called incorrectly
      printf("error1: usage: ./assembler <assembly_file.asm>\n");
	  return 1;	//return from main()
   	}

	//clear out program array in order to be able to detect length
	for(i = 0; i < ROWS; i++) {
		program[i][0] = '\0';
		program_bin[i] = 0;
	}

	ret = read_asm_file(argv[1], program); // read in contents of asm file to array
	if (ret != 0) {
		return ret;
	}

	while (program[j][0] != '\0') { //read in all lines of code
		ret = parse_instruction(program[j], program_bin_str[j]);	//parse all of the instructions
		if (ret != 0) {
			return ret;
		}
		program_bin[j] = str_to_bin(program_bin_str[j]);	//convert instructions to binary
		j++;	//jump to next line
	}

	ret = write_obj_file (argv[1], program_bin);	//write the object file

	return ret;	//return 0 from main if successful
}
