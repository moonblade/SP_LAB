#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <list>
#include <iomanip>
#include <algorithm>

using namespace std;
map<string,string> optab;
map<string,string> symtab;
list<pair<int, string> > trs;
class Line
{
public:
	string loc,label,opcode,operand;
	Line(string lo,string l,string o, string op):loc(lo),label(l),opcode(o),operand(op){}
};
Line readline(istream &f)
{
	string line="",lo,l,o,op;
	while(line.length()==0)
		getline(f,line);
	stringstream s(line);
	getline(s,lo,' ');
	getline(s,l,' ');
	getline(s,o,' ');
	getline(s,op,' ');
	return Line(lo,l,o,op);
}

int main(int argc, char const *argv[])
{

	ifstream f("optab");
	int locctr=0,saddress,psize,loc;
	string opcode,operand,label,hr,er,start;
	pair<int,string> tr(0,"");
	while(getline(f,opcode,' ')&&getline(f,operand))
		optab[opcode]=operand;
	f.close();
	ifstream g("symtab");
	while(getline(g,opcode,' ')&&getline(g,operand))
	{
		int b=strtol(operand.c_str(),NULL,10);
		stringstream s;
		s<<hex<<b;
		symtab[opcode]=s.str();
	}
	g.close();
	// freopen("intermediary","r",stdin);
	Line line = readline(cin);
	hr="H^ DEFLT^000000^";
	if(line.opcode=="START")
	{
		stringstream s,t;
		if(line.label=="")
			line.label="DEFLT";
		t<<setw(6)<<setfill(' ')<<hex<<line.label;
		s<<setw(6)<<setfill('0')<<hex<<line.loc;
		psize=strtol(symtab["progSize"].c_str(),NULL,10);
		start=s.str();
		s>>locctr;
		tr.first=locctr;
		hr="H^"+t.str()+"^"+s.str()+"^";
		cout<<hr<<setw(6)<<setfill('0')<<psize<<endl;
		line=readline(cin);
	}
	while(line.opcode!="END")
	{
		stringstream t;
		t<<setw(6)<<setfill('0')<<hex<<line.loc;
		t>>locctr;
		if(optab[line.opcode]!="")
		{
			tr.second+="^"+optab[line.opcode];
			if(line.operand!="")
			{
					if(symtab[line.operand]=="")
					{
						cout<<"ERROR undefined variable "<<line.operand;
						return 1;
					}
				tr.second+=symtab[line.operand];
			}
			else
				tr.second+="0000";
		}
		else if(line.opcode=="BYTE")
		{
			int toAdd = line.operand.length()-3;
			tr.second+="^";
			stringstream s;
			if(line.operand[0]=='C')
				for(int i=2;i<line.operand.length()-1;i++) s<<hex<<(int) line.operand[i];
			if(line.operand[0]=='X')
			{	
				s<<line.operand.substr(2,toAdd);
				toAdd/=2;
			}
			tr.second+=s.str();
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
			trs.push_back(tr);
			locctr+=toAdd;
			tr =*new pair<int,string>(locctr,"");
		}
		else if(line.opcode=="WORD"){
			stringstream t;
			t<<setw(6)<<setfill('0')<<line.operand;
			tr.second+="^"+t.str();			
		}
		line=readline(cin);
	}
	trs.push_back(tr);
	for(auto i=trs.begin();i!=trs.end();i++)
	{
		if(i->second.length()>0)
		{
			cout<<"T^"<<setw(6)<<setfill('0')<<i->first<<"^";
			string temp=i->second;
			temp.erase(remove(temp.begin(), temp.end(),'^'),temp.end());
			cout<<setw(2)<<setfill('0')<<hex<<temp.length()/2;
			cout<<i->second<<endl;
		}
	}

	cout<<"E^"<<start<<endl;	
	return 0;
}