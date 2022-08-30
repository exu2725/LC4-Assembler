all: asm_parser.o assembler.o # Generate the executable target ("assembler:)
	clang -o assembler asm_parser.o assembler.o

# These files compile via implicit rules
assembler.o: assembler.c
asm_parser.o: asm_parser.c

# delete object files and executable files
clean:
	rm *.o assembler
