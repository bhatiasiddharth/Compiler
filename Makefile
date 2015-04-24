CC = gcc
CFLAGS = -g -std=c99
DEPS = parser.o lexer.o stack.o tree.o ast.o symtab.o analyzer.o codegen.o compiler.o

SAMPLE = samples/06-prime
.PHONY = debug

run: compiler
	@./compiler.out $(SAMPLE).co

chrome:	run create-tree
	@google-chrome $(SAMPLE)-syntax.svg

create-tree: $(SAMPLE)-syntax.tree $(SAMPLE)-parse.tree
	@node tree-printer/main.js $(SAMPLE)-syntax.tree

compiler: $(DEPS)
	@$(CC) $(CFLAGS) $(DEPS) -o $@.out

exec: run
	@scripts/masm.sh $(SAMPLE).asm

%.o: %.c %.h
	@$(CC) -c $(CFLAGS) $<

debug:
	lldb ./compiler.out $(SAMPLE).co

clean:
	@rm -rf *.out *.dSYM *.o compiler samples/*.tree samples/*.out samples/*.asm samples/*.svg samples/*.tokens samples/*.symbols Group_15.T2 Group_15.T2.tar

submit: clean
	scripts/submit.sh