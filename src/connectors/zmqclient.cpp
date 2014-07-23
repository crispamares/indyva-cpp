/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    zmqclient.cpp
 * @date    23.07.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include "zmqclient.h"
#include <iostream>

using namespace std;

namespace jsonrpc
{

    ZmqClient::ZmqClient(const std::string& url, const zmq::context_t& context) throw(JsonRpcException)
        : AbstractClientConnector(), url(url), socket(context, ZMQ_REQ)
    {
	// Create and connect the zmq's REQ socket
   
	this->socket.connect(url.c_str());
    }
    
    ZmqClient::~ZmqClient()
    {
	// Close the socket because calls the m_socket's destructor
    }
    
    void ZmqClient::SendMessage(const std::string& message, std::string& result) throw (JsonRpcException)
    {
	// socket.send
	zmq::message_t request(message.size());
	memcpy (request.data(), message.data(), message.size());

	bool rc = this->socket.send(request);
	stringstream str;
	if (!rc) {
	    str << ": Error sending message to " << this->url;
	    throw JsonRpcException(Errors::ERROR_CLIENT_CONNECTOR, str.str());
	}

	// scocket reciv
	zmq::message_t response;
	rc = this->socket.recv(&response);
	if (!rc) {
	    str << ": Error reciving message from " << this->url;
	    throw JsonRpcException(Errors::ERROR_CLIENT_CONNECTOR, str.str());
	}

	result = std::string(static_cast<char*>(response.data()), response.size());
    }
    
    
} /* namespace jsonrpc */
