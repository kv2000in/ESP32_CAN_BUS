// CAN Receive Example
//

/* Up until these changes - this compiles for both Atmega 328 Minicore and ESP32 Dev module using Arduino 1.8.6 hourly build 2018/05/28 10:12
 *  ESP32-S initial boot serial message
 *  
 *  21:41:35.658 -> ets Jun  8 2016 00:22:57
21:41:35.659 -> 
21:41:35.659 -> rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
21:41:35.659 -> configsip: 0, SPIWP:0xee
21:41:35.659 -> clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
21:41:35.659 -> mode:DIO, clock div:2
21:41:35.659 -> load:0x3fff0030,len:4
21:41:35.659 -> load:0x3fff0034,len:6968
21:41:35.659 -> load:0x40078000,len:13072
21:41:35.659 -> ho 0 tail 12 room 4
21:41:35.659 -> load:0x40080400,len:3896
21:41:35.659 -> entry 0x40080688
21:41:35.659 -> [0;32mI (30) boot: ESP-IDF v4.1-dirty 2nd stage bootloader[0m
21:41:35.692 -> [0;32mI (30) boot: compile time 16:15:01[0m
21:41:35.692 -> [0;32mI (30) boot: chip revision: 1[0m
21:41:35.692 -> [0;32mI (33) boot_comm: chip revision: 1, min. bootloader chip revision: 0[0m
21:41:35.692 -> [0;32mI (40) boot.esp32: SPI Speed      : 40MHz[0m
21:41:35.692 -> [0;32mI (45) boot.esp32: SPI Mode       : DIO[0m
21:41:35.692 -> [0;32mI (50) boot.esp32: SPI Flash Size : 4MB[0m
21:41:35.692 -> [0;32mI (54) boot: Enabling RNG early entropy source...[0m
21:41:35.725 -> [0;32mI (60) boot: Partition Table:[0m
21:41:35.725 -> [0;32mI (63) boot: ## Label            Usage          Type ST Offset   Length[0m
21:41:35.725 -> [0;32mI (70) boot:  0 nvs              WiFi data        01 02 00009000 00005000[0m
21:41:35.725 -> [0;32mI (78) boot:  1 otadata          OTA data         01 00 0000e000 00002000[0m
21:41:35.725 -> [0;32mI (85) boot:  2 app0             OTA app          00 10 00010000 00300000[0m
21:41:35.761 -> [0;32mI (93) boot:  3 spiffs           Unknown data     01 82 00310000 000f0000[0m
21:41:35.761 -> [0;32mI (100) boot: End of partition table[0m
21:41:35.761 -> [0;32mI (105) boot_comm: chip revision: 1, min. application chip revision: 0[0m
21:41:35.761 -> [0;32mI (112) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x1d2048 (1908808) map[0m
21:41:36.501 -> [0;32mI (848) esp_image: segment 1: paddr=0x001e2070 vaddr=0x3ffbdb60 size=0x04d3c ( 19772) load[0m
21:41:36.501 -> [0;32mI (856) esp_image: segment 2: paddr=0x001e6db4 vaddr=0x40080000 size=0x00400 (  1024) load[0m
21:41:36.501 -> [0;32mI (857) esp_image: segment 3: paddr=0x001e71bc vaddr=0x40080400 size=0x08e54 ( 36436) load[0m
21:41:36.534 -> [0;32mI (879) esp_image: segment 4: paddr=0x001f0018 vaddr=0x400d0018 size=0x9df74 (647028) map[0m
21:41:36.775 -> [0;32mI (1126) esp_image: segment 5: paddr=0x0028df94 vaddr=0x40089254 size=0x0b6a0 ( 46752) load[0m
21:41:36.812 -> [0;32mI (1158) boot: Loaded app from partition at offset 0x10000[0m
21:41:36.812 -> [0;32mI (1158) boot: Disabling RNG early entropy source...[0m
21:41:36.812 -> E (59) psram: PSRAM ID read error: 0xffffffff
21:41:36.847 -> 
21:41:36.880 -> E (116) sdmmc_common: sdmmc_init_ocr: send_op_cond (1) returned 0x107
21:41:36.880 -> Card Mount Failed
21:41:36.880 -> PSRAM Initialization Failure
21:41:37.088 -> [E][camera.c:1113] camera_probe(): Detected camera not supported.
21:41:37.088 -> [E][camera.c:1379] esp_camera_init(): Camera probe failed with error 0x20004
21:41:37.088 -> Camera init failed with error 0x20004


 *  
 *  
 *  
 *  
 *  ESP32 Default SPI pins are
 *  
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
 *  
 *  
 *  CAN HIGH LOW Wire polarity matters. Current setup - Yellow high, Green Low
 *  
 */





// Load Wi-Fi library
#include <WiFi.h>
#include <mcp_can.h>
#include <SPI.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>




#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <WebsocketHandler.hpp>
#define MAX_CLIENTS 5



using namespace httpsserver;

// Include your generated certificate bytes
static const uint8_t PKEY[] = R"EOF(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDuNcI+/Iz43zJf
4YllUwDDwnHoTWhJhl3Wr8C5e/fGLF/Y1r+r6VbxsWqTGW1fAgVsjgzEqnISXtuk
NSs6WExdpylPdr9NKmFHuqzhAAU4q/lZ09w6OwXtDhoGAobBENuES3PhUzGaJUAT
vtbD+WG6wJVogdgyyB0D+MTpZXQ1iER6+OqthKR8E2AZUZsarTWrZyZdFXt+Usfg
+qdc/wYGhCR4Hx/9dj5shhRkP6+ztE9JAbnoC3B3ccLXDxbANh881UBowsUQvixV
DMSaCx4nd1aLABdhVSqGjHRRYZJnY7auaFweAzKI5lpve54oit7PF3aUjPVFYuRq
NdeRaOU5AgMBAAECggEBAMrAyzMdd0glghUGPRIXUiFNkfKuYEOksJ6ZPasjD0SI
ZJSGROKoW66g2huWmvcxGzjIt6l1gl6Mitr7vm2M1oMoUwsmAbJGjkKla5yfjdIs
3rJMl7igoGyjghb3c4dFN/Mk3d8+FowjhKTJ3Lc1vq9cWE95pXGjzr6gscoO5pjg
XTQmp/4CdROV6fIucYAURNzBYbG2udslO32lDNbCXmwOByjDXc0RA76LCAT90eZe
BbOirR9qhbzm+/3QAV2B/CdCSKThojhsXJndQaTe/kHjPaLKcb9/fKOTZoYfvr5B
lEq7afQgfS5bCu2LyktbxTiCzGIhyzNbx97w4aQfc2ECgYEA+O52XViiYhelqq2h
TjnHamj0+s/XuJ1KZS6GsPb+Y6X8PspxMS6RfV8TkdrtjvAbzK6IB+d6Xdd3UOBK
61zGbShMc5XW3qHSGT19qVFNR1NSpy9haX1Bv0AHyBXuadnBTGu1zt4OhWIYoFVO
7RPOavjSrVH+F4jbhsygfhHO3PUCgYEA9PlcB2UCL8l/pVIroBIh9I751jae+UDq
nc/1rA2BlC9eRqDTtZY9VTvSufmYEuwAEAwKUaBp1pJyHItZascqJqkyrYdcsNR/
uc+Ux/zlXb2fELcMIabLP0cRnAsa3/ECcF8+habtphSHLKhR8G0k3Y3/FHJb/gfV
p0U6EzagfLUCgYEA7mUY8B6RLJXu7zznTQ4ifzLS4lwoAMKJ28qp7VItn+r3Xn7r
1ij95m4mRLnAZfJm/SdsWP2C/9n4h3i15x9rXGCKjutB4uZgWhG8hWm6M4OFlVzX
0nnFfE3q5eCy+aYz62b/r4STrYObh/kK5BrixSlUQe5TA+DqM/dGAFY3oP0CgYBM
upnPmxyfOR6o+l69qVBHWEz6gmsyqNmTimJpfGV8s4V028TT2HXnb3BmXb37Fz5B
yHOm83aTBZWGSUM9hZo3N8GuxnoKzNQgr9rq9NQmk1DyHFNJawO4Ext546SMReG8
rqhXllxTK5TXMPVRN+5XErW1gsg7fdq2pKE3CWIlDQKBgG1w8j3bUEtkX8SDcXYX
p+ichM5xq36KTwlEzcByxRa8lGZBUDQ+WsOfwP0UeS6codJKLcCMkU28H3/sAGJv
t8SD8//OEqYM/KwpuWt7R0VLoq+y8NRA3RNlZvhAo0WxTVdLPE0N5tXI+aHoHpQa
azohGB/ILiW1rqK6JlZZJDHs
-----END PRIVATE KEY-----
)EOF";

static const uint8_t CERTFCT[] = R"EOF(
-----BEGIN CERTIFICATE-----
MIIC0TCCAbmgAwIBAgIUWmYpnRzT9VNb7OeiOy/WgKgMPFYwDQYJKoZIhvcNAQEL
BQAwGDEWMBQGA1UEAwwNMTkyLjE2OC4xLjI1NDAeFw0yNjAzMjMwMzA1NTRaFw0z
NjAzMjAwMzA1NTRaMBgxFjAUBgNVBAMMDTE5Mi4xNjguMS4yNTQwggEiMA0GCSqG
SIb3DQEBAQUAA4IBDwAwggEKAoIBAQDuNcI+/Iz43zJf4YllUwDDwnHoTWhJhl3W
r8C5e/fGLF/Y1r+r6VbxsWqTGW1fAgVsjgzEqnISXtukNSs6WExdpylPdr9NKmFH
uqzhAAU4q/lZ09w6OwXtDhoGAobBENuES3PhUzGaJUATvtbD+WG6wJVogdgyyB0D
+MTpZXQ1iER6+OqthKR8E2AZUZsarTWrZyZdFXt+Usfg+qdc/wYGhCR4Hx/9dj5s
hhRkP6+ztE9JAbnoC3B3ccLXDxbANh881UBowsUQvixVDMSaCx4nd1aLABdhVSqG
jHRRYZJnY7auaFweAzKI5lpve54oit7PF3aUjPVFYuRqNdeRaOU5AgMBAAGjEzAR
MA8GA1UdEQQIMAaHBMCoAf4wDQYJKoZIhvcNAQELBQADggEBAMZXRYXRnWmUF68I
tZLBef4BmhqL1et+1sfdX+lGz4KZDdej5exs9CnAsP45uAzrm6ynOebpKJW4P/hQ
YWQZvWT5kuO4+0YREWuVSTTeJ9lNRxRpnx2SGOyxn/v5u6xpJB7fOsMa1YE1fEJN
PfFkpJSFXvByhtPjT3m3vz4Ck4LUKuQSurN8LK2OGAPgzqQx89MgBJy3lZgMsJkD
gf2n87+6q8cNFGsJiw+6k8Nf3fPfcKNaSgHcNlNHJT1UsZXKQ1zVYI7bs+5NG2R5
qyTFquwiNgi3x4axdJRlfgcRrPzbCz6sHaKfY1gjhENmsf4W3uwfyuKKvs9+eqsg
bCeCOOg=
-----END CERTIFICATE-----
)EOF";








long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 4                              // Set INT to pin 4 (2 for Atmega)
MCP_CAN CAN0(5);                               // Set CS to pin 5 (10 for Atmega)


// ---------------- WEB SERVER ----------------
SSLCert cert((uint8_t*)CERTFCT, sizeof(CERTFCT), (uint8_t*)PKEY, sizeof(PKEY));
HTTPSServer secureServer(&cert);

class WSSHandler : public WebsocketHandler {
public:
  // This method is called by the webserver to instantiate a new handler for each
  // client that connects to the websocket endpoint
  static WebsocketHandler* create();

  // This method is called when a message arrives
  void onMessage(WebsocketInputStreambuf * input);

  // Handler function on connection close
  void onClose();
};



WSSHandler* activeClients[MAX_CLIENTS] = { nullptr };


// -----------------WEB SOCKET HANLDER ----------
WebsocketHandler * WSSHandler::create() {

    Serial.println("Client connected");

    WSSHandler* handler = new WSSHandler();

    // Store client
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (activeClients[i] == nullptr) {
        activeClients[i] = handler;
        break;
      }
    }


    return handler;
  }

  void WSSHandler::onClose() {
    Serial.println("Client disconnected");

    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (activeClients[i] == this) {
        activeClients[i] = nullptr;
      }
    }
  }

void WSSHandler::onMessage(WebsocketInputStreambuf* inbuf) {

    std::ostringstream ss;
    ss << inbuf;
    std::string msg = ss.str();

    Serial.println(("WS received: " + msg).c_str());


  }


void sendBinary(uint8_t* msg, uint16_t len) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (activeClients[i] != nullptr) {
      activeClients[i]->send(msg, len, WebsocketHandler::SEND_TYPE_BINARY);
    }
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
  // WebSocket
  WebsocketNode* wsNode = new WebsocketNode("/ws", WSSHandler::create);
  secureServer.registerNode(wsNode);
  secureServer.start();
  Serial.println("Secure WSS server running at wss://" + WiFi.softAPIP().toString());
ArduinoOTA.begin();

}

void loop()
{
  ArduinoOTA.handle();
secureServer.loop();

  if (!digitalRead(CAN0_INT))
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);

    uint8_t wsBuf[16];
    uint16_t idx = 0;

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
    sendBinary(wsBuf, idx);

    // ---- Optional Serial debug ----
    Serial.print("WS BIN: ");
    for (uint8_t i = 0; i < idx; i++) {
      Serial.printf("%02X ", wsBuf[i]);
    }
    Serial.println();
  }
}
