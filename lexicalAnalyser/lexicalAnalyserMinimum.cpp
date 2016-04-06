#include "iostream"
#include "regex"
#include "fstream"
#include "string"
#include "algorithm"
using namespace std;
map<string,regex> regexes;
map<int,string> tokens;
int main(int argc, char const *argv[])
{
	ifstream f("input");
	regexes["preprocessor"]=regex("#");
	regexes["keyword"]=regex("(include)|(cout)|(int)|(float)");
	regexes["string_constant"]=regex("\".*\"");
	regexes["headerFile"]=regex("[a-zA-Z_][a-zA-Z0-9_]*.h");
	regexes["variable"]=regex("[a-zA-Z_][a-zA-Z0-9_]*");
	regexes["operator"]=regex("<<|<|>|=|\\+|,");
	regexes["function"]=regex("[a-zA-Z_][a-zA-Z0-9_]*\\(\\)");
	regexes["delimiter"]=regex(";");
	regexes["brace"]=regex("\\{|\\}");
	smatch matched;
	string line,unedited;
	while(getline(f,line,'\n'))
	{
		cout<<endl<<(unedited=line)<<endl;
		while(line.find_first_not_of(' ') != std::string::npos)
			for(map<string,regex>::iterator riter=regexes.begin();riter!=regexes.end();riter++)
				if(regex_search(line,matched,riter->second)){
			            tokens[unedited.find(string(matched[0]))]="\t<"+riter->first + ", \"" + string(matched[0]) + "\">\n";
			            line.erase(line.find(string(matched[0])),string(matched[0]).length());
				}
		for(map<int,string>::iterator titer=tokens.begin();titer!=tokens.end();titer++)
			cout<<titer->second;
		tokens.clear();
	}
	f.close();
	return 0;
}