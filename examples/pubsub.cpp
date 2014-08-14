/*************************************************************************
 * indyva-cpp
 *************************************************************************
 * @file    echo.cpp
 * @date    31.07.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>

#include <jsonrpc/client.h>
#include <jsonrpc/connectors/zmqclient.h>
#include <jsonrpc/connectors/zmq.hpp>

#include <indyva/hub.h>


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

    jsonrpc::ZmqClient zmqClient("tcp://localhost:18000", context);
    jsonrpc::Client client(&zmqClient);

    indyva::Hub hub(client, context, "tcp://localhost:18001");

    hub.subscribe("r:", render);
    hub.subscribe("name:change", p);

    try {

	/**
	 * Publish an event
	 *
	 */
        Json::Value msg("Paco");
	std::string topic("name:change");

        std::cout << "Pubishing-->\t  topic: " << topic << "\t msg: " << msg.asString()  << std::endl;
	hub.publish(topic, msg);

	hub.receive_forever();

        return 0;

    } catch(jsonrpc::JsonRpcException e) {

        std::cerr << "Exception occured: " << e.what() << std::endl;
        return -999;
    }
}
