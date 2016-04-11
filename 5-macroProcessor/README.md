#Macro Processor
A 
macro name
is an abbreviation, which stands for some related lines of code. Macros 
are useful for the following purposes:
* to simplify and reduce the amount of repetitive coding
* to reduce errors caused by repetitive coding
* to make an assembly program more readable.

*Source Code* -> **Macro Processor** -> *Expanded Code* -> **Compiler or Assembler**

##Usage
```
g++ macroProcessor.cpp -o macroProcessor
./macroProcessor
```

##Algorithm
```
begin {macro processor}
	EXPANDING := FALSE
	while OPCODE != 'END' do
		begin
			GETLINE
			PROCESSLINE
		end {while}
end {macro processor}

procedure PROCESSLINE
	begin
		search NAMTAB for OPCODE
		if found then
			EXPAND
		else if OPCODE = 'MACRO' then
			DEFINE
		else write source line to expanded file
	end {PROCESSLINE}	

procedure DEFINE
	begin
		enter macro name into NAMTAB
		enter macro prototype into DEFTAB
		LEVEL := 1
		while LEVEL > 0 do
			begin
				GETLINE
				if this is not a comment line then
					begin
						substitute positional notaion for parameters
						enter line into DEFTAB
						if OPCODE = 'MACRO' then
							LEVEL := LEVEL + 1
						else if OPCODE = 'MEND' then
							LEVEL := LEVEL -1
					end {if not comment}
			end {while}
		store in NAMTAB pointers to beginning and end of definition
	end {DEFINE}
	
procedure EXPAND
	begin
		EXPANDING := TRUE
		get first line of macro definition {prototype} from DEFTAB
		set up arguments from macro invocation in ARGTAB
		write macro invocation to expanded file as a comment
		while not end of macro definition do
			begin
				GETLINE
				PROCESSLINE
			end {while}
		EXPANDING := FALSE
	end {EXPAND}

procedure GETLINE
	begin
		if EXPANDING then
			begin
				get next line of macro definition from DEFTAB
				substitute arguments from ARGTAB for positional notations
			end {if}
		else
			read next line from input file
	end {GETLINE}
```
	
##Sample Input
Formatted for clarity
```
COPY	START	0
RDBUFF	MACRO	&INDEV,&BUFADR,&RECLTH
		CLEAR	X
		CLEAR	A
		CLEAR	S
		+LDT	#4096
		TD		=X'&INDEV'
		JEQ		*-3
		RD		=X'&INDEV'
		COMPR	A,S
		JEQ		*+11
		STCH	&BUFADR,X
		TIXR	T
		JLT		*-19
		STX		&RECLTH
		MEND		
FIRST	STL		RETADR
CLOOP	RDBUFF	F1,BUFFER,LENGTH
		LDA		LENGTH
		COMP	#0
TEST	RDBUFF	F2,RETADR,THREE
EOF		BYTE	C'EOF'
THREE	WORD	3
RETADR	RESW	1
BUFFER	RESB	4096
		END		FIRST
```

##Sample Output
Formatted for clarity
```
COPY	START	0
FIRST	STL		RETADR
.CLOOP	RDBUFF	F1,BUFFER,LENGTH
		CLEAR	X
		CLEAR	A
		CLEAR	S
		+LDT	#4096
		TD		=X'F1'
		JEQ		*-3
		RD		=X'F1'
		COMPR	A,S
		JEQ		*+11
		STCH	BUFFER,X
		TIXR	T
		JLT		*-19
		STX		LENGTH
		LDA		LENGTH
		COMP	#0
.TEST	RDBUFF	F2,RETADR,THREE
		CLEAR	X
		CLEAR	A
		CLEAR	S
		+LDT	#4096
		TD		=X'F2'
		JEQ		*-3
		RD		=X'F2'
		COMPR	A,S
		JEQ		*+11
		STCH	RETADR,X
		TIXR	T
		JLT		*-19
		STX		THREE
EOF		BYTE	C'EOF'
THREE	WORD	3
RETADR	RESW	1
BUFFER	RESB	4096
```