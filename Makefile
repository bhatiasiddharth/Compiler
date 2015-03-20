CC = gcc
CFLAGS = -g -std=c99
DEPS = parser.o lexer.o stack.o tree.o ast.o compiler.o symtab.o analyzer.o

SAMPLE = samples/01-sum
run: compiler
	./compiler.out $(SAMPLE).co 
	# && node tree-printer/main.js $(SAMPLE)-parse.tree $(SAMPLE)-syntax.tree && open -a "Google Chrome" $(SAMPLE)-parse.svg $(SAMPLE)-syntax.svg

compiler: $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $@.out

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean: 
	rm -rf *.out *.dSYM *.o compiler samples/*.tree samples/*.svg *.tokens