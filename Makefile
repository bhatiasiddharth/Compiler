parser: parser.o lexer.o stack.o
	gcc parser.o lexer.o stack.o -o parser

parser.o: parser.c
	gcc -c parser.c

lexer.o: lexer.c
	gcc -c lexer.c

stack.o: stack.c
	gcc -c stack.c

clean: 
	rm -rf *.out *.dSYM *.o parser