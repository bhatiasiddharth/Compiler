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

- Create driver.c with command line arguments to show output for each stage
- Refactor AST algorithm to reduce passes
	- Check `Reltype` and `if`
- Check parse table again
- Error handling in parser
- ~~Create javascript tool for trees~~
- Use linked list in tree.c