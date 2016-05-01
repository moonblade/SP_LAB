#include <iostream>
#include <map>
#include <map>
#include <list>
#include <set>
#include <sstream>
#include<ctype.h>
using namespace std;
map<char,pair<set<char>,set<char> > > fl;
map<char,map<char, char> > table;
list<pair<char,char> > tbeforent;
list<pair<char,char> > tafternt;
list<pair<char,char> > tntt;
set<char> t;
int isT(char a)
{
	return !isupper(a);
}
int main(int argc, char const *argv[])
{
	t.insert('$');
	freopen("etGrammer","r",stdin);
	// freopen("opGrammer","r",stdin);
	string line,element;
	while(getline(cin,line))
	{
		char s=line[0];
		line=line.substr(2,line.length()-2);	
		stringstream l(line);
		while(getline(l,element,'|'))
		{
			for(int i=0;i<element.length();++i)
				if(isT(element[i]))
					t.insert(element[i]);
			if(fl.find(s)==fl.end())
				fl[s]=*new pair<set<char>,set<char> >(*new set<char>,*new set<char>);
			char last=element[element.length()-1],first=element[0];
			for(int i=1;i<element.length();++i)
			{
				// terminal before non terminal
				if(!isT(element[i])&&isT(element[i-1]))
					tbeforent.push_back(*new pair<char,char>(element[i-1],element[i]));
				// terminal after non terminal
				else if(!isT(element[i-1])&&isT(element[i]))
					tafternt.push_back(*new pair<char,char>(element[i-1],element[i]));
			}
			// terminal enclosing non terminal t-nt-t or t-t
			for(int i=1;i<element.length();++i)
			{
				if(isT(element[i])&&isT(element[i-1]))
					tntt.push_back(*new pair<char,char>(element[i-1],element[i]));
			}
			for(int i=2;i<element.length();++i)
			{
				if(isT(element[i])&&!isT(element[i-1])&&isT(element[i-2]))
					tntt.push_back(*new pair<char,char>(element[i-2],element[i]));
			}
			
			// First Op
			if(isT(first))
				fl[s].first.insert(first);
			else
			{
				fl[s].first.insert(first);
				if(element.length()>1)
					fl[s].first.insert(element[1]);
			}
			// Last Op
			if(isT(last))
				fl[s].second.insert(last);
			else
			{
				fl[s].second.insert(last);
				if(element.length()>1)
					fl[s].second.insert(element[element.length()-2]);
			}
		}
	}
	// Warshalls closure
	for(auto i=fl.begin();i!=fl.end();i++)
	{
		char m=i->first;
		auto d=i->second.first;
		auto e=i->second.second;
		for(auto j=fl.begin();j!=fl.end();j++)
		{
			if(m!=j->first)
			{
				auto b=j->second.first;
				for(auto k=b.begin();k!=b.end();k++)
					if((*k)==m)
						for(auto l=d.begin();l!=d.end();l++)
								j->second.first.insert(*l);
				auto c=j->second.second;
				for(auto k=c.begin();k!=c.end();k++)
					if((*k)==m)
						for(auto l=e.begin();l!=e.end();l++)
							j->second.second.insert(*l);
			}
		}
	}
	// Remove nonterminals
	for(auto i=fl.begin();i!=fl.end();++i)
	{
		auto a=i->second.first;
		auto b=i->second.second;
		for(auto j=a.begin();j!=a.end();++j)
			if(!isT(*j))
				i->second.first.erase(*j);
		for(auto j=b.begin();j!=b.end();++j)
			if(!isT(*j))
				i->second.second.erase(*j);
	}
	//fill start
	// for(auto i=fl.begin();i!=fl.end();i++)
	{
		auto i=fl.begin();
		char a=i->first;
		for(auto j=i->second.first.begin(); j!=i->second.first.end();j++)
			{
				table['$'][*j]='<';
			}			
		for(auto j=i->second.second.begin(); j!=i->second.second.end();j++)
			{
				table[*j]['$']='>';
			}
	}
	// Make table
	for(auto i=tbeforent.begin();i!=tbeforent.end();i++)
	{
		char a=i->first;
		auto b=fl[i->second].first;
		for(auto j=b.begin();j!=b.end();++j)
			table[a][*j]='<';
		
	}
	for(auto i=tafternt.begin();i!=tafternt.end();i++)
	{
		char a=i->second;
		auto b=fl[i->first].second;
		for(auto j=b.begin();j!=b.end();++j)
			table[*j][a]='>';
	}
	for(auto i=tntt.begin();i!=tntt.end();i++)
	{
		char a=i->first;
		char b=i->second;
		if(table.find(a)==table.end())
			table[a]=*new map<char,char>();
		if(table[a][b]==0)
			table[a][b]='=';
	}
// Print
	cout<<"  ";
	for(auto i=t.begin();i!=t.end();i++)
		cout<<*i<<" ";
	cout<<endl;
	for(auto i=t.begin();i!=t.end();i++)
	{
		cout<<*i<<" ";
		for(auto j=t.begin();j!=t.end();++j)
		{
			if(table[*i][*j]==0)
				cout<<"  ";
			else 
				cout<<table[*i][*j]<<" ";
		}
		cout<<endl;
	}
	//print firstop and lastop
	// for(auto i=fl.begin();i!=fl.end();i++)
	// {
	// 	cout<<i->first<<endl;
	// 	for(auto j=i->second.first.begin(); j!=i->second.first.end();j++)
	// 		cout<<*j<<", ";
	// 	cout<<endl;
	// 	for(auto j=i->second.second.begin(); j!=i->second.second.end();j++)
	// 		cout<<*j<<", ";
	// 	cout<<endl<<endl;
	// }
	return 0;
}