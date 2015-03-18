CC = gcc
CFLAGS = -g -std=c99
DEPS = parser.o lexer.o stack.o tree.o ast.o compiler.o

run: compiler
	./compiler < samples/01-sum.co > parse_tree.txt
	node tree-printer/main.js parse_tree.txt
	open -a "Google Chrome" parse_tree.svg

compiler: $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $@

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean: 
	rm -rf *.out *.dSYM *.o parser *.txt