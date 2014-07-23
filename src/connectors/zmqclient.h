/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    zmqclient.h
 * @date    23.07.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/

#ifndef ZMQCLIENT_H_
#define ZMQCLIENT_H_

#include "../../vendor/libjson-rpc-cpp/src/jsonrpc/clientconnector.h"
#include "../../vendor/libjson-rpc-cpp/src/jsonrpc/exception.h"

#include <zmq.hpp>


namespace jsonrpc
{
    
    class ZmqClient : public AbstractClientConnector
    {
      public:
ZmqClient(const std::string& url, const zmq::context_t& context) throw (JsonRpcException);
	virtual ~ZmqClient();
	
	virtual void SendMessage(const std::string& message, std::string& result) throw (JsonRpcException);
	
      private:
        std::string url;
	zmq::socket_t socket;
    };
    
} /* namespace jsonrpc */
#endif /* ZMQCLIENT_H_ */
