#include
#include
#include #include
#define ANALOGPIN A0

const char* ssid = "ssid";
const char* password = "password";

const char* mqttServer = "mqqt broker address";
const int mqttPort = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "password";

WiFiClient espClient;
PubSubClient client(espClient);

MQ135 gasSensor = MQ135(ANALOGPIN);
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
delay(100);
pinMode(2, OUTPUT);
// WiFi.softAP(hostssid, hostpassword);/
Serial.println();
WiFi.begin(ssid, password);
int i = 0;
while ( (i &lt;= 10) &amp;&amp; (WiFi.status() != WL_CONNECTED) )
{
delay(500);
Serial.print(".");
i++;
}
Serial.println();
// WiFi.config(ip,gateway,subnet,dns);
Serial.println("");
Serial.println("WiFi connected");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());

client.setServer(mqttServer, mqttPort);
client.setCallback(callback);

while (!client.connected()) {
Serial.println("Connecting to MQTT...");

if (client.connect("ESP8266Client1")) {

Serial.println("connected");

} else {

Serial.print("failed with state ");
Serial.print(client.state());
delay(2000);

}
}

client.publish("sendData", "Hello from ESP8266");
client.subscribe("sendData");

}

void callback(char* topic, byte* payload, unsigned int length) {

Serial.print("Message arrived in topic: ");
Serial.println(topic);

Serial.print("Message:");
for (int i = 0; i &lt; length; i++) {
Serial.print((char)payload[i]);
}

Serial.println();
Serial.println("-----------------------");

}

void loop() {
// put your main code here, to run repeatedly:
float rzero = gasSensor.getRZero(); //this to get the rzero value, uncomment this to get ppm value
Serial.print("RZero=");
Serial.println(rzero); // this to display the rzero value continuously, uncomment this to get ppm value

float ppm = gasSensor.getPPM(); // this to get ppm value, uncomment this to get rzero value
Serial.print("PPM=");
Serial.println(ppm); // this to display the ppm value continuously, uncomment this to get rzero value
DynamicJsonBuffer jsonBuffer;
JsonObject&amp; json = jsonBuffer.createObject();
json["ppm"] = ppm;
String d;
json.printTo(d);
client.publish("sendData", d.c_str());
Serial.println(d);
digitalWrite(2, HIGH);
delay(500);
digitalWrite(2, LOW);
delay(500);
}
