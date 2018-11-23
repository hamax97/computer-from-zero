all:
	bison parser.y
	flex scanner.l 
	g++ -c ast.cpp -o ast.cpp.o
	g++ -c main.cpp -o main.cpp.o
	g++  ast.cpp.o main.cpp.o -o compiler
clean:
	rm -rf *.o
	rm -rf *~
	rm -rf parser.tab.c
	rm -rf lex.yy.c
	rm -rf *.xml
