// CAN Send Example
//

#include <mcp_can.h>
#include <SPI.h>

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);     // Set CS to pin 10

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string


void setup()
{
  Serial.begin(115200);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
  pinMode(CAN0_INT, INPUT); 
}

void loop()
{
  // Check for incoming CAN messages
  if (!digitalRead(CAN0_INT)) {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);

    if ((rxId & 0x80000000) == 0x80000000)  // Extended ID
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);

    Serial.print(msgString);

    if ((rxId & 0x40000000) == 0x40000000) {  // Remote request frame
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for (byte i = 0; i < len; i++) {
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }

    Serial.println();
  }

  // Check for Serial input
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'A' || cmd == 'a') {
      byte data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x000002E2, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

    if (cmd == 'B' || cmd == 'b') {
      byte data[4] = {0x0d, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x000002E2, 1, 4, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }


        if (cmd == 'C' || cmd == 'c') {
      byte data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
      byte sndStat = CAN0.sendMsgBuf(0x0000002A, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }


           if (cmd == 'D' || cmd == 'd') {
      byte data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
      byte sndStat = CAN0.sendMsgBuf(0x00000028, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

         if (cmd == 'E' || cmd == 'e') {
      byte data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
      byte sndStat = CAN0.sendMsgBuf(0x0000002f, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

        if (cmd == 'F' || cmd == 'f') {
      byte data[4] = {0x0A, 0x00, 0x00, 0x00 };
      byte sndStat = CAN0.sendMsgBuf(0x00000064, 1, 4, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

            if (cmd == 'G' || cmd == 'g') {
      byte data[4] = {0x0B, 0x00, 0x00, 0x00 };
      byte sndStat = CAN0.sendMsgBuf(0x00000025, 1, 4, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

    
            if (cmd == 'H' || cmd == 'h') {
      byte data[4] = {0x55, 0xAA, 0x00, 0x01 };
      byte sndStat = CAN0.sendMsgBuf(0x00000025, 1, 4, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

    
            if (cmd == 'I' || cmd == 'i') {
      byte data[4] = {0x01, 0x00, 0x00, 0x00 };
      byte sndStat = CAN0.sendMsgBuf(0x00000025, 1, 4, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

    
            if (cmd == 'J' || cmd == 'j') {
      byte data[8] = {0x01, 0x00, 0xFE, 0x2D, 0x99, 0x04, 0x05, 0x20};
      byte sndStat = CAN0.sendMsgBuf(0x18A00025, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }


                if (cmd == 'K' || cmd == 'k') {
      byte data[8] = {0x01, 0x00, 0xFE, 0x2D, 0x99, 0x04, 0x05, 0x20};
      byte sndStat = CAN0.sendMsgBuf(0x18A, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }



                if (cmd == 'L' || cmd == 'l') {
      byte data[8] = {0x02,0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x18A, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }


                if (cmd == 'M' || cmd == 'm') {
      byte data[8] = {0x40, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x601, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }


                if (cmd == 'N' || cmd == 'n') {
      byte data[8] = {0x2B, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x601, 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }



                if (cmd == 'O' || cmd == 'o') {
      byte data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x18FF50E5  , 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }



    
                if (cmd == 'P' || cmd == 'p') {
      byte data[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      byte sndStat = CAN0.sendMsgBuf(0x18FF50E0  , 1, 8, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }


                    if (cmd == 'Q' || cmd == 'q') {
      byte data[4] = {0x01, 0x00, 0x00, 0xFE};
      byte sndStat = CAN0.sendMsgBuf(0x00000024, 1, 4, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

                    if (cmd == 'R' || cmd == 'r') {
      byte data[4] = {0x03, 0x00, 0x00, 0xFC};
      byte sndStat = CAN0.sendMsgBuf(0x00000024, 1, 3, data);  // '1' sets Extended ID
      if (sndStat == CAN_OK) {
        Serial.println("Message Sent Successfully!");
      } else {
        Serial.println("Error Sending Message...");
      }
    }

    
  }

  
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
