/*************************************************************************
 * indyva-cpp
 *************************************************************************
 * @file    hub.cpp
 * @date    14.08.2014
 * @author  Juan Morales <juan.morales@upm.es>
 * @license See attached LICENSE.txt
 ************************************************************************/
#include "hub.h"

#include <vector>
#include <map>

#include <jsonrpc/client.h>
#include <jsonrpc/connectors/zmqclient.h>
#include <jsonrpc/connectors/zmq.hpp>

#include <iostream>


namespace indyva {

    const std::string GATEWAY("gtzmq");

    unsigned int extract_port(const std::string& url)
    {
	unsigned int port = 0;
	size_t found = url.rfind(":");

	if (found != std::string::npos) {
	    port = std::stoi(url.substr(found+1));
	}

	return port;
    }

    Hub::Hub(jsonrpc::Client& c, zmq::context_t& context, const std::string& pubsub_url)
	: client(c), url(pubsub_url), socket(context, ZMQ_SUB), subscriptions()
    {

	Json::Value v;
	v.append(GATEWAY);
	v.append("zmq");
	v.append(extract_port(this->url));
	this->client.CallMethod("HubSrv.new_gateway", v);

	// Connect to the PUB socket and subscribe to everything
	this->socket.connect(this->url.c_str());
	this->socket.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    }

    void Hub::publish(const std::string &topic, const Json::Value &msg) throw (jsonrpc::JsonRpcException)
    {
	Json::Value v;
        v["topic"] = topic;
        v["msg"] = msg;

        this->client.CallMethod("HubSrv.publish", v);
    }

    void Hub::subscribe(const std::string &topic, callback_t callback, bool only_once) throw (jsonrpc::JsonRpcException) 
    {
	
	bool new_topic = (this->subscriptions.count(topic) == 0);
	
	subscription_t subscription;
	subscription.only_once = only_once;
	subscription.callback = callback;
	
	this->subscriptions[topic].push_back(subscription);

	Json::Value v;
	v.append(GATEWAY);
	v.append(topic);

	if (new_topic && only_once)
	    this->client.CallMethod("HubSrv.subscribe_once", v);
	else if (new_topic && !only_once)
	    this->client.CallMethod("HubSrv.subscribe", v);
    }

    void Hub::subscribe(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException)
    {
	subscribe(topic, callback, false);
    }

    void Hub::subscribe_once(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException)
    {
	subscribe(topic, callback, true);
    }
//    void Hub::unsubscribe(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException){}


    void Hub::receive() {
	zmq::message_t event;

	this->socket.recv(&event);
	std::string topic = std::string(static_cast<char*>(event.data()), event.size());

	this->socket.recv(&event);
	std::string msg_json = std::string(static_cast<char*>(event.data()), event.size());

        Json::Reader reader;
        Json::Value value;
        reader.parse(msg_json, value); // @TODO: control errors

	if (this->subscriptions.count(topic)) {
	    std::vector<subscription_t> matching_subscriptions = this->subscriptions.at(topic);

	    for (auto it = matching_subscriptions.begin() ; it != matching_subscriptions.end(); ++it) {
		it->callback(topic, value);
	    }
	}
	else {
	    std::cerr << "Warning: Event with topic '" << topic << "' DISCARTED because there are no subscriptions to the topic" << std::endl;
	}
    }

    void Hub::receive_forever()
    {
	while(1) {
	    receive();
	}
    }

} /* namespace indyva */
