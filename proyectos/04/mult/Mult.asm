// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

	// Put your code here.
	@i      //RAM[6151]
	M=1     //i = 1
	@2      //RAM[2]
	M=0     //RAM[2] = 0
(LOOP)          
	@i
	D=M     //D=i
	@1
	D=D-M   //D=i - R1
	@END
	D;JGT   //IF i > R1 GOTO END
	@0
	D=M     //D = RAM[0]
	@2
	M=M+D   //R2 = R2+R0
	@i
	M=M+1   //++i
	@LOOP
	0;JMP
(END)
	0;JMP
	
	
	
