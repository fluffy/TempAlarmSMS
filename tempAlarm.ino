/*
  Cullen Jennings
 * Ethernet & SD share SPI bus on pin 11, 12, 13
 * SD Card Select is on pin 4
 * Ethernet Select ison pin 10
 * Temp sensor on oneWire bus at pin 2
 */

#include <SPI.h>
#include <Ethernet.h>

#include <OneWire.h> // get from http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h> // get from https://github.com/milesburton/Arduino-Temperature-Control-Library

// replace this with your MAC - should be on sticker on sheild
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x58, 0xE7 };

// replace following with the token for your Tropo applicaiton
char token[] = "0a70119d927d9c4bbd4dc8f20bee794cfd5c387ddbd7c8798cd5dd273cad590730df8af3999bd5ce8399519a";

const float alarmLow  = 10.0; // send alarm if temp in degreee C is less than this
const float alarmHigh = 20.0; // send alarm if temp in degreee C is less than this

char server[] = "api.tropo.com";

char url[] = "/1.0/sessions?action=create&token=";
char vars[] = "&temp=";
float tempC;

OneWire oneWire(2); //on pin 2
DallasTemperature sensors(&oneWire);

enum  { waiting, starting, sensing, sending, maintainDHCP, startWait } state;

EthernetClient client;

unsigned long startWaitTime;

void setup() {
  Serial.begin(9600);

  sensors.begin();

  Serial.print("Looking for IP address ...");
  while (Ethernet.begin(mac) == 0) {
    Serial.println("No DHCP");

    delay( 5000 /*ms*/ );
  }
 Serial.println("Done");
  delay(1000);

  state = starting;
}

void startHttpReq()
{
  Serial.print("Connecting...");

  if (client.connect(server, 80)) {
    Serial.println("connected");

    client.print("GET ");
    client.print(url);
    client.print(token);
    client.print(vars);
    client.print( tempC );
    client.println(" HTTP/1.1");

    client.print("Host: ");
    client.println(server);

    client.println("Connection: close");
    client.println();
  }
  else {

    Serial.println("connection failed");
  }
}

void loop()
{
  if ( state == starting ) {
    Serial.print("Reading Temp...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("Done");
    state = sensing;
  }

  if ( state == sensing ) {

    tempC = sensors.getTempCByIndex(0);
    Serial.print("Tempm = ");
    Serial.println( tempC );

    if ( (tempC > alarmHigh) || (tempC < alarmLow) )
    {
      startHttpReq();
      state = sending;
    }
    else
    {
      // skip sending to web server
      state = maintainDHCP;
    }
  }

  if ( state == sending )
  {
    if  (client.available()) {
      char c = client.read();
      Serial.print(c);
    }

    if (!client.connected()) {
      Serial.println();
      Serial.println("Done Sending");
      client.stop();

      state = maintainDHCP;
    }
  }

  if ( state == maintainDHCP  )
  {
    byte r;
    r = Ethernet.maintain();
    Serial.print("Ethernet maintain = " );
    Serial.println( r );
    state = startWait;
  }

  if ( state == startWait  )
  {
    startWaitTime = millis();
    state = waiting;
  }

  if ( state == waiting  )
  {
    if ( millis() - startWaitTime > (unsigned long)600000 )
    {
      state = starting;
      Serial.println( '*' );
    }
    delay( 5000 ); // could power down
    Serial.print( '.' );
    Serial.println( millis() - startWaitTime );
  }
}

