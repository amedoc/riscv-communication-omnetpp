#include <iostream>
#include <fstream>

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;
using namespace std;

/**
 * Derive the source class from cSimpleModule. In the riescv_network ,
 * the `sender'  modules is a 'source' object, created by OMNeT++
 * at the beginning of the simulation.
 */
class source : public cSimpleModule
{
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    //virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(source);

void source::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs
    // to send the first message. Let this be `tic'.

    // Am I a source? // could be removed after and only send the msg

        // create and send first message on gate "out". "tictocMsg" is an
        // arbitrary string which will be the name of the message object.
     cMessage *msg = new cMessage("sendettoriscv");
     scheduleAt(simTime()+par("sendIaTime").doubleValue(), msg);
        //send(msg, "out"); // to send message via out

    // create a file to store the message

    fstream file;

    file.open("message.txt",ios::out);
    if(!file)
    {
      cout<<"Error in creating file.."<<endl;
      return ;
    }
    cout<<"\nFile created successfully."<<endl;

    file<< msg <<endl;

    file.close();
    cout<<"\nFile saved and closed succesfully."<<endl;



}




