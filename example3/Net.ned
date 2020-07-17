//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


package example3;

//
// This example shows how to easily wrap a SystemC model that contains
// modules as local variables in sc_start(), and/or several sc_start()
// statements all over the code.
//
// When run inside OMNeT++, sc_start() calls are forbidden (as the OMNeT++
// SystemC scheduler already takes care of processing simulation events),
// and should be replaced with wait() while the whole code runs inside
// a coroutine. Using a coroutine also solves the problem that sc_start()
// contains modules as local variables.
//
network Net
{
    submodules:
        sc_riscv : SC_riscv;
}
