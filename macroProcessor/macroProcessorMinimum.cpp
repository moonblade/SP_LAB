#include "iostream"
#include "string"
#include "fstream"
#include "sstream"
#include "algorithm"
#include "map"
#include "list"
using namespace std;
fstream inF,outF;
class Line
{
public:
	string label,opcode,operand;
	Line(string l,string o,string op):label(l),opcode(o),operand(op){}
	string toString(){return label+" "+opcode+" "+operand+"\n";}
};
map<string,list<Line> > deftab;
Line getLine()
{
	string l,o,op;
	getline(inF,l,' ');
	getline(inF,o,' ');
	getline(inF,op,'\n');
	return Line(l,o,op);
}
void define(Line line)
{
	int level = 1;
	deftab[line.label].push_back(line);
	while(level>0)
	{
		Line defLine = getLine();
		if(defLine.opcode!="MEND")
	 		deftab[line.label].push_back(defLine);
	 	else
	 		level-=1;
	}
}
void expand(Line line)
{
	outF<<"."+line.toString();
	stringstream from((*(deftab[line.opcode].begin())).operand),to(line.operand);
	map<string,string> replacement;
	string fromString,toString;
	while(getline(from,fromString,',') && getline(to,toString,','))
		replacement[fromString]=toString;
	for(list<Line>::iterator iter=++deftab[line.opcode].begin();iter!=deftab[line.opcode].end();iter++)
	{
		string toWrite = (*iter).toString();
		for(map<string,string>::iterator miter=replacement.begin();miter!=replacement.end();miter++)
		{
			int position = toWrite.find(miter->first); 
			while (position != -1)
			{
			 toWrite.replace(position, miter->first.size(), miter->second); 
			 position = toWrite.find(miter->first);
			}
		}
		outF<<toWrite;
	}
}
void processLine(Line line)
{
	if(deftab.find(line.opcode)!=deftab.end())
		expand(line);
	else if(line.opcode=="MACRO")
		define(line);
	else
		outF<<line.toString();
}
int main(int argc, char const *argv[])
{
	inF.open("input",ios::in);
	outF.open("output",ios::out);
	Line line=Line("","","");
	while((line=getLine()).opcode!="END")
		processLine(line);
	inF.close();
	outF.close();
	return 0;
}