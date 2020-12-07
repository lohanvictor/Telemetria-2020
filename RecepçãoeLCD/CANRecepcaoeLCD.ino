#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];
float dados[6];
int dadosInt[2];

LiquidCrystal_I2C lcd(0x27, 20, 4);

int contador = 0;

#define CAN0_INT 2
MCP_CAN CAN0(5); //cs pin 5

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.print("Hello World");

  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!"); //iniciando
  else Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL);
  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  Serial.println("MCP2515 Library Receive Example...");
}

void loop() {
  if (!digitalRead(CAN0_INT))                        // se can0_int for 0, ele le o buffer recebido
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // lendo dado: len = tamanho do dado, buf = dado byte(s)

    if ((rxId & 0x80000000) == 0x80000000)    // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);

    Serial.print(msgString);

    if ((rxId & 0x40000000) == 0x40000000) {  // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for (byte i = 0; i < len; i++) {
        if (contador <= 5) {
          contador++;
          dados[contador] = rxBuf[i];
          Serial.print(rxBuf[i]);

        } else {
          contador++;
          dadosInt[i] = rxBuf[i];
          if (contador > 7) {
            contador = 0;
          }
        }
      }
    }
  }
  for (int i = 0; i++; i < 6) {
    Serial.print(dados[i]);
    Serial.print(" ");
  }
  for (int i = 0; i++; i < 6) {
    Serial.print(dadosInt[i]);
    Serial.print(" ");
  }
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print(palavra);
  //lcd.setCursor(0, 1);
  //lcd.print(contador);

  Serial.println();
  delay(100);
}
