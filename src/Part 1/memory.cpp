#include <systemc.h>
#include "params.h"
#include "simple_mem_if.h"
#include <iostream>
#include <fstream>
using namespace std;

class Memory : public sc_channel, public simple_mem_if
{
private:
    int memData[MEM_SIZE];
	
    char* memFilename;

public:
    Memory(sc_module_name name, char* memInitFilename) : sc_module(name) 
    {
        memFilename = memInitFilename;
        
		unsigned int initData;
		unsigned int initLocation;
		
		// initiliaze memory to 0
		for(int i=0; i<MEM_SIZE; i++) { memData[i] = 0; }
		
		// initialize memory contents from file
		ifstream ifs(memFilename, ifstream::in);
		
		//check if able to read memory file
		if(!ifs.good()) { 
			cout << "Warning: Could not open memory initialization file: " << memFilename << "." << endl;
			cout << "Memory contents initialized to all 0s." << endl << endl; 
			cout << flush;
		}

		initLocation = 0;
		
		//read memory file into memData memory array.
		while((ifs >> initData) && initLocation < MEM_SIZE) {			
				memData[initLocation] = initData;
				initLocation++;
		}
		ifs.close();

    }
	
//using the interface	
public:
	bool Write(unsigned int addr, unsigned int data)  //write interface
	{
		if( addr >= MEM_SIZE ) { return false; } //make sure the address is valid
        memData[addr] = data;
		return true;
    }
	
    bool Read(unsigned int addr, unsigned int& data)  //read interface
	{
		if( addr >= MEM_SIZE ) { return false; }
        data = memData[addr];
		return true;
    }
	
};