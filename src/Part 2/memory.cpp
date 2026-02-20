#include <systemc.h>
#include "params.h"
#include "simple_mem_if.h"
#include <iostream>
#include <fstream>
using namespace std;


class MEMORY_RTL : public sc_module
{
public:
    sc_in<sc_logic>   Clk;
    sc_in<sc_logic>   Ren, Wen;
    sc_in<int>        Addr;
    sc_in<int>        DataIn;
    sc_out<int>       DataOut;
    sc_out<sc_logic>  Ack;

    unsigned int memData[MEM_SIZE];

    SC_HAS_PROCESS(MEMORY_RTL);

    MEMORY_RTL(sc_module_name name, char* memInitFilename) : sc_module(name)
    {
        
        for (int i = 0; i < MEM_SIZE; i++) memData[i] = 0;

        
        ifstream ifs(memInitFilename, ifstream::in);
        if (!ifs.good()) {
            cout << "Warning: Could not open memory initialization file: "
                 << memInitFilename << endl;
            cout << "Memory contents initialized to all 0s." << endl;
        } else {
            unsigned int initData;
            int initLocation = 0;
            while ((ifs >> initData) && initLocation < MEM_SIZE) {
                memData[initLocation] = initData;
                initLocation++;
            }
            ifs.close();
        }

        SC_METHOD(rtl);
        sensitive << Clk.pos();
        dont_initialize();
    }

    void rtl()
    {
        Ack.write(SC_LOGIC_0);

        int a = Addr.read();
        if (a < 0 || a >= MEM_SIZE) {
            return;
        }

        if (Ren.read() == SC_LOGIC_1) {
            DataOut.write((int)memData[a]);
            Ack.write(SC_LOGIC_1);
        }
        else if (Wen.read() == SC_LOGIC_1) {
            memData[a] = (unsigned int)DataIn.read();
            Ack.write(SC_LOGIC_1);
        }
    }
};


class Memory : public sc_channel, public simple_mem_if
{
private:
    sc_signal<sc_logic>  clk;
    sc_signal<sc_logic>  Ren, Wen;
    sc_signal<int>       Addr;
    sc_signal<int>       DataIn;
    sc_signal<int>       DataOut;
    sc_signal<sc_logic>  Ack;

    MEMORY_RTL mem_rtl_0;

    void wait_posedge()
    {
        do {
            wait(clk.value_changed_event());
        } while (clk.read() != SC_LOGIC_1);

        wait(SC_ZERO_TIME);
    }

public:
    SC_HAS_PROCESS(Memory);

    Memory(sc_module_name name, char* memInitFilename)
        : sc_channel(name)
        , mem_rtl_0("MEMORY_RTL_0", memInitFilename)
    {
        mem_rtl_0.Clk(clk);
        mem_rtl_0.Ren(Ren);
        mem_rtl_0.Wen(Wen);
        mem_rtl_0.Addr(Addr);
        mem_rtl_0.DataIn(DataIn);
        mem_rtl_0.DataOut(DataOut);
        mem_rtl_0.Ack(Ack);

        clk.write(SC_LOGIC_0);
        Ren.write(SC_LOGIC_0);
        Wen.write(SC_LOGIC_0);
        Addr.write(0);
        DataIn.write(0);

        SC_THREAD(oscillator);
    }

    void oscillator ()
    {
       while (true) {       
			clk.write (SC_LOGIC_0 );
       		wait(2, SC_NS) ;
        	clk.write (SC_LOGIC_1 );
        	wait(2, SC_NS) ;
       }
    }

public:
    bool Write(unsigned int addr, unsigned int data) override
    {
        Addr.write((int)addr);
        DataIn.write((int)data);
        Ren.write(SC_LOGIC_0);
        Wen.write(SC_LOGIC_1);

        wait_posedge();

        bool check = (Ack.read() == SC_LOGIC_1);

        Wen.write(SC_LOGIC_0);
        return check;
    }

    bool Read(unsigned int addr, unsigned int &data) override
    {
        Addr.write((int)addr);
        Wen.write(SC_LOGIC_0);
        Ren.write(SC_LOGIC_1);

        wait_posedge();

        bool check = (Ack.read() == SC_LOGIC_1);
        if (check) data = (unsigned int)DataOut.read();

        Ren.write(SC_LOGIC_0);
        return check;
    }
};
