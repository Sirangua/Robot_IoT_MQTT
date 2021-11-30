////////////////////  Libraries ////////////////////
#include <ESP32Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>


////////////////////  MQTT Info //////////////////// 
const char *mqtt_server = "direction or IP";                    // Add Your Own Information
const int mqtt_port = 1883;                                     // Add Your Own Information
const char *mqtt_user = "User";                                 // Add Your Own Information
const char *mqtt_pass = "Password";                             // Add Your Own Information
const char *topic_subscribe_move = "Inputs"; 
const char *topic_subscribe_servo = "servo";
const char *root_topic_publish = "Outputs"; 


////////////////////  Wifi Info ////////////////////
const char* password =  "Your Wifi Password";                   // Add Your Own Information
const char* ssid = "Your Wifi SSID";                            // Add Your Own Information


////////////////////  Pin Declarations ////////////////////
// Left motor
#define pin_in1    5  
#define pin_in2    18

// Right motor
#define pin_in3    19
#define pin_in4    21

// HC-SR04
#define pin_trigger  33
#define pin_echo  32

// Servo
#define pin_servo 27


////////////////////  Global Variables ////////////////////
// Servo
Servo servo;

// MQTT
WiFiClient espClient;
PubSubClient client (espClient);

// Comunications
String tosend = "";
char buf[23];



////////////////////  Functions Definition ////////////////////
void move (String side); 
void set_angle (String angle);
void setup_wifi ();
void callback (char* topic, byte* payload, unsigned int length);
void reconnect ();
void suscribe ();
int distance ();



////////////////////  Setup Function ////////////////////
void setup () {
    Serial.begin (115200);

    // Left motor
    pinMode (pin_in1, OUTPUT);
    pinMode (pin_in2, OUTPUT);
    
    // Right motor
    pinMode (pin_in3, OUTPUT);
    pinMode (pin_in4, OUTPUT);

    // Servo
    servo.attach (pin_servo);

    // HC-SR04
    pinMode (pin_trigger, OUTPUT);
    pinMode (pin_echo, INPUT);

    // Wifi & MQTT
    setup_wifi ();
    client.setServer (mqtt_server, mqtt_port);
    client.setCallback (callback);
}



////////////////////  Loop Function ////////////////////
void loop () {
    if (!client.connected ()) {
        reconnect ();
    }

    tosend = String (distance ());
    tosend.toCharArray (buf, 23);
    client.publish (root_topic_publish, buf); 
    client.loop ();
    delay (333);
}



/////////////////////////////////////////////////////
//////////////////// FUNCTIONS /////////////////////
///////////////////////////////////////////////////


////////////////////  Move Function ////////////////////
void move (String side){

    if (side == "straight"){
        digitalWrite (pin_in1, LOW);
        digitalWrite (pin_in2, HIGH);
        digitalWrite (pin_in3, HIGH);
        digitalWrite (pin_in4, LOW);
    }

    if (side == "back"){
        digitalWrite (pin_in1, HIGH);
        digitalWrite (pin_in2, LOW);
        digitalWrite (pin_in3, LOW);
        digitalWrite (pin_in4, HIGH);
    }

    if (side == "left"){
        digitalWrite (pin_in1, HIGH);
        digitalWrite (pin_in2, LOW);
        digitalWrite (pin_in3, HIGH);
        digitalWrite (pin_in4, LOW);
    }

    if (side == "right"){
        digitalWrite (pin_in1, LOW);
        digitalWrite (pin_in2, HIGH);
        digitalWrite (pin_in3, LOW);
        digitalWrite (pin_in4, HIGH);
    }

    if (side == "stop"){
        digitalWrite (pin_in4, LOW);
        digitalWrite (pin_in1, LOW);
        digitalWrite (pin_in2, LOW);
        digitalWrite (pin_in3, LOW);
    }
}


////////////////////  Sensor Angle Function ////////////////////
void set_angle (String angle){
    if (angle == "0"){
        servo.write (180);       
    }
    if (angle == "1"){
        servo.write (135);       
    }
    if (angle == "2"){
        servo.write (90);       
    }
    if (angle == "3"){
        servo.write (45);       
    }
    if (angle == "4"){
        servo.write (0);       
    }       
}


////////////////////  Wifi Setup ////////////////////
void setup_wifi (){
    delay (10);
    Serial.println ();
    Serial.print ("Conectando a  ");
    Serial.print (ssid);

    WiFi.begin (ssid, password);

  
    while (WiFi.status () != WL_CONNECTED) {
        delay (500);
        Serial.print (".");
    }

    Serial.println ("");
    Serial.println ("Conectado a red WiFi");
    Serial.print ("Dirección IP: ");
    Serial.println (WiFi.localIP ());
}

//////////////////// Callback ////////////////////
void callback(char* topic, byte* payload, unsigned int length){
	String incoming = "";
	Serial.print("Mensaje recibido desde -> ");
    Serial.print(topic);
	Serial.println("");

	for (int i = 0; i < length; i++) {
		incoming += (char) payload [i];
	}

	incoming.trim ();
	Serial.println ("Mensaje: " + incoming);

    String topic_str (topic);

    ////////// Calling functions for message //////////
	if(topic_str == topic_subscribe_move){  
        move (incoming);
    }

    if(topic_str == topic_subscribe_servo){  
        set_angle (incoming);
    }
}

////////////////////  MQTT Conection ////////////////////
void reconnect () {
    while (!client.connected ()) {
        Serial.print("Conectando MQTT...");
        // Creamos un cliente ID
        String clientId = "ESP32_";
        clientId += String (random (0xffff), HEX);
        // Intentamos conectar
        if (client.connect (clientId.c_str (), mqtt_user, mqtt_pass)) {
            Serial.println ("conectado");
            subscribe ();
            tosend = String ("🤖 Robot conectado");
            tosend.toCharArray (buf, 23);
            client.publish ("Board", buf);
        } else {
            Serial.print ("Ha fallado la conexión MQTT   [");
            Serial.print (client.state ());
            Serial.println ("]   Intentamos de nuevo en 5 segundos");
            delay (5000);
        }
    }
}

////////////////////  MQTT Suscription ////////////////////
void subscribe () {
    //Suscribe to inputs
    if(client.subscribe (topic_subscribe_move)) {
        Serial.print ("Suscripcion realizada a: ");
        Serial.println (topic_subscribe_move);
    } else{
        Serial.println ("Ha fallado la suscripción");
    }

    //Suscribe to servo
    if(client.subscribe (topic_subscribe_servo)) {
        Serial.print ("Suscripcion realizada a: ");
        Serial.println (topic_subscribe_servo);
    } else{
        Serial.println ("Ha fallado la suscripción");
    }
}

////////////////////  Distance Compute ////////////////////
int distance () {
    digitalWrite (pin_trigger, HIGH);
    delayMicroseconds (10);
    digitalWrite (pin_trigger, LOW);

    long time = pulseIn (pin_echo, HIGH);
    int distance = time / 59;
    return distance;
}
