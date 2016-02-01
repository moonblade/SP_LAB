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
// location counter and starting address default value
int locctr=0,startaddress=0;
// line number in case of errors
int linenumber=0;

// A class for each entry in symtab,
// It consists of a label and address and a list of forward references
class symtabentry{
    public:
    string label;
    int address;
    list<int> fwdref;
    // For inserting a new entry into symtab, without any previous entries
    symtabentry(string label,int address)
    {
        this->label=label;
        this->address=address;
    }
    // for inserting entry without value but with forward references
    symtabentry(string label, list<int> fwdref)
    {
        this->label=label;
        this->address=-1;
        this->fwdref = fwdref;
    }
};
// Symtab is a list of sytabEntries
// push_back function to this list inserts to symtab
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
    // rest of the record appended together
    string record;
    
    // Crete a new textRecord as continuation, so location counter is chosen as start
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
    
    // During forward reference resolution, new textrecords are to be made to insert the forward referenced correction in the correct location, so start is the given address
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
        // textRecord maximum size is 1E
        if(this->size+size>30)
        return 0;
        // Insert if maximum size is not violated
        this->record+=record+"^";
        this->size+=size;
        return 1;
    }
    
    // To return the string record for final printing
    string toString()
    {
        // Don't return if it is an empty textRecord
        if(size==0)
        return "";
        stringstream s;
        // convert size to hex string of two bit width
        s<<setw(2)<<setfill('0')<<hex<<size;
        // remove the last ^ before returning
        return start+s.str()+record.substr(0,record.length()-1);
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
here locctr is the value of the label
*/
int insertToSymtab(string label,int locctr, textRecord &currentTextRecord)
{
    list<symtabentry>::iterator iter;
    for(iter=symtab.begin();iter!=symtab.end();++iter)
    {
        
        if((*iter).label==label)
        {
            // Label already exists, it may or may not have a value
            {
                if((*iter).address==-1)
                {
                    // No declaration found, ie there is no value for it, which means there was a forward reference so make textRecords for forward references
                    (*iter).address = locctr;
                    if(currentTextRecord.size>0)
                    {
                        // print the previous textRecord
                        textRecords.push_back(currentTextRecord);
                        currentTextRecord = *new textRecord();
                    }
                    // loop through the list of forward references and make a textRecord for each of them
                    // iterator to loop through the list
                    list<int>::iterator iterint;
                    stringstream s;
                    s<<setw(4)<<setfill('0')<<hex<<locctr;
                    
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
                    cout<<"ERROR : REDECLARATION OF VARIABLE" + label + " - line "<<dec<<linenumber;
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
    // No label exists, create one with -1 as address, and add fwdref to it
    list<int> oneItemList;
    oneItemList.push_back(locctr);
    symtab.push_back(*new symtabentry(label,oneItemList));
}

// Search the optab and find the hex equivalent of the opcode, if not found, return null
string getHexOpcode(string opcode)
{
    g.seekg(0,ios::beg);
    string line,opcod,value;
    while(getline(g,line))
    {
        // loop through each line in optab
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
    // open source and optab
    f.open("source",ios::in);
    g.open("optab",ios::in);
    // strings required in the procedure
    string line,opHex;
    string headerRecord,endRecord;
    string label,opcode,operand;
    // get the first line, to check for start
    getline(f,line);
    linenumber++;
    stringstream s(line);
    // split the line
    getline(s,label,' ');
    getline(s,opcode,' ');
    getline(s,operand,' ');
    if(opcode=="START")
    {
        // if start line is defined
        stringstream str;
        str<<setw(6)<<setfill('0')<<hex<<operand;
        str>>locctr;
        startaddress=locctr;
        // Create header record, without size
        headerRecord="H^"+label+"^"+str.str()+"^"; // append size here
        cout<<setw(4)<<setfill('0')<<hex<<locctr<<": "<<label<<" "<<opcode<<" "<<operand<<endl;
        // Get next line since, first line is already processed, and split it
        getline(f,line);
        linenumber++;
        stringstream s(line);
        getline(s,label,' ');
        getline(s,opcode,' ');
        getline(s,operand,' ');
        
    }
    
    // Current textRecord to which insertion takes place
    textRecord currentTextRecord;
    // Loop till end of file
    while(getline(f,line))
    {
        // the next line is prefetched, but previous line is processed here
        // If not a comment line
        if(label[0]!='.')
        {
            
            // insertion length
            int lengthCurrentEntry = 0; // for uniformity
            bool breakOff = 0;
            // record to be inserted
            string record="";
            // print the line
            cout<<setw(4)<<setfill('0')<<hex<<locctr<<": "<<label<<" "<<opcode<<" "<<operand<<endl;
            if(label!=""){
                // If a label is found, immediately insert it to symtab
                insertToSymtab(label,locctr,currentTextRecord);
            }
            // Try to get hex equivalent of opcode
            if(getHexOpcode(opcode)!="null")
            {
                // opcode is valid
                lengthCurrentEntry=3;
                record+=getHexOpcode(opcode);
                // if there is an operand
                if(operand!="")
                {
                    // get operand address
                    int value=getSymbolValue(operand);
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
            }
            // opcode is assembler directive
            else if(opcode=="WORD")
            {
                // insert operand as 6 hex bit wide in textRecord
                stringstream s;
                s<<setw(6)<<setfill('0')<<hex<<operand;
                record=s.str();
                lengthCurrentEntry=3;
            }
            else if(opcode=="RESW")
            {
                // When resw is encountered, the textRecord is broken off and a new one is made with the size reserved in between
                lengthCurrentEntry = 3*atoi(operand.c_str());
                breakOff = 1;
            }
            else if(opcode=="BYTE")
            {
                int len=operand.length()-3;
                if(operand[0]=='X')
                {
                    // Get substring excluding X'' and directly write to record
                    // substring from second index, length = total length - 3
                    record=operand.substr(2,len);
                    len/=2;
                }
                else
                {
                    // take the substring and convert each letter into correspoding hex value
                    string toConvert = operand.substr(2,len);
                    for(int i=0;i<toConvert.length();++i)
                    {
                        // convert to int
                        int intValueOfChar = toConvert[i];
                        // convert int back to hex string
                        stringstream s;
                        s<<hex<<intValueOfChar;
                        record+=s.str();
                    }
                }
                // length of current entry, for uniformity
                lengthCurrentEntry=len;
            }
            else if(opcode=="RESB")
            {
                // same as resw, just one third the fun
                lengthCurrentEntry=atoi(operand.c_str());
                breakOff=1;
            }
            else
            {
                // Invalid opcode encountered
                cout<<"Error: INVALID OPCODE " + opcode + " line "<<dec<<linenumber;
                return 0;
            }
            locctr+=lengthCurrentEntry;
            if(breakOff==1)
            {
                // break of textRecord in case of reserve statements
                textRecords.push_back(currentTextRecord);
                currentTextRecord = *new textRecord();
            }
            else
            {
                // Try to insert into current
                if(!currentTextRecord.insertRecord(record,lengthCurrentEntry))
                {
                    // if space is limited create a new textRecord and save last one
                    textRecords.push_back(currentTextRecord);
                    currentTextRecord = *new textRecord();
                    currentTextRecord.insertRecord(record,lengthCurrentEntry);
                }
            }
        }
        // increase line no.
        linenumber++;
        // split next line, which was prefetched
        stringstream s(line);
        getline(s,label,' ');
        getline(s,opcode,' ');
        getline(s,operand,' ');
    }
    // push the last record
    textRecords.push_back(currentTextRecord);
    // close files
    f.close();
    g.close();
    
    // open output file
    f.open("output",ios::out);
    
    // Complete the header record with program size = locctr - startAdress in hex with 6 bit width
    stringstream headerCompleter;
    int sizeOfProgram = locctr-startaddress;
    headerCompleter<<setw(6)<<setfill('0')<<hex<<sizeOfProgram;
    headerRecord+=headerCompleter.str();
    
    cout<<headerRecord<<endl;
    f<<headerRecord<<endl;
    
    // Iterate though textRecords and print each one, the toString function takes care of its size
    list<textRecord>::iterator iterator;
    for(iterator=textRecords.begin(); iterator!=textRecords.end();iterator++)
    {
        if((*iterator).toString()!="")
        {
            // print only if non Empty
            cout<<(*iterator).toString()<<endl;
            f<<(*iterator).toString()<<endl;
        }
    }
    
    // Create Endrecord
    stringstream endRecordCompleter;
    endRecordCompleter<<setw(6)<<setfill('0')<<hex<<startaddress;
    endRecord = "E^"+endRecordCompleter.str();
    
    cout<<endRecord<<endl;
    f<<endRecord<<endl;
    
    // Successful Execution
    return 0;
}



/*
SAMPLE INPUT
=============
COPY START 1000
LDA ALPHA
ADD BETA
STA GAMMA
EOF BYTE C'EOF'
. this is a comment
HEX BYTE X'F0'
ALPHA WORD 1
GAMMA RESW 1
BETA WORD 2
END COPY

SAMPLE OPTAB
============
LDA 00
STA 50
ADD 6B

SAMPLE OUTPUT
============
1000: COPY START 1000
1000:  LDA ALPHA
1003:  ADD BETA
1006:  STA GAMMA
1009: EOF BYTE C'EOF'
100c: HEX BYTE X'F0'
100d: ALPHA WORD 1
1010: GAMMA RESW 1
1013: BETA WORD 2

H^COPY^001000^000016
T^001000^0d^000000^6B0000^500000^454f46^F0
T^001001^02^100d
T^00100d^03^000001
T^001007^02^1010
T^001004^02^1013
T^001013^03^000002
E^001000
*/


//MOONBLADE TM