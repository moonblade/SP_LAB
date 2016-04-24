#include "iostream"
#include "fstream"
#include "sstream"
#include "map"
using namespace std;
int main(int argc, char const *argv[])
{
	map<string,string> op={{"=","MOV"},{"+","ADD"},{"-","SUB"},{"*","MUL"},{"/","DIV"}};
	ifstream f("input");
	string line,element,first,operatr;
	stringstream s;
	while(getline(f,line)&&(s=stringstream(line))&&getline(s,first,' '))
		while(getline(s,element,' ')&&getline(s,operatr,' '))
			cout<<op[element]<<" "<<first<<", "<<operatr<<"\n";
	f.close();
	return 0;
}