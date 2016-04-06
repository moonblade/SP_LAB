#include "iostream"
#include "fstream"
#include "sstream"
#include "string"
#include "algorithm"
#include "ctype.h"
#include "stack"
using namespace std;

stack<string> expression;
string postfixExpression="";
bool sisalnum(const std::string &str)
{
    return find_if(str.begin(), str.end(), [](char c) { return (!isalnum(c)); }) == str.end();
}
int isp(string e)
{
	if(e=="+"){return 2;}
	else if(e=="-"){return 2;}
	else if(e=="*"){return 3;}
	else if(e=="/"){return 3;}
	else if(e=="^"){return 4;}
	else if(e=="("){return 6;}
	else if(e==")"){return 1;}
	else if(e=="="){return 0;}
	return 0;
}
int icp(string e)
{
	if(e=="+"){return 2;}
	else if(e=="-"){return 2;}
	else if(e=="*"){return 3;}
	else if(e=="/"){return 3;}
	else if(e=="^"){return 5;}
	else if(e=="("){return 7;}
	else if(e==")"){return 1;}
	else if(e=="="){return 0;}
	return 0;
}
int main(int argc, char const *argv[])
{
	ifstream f("input");
	string line,element;
	int i=0;
	while(getline(f,line))
	{
		stringstream s(line);
		while(getline(s,element,' '))
		{
			if(sisalnum(element))
				postfixExpression+=element+" ";
			else if(expression.empty() || icp(element)>isp(expression.top()))
				expression.push(element);
			else
			{
				while(!expression.empty() && isp(expression.top())>=icp(element))
				{
					if(expression.top()!="(")
						postfixExpression+=expression.top()+" ";
					else{
						expression.pop();
						break;
					}
					expression.pop();
				}
				if(element!=")")
					expression.push(element);
			}
		}
		while(!expression.empty())
		{
			postfixExpression+=expression.top()+" ";
			expression.pop();
		}
		stringstream ss(postfixExpression);
		while(getline(ss,element,' '))
			if(sisalnum(element))
				expression.push(element);
			else
			{
				string one=expression.top(); expression.pop();
				string two=expression.top(); expression.pop();
				string result;
				if(element=="="){
					result = two+element+one;
				}
				else{
					cout<<"t"+to_string(i)+"="+two+element+one+"\n";
					result="t"+to_string(i);
				}
					expression.push(result);
				i++;
			}
		while(!expression.empty())
		{
			cout<<expression.top();
			expression.pop();
		}
		cout<<endl;
		postfixExpression="";
	}
	return 0;
}