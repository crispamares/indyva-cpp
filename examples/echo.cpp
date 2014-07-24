/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    helloworld.cpp
 * @date    08.03.2013
 * @author  Peter Spiess-Knafl <peter.knafl@gmail.com>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <stdio.h>
#include <string>
#include <iostream>

#include <jsonrpc/procedure.h>
#include <jsonrpc/specificationwriter.h>
#include <jsonrpc/client.h>
#include <jsonrpc/connectors/zmqclient.h>
#include <jsonrpc/connectors/zmq.hpp>

using namespace jsonrpc;
using namespace std;



int main(int argc, char** argv)
{
    zmq::context_t context(1);
    
    
    cout << "Conecting to tcp://localhost:18000" << endl;
    
    ZmqClient *zmqClient = new ZmqClient("tcp://localhost:18000", context);
    Client* client = new Client(zmqClient);
    
    try {
	
	/**
	 * Call echo with position arguiments
	 *
	 */
        Json::Value v;
        v.append("Paco");  // Using append
        Json::Value result = client->CallMethod("echo", v);

        if(result.asString() != "Paco") {
            cerr << "Echo returned " << result.asString() << " but should be \"Paco\"" << endl;

            delete client;
            delete zmqClient;

            return -1;
        }

        cout << "OK positional:\t Echo returned " << result.asString()  << endl;

	/**
	 * Call echo with named arguiments
	 * The api is:  string echo (string a)
	 */
        Json::Value v2;
        v2["a"] = "Paco Pepe";  // like this
        result = client->CallMethod("echo", v2);

        if(result.asString() != "Paco Pepe") {
            cerr << "Echo returned " << result.asString() << " but should be \"Paco Pepe\"" << endl;

            delete client;
            delete zmqClient;

            return -1;
        }

        delete client;
        delete zmqClient;
        cout << "OK named:\t Echo returned " << result.asString()  << endl;

        return 0;



	



    } catch(jsonrpc::JsonRpcException e) {

        cerr << "Exception occured: " << e.what() << endl;
        delete client;
        delete zmqClient;
        return -999;
    }
}
