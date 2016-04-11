#include "iostream"
#include "string"
#include "fstream"
using namespace std;
void t();
void e();
void ebar();
void tbar();
void f();
/*
The grammer is of the form

E  --> E+T | T
T  --> T*F | F
F  --> (E) | x
----------------------
Eliminating left recursion we get the form

E  --> TE'
E' --> +TE' | epsilon 
T  --> FT'
T' --> *FT' | epsilon
F  --> (E) | x
*/
/*
Each production is converted into a function, 
non terminals are function names so E->TE' would be e(){t();ebar();}
but when a terminal comes, the input symbol is checked and if it matches, advanced, else errored
*/
// The current input will be in the string input
string input;
// A global variable for current string pointer, ie the current input symbol
int position = 0;
/*
E  --> TE'
function e calling other functions t and ebar
*/
void e()
{
	t();
	ebar();
}

/*
E' --> +TE' | epsilon
if input symbol is +, advance and call other functions, else do nothing since ebar could be epsilon
*/
void ebar()
{
	if(input[position]=='+')
	{
		//Advance to next input symbol
		position++;
		t();
		ebar();
	}
}
/*
T  --> FT'
function t calling other functions f and tbar
*/
void t()
{
	f();
	tbar();
}
/*
T' --> *FT' | epsilon
if input symbol is *, advance and call other functions, else do nothing since tbar could be epsilon
*/
void tbar()
{
	if(input[position]=='*')
	{
		//Advance to next input symbol
		position++;
		f();
		tbar();
	}
}
/*
F  --> (E) | x
if input symbol is '(' call e and check if outer bracket is matched, else error
or if input symbol is x, advance
else its an error
*/
void f()
{
	if(input[position]=='(')
	{
		position++;
		e();
		// After e, if closing bracket doesn't exist, call error, else advance
		if(input[position]==')')
		{
			position++;
		}
		else
		{
			return;
		}
	}
	else if(input[position]=='x')
	{
		position++;
	}
	// Since there is no epsilon in the production, this case is error
	else
	{
		return;
	}
}

/*
The following program opens an input file and takes each line and does top down parsing on it
if after parsing the current pointer points to the end of the input line, 
ie position is equal to length of input, then input is accepted, else rejected
*/
int main(int argc, char const *argv[])
{
	ifstream f("input");
	// for each line input do e() and check position
	while(getline(f,input,'\n'))
	{
		position = 0;
		e();
		// if current symbol is at end of string input, then input has been accepted, else rejected
		if(position==input.length())
		{
			cout<<"Accepted : "<<input<<endl;
		}
		else
		{
			cout<<"Rejected : "<<input<<endl;
		}

	}
	f.close();
	return 0;
}