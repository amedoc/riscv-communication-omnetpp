#include "riscv.h"


using namespace std;
using namespace sc_core;

////////////////////////******** RISC-V addition *********** //////////////////////////////////
// Adding necessary libraries and header files to create riscv vp module
#include <cstdlib>
#include <ctime>
#include <boost/io/ios_state.hpp>
#include <boost/program_options.hpp>
#include <iomanip>
#include <iostream>

#include "include_vp/common/gdb_server.h"
#include "include_vp/common/gdb_runner.h"

#include "include_vp/common/clint.h"
#include "include_vp/rv32/elf_loader.h"
#include "include_vp/common/gdb_stub.h"
#include "include_vp/rv32/iss.h"
#include "include_vp/rv32/mem.h"
#include "include_vp/memory.h"
#include "include_vp/rv32/syscall.h"
////////////////////////******** RISC-V addition *********** //////////////////////////////////
using namespace rv32;



/**
 * A dummy simulation model -- a counter which gets incremented
 * increasingly slower over time.
 */
/*
class Counter : public sc_module
{
  protected:
    int counter;
    int dt;

  public:
    SC_HAS_PROCESS(Counter);

    Counter(sc_module_name name) : sc_module(name)
    {
        SC_THREAD(main);
    }

    void main()
    {
        counter = 0;
        dt = 1;
        while (true)
        {
            counter++;
            cout << "inc at " << sc_time_stamp() << endl;
            wait(dt, SC_MS);
            dt++;
        }
    }

    int getCounter()
    {
        return counter;
    }
};
*/
//---

riscv::riscv(sc_module_name name_) : sc_module(name_)//, //model(nullptr)
{
    //model = new Counter("ctr");
    SC_THREAD(main);
}

void riscv::main()

{
    struct Options {
    typedef unsigned int addr_t;

    Options &check_and_post_process() {
        mem_end_addr = mem_start_addr + mem_size - 1;
        return *this;
    }

    std::string input_program = "/home/ahmed/Desktop/Work_Productive_4.0/riscv-vp/sw/hello_world/main";

    addr_t mem_size = 1024 * 1024 * 32;  // 32 MB ram, to place it before the CLINT and run the base examples (assume
                                         // memory start at zero) without modifications
    addr_t mem_start_addr = 0x00000000;
    addr_t mem_end_addr = mem_start_addr + mem_size - 1;
    addr_t clint_start_addr = 0x02000000;
    addr_t clint_end_addr = 0x0200ffff;
    addr_t sys_start_addr = 0x02010000;
    addr_t sys_end_addr = 0x020103ff;

    bool use_debug_runner = false;
    bool use_instr_dmi = false;
    bool use_data_dmi = false;
    bool trace_mode = false;
    bool intercept_syscalls = false; //true
    bool quiet = false;
    bool use_E_base_isa = false;
    unsigned int debug_port = 5005;

    unsigned int tlm_global_quantum = 10;
};

    Options opt ; //= parse_command_line_arguments(argc, argv);

        std::srand(std::time(nullptr));  // use current time as seed for random generator

        tlm::tlm_global_quantum::instance().set(sc_core::sc_time(opt.tlm_global_quantum, sc_core::SC_NS));

        ISS core(0, opt.use_E_base_isa);
        MMU mmu(core);
        CombinedMemoryInterface core_mem_if("MemoryInterface0", core, &mmu);
        SimpleMemory mem("SimpleMemory", opt.mem_size);
        ELFLoader loader(opt.input_program.c_str());
        SimpleBus<2, 3> bus("SimpleBus");
        SyscallHandler sys("SyscallHandler");
        CLINT<1> clint("CLINT");
        DebugMemoryInterface dbg_if("DebugMemoryInterface");

        MemoryDMI dmi = MemoryDMI::create_start_size_mapping(mem.data, opt.mem_start_addr, mem.size);
        InstrMemoryProxy instr_mem(dmi, core);

        std::shared_ptr<BusLock> bus_lock = std::make_shared<BusLock>();
        core_mem_if.bus_lock = bus_lock;

        instr_memory_if *instr_mem_if = &core_mem_if;
        data_memory_if *data_mem_if = &core_mem_if;
        if (opt.use_instr_dmi)
            instr_mem_if = &instr_mem;
        if (opt.use_data_dmi) {
            core_mem_if.dmi_ranges.emplace_back(dmi);
        }

        loader.load_executable_image(mem.data, mem.size, opt.mem_start_addr);
        core.init(instr_mem_if, data_mem_if, &clint, loader.get_entrypoint(), rv32_align_address(opt.mem_end_addr));
        sys.init(mem.data, opt.mem_start_addr, loader.get_heap_addr());
        sys.register_core(&core);

        if (opt.intercept_syscalls)
            core.sys = &sys;

        // setup port mapping
        bus.ports[0] = new PortMapping(opt.mem_start_addr, opt.mem_end_addr);
        bus.ports[1] = new PortMapping(opt.clint_start_addr, opt.clint_end_addr);
        bus.ports[2] = new PortMapping(opt.sys_start_addr, opt.sys_end_addr);

        // connect TLM sockets
        core_mem_if.isock.bind(bus.tsocks[0]);
        dbg_if.isock.bind(bus.tsocks[1]);
        bus.isocks[0].bind(mem.tsock);
        bus.isocks[1].bind(clint.tsock);
        bus.isocks[2].bind(sys.tsock);

        // connect interrupt signals/communication
        clint.target_harts[0] = &core;

        // switch for printing instructions
        core.trace = opt.trace_mode;

        std::vector<debug_target_if *> threads;
        threads.push_back(&core);

        if (opt.use_debug_runner) {
            auto server = new GDBServer("GDBServer", threads, &dbg_if, opt.debug_port);
            new GDBServerRunner("GDBRunner", server, &core);
        } else {
            new DirectCoreRunner(core);
        }

        if (opt.quiet)
             sc_core::sc_report_handler::set_verbosity_level(sc_core::SC_NONE);

       // sc_core::sc_start();
        if (!opt.quiet) {
            core.show();
        }

        //return 0;
}

void riscv::setup()
{

}

void riscv::run()
{
    sc_start(1000, SC_MS);
    //sc_core::wait(50, SC_MS);
    cout << "simulation running" << endl;
   // cout << "simulation running" << ((Counter *)model)->getCounter() << endl;
    //sc_start(100, SC_MS);
    //sc_core::wait(50, SC_MS);
    //cout << "counter = " << ((Counter *)model)->getCounter() << endl;
}
/*
void Tester::wrapup()
{
    //sc_start(1, SC_MS);
    sc_core::wait(1, SC_MS);
}
*/
