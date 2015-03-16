CC = gcc
CFLAGS = -w -g -std=c99
DEPS = parser.o lexer.o stack.o

parser: $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $@

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean: 
	rm -rf *.out *.dSYM *.o parser