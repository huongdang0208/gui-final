
// #include <mosquitto.h>
// #include "../protos/proto3/led_demo.pb.h"

// void on_connect(struct mosquitto *mosq, void *obj, int rc) {
//     if (rc) {
//         std::cerr << "Error: Unable to connect (" << rc << ")" << std::endl;
//         return;
//     }
//     std::cout << "Connected to the broker." << std::endl;
//     mosquitto_subscribe(mosq, NULL, "sensor/data", 0);
// }

// void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
//     std::cout << "Received message: " << msg->topic << std::endl;

//     // Deserialize the protobuf message
//     SensorData sensorData;
//     sensorData.ParseFromArray(msg->payload, msg->payloadlen);
//     std::cout << "ID: " << sensorData.id() << ", Temperature: " << sensorData.temperature()
//               << ", Humidity: " << sensorData.humidity() << std::endl;
// }

// int main(int argc, char *argv[]) {
//     // Initialize the Mosquitto library
//     mosquitto_lib_init();

//     struct mosquitto *mosq = mosquitto_new("mqtt_client", true, NULL);
//     if (!mosq) {
//         std::cerr << "Error: Unable to create Mosquitto instance." << std::endl;
//         return 1;
//     }

//     mosquitto_connect_callback_set(mosq, on_connect);
//     mosquitto_message_callback_set(mosq, on_message);

//     if (mosquitto_connect(mosq, "localhost", 1883, 60)) {
//         std::cerr << "Error: Unable to connect to broker." << std::endl;
//         return 1;
//     }

//     mosquitto_loop_start(mosq);

//     // Serialize and publish a protobuf message
//     SensorData sensorData;
//     sensorData.set_id(1);
//     sensorData.set_temperature(23.5);
//     sensorData.set_humidity(56.2);

//     std::string payload;
//     sensorData.SerializeToString(&payload);
//     mosquitto_publish(mosq, NULL, "sensor/data", payload.size(), payload.data(), 0, false);

//     std::this_thread::sleep_for(std::chrono::seconds(5));

//     mosquitto_loop_stop(mosq, true);
//     mosquitto_disconnect(mosq);
//     mosquitto_destroy(mosq);
//     mosquitto_lib_cleanup();

//     return 0;
// }
