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

#include <list>
#include <map>
#include <boost/function.hpp> 

#include <jsonrpc/client.h>
#include <jsonrpc/connectors/zmqclient.h>
#include <jsonrpc/connectors/zmq.hpp>


namespace indyva {

    typedef boost::function<void(const std::string &topic, const Json::Value &msg)> callback_t;

    class Hub {
        public:

	Hub(jsonrpc::Client& rpc_client, zmq::context_t& context, const std::string& pusub_url);
	
	void publish(const std::string &topic, const Json::Value &msg) throw (jsonrpc::JsonRpcException);
	std::string subscribe(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException);
	std::string subscribe_once(const std::string &topic, callback_t callback) throw (jsonrpc::JsonRpcException);
	void unsubscribe(const std::string &token);
	void close(const std::string &topic);
	void clear();
	
        void receive();
        void receive_forever();

        private:

        struct subscription_t {
            bool only_once;
            callback_t callback;
	    std::string token;
	    std::string topic;

	    bool operator==(const subscription_t& other) const
	    {
		return token == other.token;
	    }
        };

        std::string subscribe(const std::string &topic, callback_t callback, bool only_once);
	bool has_callback(const subscription_t & subscription, const callback_t & callback);

        jsonrpc::Client& client;
	std::string url;
	zmq::socket_t socket;
        std::map<std::string, std::list<subscription_t>> subs_by_topic;
        std::map<std::string, subscription_t> subs_by_token;
	
    };
    
} /* namespace indyva */
#endif /* HUB_H_ */


