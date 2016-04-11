#One pass Assembler
The program as```
sembles the given assembly code into sic machine code in a single pass, when the optab is give

##Algorithm
```
begin
	read first input line
	if OPCODE = 'START' the
		begin
			save #[OPERAND] as starting address
			initialize LOCCTR as starting address
			read next input line
		end {if START}
	else
		initialize LOCCTR to 0
	while OPCODE != 'END' do
		begin
			if this is not a comment line then
				begin
					if there is a symbol in the LABEL field then
						begin
							search SYMTAB for LABEL
							if found then
								begin
									if symbol value is null
										set symbol value as LOCCTR and search the linked list with the corresponding operand PTR addresses and generate operand addressses as corresponding symbol values. Set symbol value as LOCCTR in symbol table and delete the linked list
								end {if found}
							else
								insert (LABEL,LOCCTR) into SYMTAB
						end {if SYMBOL}
					search OPTAB for OPCODE
					if found then
						begin
							search SYMTAB for OPERAND address
							if found then
								if symbol value not equal to null then
									store symbol value as OPERAND address
								else
									insert at the end of the linked list with a node with address as LOCCTR
							else
								insert (symbol name,null)
							add 3 to LOCCTR
						end {if found OPCODE}
					else if OPCODE = 'WORD' then
						add 3 to LOCCTR and convert constant to object code
					else if OPCODE ='RESW' then
						add 3 * #[OPERAND] to LOCCTR
					else if OPCODE = 'RESB' then
						add #[OPERAND] to LOCCTR
					else if OPCODE = 'BYTE' then
						begin
							find length of constant in bytes
							add length to LOCCTR
							convert constant to object code
						end
					if object code will not fit into current Text record then
						begin
							write text record to object program
							initialize new text record
						end
					add object code to Text record
				end {if not comment}
			write listing line
			read next input line
		end {if not END}
	write last Text record to object program
	write End record to object program
	write last listing line
end {PASS 1}
```

##Usage
```
g++ onePassAssembler.cpp -o onePassAssembler
./onePassAssembler
```

##Sample Input
```
COPY START 1000
 LDA ALPHA
 STA ALPHA
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

##Sample Output
```
H^COPY^001000^003016
T^001000^10^000000^500000^6B0000^500000^454f46^F0
T^001001^02^1010
T^001004^02^1010
T^001010^03^000001
T^001007^02^1013
T^001013^03^000002
T^00100a^02^1016
E^001000
```
