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
 *  
 */





// Load Wi-Fi library
#include <WiFi.h>
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 4                              // Set INT to pin 4 (2 for Atmega)
MCP_CAN CAN0(5);                               // Set CS to pin 5 (10 for Atmega)



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

  
}

void loop()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();
  }
}
