#Two Pass Assembler
The program assembles a given Assembly code into sic machine code, when provided with the optab and source file

##Usage
```
g++ twopass.cpp -o twopass
./twopass
```

##Algorithm
###Pass 1
```
begin
	read first input line
	if OPCODE = 'START' then
		begin
			save #[OPERAND] as starting address
			iniliase LOCCTR to starting address
			write line to intermediate file
			read next input line
		end {if START}
	else
		initlise LOCCTR to 0
	while OPCODE != 'END' do
		begin
			if this is not a comment line then
				begin
					if there is a symbol in the LABEL field then
						begin
							search SYMTAB for LABEL
							if found then
								set error flag (duplicate symbol)
							else
								insert (LABEL,LOCCTR) into SYMTAB
						end {if symbol}
					search OPTAB for OPCODE
					if found then
						add 3 { instruction length } to LOCCTR
					else if OPCODE = 'WORD' then
						add 3 to LOCCTR
					else if OPCODE = 'RESW' then
						add 3 * #[OPERAND] to LOCCTR
					else if OPCODE = 'RESB' then
						add #[OPERAND] to LOCCTR
					else if OPCODE = 'BYTE' then
						begin
							find length of constant in bytes
							add length to LOCCTR
						end { if BYTE }
					else
						set error flag (invalid operation code)
				end {if not comment}
			write line ot intermediate file
			read next input line
		end {while not END}
	write last line to intermediate file
	save (LOCCTR - starting address) as program length
end {PASS 1}
```

###Pass 2
```
begin
	read first input line {from intermediate file}
	if OPCODE = 'START' then
		begin
			write listing line
			read next input line
		end {if START}
	write Header record to object program
	initialize first Text record
	while OPCODE != 'END' do
		begin
			if this is not a comment line then
				begin
					search OPTAB for OPCODE
					if found then
						begin
							if there is a symbol in OPERAND field then
								begin
									search SYMTAB for OPERAND
										if found then
											store symbol value as operand address
										else
											begin
												store 0 as operand address
												set error flag (undefined symbol)
											end
								end {if symbol}
							else
								store 0 as operand address
							assemble the object code instructions
						end {if opcode found}
					else if OPCODE = 'BYTE' or 'WORD' then
						convert constant to object code
					if object code will not fit into the current Text Record
						begin
							write Text record to object program
							initialise new Text record
						end
					add object code to Text record
				end {if not comment}
			write listing line
			read next input line
		end {while not END}
	write last Text record to object program
	write End record to object program
	write last listing line
end {PASS 2}
```

##Sample Input
```
COPY START 1000
 LDA ALPHA
 ADD BETA
 STA GAMMA
EOF BYTE C'EOF'
. this is a comment
HEX BYTE X'F0'
ALPHA WORD 1
BETA WORD 2
GAMMA RESW 4096
END COPY 
```

##Sample Intermediary
```
1000 COPY START 1000
1000  LDA ALPHA
1003  ADD BETA
1006  STA GAMMA
1009 EOF BYTE C'EOF'
100c HEX BYTE X'F0'
100d ALPHA WORD 1
1010 BETA WORD 2
1013 GAMMA RESW 4096
```

##Sample Output
```
H^COPY^001000^003013
T^001000^13^00100d^6B1010^501013^454f46^F0^000001^000002
E^001000

```
