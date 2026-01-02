// CAN Receive Example
//

/* Up until these changes - this compiles for both Atmega 328 Minicore and ESP32 Dev module using Arduino 1.8.6 hourly build 2018/05/28 10:12
 *  ESP32 Default SPI pins are
 *  GPIO 5 = CS
 *  GPIO 18  =SCK
 *  GPIO 19 = MISO
 *  GPIO 23 = MOSI
 *  
 *  Connect MOSI to MOSI and MISO to MISO
 *  
 *  This works. ESP32-S to MCP2515 direct connection at 3.3 V logic level.
 *  INT to 4
 *  SCK-18
 *  SI-23
 *  SO-19
 *  CS-5
 *  GND-GND
 *  VCC-3.3V
 *  
 *  Create an AP
 *  Webpage stays locally on the client. Clinet connects via websocket to ESP32
 *  push CANBUS messages to WebSocket
 *  Capture events like Start saving. Stop Saving. Cruising, pedaling, etc 
 *  
 *  no default IDE option to erase Flash
 *  use this
 *  /Users/kv2000in/Library/Arduino15/packages/esp32/tools/esptool_py/3.0.0/esptool --chip esp32 --port /dev/cu.wchusbserialfa14200 --baud 115200 erase_flash
 *  
 *  however, erasing flash doesn't lead to successful custom AP name creation.
 *  
 *  from the docs https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html#ap-example
 *  
 *  "// a valid password must have more than 7 characters" - this worked
 *  ASYNCTCP by ESP32ASYNC version 3.4.10 doesn't compile. Version 3.0.0 compiles.
 *  
 */





// Load Wi-Fi library
#include <WiFi.h>
#include <mcp_can.h>
#include <SPI.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 4                              // Set INT to pin 4 (2 for Atmega)
MCP_CAN CAN0(5);                               // Set CS to pin 5 (10 for Atmega)



AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");

void handlecommandsfromwebsocketclients(uint8_t *datasentbywebsocketclient, size_t len) {
  Serial.printf("Handling command data");

  // Only process data further if it is 24 bytes
  if (len != 24) {
    Serial.printf("Invalid data length");
    return;
  }

  // Print received data
  for (size_t i = 0; i < len; i++) {
    Serial.printf("%02x ", datasentbywebsocketclient[i]);
  }
  Serial.println();

  
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if(type == WS_EVT_CONNECT){
    //client connected
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    //client->printf("Z#a020a60a86ce|-811493|2022-4-12 21:33:49");
    //client->printf("C#a020a60a86ce|500/285|2022-4-12 21:33:49"); 
    //client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    //client disconnected
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    //error was received from the other end
    Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    //pong message was received (in response to a ping request maybe)
    Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
  
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      //the whole message is in a single frame and we got all of it's data
      Serial.printf("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", info->len);
      if(info->opcode == WS_TEXT){
        data[len] = 0;
        Serial.printf("%s\n", (char*)data);
//        
      } else {
        for(size_t i=0; i < info->len; i++){
          Serial.printf("%02x ", data[i]);
        }
        Serial.printf("\n");
      }
      if(info->opcode == WS_TEXT){
        //client->text("I got your text message");
      }
      else{
        //client->binary("I got your binary message");
      }
    
        
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->index == 0){
        if(info->num == 0)
          Serial.printf("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
        Serial.printf("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
      }

      Serial.printf("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if(info->message_opcode == WS_TEXT){
        data[len] = 0;
        Serial.printf("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < len; i++){
          Serial.printf("%02x ", data[i]);
        }
        Serial.printf("\n");
      }

      if((info->index + len) == info->len){
        Serial.printf("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          Serial.printf("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT){
           // client->text("I got your text message");
          }
          else {
         //   client->binary("I got your binary message");
        }
        }
      }
    }
 //data packet
    handlecommandsfromwebsocketclients(data,len);
    
  }
}

void setup()
{

Serial.begin(115200);
Serial.println("############################");
Serial.println("ESP32 Information");
  Serial.printf(
    "Internal Total Heap %d, Internal Used Heap %d, Internal Free Heap %d\n",
    ESP.getHeapSize(),
    ESP.getHeapSize() - ESP.getFreeHeap(),
    ESP.getFreeHeap()
  );

  Serial.printf(
    "Sketch Size %d, Free Sketch Space %d\n",
    ESP.getSketchSize(),
    ESP.getFreeSketchSpace()
  );

  Serial.printf(
    "SPIRam Total Heap %d, SPIRam Free Heap %d\n",
    ESP.getPsramSize(),
    ESP.getFreePsram()
  );

  Serial.printf(
    "Chip Model %s, Chip Revision %d, CPU Freq %d MHz, SDK Version %s\n",
    ESP.getChipModel(),
    ESP.getChipRevision(),
    ESP.getCpuFreqMHz(),
    ESP.getSdkVersion()
  );

  Serial.printf(
    "Flash Size %d, Flash Speed %d\n",
    ESP.getFlashChipSize(),
    ESP.getFlashChipSpeed()
  );
Serial.println("############################");


// Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  //CAN0.setMode(MCP_LISTENONLY);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
CAN0.setMode(MCP_NORMAL);
  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");

    Serial.print("Setting AP (Access Point)…");
  delay(1000);

  // Disconnect and erase stored WiFi credentials
  WiFi.disconnect(true, true);
  delay(1000);

  WiFi.mode(WIFI_MODE_NULL);
  delay(1000);

  // Force AP-only mode
  WiFi.mode(WIFI_AP);

  const char* ssid = "ESP32CANBUS";
  const char* password = "hanuman123";

  bool ok = WiFi.softAP(ssid, password);

  Serial.println(ok ? "AP started" : "AP failed");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

   webSocket.onEvent(onEvent);
  server.addHandler(&webSocket);
  server.begin();
}

void loop()
{
  webSocket.cleanupClients();

  if (!digitalRead(CAN0_INT))
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);

    uint8_t wsBuf[16];
    uint8_t idx = 0;

    // ---- CAN ID (4 bytes, big-endian) ----
    wsBuf[idx++] = (rxId >> 24) & 0xFF;
    wsBuf[idx++] = (rxId >> 16) & 0xFF;
    wsBuf[idx++] = (rxId >> 8)  & 0xFF;
    wsBuf[idx++] = rxId & 0xFF;

    // ---- DLC ----
    wsBuf[idx++] = len;

    // ---- Flags ----
    uint8_t flags = 0;
    if (rxId & 0x80000000) flags |= 0x01; // Extended
    if (rxId & 0x40000000) flags |= 0x02; // RTR
    wsBuf[idx++] = flags;

    // ---- Data ----
    if (!(flags & 0x02)) {
      memcpy(&wsBuf[idx], rxBuf, len);
      idx += len;
    }

    // ---- Send binary to ALL clients ----
    webSocket.binaryAll(wsBuf, idx);

    // ---- Optional Serial debug ----
    Serial.print("WS BIN: ");
    for (uint8_t i = 0; i < idx; i++) {
      Serial.printf("%02X ", wsBuf[i]);
    }
    Serial.println();
  }
}
