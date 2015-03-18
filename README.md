Stage 1
========

- Grammer

Stage 2 
========
Deadline - March 20

- Lexical Analyzer
	- Print tokens
	- Report lexical errors
- Parser
- Abstract Syntax Tree
- Symbol Table

Plan / list your semantic rules of your language (March 25)

Stage 3 
========
Deadline - April 20

- Semantic Analyzer
- Code Generation

----

Instructions
=============

	make && ./parser < samples/01-sum.co

---

TODO
=====

- Put Token in tree
- Create driver.c with command line arguments to show output for each stage
- Use twin buffer for lexer
- Refactor AST algorithm to reduce passes
	- Check `Idlist`
- Check parse table again
- Refactor parser error messages.
- Use linked list in tree.c
- Error Handling overall
- Handle float, boolean, grid

- ~~Error handling in parser:Not required~~
- ~~Create javascript tool for trees~~
- ~~Javascript calling from Makefile~~