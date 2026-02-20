#include <systemc.h>
#include <iomanip>

#include "params.h"
#include "sad.cpp"
#include "memory.cpp"


//Top module to connect the components
class Top : public sc_module
{
public:
	Memory memory_0;
    SAD sad_0;
	   
public:
    Top(sc_module_name name, char* memInitFilename) : sc_module(name), sad_0("SAD_0"), memory_0("Memory_0", memInitFilename) {	
		sad_0.bus(memory_0);
    }
};

int sc_main(int argc, char* argv[])
{	
	if (argc < 2) { cout << "Usage: sad memfile\n"; return 1; }
	Top* top_0;
	top_0 = new Top("Top_0", argv[1]);
	sc_start();
	return 0;
}
