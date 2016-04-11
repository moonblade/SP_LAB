#include "iostream"
#include "string"
#include "map"
#include "algorithm"
#include "sstream"
#include "fstream"
using namespace std;
map<int,string> memory;
int main(int argc, char const *argv[])
{
 	ifstream f("input");
 	string line,record;
	int pStart = strtol(argv[1],NULL,16),address,mask;	
	while(getline(f,line,'\n') && line[0]!='E')
	{
		replace(line.begin(),line.end(),'^',' ');
		stringstream s(line.substr(2,13)),recordStream(line.substr(16,line.length()-16));
		s>>hex>>address>>mask>>mask;
		while(getline(recordStream,record,' '))
		{
			if(mask&0x800)
			{
				stringstream a;
				a<<hex<<strtol(record.c_str(),NULL,16)+pStart;
				record = a.str();
			}
			mask=mask<<1;
			for(int j=0;j<record.length();j+=2)
				memory[address+++pStart]=record.substr(j,2);
		}
	}
	for(map<int,string>::iterator mIter = memory.begin(); mIter != memory.end(); mIter++)
 		cout<<hex<<mIter->first<<" : "<<mIter->second<<endl;
	f.close();
	return 0;
}