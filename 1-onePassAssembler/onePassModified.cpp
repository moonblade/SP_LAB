#include <iostream>
#include <fstream>
#include <list>
#include <iomanip>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;
map<string, pair<int, list<int> > > symtab;
map<string,string> optab;
list<pair<int, string> > textRecords;
int main(int argc, char const *argv[])
{
	pair<int,string> textRecord(0,"");
	ifstream f("optab");
	int locctr,saddress,linum=0;
	string line,nmemonic,opcode,label,operand,headerRecord,endRecord;
	while(getline(f,line))
	{
		stringstream s(line);
		s>>nmemonic>>opcode;
		optab[nmemonic]=opcode;
	}
	f.close();
	// freopen("source","r",stdin);
	while(getline(cin,line))
	{
		linum++;
		if(line[0]!='.')
		{
			stringstream s(line);
			getline(s,label,' ');
			getline(s,opcode,' ');
			getline(s,operand,' ');
			if(opcode=="START")
			{
				stringstream ss;
				ss<<setw(6)<<setfill('0')<<hex<<operand;
				ss>>saddress;
				locctr=saddress;
				stringstream name;
				name<<setw(6)<<setfill(' ')<<label;
				headerRecord="H^"+name.str()+"^"+ss.str()+"^";
			}
			else if(opcode=="END")
			{
				stringstream end;
				// end<<setw(6)<<setfill('0')<<hex<<saddress;
				endRecord="E^"+end.str();
			}
			else
			{
				if(label!="")
				{
					if(symtab.find(label)!=symtab.end())
					{
						if(symtab[label].first==-1)
						{
							// Forward reference resolution
							symtab[label].first=locctr;
							stringstream ss;
							textRecords.push_back(textRecord);
							for(auto i=symtab[label].second.begin();i!=symtab[label].second.end();i++)
							{
								// Resolve
								stringstream loc;
								loc<<hex<<setw(4)<<setfill('0')<<locctr;
								textRecords.push_back(*new pair<int,string>(*i,"^"+loc.str()));
							}
							textRecord = *new pair<int,string>(locctr,"");
						}
						else
						{
							// Redeclaration Error
							cout<<"Error at "<<linum<<", Redeclaration of Variable "<<label;
							return 1;
						}
					}
					else
					{
						pair<int,list<int> > entry;
						entry.first=locctr;
						symtab[label]=entry;
					}
				}
				if(optab.find(opcode)!=optab.end())
				{
					// Valid opcode
					textRecord.second+="^"+optab[opcode];
					if(operand!="")
					{
						if(symtab.find(operand)==symtab.end())
						{
							pair<int,list<int> > entry(-1,{locctr+1});
							symtab[operand]=entry;
							textRecord.second+="0000";
						}
						else
						{
							if(symtab[operand].first==-1)
							{
								symtab[operand].second.push_back(locctr+1);
								textRecord.second+="0000";
							}
							else
							{
								stringstream loc;
								loc<<setw(4)<<setfill('0')<<hex<<symtab[operand].first;
								textRecord.second+=loc.str();
							}
						}
					}
					else
						textRecord.second+="0000";
					locctr+=3;
				}
				else if(opcode=="RESW"||opcode=="RESB")
				{
					stringstream space;
					space<<hex<<operand;
					int toAdd;
					space>>toAdd;
					if(opcode=="RESW")
						toAdd*=3;
					locctr+=toAdd;
					textRecords.push_back(textRecord);
					textRecord = *new pair<int,string>(locctr,"");
				}
				else if(opcode=="BYTE")
				{
					string toConvert=operand.substr(2,operand.length()-3);
					int toAdd = toConvert.length();
					textRecord.second+="^";
					if(operand[0]=='C')
					{
						for(int i=0;i<toConvert.length();++i)
						{
							int k=toConvert[i];
							stringstream toHex;
							toHex<<hex<<k;
							string hexEq;
							toHex>>hexEq;
							textRecord.second+=hexEq;
						}
					}
					else if(operand[0]=='X')
					{
						toAdd/=2;
						textRecord.second+=toConvert;
					}
					else
					{
						cout<<"Unknown byte declaration on line "<<linum;
						return 1;
					}
					locctr+=toAdd;
				}
				else if(opcode=="WORD")
				{
					stringstream word;
					word<<setw(6)<<setfill('0')<<operand;
					textRecord.second+="^"+word.str();
					locctr+=3;	
				}
				else
				{
					cout<<"Error unknown opcode "<<opcode<<" in line "<<linum;
					return 1;
				}
			}
		}		
	}
	stringstream headerCompleter;
	headerCompleter<<setw(6)<<setfill('0')<<hex<<(locctr-saddress);
	headerRecord+=headerCompleter.str();
	cout<<headerRecord<<endl;
	for(auto i=textRecords.begin();i!=textRecords.end();++i)
	{
		int start=(*i).first;
		stringstream startHex,size;
		string startEq,sizeEq;
		startHex<<setw(6)<<setfill('0')<<hex<<start;
		startHex>>startEq;
		string withoutbracket=(*i).second;
		withoutbracket.erase(remove(withoutbracket.begin(), withoutbracket.end(),'^'),withoutbracket.end());

		size<<setw(2)<<setfill('0')<<hex<<withoutbracket.length()/2;
		size>>sizeEq;
		if(withoutbracket.length()/2>0)
			cout<<"T^"+startEq+"^"+sizeEq<<(*i).second<<endl;
	}
	cout<<endRecord<<endl;
	return 0;
}