#include <iostream>
#include <stack>

#include <fstream>
using namespace std;
int isp(char a)
{
	switch(a)
	{
		case '+':
		case '-':return 1;
		case '*':
		case '/':return 2;
		case 'x':return 5;
		case '$':return 0;
	}
	return -1;
}

int icp(char a)
{
	switch(a)
	{
		case '+':
		case '-':return 1;
		case '*':
		case '/':return 2;
		case 'x':return 5;
		case '$':return 0;
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
					st.push(line[i]);
				}
				else
				{
					while(!st.empty() && isp(st.top())>=icp(line[i]))
						st.pop();
					st.push(line[i]);
				}
		}
		if(st.top()=='$')
			{
				st.pop();
				if(st.empty())
					cout<<"Accepted\n";
				else
					cout<<"Rejected";
		}					
		else
			cout<<"Rejected";
	}
	return 0;
}