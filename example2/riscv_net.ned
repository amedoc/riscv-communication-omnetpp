//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package example2;

import fifo.Fifo;
import fifo.Sink;
import fifo.Source;


//
// This network shows a modified version of the simple_perf SystemC sample
// simulation embedded, into an OMNeT++ model. The Source module triggers
// actions in SC_SimplePerf, which sends OMNeT++ messages into the Fifo.
//
network riscv_net
{
    @display("bgb=565,408");
    submodules:
        gen: Source {
            @display("p=149,44");
        }
        perf: SC_Riscv {
            @display("p=441,219");
        }
        fifo: Fifo {
            @display("p=318,129");
        }
        sink: Sink {
            @display("p=541,267");
        }
    connections:
        gen.out --> perf.in;
        perf.out --> fifo.in;
        fifo.out --> sink.in;


}
