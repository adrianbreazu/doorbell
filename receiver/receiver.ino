#include <VirtualWire.h>
#include <SoftwareSerial.h>

#define DEBUG true
#define ARDUINO_RX 5
#define ARDUINO_TX 6
#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_VOLUME_UP 0X04

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);

const int led_pin = 13;
const int receiver_pin = 2;

static int8_t Send_buf[8] = {0} ;

uint8_t message[VW_MAX_MESSAGE_LEN];
uint8_t messageLength = VW_MAX_MESSAGE_LEN;

void setup() {
  Serial.begin(9600);
  
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_rx_pin(receiver_pin);
  vw_rx_start();
  
  mp3.begin(9600);
  delay(500);
  
  if (DEBUG) {
    Serial.println("init");
    delay(500);
  }
  
  sendCommand(CMD_SEL_DEV, DEV_TF);
  delay(500);
}

void loop() {
  if (vw_get_message(message, &messageLength)) {
    digitalWrite(led_pin, HIGH);
    
    if (DEBUG) 
      Serial.print("received: ");
    
    sendCommand(CMD_PLAY, 0);
    delay(500);
    
    for(int i=0;i<5;i++) {
      sendCommand(CMD_VOLUME_UP, 0);
      delay(300);
    }
    
    if (DEBUG) {
      for (int i=0;i < messageLength; i++) {
        Serial.print(message[i]);
      }
      Serial.println();
    }
    
    digitalWrite(led_pin, LOW);
  }
}

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]) ;
  }
  
  if (DEBUG)
    Serial.println("Command sent.");
}
