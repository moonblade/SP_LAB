#include "iostream"
// Regular expressions to find mathing tokens
#include "regex"
#include "fstream"
#include "string"
// For string remove
#include "algorithm"
using namespace std;
// List of regexes to be used on each line
map<string,regex> regexes;
// List of tokens found in each line, ordered to the position they were found in
map<int,string> tokens;
int main()
{
	// Open input file
	ifstream f("input");
	// create a regex with the given string, name=preprocessor regex=#
	regexes["preprocessor"]=regex("#");
	// all of the keywords
	regexes["keyword"]=regex("(include)|(cout)|(int)|(float)");
	// Strings, anything enclosed within " and ", '.' is any character other than \n
	regexes["string_constant"]=regex("\".*\"");
	// Header files are varibles followed by .h
	regexes["headerFile"]=regex("[a-zA-Z_][a-zA-Z0-9_]*.h");
	// starts with letter,underscore and has only letter, digit and underscore
	regexes["variable"]=regex("[a-zA-Z_][a-zA-Z0-9_]*");
	// operators in cpp
	regexes["operator"]=regex("<<|<|>|=|\\+|,");
	// function call without any arguments
	regexes["function"]=regex("[a-zA-Z_][a-zA-Z0-9_]*\\(\\)");
	// delimiter is seperated from operators
	regexes["delimiter"]=regex(";");
	// open and closing braces
	regexes["open_brace"]=regex("\\{");
	regexes["close_brace"]=regex("\\}");
	// A variable to hold the matched string from regex header
	smatch matched;
	string line,unedited;
	// For each line in input file, do
	while(getline(f,line,'\n'))
	{
		// Print the line
		cout<<endl<<line<<endl;
		// Copy the line to unedited, gonna remove stuff from line, untill it is nothing;
		unedited=line;
		// While there are still non whitespace characters in the line
		while(line.find_first_not_of(' ') != -1)
			// Loop through the iterator map and apply each on the line
			for(map<string,regex>::iterator riter=regexes.begin();riter!=regexes.end();riter++)
				// If a regex match is found, store it in matched
				if(regex_search(line,matched,riter->second)){
						// Add the match to tokens along with where it was matched
			            tokens[unedited.find(string(matched[0]))]="\t<"+riter->first + ", \"" + string(matched[0]) + "\">\n";
						// Erase matched token from line, for no more matching
			            line.erase(line.find(string(matched[0])),string(matched[0]).length());
				}
		// print the tokens in the line
		for(map<int,string>::iterator titer=tokens.begin();titer!=tokens.end();titer++)
			cout<<titer->second;
		// Clear tokens for next line
		tokens.clear();
	}
	// Close input file
	f.close();
	return 0;
}