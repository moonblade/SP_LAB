#include "iostream"
#include "string"
#include "map"
#include "algorithm"
#include "fstream"
using namespace std;
map<int,string> memory;
int main()
{
	ifstream f("input");
	string line;
	while(getline(f,line,'\n') && line[0]!='E')
		if(line[0]=='T')
		{
			int address = strtol(line.substr(2,6).c_str(),NULL,16);
			string record = line.substr(12,line.length()-12);
			record.erase(remove(record.begin(),record.end(),'^'),record.end());
			for(int i=0;i<record.length();i+=2)
				memory[address++]=record.substr(i,2);
		}
	for(map<int,string>::iterator memoryIter = memory.begin(); memoryIter != memory.end(); memoryIter++)
		cout<<hex<<memoryIter->first<<" : "<<memoryIter->second<<endl;
	f.close();
	return 0;
}