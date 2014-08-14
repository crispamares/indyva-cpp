/*************************************************************************
 * indyva-cpp
 *************************************************************************
 * @file    rpc.cpp
 * @date    14.07.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/
#include "rpc.h"



namespace indyva {

    Rpc::Rpc(zmq::context_t& context, const std::string& url)
    {
	this->zmqclient = new jsonrpc::ZmqClient(url, context);
	this->client = new jsonrpc::Client(this->zmqclient);
    }

    Rpc::~Rpc()
    {
	delete this->zmqclient;
	delete this->client;
    }
    
    Json::Value Rpc::call(const std::string& method, const Json::Value& paramter) throw (jsonrpc::JsonRpcException)
    {
	return this->client->CallMethod(method, paramter);
    }
} /* namespace indyva */



