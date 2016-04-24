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
/*Fnf is used to store the first and follow set of an element*/
class FnF
{
public:
	/*A set is a data type in which repetitions will not occur and order is not important*/
	/*First and follow are constructed as sets*/
	set<string> first;
	set<string> follow;
	FnF(){}
	/*A constructor to iniatise first and follow with a single element
		the element may be added either in first, or in follow using the boolean toFollow
		if toFollow is true add the string to follow set else add it to first set
	*/
	FnF(string s, bool toFollow=0){toFollow?follow.insert(s):first.insert(s);}
	// Print the first and follows in the set by iterating through them using a set iterator
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
/*Each element, variable and terminal will have a first and a follow (we will not display those of terminals)
This is stored using a map of the string (which is the variable or terminal) and a Fnf object which will contain the set of firsts and follows*/
map<string,FnF> elements;
/*Each production will have a lhs and can have multiple rhs
say the production E' -> + T E' | epsilon
it could be split as E' -> +TE' and E' -> epsilon
So each production could be composed of a list of productions, and each of these productions have a list of strings
So E' -> + T E' | epsilon would be stored as a map between E' and the list of list {{+,T,E'},{epsilon}}
*/
map<string,list<list<string> > > productions;
// Every element starting with a capital letter is a terminal, else its a non terminal
bool isTerminal(string element){return !isupper(element[0]);}
/*
A function to split a line into a string lhs (Variable) and a list of list of strings productions
They are returned as a pair, A pair is a datatype used to return two data members using pairObject.first pairObject.second
Here a pair of string (variable) and list<list<string>> productions is returned
*/
pair<string,list<list<string> > > split(string line)
{
	/*The pair is initialised*/
	pair<string,list<list<string> > > temp;	
	string element,productionString,dummy;
	// The line is made into a stringstream
	stringstream s(line);
	/*The first element of the stringstream will be the lhs or the variable, it is given to the first element of the pair*/
	s>>temp.first;
	/*The second element will be the seperator ie -->, this is discarded*/
	s>>dummy;
	/*The rest of the line is left in s, it is split at | to get each production and then split at space to get each elements*/
	while(getline(s,productionString,'|'))
	{
		/*A list of strings is initialised*/
		list<string> production;
		/*One produciton is taken (till |) and then a stringstream is made of it*/
		stringstream t(productionString);
		/*Each word is made into an element and if it is not empty it is added to the production list*/
		while(getline(t,element,' '))
		{
			if(element!="")
				production.push_back(element);
		}
		/*This list is added to the list of list of string, which is the second of the pair*/
		temp.second.push_back(production);
	}
	/*The pair is returned*/
	return temp;
}
/*A function to insert the first of the next element in the list to the first of the current element
It takes the current variable under consideration, the next element and the end element as iterators
*/
int insertFirstOfNext(string variable,list<string>::iterator next,list<string>::iterator end)
{
	/*If an element is inserted anywhere, it might influence the first or follow of some other element 
	which did not know of the insertion's existence till now, So every time an insertion is made
	The whole first and follow is repeated again to make sure both are complete, when no more changes are 
	made, the process is complete. It is tracked using the repeat boolean*/
	bool repeat=0;
	/*If the next element exists, ie if the next element is not the end of the list*/
	if(next!=end)
		/*Itreate through the first of the next element*/
		for(set<string>::iterator setItor=elements[*next].first.begin();setItor!=elements[*next].first.end(); setItor++)
			/*Add each of the firsts to the first of the current element 
			(if the element already exists in the current element's first, then its return pair's 
			second will be false, if inserted, it will be true, So if an insertion has occured,
			the first and follow set were changed so a repeat must be done*/
			if(elements[variable].first.insert(*setItor).second)
				repeat=1;
			/*If the first of the next element Contains an epsilon, the first of the next element is added as well*/
			if(elements[*next].first.find(epsilon)!=elements[*next].first.end())
				if(insertFirstOfNext(variable,++next,end))
					repeat=1;
	return repeat;
}
/*A function to insert the follow of the next variable to the follow of the current one
It takes the lhs as supervariable, an iterator to the next(which is presently same, and an iterator to the end)*/
int insertFollowFromNext(string superVariable,list<string>::iterator next,list<string>::iterator end)
{
	/*Same as before, if a change has been made the whole thing is repeated*/
	bool repeat = 0;
	/*The variable is stored and next is pointed to its actual next position*/
	string variable=*next;
	next++;
	/*If the next element is not end of list, ie next element exists, then*/
	if(next!=end)
		/*If the next element is a terminal, add it directly to the follow of the current one*/
		if(isTerminal(*next) && elements[variable].follow.insert(*next).second)
			repeat=1;			
		else
		{	
			/*loop through the first of the next element and add each to the current's follow, if no addition is done, repeat remains as 0*/
			for(set<string>::iterator iter=elements[*next].first.begin();iter!=elements[*next].first.end();iter++)
				if(*iter!=epsilon && elements[variable].follow.insert(*iter).second)
						repeat = 1;
			/*If the first of the next element has an epsilon in it, then add the follow of the next element as well to the follow of the current element*/
			if(elements[*next].first.find(epsilon)!=elements[*next].first.end())
				for(set<string>::iterator iter=elements[*next].follow.begin();iter!=elements[*next].follow.end();iter++)
					if(elements[variable].follow.insert(*iter).second)
						repeat=1;
		}
	else
		/*If the variable is at the end of the list, then add the follow of the supervariable(ie the lhs of the production) to the follow of the current element */
		for(set<string>::iterator iter=elements[superVariable].follow.begin();iter!=elements[superVariable].follow.end();iter++)
			/*If no insertion has been done, repeat remains zero*/
			if(elements[variable].follow.insert(*iter).second)
				repeat=1;
	return repeat;
}
/*A function to add the first and follow of the whole productions*/
void addFirstAndFollow()
{
	/*Repeat and a simple beginning flag for the beginning, since first needs only the beginning of each production and is not to be done for each element*/
	bool repeat=0,beginning=1;
	/*iterate through all the productions, for a this a list<list<string>> iterator is required, ie three levels of iterations*/
	for(map<string, list<list<string> > >::iterator iter = productions.begin(); iter!=productions.end(); iter++)
		for(list<list<string> >::iterator liter = (iter->second).begin(); liter!=(iter->second).end(); liter++)
		{
			/*Two levels down, the firstrst word is sepearated using beggining flag for first operations*/
			beginning=1;	
			/*Iterate through the productions*/
			for(list<string>::iterator siter = (*liter).begin(); siter!=(*liter).end(); siter++)
			{
				/*If the element is at beginnign*/
				if(beginning)
				{
					/*If the element is a terminal, then add it to the first of lhs, if the lhs is not found in the list of elements, then a new object is created*/
					if(isTerminal(*siter))
					{
						if(elements.find(*siter)==elements.end())
							elements[*siter]=FnF(*siter);
						if(elements[iter->first].first.insert(*siter).second)
							repeat=1;
					}
					else
						/*if the element is non terminal, then the first of the next element is inserted as the first of this, using above logic*/
						repeat=insertFirstOfNext(iter->first,siter,(*liter).end());
					beginning=0;
				}
				/*For each element, follow (using first from next) is added to the follow of the current element,
				 also passed to it is the supervariable or the lhs of the productions*/
				if(insertFollowFromNext(iter->first,siter,(*liter).end()))
					repeat=1;
			}
		}
		/*If repeat flag is set, then repeat the whole proceedure
		It is repeated untill no more additions are done to first or follow*/
	if(repeat) addFirstAndFollow();
}
int main()
{
	ifstream f("input");
	string line;
	while(getline(f,line,'\n'))
	{
		/*For each line in input file, split it using split function into string lhs variable, and list<list<string>> productions and add it to the produciton map*/
		productions[split(line).first]=split(line).second;
		/*The follow of the first element is given a terminator symbol, all the rest are given empty fnf objects in the beginnign
		Ie if elements object exists or its size is greater than zero, then it is not the first element*/
		elements[split(line).first]=elements.size()?FnF():FnF(terminator,1);
	}
	/*Add first and follow of every element*/
	addFirstAndFollow();
	for(map<string,FnF>::iterator iter=elements.begin();iter!=elements.end();iter++)
	{
		/*Print the first and follow set of all non terminals*/
		if(!isTerminal(iter->first))
		{
			/*Print the non terminal name and an underline*/
			cout<<iter->first<<endl<<string(iter->first.length(),'-')<<endl;
			iter->second.print();
			cout<<endl;
		}
	}
	/*Close input file*/
	f.close();
	return 0;
}