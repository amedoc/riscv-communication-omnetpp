//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <omnetpp.h>

using namespace omnetpp;





/**
 * An OMNeT++/OMNEST module that wraps a SystemC module
 */
class SC_Riscv : public cSimpleModule
{
  public:
    SC_Riscv();
};

Define_Module(SC_Riscv);

void sc_create_model();

SC_Riscv::SC_Riscv()
{
    EV << "Building the SystemC model...\n";

    sc_create_model();
}

