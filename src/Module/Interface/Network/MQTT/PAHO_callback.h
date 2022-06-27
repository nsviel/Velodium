#ifndef PAHO_MQTT_CALLBACK_H
#define PAHO_MQTT_CALLBACK_H

#include <atomic>
#include "mqtt/async_client.h"


// A callback class for use with the main MQTT client.
class callback : public virtual mqtt::callback{
public:
	void connection_lost(const string& cause) override {
		cout << "\nConnection lost" << endl;
		if (!cause.empty()){
			cout << "\tcause: " << cause << endl;
    }
	}
};

// A base action listener.
class action_listener : public virtual mqtt::iaction_listener{
protected:
	void on_failure(const mqtt::token& tok) override {
		cout << "\tListener failure for token: " << tok.get_message_id() << endl;
	}
	void on_success(const mqtt::token& tok) override {
		cout << "\tListener success for token: " << tok.get_message_id() << endl;
	}
};

// A derived action listener for publish events.
class delivery_action_listener : public action_listener{
	atomic<bool> done_;

	void on_failure(const mqtt::token& tok) override {
		action_listener::on_failure(tok);
		done_ = true;
	}
	void on_success(const mqtt::token& tok) override {
		action_listener::on_success(tok);
		done_ = true;
	}

public:
	delivery_action_listener() : done_(false) {}
	bool is_done() const { return done_; }
};

#endif
