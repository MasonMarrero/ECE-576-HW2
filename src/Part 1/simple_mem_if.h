
#ifndef SimpleMemIF_H
#define SimpleMemIF_H
#include <systemc.h>

// Simple memory interface (given in the HW PDF)
class simple_mem_if : virtual public sc_interface
{
public:
    virtual bool Write(unsigned int addr, unsigned int data) = 0;
    virtual bool Read (unsigned int addr, unsigned int &data) = 0;
};

#endif