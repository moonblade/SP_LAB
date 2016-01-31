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
class symtabentry{
public:
	string label;
	int address;
	list<int> fwdref;
	symtabentry(string label,int address)
	{
		this->label=label;
		this->address=address;
	};
	symtabentry(string label)
	{
		this->label=label;
		this->address=-1;
	}
        void print()
        {
        	cout<<label<<" "<<address;
        	list<int>::iterator iter;
        	for(iter=fwdref.begin();iter!=fwdref.end();++iter)
        		cout<<" "<<*iter;
                cout<<endl;
        }
};
list<symtabentry> symtab;

void insertToSymtab(string label,int locctr)
{
	symtab.push_back(*new symtabentry(label,locctr));
        list<symtabentry>::iterator iter;
        for(iter=symtab.begin();iter!=symtab.end();++iter)
        {
        	// (*iter).print();
        }
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
	int linenumber=0;
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

        while(getline(f,line))
        {
		cout<<setw(4)<<setfill('0')<<hex<<locctr<<": "<<label<<" "<<opcode<<" "<<operand<<endl;
                if(label!=""){
                        insertToSymtab(label,locctr);
                }
                if(getHexOpcode(opcode)!="null")
                {
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