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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <jsonrpc/connectors/zmq.hpp>

#include <iostream>


namespace indyva {



    unsigned int extract_port(const std::string& url)
    {
	unsigned int port = 0;
	size_t found = url.rfind(":");

	if (found != std::string::npos) {
	    port = std::stoi(url.substr(found+1));
	}

	return port;
    }

    Hub::Hub(Rpc* r, zmq::context_t& context, const std::string& pubsub_url, const std::string& gateway_name)
	: rpc(r), url(pubsub_url), socket(context, ZMQ_SUB), subs_by_topic(), subs_by_token(), gateway(gateway_name)
    {

	Json::Value v;
	v.append(this->gateway);
	v.append("zmq");
	v.append(extract_port(this->url));
	this->rpc->call("HubSrv.new_gateway", v);

	// Connect to the PUB socket and subscribe to everything
	this->socket.connect(this->url.c_str());
	this->socket.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    }

    void Hub::publish(const std::string &topic, const Json::Value &msg) throw (jsonrpc::JsonRpcException)
    {
	Json::Value v;
        v["topic"] = topic;
        v["msg"] = msg;

        this->rpc->call("HubSrv.publish", v);
    }

    std::string Hub::subscribe(const std::string &topic, callback_t callback, bool only_once)
    {
	
	bool new_topic = (this->subs_by_topic.count(topic) == 0);

	boost::uuids::random_generator gen;
	boost::uuids::uuid u = gen();
	std::string token = boost::uuids::to_string(u);
	
	subscription_t subscription;
	subscription.only_once = only_once;
	subscription.callback = callback;
	subscription.token = token;
	subscription.topic = topic;

	this->subs_by_topic[topic].push_back(subscription);
	this->subs_by_token[token] = subscription;

	Json::Value v;
	v.append(this->gateway);
	v.append(topic);

	if (new_topic && only_once)
	    this->rpc->call("HubSrv.subscribe_once", v);
	else if (new_topic && !only_once)
	    this->rpc->call("HubSrv.subscribe", v);
	
	return token;
    }

    std::string Hub::subscribe(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException)
    {
	return subscribe(topic, callback, false);
    }

    std::string Hub::subscribe_once(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException)
    {
	return subscribe(topic, callback, true);
    }

    void Hub::unsubscribe(const std::string &token)
    {
	if (this->subs_by_token.count(token) == 0) {
	    std::stringstream msg;
	    msg << "There is no subscription with token: '" << token << "' to unsubscribe";
	    throw msg.str();
	}

        subscription_t subscription = this->subs_by_token.at(token);
	std::list<subscription_t> subscriptions = this->subs_by_topic.at(subscription.topic);
	
        subscriptions.remove(subscription);
	this->subs_by_token.erase(token);

	if (subscriptions.empty()) {
	    Json::Value v;
	    v.append(this->gateway);
	    v.append(subscription.topic);
	    this->rpc->call("HubSrv.unsubscribe", v);

	    this->subs_by_topic.erase(subscription.topic);
	}
    }

    void Hub::close(const std::string &topic) 
    {
	if (this->subs_by_topic.count(topic) == 0) {
	    std::stringstream msg;
	    msg << "There is no subscription with topic: '" << topic << "' to unsubscribe";
	    throw msg.str();
	}

	std::list<subscription_t> subscriptions = this->subs_by_topic.at(topic);    
	std::list<std::string> tokens;

	for (auto it = subscriptions.begin() ; it != subscriptions.end(); ++it) {
	    tokens.push_back(it->token);
	}
	for (auto it = tokens.begin() ; it != tokens.end(); ++it) {
	    unsubscribe(*it);
	}
    }

    void Hub::clear()
    {
	this->subs_by_topic.clear();
	this->subs_by_token.clear();

	Json::Value v;
	v.append(this->gateway);
	this->rpc->call("HubSrv.clear", v);
    }

    void Hub::receive() {
	zmq::message_t event;

	this->socket.recv(&event);
	std::string topic = std::string(static_cast<char*>(event.data()), event.size());

	this->socket.recv(&event);
	std::string msg_json = std::string(static_cast<char*>(event.data()), event.size());

        Json::Reader reader;
        Json::Value value;
        reader.parse(msg_json, value); // @TODO: control errors

	if (this->subs_by_topic.count(topic)) {
	    std::list<subscription_t> subscriptions = this->subs_by_topic.at(topic);

	    for (auto it = subscriptions.begin() ; it != subscriptions.end(); ++it) {
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
