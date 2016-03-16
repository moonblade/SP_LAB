#Relocating Loader
Relocating Loader loads the object code into memory, at available memory location and makes correction for forward reference text records, and changes calls to variables by incrementing program start with it.

##Algorithm
```
begin
	read Header record
	verify program name and length
	read first Text record
	get memory start location for program into progStart
	while record type != ‘E’ do
		begin
			{if object code is in character form, convert into
			internal representation}
			get bitmask of text record
			for each recordEntry where bitmask is one do
				begin
					recordEntry = recordEntry + progStart
				end
			move object code to specified location in memory
			read next object program record
		end
	jump to address specified in End record
end
```

##Sample Input
```
H^  COPY^000000^000016
T^000000^0d^e00^10000d^6B0013^500010^454f46^F0
T^00000d^03^000^000001
T^000013^03^000^000002
E^000000
```

##Sample Output
```
Enter program start address : 2000

Program Name   COPY

Memory Map 
2000 : 10
2001 : 20
2002 : 0d
2003 : 6b
2004 : 20
2005 : 13
2006 : 50
2007 : 20
2008 : 10
2009 : 45
200a : 4f
200b : 46
200c : F0
200d : 00
200e : 00
200f : 01
2013 : 00
2014 : 00
2015 : 02

Start Program At 2000
```