//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


package fifo;

//
// Single-server queue, service rate independent of message length
//
simple Fifo
{
    parameters:
        volatile double serviceTime @unit(s);
        @display("i=block/queue;q=queue");
    gates:
        input in;
        output out;
}
