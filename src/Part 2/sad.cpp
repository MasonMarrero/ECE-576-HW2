#include <systemc.h>
#include <iostream>

#include "params.h"
#include "simple_mem_if.h"

//ALU module
class SAD : public sc_module {
//module ports
public:	
	unsigned int input1, input2;
	int sad;

//alternative constructor to SC_CTOR
public:
	SC_HAS_PROCESS(SAD);
	
public:
	SAD(sc_module_name name) : sc_module(name) { //constructor definition; name: instance name
		SC_THREAD(sad_main); //Changed this to a thread to run through just once
	}
	
	//function declaration and implementation
public:
	void sad_main() {
        //Perform SAD operation
		int i, v;
        unsigned int block;

        for(block = 0; block<NUM_BLOCKS; block++){
            sad = 0;
            for (i =  0; i < BLOCK_SIZE; i++){
				wait(30, SC_NS) ; // 30 for mult and adds line 34
                if(!bus->Read(INPUT1_ADDR + (block * BLOCK_SIZE) + i, input1)) { //read the data in INPUT1_ADDR via the Read port of the interface
			        cout << "Error: Invalid input 1 data!! " << endl;
			        sc_stop();
		        }
		        //cout << " INPUT 1 for the SAD is: " << input1 << endl;
				wait(30, SC_NS) ; //30 for add and mul line 40
                if(!bus->Read(INPUT2_ADDR + (block * BLOCK_SIZE) + i, input2)) {
                    cout << "Error: Invalid input 2 data!! " << endl;
                    sc_stop();
                }
                //cout << " INPUT 2 for the SAD is: " << input2 << endl;
				//10Nns FOR Subtraction line 46
                v  = (int)input1 - (int)input2;
				wait(10, SC_NS) ;
				
				//10ns for comparison line 49. 
				wait(10, SC_NS) ;
                if (v < 0) { v = -v; wait(10, SC_NS) ;}
				

				//10ns for addition line 55
                sad += v;
				wait(10, SC_NS) ;

				//10ns for loop comparison and addition line 32
				wait(20, SC_NS) ;
            }
		wait(10, SC_NS) ; // 10ns for write line 63
        //result.write(res);
		if(!bus->Write(SAD_OUTPUT_ADDR + block, sad)) {
			cout << "Invalid write address for the SAD result!!" << endl;
			cout << "Stop the simulation!!!" << endl;
			sc_stop();
		}
		

		cout << sc_time_stamp() << " SAD[" << block << "] = " << sad << endl;
        }
		
		sc_stop();
		
	}
	
public:
	sc_port<simple_mem_if> bus; //instantiate and use the interface on the SAD
	
};
