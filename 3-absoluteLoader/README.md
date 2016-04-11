#Absolute Loader
Absolute Loader loads the object code into memory and makes correction for forward reference text records.

##Algorithm
```
begin
	read Header record
	verify program name and length
	read first Text record
	while record type != ‘E’ do
		begin
			{if object code is in character form, convert into
			internal representation}
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
Program Name   COPY

Memory Map 
1000 : 00
1001 : 10
1002 : 0d
1003 : 6B
1004 : 10
1005 : 13
1006 : 50
1007 : 10
1008 : 10
1009 : 45
100a : 4f
100b : 46
100c : F0
100d : 00
100e : 00
100f : 01
1013 : 00
1014 : 00
1015 : 02

Start Program At 1000
```