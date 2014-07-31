/*************************************************************************
 * indyva-cpp
 *************************************************************************
 * @file    hub.h
 * @date    31.07.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef HUB_H_
#define HUB_H_

#include "clientconnector.h"
#include "rpcprotocolclient.h"
#include "json/json.h"

#include <jsonrpc/client.h>
#include <jsonrpc/connectors/zmqclient.h>
#include <jsonrpc/connectors/zmq.hpp>


#include <vector>
#include <map>

namespace indyva {

    class Hub {
        public:
	Client(ZmqClient* client);
	
	void publish(const std::string &topic, const Json::Value &msg) throw (JsonRpcException);
	Json::Value CallMethod(const std::string& name, const Json::Value& paramter) throw (JsonRpcException);
	void CallNotification(const std::string& name, const Json::Value& paramter) throw (JsonRpcException);
	
        private:
	ZmqClient* client;
	
    };
    
} /* namespace indyva */
#endif /* HUB_H_ */


