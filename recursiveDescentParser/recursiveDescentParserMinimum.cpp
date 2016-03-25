#include "iostream"
#include "string"
#include "fstream"
using namespace std;
void t(),e(),ebar(),tbar(),f();
string i;
int p = 0;
void e()
{
	t();
	ebar();
}
void ebar()
{
	if(i[p]=='+')
	{
		p++;
		t();
		ebar();
	}
}
void t()
{
	f();
	tbar();
}
void tbar()
{
	if(i[p]=='*')
	{
		p++;
		f();
		tbar();
	}
}
void f()
{
	if(i[p]=='(')
	{
		p++;
		e();
		if(i[p]==')')
			p++;
		else
			return;
	}
	else if(i[p]=='x')
		p++;
	else
		return;
}
int main()
{
	ifstream f("input");
	while(getline(f,i,'\n'))
	{
		p = 0;
		e();
		if(p==i.length())
			cout<<"Accepted : "<<i<<endl;
		else
			cout<<"Rejected : "<<i<<endl;
	}
	f.close();
	return 0;
}