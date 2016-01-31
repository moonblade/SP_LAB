#include<fstream>
#include<cstring>
#include<sstream>
#include<iostream>
#include<vector>
#include<iomanip>
#include<list>
#include<stdlib.h>
using namespace std;
fstream f,g;
int locctr=0,startaddress=0;
int linenumber=0;

// A class for each entry in symtab, 
// It consists of a label and address and a list of forward references
class symtabentry{
public:
	string label;
	int address;
	list<int> fwdref;
		// For inserting a new entry into symtab
	symtabentry(string label,int address)
	{
		this->label=label;
		this->address=address;
	}
		// When the address is not known, say in the case of forward reference
	symtabentry(string label, list<int> fwdref)
	{
		this->label=label;
		this->address=-1;
		this->fwdref = fwdref;
	}

		// For logging purpose, not of actual consequence
	void print()
	{
		cout<<label<<" "<<address;
		list<int>::iterator iter;
		for(iter=fwdref.begin();iter!=fwdref.end();++iter)
			cout<<" "<<*iter;
		cout<<endl;
	}
};
// Symtab as a list of sytabEntries
list<symtabentry> symtab;


/*
Need a list of textRecords, so a class textrecord is made
*/
class textRecord
{
public:
		// the start of the text record which is T^locctr 
	string start;
		// the size of the text record which is incremented every time a record is inserted
	int size;
	string record;
	textRecord()
	{
				// need to convert location counter to hex string to create new textRecord, so string stream is required
		stringstream s;
		s<<setw(6)<<setfill('0')<<hex<<locctr;
				// Start is set as T^ appended with the location counter in hex with 6 width length and filled with '0'
				// ie if location counter is 1A, it would become 00001A
		start = "T^" + s.str() + "^";
				// size starts as zero, upon each insertion size is increased and during printing, it is appended in hex as second element
		size=0;
				// the ^ after size is inserted in record, rest of the insertions are appended in record
		record = "^";
	}

		// During forward reference resolution, new textrecords are to be made to insert the forward referenced correction in the correct location
		// It can be grouped as a single constructor with defualat constructor, to avoid confusion two have been made
		/*
				consider, the following
						1000:            ADD  BETA                      T^001000^03^5B0000 // zero since beta is not known
						10003:  BETA WORD 2                             T^001001^02^1003   // 1001 ie 0000 is corrected to 1003 when value of beta is known
				 so T^1001^02^2001
		*/

				 textRecord(int locctr)
				 {
                                 	stringstream s;
				// here the locctr used is local one not global one, rest is same as earlier
                                 	s<<setw(6)<<setfill('0')<<hex<<locctr;    
				// Start is set as T^ appended with the location counter in hex with 6 width length and filled with '0'
				// ie if location counter is 1A, it would become 00001A
                                 	start = "T^" + s.str() + "^";
				// size starts as zero, upon each insertion size is increased and during printing, it is appended in hex as second element
                                 	size=0;
				// the ^ after size is inserted in record, rest of the insertions are appended in record
                                 	record = "^";    
				 }

// To insert an entry into the textRecord
				 int insertRecord(string record, int size)
				 {
                                 	if(this->size+size>30)
                                 		return 0;
                                 	this->record+=record+"^";
                                 	this->size+=size;
                                 	return 1;
				 }

// To return the string record for final printing
				 string getRecord()
				 {
                                 	stringstream s;
                                 	s<<setw(2)<<setfill('0')<<hex<<size;
                                 	return start+s.str()+record;
				 }

                             };

// The list of previous textRecords, they cannot be editted
                             list<textRecord> textRecords;


/*
A function to insert an entry into symtab
Cases for symtab edit and insert value:
		1. entry does not exist, value is known (no forward reference)
		2. entry already exists without value, insert value
		3. entry already exists with value, insert another value - ERROR
*/
// here locctr is the value of the label
		int insertToSymtab(string label,int locctr, textRecord &currentTextRecord)
		{
                	list<symtabentry>::iterator iter;
                	for(iter=symtab.begin();iter!=symtab.end();++iter)
                	{

                		if((*iter).label==label)
                		{
// Label already exists
                			{
                				if((*iter).address==-1)
                				{
												// No declaration found
                					(*iter).address = locctr;
                					if(currentTextRecord.size>0)
                					{
                						textRecords.push_back(currentTextRecord);
                						currentTextRecord = *new textRecord();
                					}
												// loop through the list of forward references and make a textRecord for each of them4
												// iterator to loop through the list
                					list<int>::iterator iterint;
                					stringstream s;
                					s<<setw(4)<<setfill('0')<<locctr;

                					for(iterint = (*iter).fwdref.begin(); iterint!=(*iter).fwdref.end();iterint++)
                					{
														// Create a new text record to insert
                						textRecord forwardReferenceTextRecord = *new textRecord(*iterint);
														// locctr is to be converted to 4 width string of bytesize=2 to insert into textRecord
                						forwardReferenceTextRecord.insertRecord(s.str(),2);
														// insert it into the list of textRecords
                						textRecords.push_back(forwardReferenceTextRecord);
                					}
                					return 1;
                				}
                				else
                				{
												// Redeclaration of symbol, ERROR
                					cout<<"ERROR - line "<<linenumber;
                					return 0;
                				}
                			}
                		}
                	}
				// Label does not exist, add as new entry
                	symtab.push_back(*new symtabentry(label,locctr));
                	return 1;
		}

		// Get value from symtab. will return -1 if value or entry does not exist Call insertForwardReference then
		int getSymbolValue(string label)
		{
			// Iterator to iterate through symtab
                	list<symtabentry>::iterator iter;
                	for(iter=symtab.begin();iter!=symtab.end();++iter)
                	{
				// return address if found(can be -1)
                		if((*iter).label==label)
                			return (*iter).address;
                	}
                	return -1;
		}

/*
Cases where forard reference is used
                1. entry does not exist, value is not known (forward reference to be added)
                2. entry already exists without value, insert another forward reference
*/

                void insertForwardReference(string label,int locctr)
                {
                	list<symtabentry>::iterator iter;
                	for(iter=symtab.begin();iter!=symtab.end();++iter)
                	{
                			// If label already exists, insert forward reference at end
                		if((*iter).label==label)
                		{
                			(*iter).fwdref.push_back(locctr);
                			return;
                		}
                	}
                	list<int> oneItemList;
                	oneItemList.push_back(locctr);
                	symtab.push_back(*new symtabentry(label,oneItemList));
                }

		string getHexOpcode(string opcode)
		{
                	g.seekg(0,ios::beg);
                	string line,opcod,value;
                	while(getline(g,line))
                	{
                		stringstream s(line);
                		getline(s,opcod,' ');
                		getline(s,value);
                		if(opcode==opcod)
                			return value;
                	}
                	return "null";

		}
		int main(int argc, char const *argv[])
		{
                	f.open("source",ios::in);
                	g.open("optab",ios::in);
                	string line,opHex;
                	string label,opcode,operand;
                	getline(f,line);
                	linenumber++;
                	stringstream s(line);
                	getline(s,label,' ');
                	getline(s,opcode,' ');
                	getline(s,operand,' ');
                	if(opcode=="START")
                	{
                		locctr=atoi(operand.c_str());
                		startaddress=locctr;
                		cout<<setw(4)<<setfill('0')<<hex<<locctr<<": "<<label<<" "<<opcode<<" "<<operand<<endl;
                		getline(f,line);
                		linenumber++;
                		stringstream s(line);
                		getline(s,label,' ');
                		getline(s,opcode,' ');
                		getline(s,operand,' ');

                	}

// Current textRecord to which insertion takes place
                	textRecord currentTextRecord;
                	while(getline(f,line))
                	{
// insertion length
                        int lengthCurrentEntry = 0;
                        // record to be inserted
                        string record="";
                		cout<<setw(4)<<setfill('0')<<hex<<locctr<<": "<<label<<" "<<opcode<<" "<<operand<<endl;
                		if(label!=""){
                			insertToSymtab(label,locctr,currentTextRecord);
                		}
                		if(getHexOpcode(opcode)!="null")
                		{
                                		record+=getHexOpcode(opcode);
                                	// if there is an operand
                                	if(operand!="")
                                	{
                                        	int value=getSymbolValue(operand)
                                        	if(value==-1)
                                        	{
                                        		// value does not exist, add forward reference to locctr + 1, since entry to change is in next byte
                                        		insertForwardReference(operand,locctr+1);
                                                        record+="0000";
                                        	}
                                                else
                                                {
                                                	// enter value as four hex bit wide string at the end of hex opcode
                                                		stringstream s;
                                                		s<<setw(4)<<setfill('0')<<hex<<value;
                                                		record+=s.str();
                                                }
                                	}
                                	else
                                	{
                                		// If there is no operand, operand is set as 0000
                                		record += "0000";
                                	}
                                        locctr+=3;
                		}
                		else if(opcode=="WORD")
                		{
                                		
                			locctr+=3;
                		}
                		else if(opcode=="RESW")
                		{
                			locctr+=3*atoi(operand.c_str());
                		}
                		else if(opcode=="BYTE")
                		{
                			int len=operand.length()-3;
                			if(operand[0]=='X')
                			{
                				len/=2;
                			}
                			else
                			{

                			}
                			locctr+=len;
                		}
                		else if(opcode=="RESB")
                		{
                			locctr+=atoi(operand.c_str());
                		}
                		else
                		{
                			cout<<"Error: line "<<linenumber;
                		}
                		linenumber++;
                		stringstream s(line);
                		getline(s,label,' ');
                		getline(s,opcode,' ');
                		getline(s,operand,' ');
                	}
                	f.close();
                	g.close();

                	return 0;
		}