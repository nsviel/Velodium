//Sample at https://github.com/eclipse/paho.mqtt.cpp/blob/master/src/samples/async_publish.cpp
#include "MQTT.h"

#include "MQTT_callback.h"

#include "mqtt/async_client.h"

#include "../../../../Load/Processing/file_JSON.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>
#include <cstring>


//Constructor / Destructor
MQTT::MQTT(){
  //---------------------------

  //Connection
  this->server_address = "tcp://localhost:1883";
  this->client_topic = "Obstacle";
  this->client_message = "Hello world!";
  this->client_ID = "velodium";
  this->client_json = "{\nsepalLength: 6.4,\nsepalWidth:  3.2,\npetalLength: 4.5,\npetalWidth:  1.5,\n}";

  //Parameters
  this->qos = 1;
  this->persist_dir	= "./persist";
  this->timeout = std::chrono::seconds(2);

  //---------------------------
}
MQTT::~MQTT(){
  //---------------------------

  //---------------------------
}

void MQTT::mqtt_test_localhost(){
  //---------------------------

  string server_address  = "tcp://localhost:1883";
  string client_ID = "paho_cpp_async_publish";

  //Initialize connection parameters
  cout << "Initializing for server '" << server_address << "'..." << flush;
  mqtt::async_client client(server_address, client_ID, persist_dir);

  callback cb;
  client.set_callback(cb);

  auto connOpts = mqtt::connect_options_builder()
    .clean_session()
    .will(mqtt::message("Test_localhost", "Test_0", qos))
    .finalize();

  cout << " [OK]" << endl;

  try {
    //Serveur connection
    cout << "Connecting..." << endl;
    mqtt::token_ptr conntok = client.connect(connOpts);
    cout << "Waiting for the connection..." << flush;
    conntok->wait();
    cout << " [OK]" << endl;

    // First use a message pointer.
    cout << "Sending message..." << flush;
    mqtt::message_ptr pubmsg = mqtt::make_message("Test_localhost", "Test_1");
    pubmsg->set_qos(qos);
    client.publish(pubmsg)->wait_for(timeout);

    // Now try with itemized publish.
    cout << "Sending next message..." << flush;
    mqtt::delivery_token_ptr pubtok;
    pubtok = client.publish("Test_localhost", "Test_2", strlen("Test_2"), qos, false);
    cout << "  ...with token: " << pubtok->get_message_id() << endl;
    cout << "  ...for message with " << pubtok->get_message()->get_payload().size()
      << " bytes" << endl;
    pubtok->wait_for(timeout);

    // Now try with a listener
    cout << "Sending next message..." << flush;
    action_listener listener;
    pubmsg = mqtt::make_message("Test_localhost", "Test_3");
    pubtok = client.publish(pubmsg, nullptr, listener);
    pubtok->wait();

    // Finally try with a listener, but no token
    cout << "Sending final message..." << flush;
    delivery_action_listener deliveryListener;
    pubmsg = mqtt::make_message("Test_localhost", "Test_4");
    client.publish(pubmsg, nullptr, deliveryListener);

    while (!deliveryListener.is_done()) {
      this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Double check that there are no pending tokens
    auto toks = client.get_pending_delivery_tokens();
    if (!toks.empty())
      cout << "\n[Error] There are pending delivery tokens!" << endl;

    // Disconnect
    cout << "\nDisconnecting..." << endl;
    client.disconnect()->wait();
  }
  catch (const mqtt::exception& exc) {
    cerr << exc.what() << endl;
  }

  //---------------------------
}
void MQTT::mqtt_sendMessages(){
  //---------------------------

  //Initialize connection parameters
  cout << "Initializing for server '" << server_address << "'..." << flush;
  mqtt::async_client client(server_address, client_ID, persist_dir);

  callback cb;
  client.set_callback(cb);

  auto connOpts = mqtt::connect_options_builder()
    .clean_session()
    .finalize();

  cout << " [OK]" << endl;

  try {
    //Serveur connection
    cout << "Connecting..." << flush;
    mqtt::token_ptr conntok = client.connect(connOpts);
    conntok->wait();
    cout << " [OK]" << endl;

    // First use a message pointer.
    cout << "Sending message..." << flush;
    mqtt::message_ptr pubmsg = mqtt::make_message(client_topic, client_message);
    pubmsg->set_qos(qos);
    client.publish(pubmsg)->wait_for(timeout);
    cout << " [OK]" << endl;

    // Disconnect
    client.disconnect()->wait();
    cout << "Disconnecting... [OK]" << endl;
  }
  catch (const mqtt::exception& exc) {
    cerr << exc.what() << endl;
  }

  //---------------------------
}
