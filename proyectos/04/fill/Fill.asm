// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
(INFINITE_LOOP)
	@24576    //KBD
	D=M   		
	@0
	D=D-A     
	@INFINITE_LOOP
	D;JEQ      //IF D == 0 GOTO INFINITE_LOOP

(BLACK)
	@i
	M=0          //i = 0
(BLACK_WHILE)
	@i
	D=M     //D=i
	@8191     //MAX NUMBER OF PIXELS
	D=D-A
	@INFINITE_LOOP
	D;JGT      //IF i > 8191 THEN GOTO INFINITE_LOOP
	@24576    //KBD
	D=M
	@0
	D=D-A
	@WHITE
	D;JEQ    //IF KBD == 0 THEN GOTO WHITE
	@i
	D=M
	M=M+1    //i++
	@16384     //SCREEN
	A=A+D    //RAM[SCREEN + i]
	M=-1     //RAM[SCREEN + i] = -1
	@BLACK_WHILE
	0;JMP

(WHITE)
	@i
	M=0    //i = 0
(WHITE_WHILE)
	@i
	D=M    //D = i
	@8191      //MAX NUMBER OF PIXELS
	D=D-A
	@INFINITE_LOOP
	D;JGT     //IF i > 8191 THEN GOTO INFINITE_LOOP
	@24576      //KBD
	D=M
	@0
	D=D-A
	@BLACK
	D;JGT      //IF KBD != 0 THEN GOTO BLACK
	@i
	D=M
	M=M+1      //i++
	@16384     //SCREEN
	A=A+D      //RAM[SCREEN + i]
	M=0        //RAM[SCREEN + i] = 0
	@WHITE_WHILE  
	0;JMP
