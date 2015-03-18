CC = gcc
CFLAGS = -g -std=c99
DEPS = parser.o lexer.o stack.o tree.o ast.o compiler.o

run: compiler
	./compiler samples/01-sum.co && node tree-printer/main.js samples/01-sum-parse.tree samples/01-sum-syntax.tree && open -a "Google Chrome" samples/01-sum-parse.svg samples/01-sum-syntax.svg

compiler: $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $@

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean: 
	rm -rf *.out *.dSYM *.o compiler *.txt
	rm samples/*.tree samples/*.svg