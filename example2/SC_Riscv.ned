//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


package example2;

@namespace("");

//
// A hybrid SystemC-and-OMNeT++ module
//
simple SC_Riscv
{
    parameters:
        @display("i=block/cogwheel");
    gates:
        input in @loose;
        output out@loose;
}
