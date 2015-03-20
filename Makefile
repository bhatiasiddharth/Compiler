CC = gcc
CFLAGS = -g -std=c99
DEPS = parser.o lexer.o stack.o tree.o ast.o compiler.o symtab.o analyzer.o

SAMPLE = samples/01-sum
run: compiler
	./compiler.out $(SAMPLE).co 

chrome:	run
	node tree-printer/main.js $(SAMPLE)-parse.tree $(SAMPLE)-syntax.tree 
	google-chrome $(SAMPLE).tokens $(SAMPLE)-parse.svg $(SAMPLE)-syntax.svg $(SAMPLE).symbols

compiler: $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $@.out

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean: 
	rm -rf *.out *.dSYM *.o compiler samples/*.tree samples/*.svg samples/*.tokens samples/*.symbols Group_15.T2 Group_15.T2.tar

submit: clean
	./submit.sh