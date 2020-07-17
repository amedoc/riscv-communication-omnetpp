#ifndef RISCV_H
#define RISCV_H

#include <systemc>

using namespace sc_core;

class riscv : public sc_module
{
  protected:
    sc_module *model;
  public:
    SC_HAS_PROCESS(riscv);
    riscv(sc_module_name name_);

    void main();

   // void setup();
    void run();
   // void wrapup();
};

#endif
