#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <list>
#include <iomanip>

using namespace std;
map<string,string> optab;
map<string,int> symtab;
class Line
{
public:
	string label,opcode,operand;
	Line(string l,string o, string op):label(l),opcode(o),operand(op){}
};
Line readline(istream &f)
{
	string line="",l,o,op;
	while(line.length()==0)
		getline(f,line);
	stringstream s(line);
	getline(s,l,' ');
	getline(s,o,' ');
	getline(s,op,' ');
	return Line(l,o,op);
}

int main(int argc, char const *argv[])
{
	ifstream f("optab");
	int locctr=0,saddress,psize;
	string opcode,operand,label;
	while(getline(f,opcode,' ')&&getline(f,operand))
		optab[opcode]=operand;
	f.close();
	// freopen("source","r",stdin);
	Line line = readline(cin);
	if(line.opcode=="START")
	{
		stringstream s,t;
		t<<setw(4)<<setfill('0')<<hex<<line.operand;
		t>>locctr;
		saddress=locctr;
		cout<<t.str()+" "+line.label+" "+line.opcode+" "+line.operand<<endl;
		line=readline(cin);
	}
	while(line.opcode!="END")
	{
		if(line.label=="" || line.label[0]!='.')
		{
			stringstream t;
			t<<setw(4)<<setfill('0')<<hex<<locctr;
			cout<<t.str()+" "+line.label+" "+line.opcode+" "+line.operand<<endl;
			if(line.label!="")
				symtab[line.label]=locctr;
			if(optab[line.opcode]!="")
			{
				locctr+=3;
			}
			else if(line.opcode=="BYTE")
			{
				int toAdd = line.operand.length()-3;
				if(line.operand[0]=='X')
					toAdd/=2;
				locctr+=toAdd;
			}
			else if(line.opcode=="RESW"||line.opcode=="RESB")
			{
				int toAdd;
				stringstream toHex;
				toHex<<hex<<line.operand;
				toHex>>toAdd;
				if(line.opcode=="RESW")
					toAdd*=3;
				locctr+=toAdd;
			}
			else if(line.opcode=="WORD")
				locctr+=3;
		}
		line=readline(cin);
	}
	psize=locctr-saddress;
	stringstream t;
	t<<setw(4)<<setfill('0')<<hex<<locctr;
	cout<<t.str()+" "+line.label+" "+line.opcode+" "+line.operand<<endl;
	symtab["progSize"]=psize;
	ofstream g("symtab");
	for(auto i=symtab.begin();i!=symtab.end();i++)
		g<<i->first<<" "<<i->second<<endl;
	g.close();
	return 0;
}