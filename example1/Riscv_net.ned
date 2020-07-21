//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


package example1;






//
// This example shows the first step of creating a mixed SystemC-OMNeT++
// simulation. The network contains two simulation models, the OMNeT++ Fifo
// example and a SystemC model wrapped in SC_Wrapper. The two models execute
// under the same roof and with their simulation times synchronized, but
// otherwise they are independent.
//
network Riscv_net
{
    submodules:
        sc_riscv_wrapper: SC_Riscv {
            @display("p=200,180");
        }
       sender: source {
          @display("p=75,96");
      }

    connections:
       sender.out --> {  delay = 100ms; } --> sc_riscv_wrapper.in;

}
