#include "iostream"
#include "fstream"
#include "string"
#include "sstream"
#include "list"
using namespace std;
int p(string input)
{
	if(input=="$"){return 0;}
	if(input=="x"){return 5;}
	if(input=="+"){return 1;}
	if(input=="-"){return 1;}
	if(input=="*"){return 2;}
	if(input=="/"){return 2;}
	return -1;
}
int main(int argc, char const *argv[])
{
	ifstream f("input");
	string line,element;
	while(getline(f,line))
	{
		stringstream s(line);
		list<string> expression;
		expression.push_back("$");
		while(getline(s,element,' '))
			expression.push_back(element);
		expression.push_back("$");
		for(auto iter=expression.begin();iter!=expression.end();iter++)
			{
				auto after=iter; after++;	
				if(after!=expression.end())
				{
					if(p(*iter)<=p(*after))
						cout<<*iter<<"<";
					else if(p(*iter)>p(*after))
						cout<<*iter<<">";
				}
				else
					cout<<*iter;
			}
			cout<<endl;
	}
	return 0;
}