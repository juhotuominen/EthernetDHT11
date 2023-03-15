// Arduino webserver for reading DHT11 data, Ethernet

#include <dht11.h>
#include <SPI.h>
#include <Ethernet.h>

#define DHT11PIN 7

dht11 DHT11;

byte mac[] = {0x90, 0xA2, 0XDA, 0x00, 0x4A, 0xE0};  // mac addres for ethernetshield
EthernetServer server(80);

String readString, webPage;

void   setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server started...\nLocal IP: ");  // server on and ip to connect
  Serial.println(Ethernet.localIP());
 
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);
  EthernetClient client = server.available();
  if (client)  // if server is available go on
    {
      while(client.connected())
      {
        if(client.available())
        {
          char c = client.read();
          while(readString.length() < 100 && c != 0x0D) // read byte is under 100 in length or is not a carriage return
          {
            readString += c; // save data from client to string and read next byte
            c = client.read();
          }
          HTML_webPage();  // if client is connected and available, run webpage

         client.println(webPage); // send webpage to server
         delay(10); 
         client.stop(); // disconnect client from the server, if still available, loop continues
        
         readString="";
        }
      }
    }
}

void HTML_webPage()
  {
    webPage = "HTTP/1.1 200 OK\n\rContent-Type: text/html\n\r\n\r";
    //-------------------------------------------------------------
    webPage += "<!DOCTYPE html>";
    webPage += "<html>";
    webPage += "<head>";
    webPage += "<meta http-equiv=\"refresh\" content=\"30\">"; //refresh every 30 seconds
    webPage += "<title>DHT11 Sensor</title>";
    webPage += "</head>";
    webPage += "<style>";
    webPage += "body {font-family: monaco, Consolas;" // style for site
                    "background-color: rgba(50, 50, 50, 0.904);"
                    "color: white;}";
    webPage += "</style>";
    webPage += "<body>";
    webPage += "<h1>";
    webPage += "DHT11 Sensor<br><br>";
    webPage += "Temp&nbsp;&nbsp;&nbsp;&nbsp;: ";
    webPage += DHT11.temperature;  // Read value for temp
    webPage += "&#176;C<br>Humidity: ";
    webPage += int(DHT11.humidity); // Read value for humid
    webPage += "%</h1>";
    webPage += "</body>";
    webPage += "</html>";
  }
