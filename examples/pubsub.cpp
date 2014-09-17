/*************************************************************************
 * indyva-cpp
 *************************************************************************
 * @file    pubsub.cpp
 * @date    13.08.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>

#include <jsonrpc/connectors/zmq.hpp>

#include <indyva/hub.h>
#include <indyva/rpc.h>


void p (const std::string &topic, const Json::Value &msg) {
    std::cout << "Recived --> topic: " << topic << " msg: " << msg << std::endl;
}

void render (const std::string &topic, const Json::Value &msg) {
    msg.size();  // to avoid no-use-warning
    std::cout << topic << " " << std::flush;
}

int main()
{
    zmq::context_t context(1);

    std::cout << "Conecting to tcp://localhost:18000" << std::endl;

    indyva::Rpc rpc(context, "tcp://localhost:18000");

    indyva::Hub hub(&rpc, context, "tcp://localhost:18001");

    std::string token_name_change = hub.subscribe("name:change", p);
    std::string token_r = hub.subscribe("r:", render);

    /**
     * Construction of a message
     */
    Json::Value msg("Paco");
    std::string topic("name:change");

    std::cout << "Pubishing-->\t  topic: " << topic << "\t msg: " << msg.asString()  << std::endl;
    hub.publish(topic, msg);
    hub.receive();

    std::cout << "Pubishing-->\t  topic: " << topic << "\t msg: " << msg.asString()  << std::endl;
    std::cout << "Unsubscribe-->\t  topic: " << topic << std::endl;
    hub.unsubscribe(token_name_change);
    hub.publish(topic, msg);
    hub.receive();
    hub.receive();
    hub.receive();
    
    std::cout << std::endl << "* Note that the publised message has not been received" << std::endl;

    // TIP: hub.received_forever should be more convenient in your use case
    for (int i=0; i<10; ++i)
	hub.receive();

    hub.clear();

    /*
      This could be blocking because there are no subscriptions at
      this point (after the hub.clear)

      This hub.receive call is not blocking because I use a "false"
      argument
    */
    hub.receive(false); 
    std::cout << std::endl << "* Done" << std::endl;
    return 0;

}
