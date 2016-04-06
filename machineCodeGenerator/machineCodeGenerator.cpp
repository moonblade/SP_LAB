#include "iostream"
#include "fstream"
#include "sstream"
#include "list"
using namespace std;
int main(int argc, char const *argv[])
{
	ifstream f("input");
	string line,element;
	while(getline(f,line))
	{
		list<string> linel;
		stringstream s(line);
		while(getline(s,element,' '))
			linel.push_back(element);
		for(list<string>::iterator iter=linel.begin(); iter!=linel.end(); iter++)
		{
			list<string>::iterator before=iter;
			before--;
			list<string>::iterator after=iter;
			after++;
			if(*iter=="=")
				cout<<"MOV "<<*before<<", "<<*after<<"\n";
			if(*iter=="+")
				cout<<"ADD "<<*linel.begin()<<", "<<*after<<"\n";
			if(*iter=="*")
				cout<<"MUL "<<*linel.begin()<<", "<<*after<<"\n";
			if(*iter=="-")
				cout<<"SUB "<<*linel.begin()<<", "<<*after<<"\n";
			if(*iter=="/")
				cout<<"DIV "<<*linel.begin()<<", "<<*after<<"\n";
		}
	}
	f.close();
	return 0;
}