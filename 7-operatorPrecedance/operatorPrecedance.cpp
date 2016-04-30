#include <iostream>
#include <stack>
#include <fstream>
using namespace std;
int icp(char a)
{
	switch(a)
	{
		case '+':
		case '-':return 5;
		case '*':
		case '/':return 10;
		case 'x':return 20;
		case '$':return 0;
		case '(':return 50;
		case ')':return 2;
	}
	return -1;
}

int isp(char a)
{
	switch(a)
	{
		case '+':
		case '-':return 7;
		case '*':
		case '/':return 12;
		case 'x':return 20;
		case '$':return 2;
		case '(':return 3;
	}
	return -1;
	
}

int main(int argc, char const *argv[])
{
	stack<char> st;
	ifstream f("input");
	string line;
	while(getline(f,line))
	{
		line="$"+line+"$";
		cout<<line<<" ";
		for(int i=0;i<line.length();++i)
		{
				if(st.empty() || isp(st.top())<icp(line[i])){
					// cout<<"push "<<line[i]<<endl;
					st.push(line[i]);
				}
				else
				{
					while(!st.empty() && isp(st.top())>icp(line[i]))
					{
							// cout<<"now "<<st.top()<<" "<<line[i]<<endl;
							// cout<<st.top();
							st.pop();
							if(!st.empty() && st.top()=='(')
								break;

					}
					// cout<<"now "<<st.top()<<" "<<line[i]<<endl;
					if(!st.empty() && isp(st.top())==icp(line[i]))
							break;
					// cout<<"push "<<line[i]<<endl;
					if(line[i]!=')')
						st.push(line[i]);
				}
		}
		if(st.top()=='$')
			{
				st.pop();
				if(st.empty())
					cout<<"Accepted\n";
				else
					cout<<"Rejected\n";
		}					
		else
			cout<<"Rejected\n";
	}
	return 0;
}