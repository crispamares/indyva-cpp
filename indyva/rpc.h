/*************************************************************************
 * indyva-cpp
 *************************************************************************
 * @file    rpc.h
 * @date    14.07.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef RPC_H_
#define RPC_H_

#include <jsonrpc/client.h>
#include <jsonrpc/connectors/zmq.hpp>
#include <jsonrpc/connectors/zmqclient.h>

namespace indyva {
    
    class Rpc
    {
        public:

        Rpc(zmq::context_t& context, const std::string& url);
        ~Rpc();

        Json::Value call(const std::string& method, const Json::Value& paramter) throw (jsonrpc::JsonRpcException);

        private:
	
        jsonrpc::ZmqClient * zmqclient;
        jsonrpc::Client * client;
    };
    
} /* namespace indyva */
#endif /* RPC_H_ */


