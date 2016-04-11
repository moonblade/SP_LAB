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
list<string> rm(string in)
{
	list<string> out;
	bool f=0;
	for(int i=0;i<in.length();i++)
		if((in[i]!='>' && in[i]!='<') && !(in[i]!='$' && in[i+1]=='>' && in[i-1]=='<'))
					out.push_back(string(1,in[i]));
	return out;
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
		while(expression.size()>2)
		{
			string ex="";
			for(auto iter=expression.begin();iter!=expression.end();iter++)
				{
					auto after=iter; after++;	
					if(after!=expression.end())
					{
						if(p(*iter)<=p(*after))
							ex+=*iter+"<";
						else if(p(*iter)>p(*after))
							ex+=*iter+">";
					}
					else
						ex+=*iter;
				}
				cout<<ex;
				expression=rm(ex);
				cout<<endl;
		}
		cout<<endl;
	}
	return 0;
}