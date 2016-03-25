#include "iostream"
#include "string"
#include "fstream"
#include "sstream"
#include "ctype.h"
#include "map"
#include "set"
#include "list"
using namespace std;
#define seperator "-->"
#define epsilon "epsilon"
#define terminator "$"
class FnF
{
public:
	set<string> first;
	set<string> follow;
	FnF(){}
	FnF(string s, bool toFollow=0)
	{
		if(toFollow)
			follow.insert(s);
		else
			first.insert(s);
	}
	void print()
	{
		cout<<"First : ";
		for(set<string>::iterator iter=first.begin(); iter!=first.end();iter++)
			cout<<(*iter)<<", ";
		cout<<endl;
		cout<<"Follow : ";
		for(set<string>::iterator iter=follow.begin(); iter!=follow.end();iter++)
			cout<<(*iter)<<", ";
		cout<<endl;
	}
	
};
map<string,FnF> elements;
map<string,list<list<string> > > productions;

bool isTerminal(string element)
{
	return !isupper(element[0]);
}

string getNonTerminal(string line)
{
	stringstream s(line);
	string nonterminal;
	getline(s,nonterminal,' ');
	return nonterminal;
}

list<list<string> > getProductions(string line)
{
	list<list<string> > productions;
	int lengthToSkip = line.find(seperator)+string(seperator).length();
	string produnctionLine = line.substr(lengthToSkip,line.length()-lengthToSkip);
	stringstream s(produnctionLine);
	string productionString;
	while(getline(s,productionString,'|'))
	{
		list<string> production;
		string element;
		stringstream t(productionString);
		while(getline(t,element,' '))
		{
			if(element!="")
				production.push_back(element);
		}
		productions.push_back(production);
	}
	return productions;
}

int insertFirstOfNext(string variable,list<string>::iterator next,list<string>::iterator end)
{
	int repeat=0;
	if(next!=end)
	{
		for(set<string>::iterator setItor=elements[*next].first.begin();setItor!=elements[*next].first.end(); setItor++)
			if(elements[variable].first.insert(*setItor).second)
				repeat=1;
			if(elements[*next].first.find(epsilon)!=elements[*next].first.end())
				if(insertFirstOfNext(variable,++next,end))
					repeat=1;
	}
	return repeat;
}

void createFirstSet()
{
	bool repeat=0;
	for(map<string, list<list<string> > >::iterator iter = productions.begin(); iter!=productions.end(); iter++)
	{
		string variable = iter->first;
		for(list<list<string> >::iterator liter = (iter->second).begin(); liter!=(iter->second).end(); liter++)
		{
			for(list<string>::iterator siter = (*liter).begin(); siter!=(*liter).end(); siter++)
			{
				if(isTerminal(*siter))
				{
					if(elements.find(*siter)==elements.end())
						elements[*siter]=FnF(*siter);
					if(elements[variable].first.insert(*siter).second)
						repeat=1;
					break;
				}
				else
				{
					repeat=insertFirstOfNext(variable,siter,(*liter).end());
					break;
				}
			}
		}
	}
	if(repeat)
		createFirstSet();
}

int insertFollowFromNext(string superVariable,string variable,list<string>::iterator next,list<string>::iterator end)
{
	bool repeat = 0;
		
	if(next!=end)
	{
		// cout<<variable<<endl;
		if(isTerminal(*next) && elements[variable].follow.insert(*next).second)
			repeat=1;			
		else
		{	
			for(set<string>::iterator iter=elements[*next].first.begin();iter!=elements[*next].first.end();iter++)
				if(*iter!=epsilon && elements[variable].follow.insert(*iter).second)
						repeat = 1;
			if(elements[*next].first.find(epsilon)!=elements[*next].first.end())
				for(set<string>::iterator iter=elements[*next].follow.begin();iter!=elements[*next].follow.end();iter++)
					if(elements[variable].follow.insert(*iter).second)
						repeat=1;
		}
	}
	else
	{
		for(set<string>::iterator iter=elements[superVariable].follow.begin();iter!=elements[superVariable].follow.end();iter++)
			if(elements[variable].follow.insert(*iter).second)
				repeat=1;
	}
	return repeat;
}

void createFollowSet()
{
	bool repeat = 0;
	for(map<string, list<list<string> > >::iterator iter = productions.begin(); iter!=productions.end(); iter++)
	{
		string variable = iter->first;
		for(list<list<string> >::iterator liter = (iter->second).begin(); liter!=(iter->second).end(); liter++)
		{
			for(list<string>::iterator siter = (*liter).begin(); siter!=(*liter).end(); siter++)
			{
				list<string>::iterator next = siter;
				next++;
				if(insertFollowFromNext(variable,*siter,next,(*liter).end()))
					repeat=1;
			}
		}
	}
	if(repeat)
		createFollowSet();
}

int main()
{
	ifstream f("input");
	string line;
	getline(f,line,'\n');
	productions[getNonTerminal(line)]=getProductions(line);
	elements[getNonTerminal(line)]=FnF(terminator,1);
	while(getline(f,line,'\n'))
	{
		productions[getNonTerminal(line)]=getProductions(line);
	}
	createFirstSet();
	createFollowSet();
	for(map<string,FnF>::iterator iter=elements.begin();iter!=elements.end();iter++)
	{
		if(!isTerminal(iter->first))
		{
			cout<<iter->first<<endl;
			cout<<string(iter->first.length(),'-')<<endl;
			iter->second.print();
			cout<<endl;
		}
	}
	f.close();
	return 0;
}
