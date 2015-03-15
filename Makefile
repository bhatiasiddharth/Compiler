parser: parser.o lexer.o stack.o
	gcc -g parser.o lexer.o stack.o -o parser

parser.o: parser.c
	gcc -g -c parser.c

lexer.o: lexer.c
	gcc -g -c lexer.c

stack.o: stack.c
	gcc -g -c stack.c

clean: 
	rm -rf *.out *.dSYM *.o parser