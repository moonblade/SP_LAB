#include "iostream"
#include "fstream"
#include "sstream"
#include "list"
#include <map>

using namespace std;
int main(int argc, char const *argv[])
{
	map<string,string> op;
	op["="]="MOV";
	op["+"]="ADD";
	op["-"]="SUB";
	op["*"]="MUL";
	op["/"]="DIV";
	ifstream f("input");
	string line,element;
	while(getline(f,line))
	{
		list<string> linel;
		stringstream s(line);
		string first;
		getline(s,first,' ');
		while(getline(s,element,' '))
		{
			string a;
			getline(s,a,' ');
			cout<<op[element]<<" "<<first<<", "<<a<<"\n";
		}

	}
	f.close();
	return 0;
}