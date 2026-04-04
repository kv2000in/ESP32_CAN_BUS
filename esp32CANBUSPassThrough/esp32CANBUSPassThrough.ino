#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS_PIN 5

MCP_CAN CAN(CAN_CS_PIN);

void setup() {
Serial.begin(115200);
while (!Serial);

//Serial.println("Initializing MCP2515...");

// Initialize CAN @ 250 kbps
if (CAN.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {
//Serial.println("CAN Init OK");
} else {
//Serial.println("CAN Init FAIL");
while (1);
}

CAN.setMode(MCP_NORMAL);  // Set to normal mode
//Serial.println("CAN BUS Ready!");
}

void loop() {
handleSerialToCAN();
handleCANToSerial();
}

// -------------------------------
// SERIAL → CAN
// -------------------------------
void handleSerialToCAN() {
static String input = "";

while (Serial.available()) {
char c = Serial.read();

if (c == '\n') {
parseAndSendCAN(input);
input = "";
} else {
input += c;
}
}
}

void parseAndSendCAN(String line) {
int id;
int dlc;
byte data[8];

int parsed = sscanf(line.c_str(),
"%x %d %hhx %hhx %hhx %hhx %hhx %hhx %hhx %hhx",
&id, &dlc,
&data[0], &data[1], &data[2], &data[3],
&data[4], &data[5], &data[6], &data[7]);

if (parsed >= 2) {
if (dlc > 8) dlc = 8;

if (CAN.sendMsgBuf(id, 0, dlc, data) == CAN_OK) {
Serial.println("TX OK");
} else {
Serial.println("TX FAIL");
}
} else {
Serial.println("Parse Error");
}
}

// -------------------------------
// CAN → SERIAL
// -------------------------------
void handleCANToSerial() {
if (CAN.checkReceive() == CAN_MSGAVAIL) {
long unsigned int rxId;
byte len = 0;
byte buf[8];

CAN.readMsgBuf(&rxId, &len, buf);

Serial.print(rxId, HEX);
Serial.print(" ");
Serial.print(len);

for (int i = 0; i < len; i++) {
Serial.print(" ");
if (buf[i] < 0x10) Serial.print("0");
Serial.print(buf[i], HEX);
}

Serial.println();
}
}
