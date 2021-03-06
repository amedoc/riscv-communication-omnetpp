//
// This is a modified version of the simple_perf example SystemC model
//

#include <omnetpp.h>
#include <systemc>

using namespace omnetpp;

using namespace ::sc_core;
using namespace ::std;

class write_if : virtual public sc_interface
{
  public:
    virtual void write(char) = 0;
    virtual void reset() = 0;
};

class read_if : virtual public sc_interface
{
  public:
    virtual void read(char &) = 0;
    virtual int num_available() = 0;
};

class fifo : public sc_channel, public write_if, public read_if
{
  public:
    fifo(sc_module_name name, int size_) : sc_channel(name), size(size_)
    {
      data = new char[size];
      num_elements = first = 0;
      num_read = max_used = average = 0;
      last_time = SC_ZERO_TIME;
    }

    ~fifo()
    {
      delete[] data;

      cout << endl << "Fifo size is: " << size << endl;
      cout << "Average fifo fill depth: " << double(average) / num_read << endl;
      cout << "Maximum fifo fill depth: " << max_used << endl;
      cout << "Average transfer time per character: "
           << last_time / num_read << endl;
      cout << "Total characters transferred: " << num_read << endl;
      cout << "Total time: " << last_time << endl;
    }

    void write(char c) {
      if (num_elements == size)
        wait(read_event);

      data[(first + num_elements) % size] = c;
      ++ num_elements;
      write_event.notify();
    }

    void read(char &c){
      last_time = sc_time_stamp();
      if (num_elements == 0)
        wait(write_event);

      compute_stats();

      c = data[first];
      -- num_elements;
      first = (first + 1) % size;
      read_event.notify();
    }

    void reset() { num_elements = first = 0; }

    int num_available() { return num_elements;}

  private:
    char *data;
    int num_elements, first;
    sc_event write_event, read_event;
    int size, num_read, max_used, average;
    sc_time last_time;

    void compute_stats()
    {
      average += num_elements;

      if (num_elements > max_used)
         max_used = num_elements;

      ++num_read;
    }
};

class producer : public sc_module
{
  public:
    sc_port<write_if> out;
    cSimpleModule *omnetpp_parent; //Andras
    sc_event kick_event;  //Andras

    SC_HAS_PROCESS(producer);

    producer(sc_module_name name, cSimpleModule *parent) : sc_module(name), omnetpp_parent(parent) // added 2nd arg --Andras
    {
      SC_THREAD(main);
    }

    void main()
    {
      const char *str = "SystemC!";
      const char *p = str;
      int total = 100000;

      while (true)
      {
        int i = 1 + int(19.0 * rand() / RAND_MAX);  //  1 <= i <= 19

        while (--i >= 0)
        {
          EV << "producer: writing " << *p << "\n";
          out->write(*p++);
          if (!*p) {
              EV << "producer: end of string reached, blocking on kick_event...\n"; //Andras (3 lines)
              wait(kick_event);
              EV << "producer: kick received, starting anew!\n";
              p = str;
          }
          --total;
        }

        if (total <= 0)
          break;

        wait(1000, SC_NS);
      }
    }

    void kick() //Andras
    {
      kick_event.notify();
    }
};

class consumer : public sc_module
{
  public:
    sc_port<read_if> in;
    cSimpleModule *omnetpp_parent; //Andras

    SC_HAS_PROCESS(consumer);

    consumer(sc_module_name name, cSimpleModule *parent) : sc_module(name), omnetpp_parent(parent) // added 2nd arg --Andras
    {
      SC_THREAD(main);
    }

    void main()
    {
      char c;

      while (true) {
        in->read(c);

        char msg_name[] = { c, 0 };  //Andras (added 4 lines)
        cContextSwitcher dummy(omnetpp_parent);
        EV << "consumer: read " << c << ", sending it in a msg\n";
        omnetpp_parent->send(new cMessage(msg_name), "out");

        wait(100, SC_NS);
      }
    }
};

class top : public sc_module
{
  public:
    fifo fifo_inst;
    producer prod_inst;
    consumer cons_inst;

    top(sc_module_name name, int size, cSimpleModule *omnetpp_parent) : // added 3rd arg --Andras
        sc_module(name) ,
        fifo_inst("Fifo1", size),
        prod_inst("Producer1", omnetpp_parent),
        cons_inst("Consumer1", omnetpp_parent)
    {
      prod_inst.out(fifo_inst);
      cons_inst.in(fifo_inst);
    }
};

/**
 * OMNeT++ module to wrap the SystemC model
 */
class SC_Riscv : public cSimpleModule
{
   protected:
     top top_inst;

   public:
     SC_Riscv() : top_inst("top", 3, this) {}  // use a very small buffer (size=3)

     void handleMessage(cMessage *msg)
     {
       delete msg;
       EV << "kicking producer...\n";
       top_inst.prod_inst.kick();
     }
};

Define_Module(SC_Riscv);
/*
int sc_main (int argc , char *argv[])
{
    //SC_Riscv a;
  //top top1("top",3,a);
  sc_start();

  return 0;
}*/

