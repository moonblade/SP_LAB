#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <ctype.h>
#include <stack>
#include <list>

using namespace std;
map<string,string> type;
list<string> pf;
bool sisalnum(string a)
{
	return find_if(a.begin(),a.end(),[](char t){return (!isalnum(t)||t=='.');})==a.end();
}
bool sisnum(string a)
{
	return find_if(a.begin(),a.end(),[](char t){return (!isdigit(t)||t=='.');})==a.end();
}
int isp(string a)
{
	if(a=="+") return 3;
	if(a=="-") return 3;
	if(a=="*") return 5;
	if(a=="/") return 5;
	if(a=="^") return 7;
	if(a=="=") return 0;
	if(a=="(") return 1;
}
int icp(string a)
{
	if(a=="+") return 3;
	if(a=="-") return 3;
	if(a=="*") return 5;
	if(a=="/") return 5;
	if(a=="^") return 6;
	if(a=="=") return 0;
	if(a=="(") return 9;
	if(a==")") return 1;
}

string postFix(string expression)
{
	stack<string> op;
	string postFix="",element;
	stringstream s(expression);
	while(getline(s,element,' '))
	{
		if(sisalnum(element)){
			postFix+=element+" ";
		}
		else
		{
			if(op.empty()||isp(op.top())<icp(element))
				op.push(element);
			else
			{
				while(!op.empty() && isp(op.top())>=icp(element))
				{
					if(op.top()!="(")
						postFix+=op.top()+" ";
					else
					{
						op.pop();
						break;
					}
					op.pop();
				}
				if(element!=")")
					op.push(element);
			}
		}
	}
	while(!op.empty()){
		postFix+=op.top()+" ";
		op.pop();
	}
	return postFix;
}
int main(int argc, char const *argv[])
{
	freopen("input2","r",stdin);
	string line,element,temp,expression;
	while(getline(cin,line))
	{
		expression="";
		stringstream s(line);
		while(getline(s,element,' '))
		{
			if(sisnum(element)){
				type[element]="int";
			}

			if(element=="int"||element=="float")
				{
					temp=element;
					getline(s,element,' ');
					type[element]=temp;
				}
			expression+=element+" ";
		}
		pf.push_back(postFix(expression));
	}
	for(auto i=pf.begin(); i!=pf.end();++i)
	{
		string postfix=*i,element;
		stack<string> alpha;
		stringstream s(postfix);
		int t=0;
		while(getline(s,element,' '))
		{
			if(sisalnum(element))
			{
				alpha.push(element);
			}
			else
			{
				string a = alpha.top();alpha.pop();
				string b = alpha.top();alpha.pop();
				if(element=="=")
				{
					if(type[a]!=type[b])
						if(type[a]=="int")
							a="intToFloat("+a+")";
					cout<<b<<" "<<element<<" "<<a<<endl;
				}
				else
				{
					if(type[a]!=type[b])
					{
						if(type[a]=="int")
							a="intToFloat("+a+")";
						else
							b="intToFloat("+b+")";
					}
					cout<<"t"<<t<<" = "<<b<<" "<<element<<" "<<a<<endl;
					if(type[b]=="float"||type[a]=="float")
						type["t"+to_string(t)]="float";
					else
						type["t"+to_string(t)]="int";

					alpha.push("t"+to_string(t++));
				}
			}
		}
	}
	// for(auto i=type.begin();i!=type.end();++i)
		// cout<<i->first<<" "<<i->second<<endl;
	return 0;
}
