#include <mcp_can.h>
#include <SPI.h>

int i = 0;

MCP_CAN CAN0(5); //cs pin 5

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);

  if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!"); //iniciando o can
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // bla
}

void loop() {
  //Serial2.print("teste");
  BMS_decode();
}


void BMS_decode()
{
  // Declaração das variaveis locais
  int BMS_byte[58], celula, estado_de_carga, temperatura_celula, temperatura_minima, temperatura_maxima;
  float tensao_total, corrente_entrada, corrente_saida, tensao_celula, tensao_minima, tensao_maxima;


  //Coleta dos dados até o Byte 58 do BMS
  if (Serial2.available())
  {
    BMS_byte[i] = Serial2.read();

    Serial.print(BMS_byte[i]);
    Serial.print(" ");

    i++;
  }

  // Calculo e print dos dados recebidos
  if (i >= 58)
  {
    Serial.println();
    tensao_total = (BMS_byte[1] * 256 + BMS_byte[2]) * 0.005;
    corrente_entrada = (BMS_byte[4] * 256 + BMS_byte[5]) * 0.125;
    corrente_saida = (BMS_byte[7] * 256 + BMS_byte[8]) * 0.125;
    tensao_minima = (BMS_byte[12] * 256 + BMS_byte[13]) * 0.005;
    tensao_maxima = (BMS_byte[15] * 256 + BMS_byte[16]) * 0.005;
    temperatura_minima = (BMS_byte[18] * 256 + BMS_byte[19]) - 276;
    temperatura_maxima = (BMS_byte[21] * 256 + BMS_byte[22]) - 276;
    celula = BMS_byte[24];
    tensao_celula = (BMS_byte[26] * 256 + BMS_byte[27]) * 0.005;
    temperatura_celula = (BMS_byte[28] * 256 + BMS_byte[29]) - 276;
    estado_de_carga = BMS_byte[40];


    Serial.print(tensao_total);
    Serial.print(",");
    Serial.print(corrente_entrada);
    Serial.print(",");
    Serial.print(corrente_saida);
    Serial.print(",");
    Serial.print(tensao_minima);
    Serial.print(",");
    Serial.print(tensao_maxima);
    Serial.print(",");
    Serial.print(estado_de_carga);
    Serial.println();


    sendCan(tensao_total, corrente_entrada);
    sendCan(corrente_saida, tensao_celula);
    sendCan(tensao_maxima, tensao_minima);
    sendCan(celula, estado_de_carga);
    i = 0;
  }
}

void sendCan(int data1, int data2) {
  byte data[2] = {data1, data2};
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  if (sndStat == CAN_OK) {
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(500);   // mandar dado a cada 100 ms
}

void sendCan(float data1, float data2) {
  byte data[2] = {data1, data2};
  byte sndStat = CAN0.sendMsgBuf(0x100, 0, 8, data);
  if (sndStat == CAN_OK) {
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(500);   // mandar dado a cada 100 ms
}
