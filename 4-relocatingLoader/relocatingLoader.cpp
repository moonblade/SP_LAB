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
	// The name of the program from header record
	string programStart,programName;
	int programStartInt;	
	// File open
	ifstream f(inputFile);
	// Each line of the file is received into the string object, line
	string line;
	// Loop through all lines in inputFile, till end of file, the delimeter used is \n or endl
	cout<<"Enter program start address : ";
	// Enter start of program in hex
	cin>>programStart;
	// Convert hex start to int
	stringstream s(programStart);
	s>>hex>>programStartInt;
	while(getline(f,line,'\n'))
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
				int address,mask,size;
				// take address,size and bitmask string into stringstream
				stringstream s(line.substr(2,13));
				// Write it to int
				s>>hex>>address>>size>>mask;
				// String type record to save individual elements in text Record
				string record;
				// Create a string stream from the record part of textrecord
				stringstream recordStream(line.substr(16,line.length()-16));
				// Loop through individual records in it
				while(getline(recordStream,record,' '))
				{
					// Checking to see if bitmask is one
					// ie if mask and 800 [100000000000] is 800, then the bit is 1, else 0
					if(((mask&0x800)==0x800))
					{
						// take to stringstream, add program start, and replace record with the new one
						stringstream a;
						// strtol converts string in hex to int value, add programstartint to it
						a<<hex<<strtol(record.c_str(),NULL,16)+programStartInt;
						record = a.str();
					}
					// left shift bitmask by one
					mask=mask<<1;
					// Loop through the record in twos and add it to map
					for(int j=0;j<record.length();j+=2)
					{
						// Loop through the record, take them two at a time and add it to map[address]
						memory[address+programStartInt]=record.substr(j,2);
						address++;
					}
				}
			}
			/*
				If the record is an end record, take the address specified and move location to the location given to start execution of the program
			*/
			else if(line[0]=='E')
			{
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
	cout<<endl<<"Start Program At "<<hex<<programStartInt<<endl;
	f.close();
	return 0;
}