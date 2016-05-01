#include "iostream"
#include "string"
#include "fstream"
#include "sstream"
// For replace function
#include "algorithm"
// the deftab is going to be a map of string(macro name) and a list(lines of macro)
#include "map"
// lines of macro inside deftab
#include "list"
using namespace std;
// File input and output
ifstream inFile;
ofstream outFile;
// A class to split string lines into three, as label opcode and operand
class Line
{
public:
	string label,opcode,operand;
	// Initialise label, opcode and operand with values specified in the constructor
	Line(){}
	Line(string l,string o,string op):label(l),opcode(o),operand(op){}
	// To convert to a single string for writing to file
	string toString()
	{
		return label+" "+opcode+" "+operand+"\n";
	}
};
/*
The deftab is consisting of a map between a string and a list of lines
the string is the macro name and the list of lines is its definition including the definition line, but not the mend line
deftab[macroName] would return its definition list
*/
map<string,list<Line> > deftab;
/*
This procedure gets the next line from the inputFile
unlike the algorithm, this does not take the next line from deftab, since deftab is a list it is easier to get lines from it directly instead of through a function
*/
Line getLine()
{
	string l,o,op;
	// Take line till first space
	getline(inFile,l,' ');
	// Till next space
	getline(inFile,o,' ');
	// Till endl character
	getline(inFile,op,'\n');
	// Make a line from the split label, opcode and operand and return it
	return Line(l,o,op);
}

/*
This function is used for pushing lines into deftab, when MACRO is found as opcode, define function is called
It can even handle a macro definition inside a macro definition by making use of level int variable, instead of a boolean one
*/
void define(Line line)
{
	// init level as 1
	int level = 1;
	// push the header line to deftab (for later positional parameter replacement)
	deftab[line.label].push_back(line);
	// Loop till level is 0, ie till the outer most macro definition is complete
	while(level>0)
	{
		// get the next line from input file
		Line defLine = getLine();
		// If line is a macro definition add level, if its mend, 
	 	if (defLine.opcode=="MACRO")
	 		level+=1;
	 	else if(defLine.opcode=="MEND")
	 		level-=1;
		// If line is not mend or macro, add it to deftab
 		deftab[line.label].push_back(defLine);
	}
}

/*
This function is used to replace a macro call invocation with its corresponding macro
only works if the macro is defined before the invocation
it takes the first line from deftab and the first line from macro invocation, compares the operand field and finds out which parameters to be replaced with what
for each string in the deftab, it replaces actual parameters in place of formal ones
*/
void expand(Line line)
{
	// Write invocation line as comment 
	outFile<<"."+line.toString();
	// Create a stringstream from operand field of first line of deftab
	stringstream from(deftab[line.opcode].begin()->operand);
	
	// Create a stringstream from operand field of macro invocation
	stringstream to(line.operand);
	// Replacement maps deftab parameter --> invocation parameter replacement relation
	map<string,string> replacement;
	// For printing deftab
	map<string,string> replacementop;
	// For printing argtab (for order preservation)
	list<string> rep;
	// for each string in the parameter list, add fromString and toString to the list
	string fromString,toString;
	int p=1;
	while(getline(from,fromString,',') && getline(to,toString,',')){
		replacement[fromString]=toString;
		cout<<p<<" "<<toString<<endl;
		// Deftab replacements
		replacementop[fromString]="?"+to_string(p++);
		// preserve order with list
		// rep.push_back(fromString);
	}
	// Print argtab by looping thorugh list and printing its replacement
	// int k=0;
	// for(auto i=rep.begin();i!=rep.end();++i)
	// {
	// 	cout<<++k<<" "<<replacement[*i]<<endl;
	// }
	// cout<<endl;

	// print deftab changing replacementop
	cout<<deftab[line.opcode].begin()->toString();
	for(auto iter=++deftab[line.opcode].begin();iter!=deftab[line.opcode].end();iter++)
	{
		// Get the string equivalent of the line from deftab
		string toWrite = (*iter).toString();
		// Replace all occurances of each element in parameter list
		// Loop through parameter list
		for(auto miter=replacementop.begin();miter!=replacementop.end();miter++)
		{
			// To find all occurances, replace each occurances of each parameter in each line 
			int position = toWrite.find(miter->first); 
			while (position != -1)
			{
			 toWrite.replace(position, miter->first.size(), miter->second); 
			 position = toWrite.find(miter->first);
			}
		}
		cout<<toWrite;
	}
	// Iterate through the lines in deftab
	for(auto iter=++deftab[line.opcode].begin();iter!=deftab[line.opcode].end();iter++)
	{
		// Get the string equivalent of the line from deftab
		string toWrite = (*iter).toString();
		// Replace all occurances of each element in parameter list
		// Loop through parameter list
		for(auto miter=replacement.begin();miter!=replacement.end();miter++)
		{
			// To find all occurances, replace each occurances of each parameter in each line 
			int position = toWrite.find(miter->first); 
			while (position != -1)
			{
			 toWrite.replace(position, miter->first.size(), miter->second); 
			 position = toWrite.find(miter->first);
			}
		}
		// After parameters have been replaced, write to file
		outFile<<toWrite;
	}
}

/* 
The processline function decides what to do with a particular line, it basically operates on three conditions
If the line is macro definition, call define, if the line is macro invocation, call expand, else write to output
*/
void processLine(Line line)
{
	// If deftab has an entry for opcode, ie the entry in deftab is before the end of the list(inside the list)
	if(deftab.find(line.opcode)!=deftab.end())
		expand(line);
	else if(line.opcode=="MACRO")
		define(line);
	else
		outFile<<line.toString();
}
int main(int argc, char const *argv[])
{
	// Open input and output files
	inFile.open("input");
	outFile.open("output");
	// Create an empty line
	Line line;
	// Loop till opcode is END, and process each line
	while((line=getLine()).opcode!="END")
		processLine(line);
	// Close the files, really important
	inFile.close();
	outFile.close();
	return 0;
}