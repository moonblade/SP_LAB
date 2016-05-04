#include <iostream>
#include <map>
#include <list>
#include <set>
#include <ctype.h>
#include <sstream>

using namespace std;

int isT(char a)
{
	return !isupper(a);
}
/*
{E,{{E,+,T},{T}}
*/
class Op
{
public:
	set<char> first;
	set<char> last;
	list<list<char> > prod;
	void print()
	{
		cout<<"firstop : ";
		for(auto i=first.begin();i!=first.end();++i)
			cout<<*i<<", ";
		cout<<"\nlastop : ";
		for(auto i=last.begin();i!=last.end();++i)
			cout<<*i<<", ";
		cout<<endl;
	}
	void build()
	{
		for(auto i=prod.begin();i!=prod.end();++i)
		{
			for(auto j=i->begin();j!=i->end();++j){
				first.insert(*j);
				if(isT(*j))
					break;
			}
			for(auto j=i->rbegin();j!=i->rend();++j){
				last.insert(*j);
				if(isT(*j))
					break;
			}

		}
	}	
};
map<char,Op> nt;
set<char> t;
map<char,map<char,char> > table;

pair<char,list<list<char> > > getProduction(string line)
{
	pair<char,list<list<char> > > retPair; 
	retPair.first=line[0];
	string pString;
	stringstream s(line.substr(2,line.length()-2));
	while(getline(s,pString,'|'))
	{
		list<char> prod;
		for(int i=0;i<pString.length();++i)
		{	
			if(isT(pString[i]))
				t.insert(pString[i]);
			prod.push_back(pString[i]);
		}
		retPair.second.push_back(prod);
	}
	return retPair;

}
void closure()
{
	for(auto i=nt.begin();i!=nt.end();++i)
	{
		char expand=i->first;
		auto a=i->second.first;
		auto b=i->second.last;
		for(auto j=nt.begin();j!=nt.end();++j)
		{
			auto c=j->second.first;
			if(c.find(expand)!=c.end())
				for(auto k=a.begin();k!=a.end();++k)
					j->second.first.insert(*k);
			auto d=j->second.last;
			if(d.find(expand)!=d.end())
				for(auto k=b.begin();k!=b.end();++k)
					j->second.last.insert(*k);
			
		}
	}
}
void printfl()
{
	for(auto i=nt.begin();i!=nt.end();++i)
	{
		cout<<endl<<i->first<<endl<<"firstop : ";
		for(auto j=i->second.first.begin();j!=i->second.first.end();j++)
			cout<<*j<<", ";
		cout<<endl<<"lastop : ";
		for(auto j=i->second.last.begin();j!=i->second.last.end();j++)
			cout<<*j<<", ";

	}
}
void remove_nt()
{
	for(auto i=nt.begin();i!=nt.end();++i)
	{
		for(auto j=i->second.first.begin();j!=i->second.first.end();)
			if(!isT(*j))
				j=i->second.first.erase(j);
			else
				j++;
		for(auto j=i->second.last.begin();j!=i->second.last.end();)
			if(!isT(*j))
				j=i->second.last.erase(j);
			else
				j++;

	}	
}
void fillstart(char s)
{
	auto a=nt[s].first;
	auto b=nt[s].last;
	for(auto j=a.begin();j!=a.end();++j)
		table['$'][*j]='<';
	for(auto j=b.begin();j!=b.end();++j)
		table[*j]['$']='>';
}
void createTable()
{
	for(auto i=nt.begin();i!=nt.end();i++)
	{
		char a=i->first;
		auto b=i->second.prod;
		for(auto j=b.begin();j!=b.end();++j)
		{
			for(auto k=j->begin();k!=j->end();++k)
				{
					auto next=k;
					auto after=k;
					if(next!=j->end()){
						next++;
						after++;
						if(next!=j->end()){
							after++;
							if(after==j->end())
								after--;
						}
						else
						{
							next--;
							after--;
						}
					}
					auto b=nt[*next].first;
					auto c=nt[*k].last;
					if(isT(*k)&&!isT(*next))
					{
						for(auto l=b.begin();l!=b.end();l++)
							table[*k][*l]='<';
						if(isT(*after))
							table[*k][*after]='=';
					}
					if(!isT(*k)&&isT(*next))
						for(auto l=c.begin();l!=c.end();l++)
							table[*l][*next]='>';
					if(k!=next&&isT(*k)&&isT(*next))
						table[*k][*next]='=';
				}	
		}
	}
}
void printtable()
{
	cout<<endl;
	for(auto i=t.begin();i!=t.end();++i)
		cout<<"  "<<*i;
	cout<<endl;
	for(auto i=t.begin();i!=t.end();++i)
	{
		cout<<*i<<"  ";
		for(auto j=t.begin();j!=t.end();++j)
		{
			if(table[*i][*j]==0)
				cout<<"  ";
			else
				cout<<table[*i][*j]<<"  ";
		}
		cout<<endl;
		
	}
}
int main(int argc, char const *argv[])
{
	freopen("etGrammer","r",stdin);
	string line,el;
	char start;
	while(getline(cin,line))
	{
		auto p = getProduction(line);
		if(nt.size()==0)
			start=p.first;
		nt[p.first].prod=p.second;
		nt[p.first].build();	
	}
	t.insert('$');
	
	closure();
	remove_nt();
	fillstart(start);
	createTable();
	printfl();
	printtable();
	return 0;
}