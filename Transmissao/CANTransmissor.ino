#include <mcp_can.h>
#include <SPI.h>

MCP_CAN CAN0(5); //cs pin 5

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!"); //iniciando o can
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // bla
}

byte data[8] = {'l', 'e', 'v', 'i', 'a', 't', 'a', '!'};

void loop() {
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data); // ID = 256 ou 0x100,Stardard CAN Frame, Tamanho = 8 bytes, o dado de fato
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(500);   // mandar dado a cada 100 ms
}
