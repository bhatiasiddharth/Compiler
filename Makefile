CC = gcc
CFLAGS = -g -std=c99
DEPS = parser.o lexer.o stack.o tree.o ast.o compiler.o symtab.o analyzer.o codegen.o

SAMPLE = samples/05-scoping

chrome:	run
	@open -a "Google Chrome" $(SAMPLE)-syntax.svg $(SAMPLE).symbols $(SAMPLE).asm

run: compiler
	@./compiler.out $(SAMPLE).co

create-tree: $(SAMPLE)-syntax.tree $(SAMPLE)-parse.tree
	@node tree-printer/main.js $(SAMPLE)-syntax.tree

compiler: $(DEPS)
	@$(CC) $(CFLAGS) $(DEPS) -o $@.out

%.o: %.c %.h
	@$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.out *.dSYM *.o compiler samples/*.tree samples/*.svg samples/*.tokens samples/*.symbols Group_15.T2 Group_15.T2.tar

submit: clean
	./submit.sh
