all:
	bison parser.y -o parser.tab.c
	flex scanner.l
	g++ -c ast.cpp -o ast.cpp.o
	g++ -c codewriter.cpp -o codewriter.cpp.o
	g++ -c main.cpp -o main.cpp.o
	g++ -c symboltable.cpp -o symboltable.o
	g++  codewriter.cpp.o symboltable.o ast.cpp.o main.cpp.o -o compiler
clean:
	rm -rf *.o
	rm -rf *~
	rm -rf parser.tab.c
	rm -rf lex.yy.c
