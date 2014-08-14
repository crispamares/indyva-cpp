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

#include <indyva/rpc.h>

using namespace std;

int main()
{
    zmq::context_t context(1);

    cout << "Conecting to tcp://localhost:18000" << endl;

    indyva::Rpc rpc = indyva::Rpc(context, "tcp://localhost:18000");

    try {

	/**
	 * Call echo with position arguiments
	 *
	 */
        Json::Value v;
        v.append("Paco");  // Using append
        Json::Value result = rpc.call("echo", v);

        if(result.asString() != "Paco") {
            cerr << "Echo returned " << result.asString() << " but should be \"Paco\"" << endl;

            return -1;
        }

        cout << "OK positional:\t Echo returned " << result.asString()  << endl;

	/**
	 * Call echo with named arguiments
	 * The api is:  string echo (string a)
	 */
        Json::Value v2;
        v2["a"] = "Paco Pepe";  // like this
        result = rpc.call("echo", v2);

        if(result.asString() != "Paco Pepe") {
            cerr << "Echo returned " << result.asString() << " but should be \"Paco Pepe\"" << endl;

            return -1;
        }

        cout << "OK named:\t Echo returned " << result.asString()  << endl;

        return 0;

    } catch(jsonrpc::JsonRpcException e) {

        cerr << "Exception occured: " << e.what() << endl;
        return -999;
    }
}
