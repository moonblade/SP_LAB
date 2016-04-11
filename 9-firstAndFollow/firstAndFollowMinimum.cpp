#include "iostream"
#include "string"
#include "fstream"
#include "sstream"
#include "ctype.h"
#include "map"
#include "set"
#include "list"
#define epsilon "epsilon"
#define terminator "$"
using namespace std;
class FnF
{
public:
	set<string> first;
	set<string> follow;
	FnF(){}
	FnF(string s, bool toFollow=0){toFollow?follow.insert(s):first.insert(s);}
	void print()
	{
		cout<<"First : ";
		for(set<string>::iterator iter=first.begin(); iter!=first.end();iter++)
			cout<<(*iter)<<", ";
		cout<<endl<<"Follow : ";
		for(set<string>::iterator iter=follow.begin(); iter!=follow.end();iter++)
			cout<<(*iter)<<", ";
		cout<<endl;
	}
};
map<string,FnF> elements;
map<string,list<list<string> > > productions;
bool isTerminal(string element){return !isupper(element[0]);}
pair<string,list<list<string> > > split(string line)
{
	pair<string,list<list<string> > > temp;	
	string element,productionString;
	stringstream s(line);
	s>>temp.first;
	s>>productionString;
	while(getline(s,productionString,'|'))
	{
		list<string> production;
		stringstream t(productionString);
		while(getline(t,element,' '))
		{
			if(element!="")
				production.push_back(element);
		}
		temp.second.push_back(production);
	}
	return temp;
}
int insertFirstOfNext(string variable,list<string>::iterator next,list<string>::iterator end)
{
	int repeat=0;
	if(next!=end)
		for(set<string>::iterator setItor=elements[*next].first.begin();setItor!=elements[*next].first.end(); setItor++)
			if(elements[variable].first.insert(*setItor).second)
				repeat=1;
			if(elements[*next].first.find(epsilon)!=elements[*next].first.end())
				if(insertFirstOfNext(variable,++next,end))
					repeat=1;
	return repeat;
}
int insertFollowFromNext(string superVariable,list<string>::iterator next,list<string>::iterator end)
{
	bool repeat = 0;
	string variable=*next;
	next++;
	if(next!=end)
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
	else
		for(set<string>::iterator iter=elements[superVariable].follow.begin();iter!=elements[superVariable].follow.end();iter++)
			if(elements[variable].follow.insert(*iter).second)
				repeat=1;
	return repeat;
}
void addFirstAndFollow()
{
	bool repeat=0,flag=1;
	for(map<string, list<list<string> > >::iterator iter = productions.begin(); iter!=productions.end(); iter++)
		for(list<list<string> >::iterator liter = (iter->second).begin(); liter!=(iter->second).end(); liter++)
		{
			flag=1;	
			for(list<string>::iterator siter = (*liter).begin(); siter!=(*liter).end(); siter++)
			{
				if(flag)
				{
					if(isTerminal(*siter))
					{
						if(elements.find(*siter)==elements.end())
							elements[*siter]=FnF(*siter);
						if(elements[iter->first].first.insert(*siter).second)
							repeat=1;
					}
					else
						repeat=insertFirstOfNext(iter->first,siter,(*liter).end());
					flag=0;
				}
				if(insertFollowFromNext(iter->first,siter,(*liter).end()))
					repeat=1;
			}
		}
	if(repeat) addFirstAndFollow();
}
int main()
{
	ifstream f("input");
	string line;
	while(getline(f,line,'\n'))
	{
		productions[split(line).first]=split(line).second;
		elements[split(line).first]=elements.size()?FnF():FnF(terminator,1);
	}
	addFirstAndFollow();
	for(map<string,FnF>::iterator iter=elements.begin();iter!=elements.end();iter++)
	{
		if(!isTerminal(iter->first))
		{
			cout<<iter->first<<endl<<string(iter->first.length(),'-')<<endl;
			iter->second.print();
			cout<<endl;
		}
	}
	f.close();
	return 0;
}