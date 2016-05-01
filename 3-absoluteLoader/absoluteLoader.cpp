#include "iostream"
#include "string"
// For memory, a map between an integer(address) and string(value)
#include "map"
// For function replace
#include "algorithm"
#include "sstream"
#include "fstream"
#define inputFile "input"
using namespace std;

/*
A map is a dynamically allocated linked datatype, an array for example is a map between int(index) and the type of array, but it is not dynamically allocated, and control over indexis limited. A map<int,string> on the other hand can act as an string array where only the indexes we specify will exist.
*/
map<int,string> memory;

int main(int argc, char const *argv[])
{
	// The address of main function or the execution start adddress of the program
	int programStart;
	// The name of the program from header record
	string programName;	
	// File open
	freopen("input","r",stdin);
	// Each line of the file is received into the string object, line
	string line;
	// Loop through all lines in inputFile, till end of file, the delimeter used is \n or endl
	while(getline(cin,line,'\n'))
	{
		// Ignore empty lines
		if(line.length()>0)
		{
			/*
				For text records, the start address is calculated as an int, (to know where in the map to start)

				The rest of the records are clubbed together into a single string and are taken two at a time(a byte at a time)

				If there is a correction text record, the map will automatically get updated, since the same address will be given again
			*/
			if(line[0]=='T')
			{
				// Replace ^ with space, for easier string stream manipulation
				replace(line.begin(),line.end(),'^',' ');
				// Address to start map from
				int address;
				// take address string into stringstream
				stringstream s(line.substr(2,6));
				// Write it to int
				s>>hex>>address;
				// Take rest of the records
				string record = line.substr(12,line.length()-12);
				// Remove space to club the entire record into a single continous string
				record.erase(remove(record.begin(),record.end(),' '),record.end());
				for(int i=0;i<record.length();i+=2)
				{
					// Loop through the record, take them two at a time and add it to map[address]
					memory[address]=record.substr(i,2);
					address++;
				}
			}
			/*
				If the record is an end record, take the address specified and move location to the location given to start execution of the program
			*/
			else if(line[0]=='E')
			{
				// Get address
				stringstream s(line.substr(2,6));
				// Save it as int
				s>>hex>>programStart;
				// Lines after this won't matter
				break;
			}
			/*
				If record is header record, save program name into variable for later confirmation
			*/
			else if(line[0]=='H')
			{
				programName=line.substr(2,6);
			}
			/*
				Absolute loader cannot process any other records, So error is given and stopped
			*/
			else
			{
				cout<<"Error unknown record type";
				return 1;
			}
		}
	}

	/*
		After the memory map has been completed, loop through the memory map and print each location and the byte saved in the corresponding memory
	*/
	// Print program name
	cout<<endl<<"Program Name "<<programName<<endl;
	// Loop through memory map
	cout<<endl<<"Memory Map "<<endl;
	for(map<int,string>::iterator memoryIter = memory.begin(); memoryIter != memory.end(); memoryIter++)
	{
		// map -> first refers to the integer address
		// map -> second refers to the string record (byte value) at the address
		cout<<hex<<memoryIter->first<<" : "<<memoryIter->second<<endl;
	}
	// Print where to start the program at
	cout<<endl<<"Start Program At "<<hex<<programStart<<endl;
	return 0;
}