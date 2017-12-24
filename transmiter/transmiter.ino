#include <VirtualWire.h>
#include <avr/sleep.h>

#define DEBUG true

const int transmit_pin = 5;
const int button = 2;
const int led_pin = 13;

void setup() {
  Serial.begin(9600);
  
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  
  pinMode(button, INPUT);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
  
  if (DEBUG) 
    Serial.println("Prepare Interrupt");

  attachInterrupt(0, wakeUp, LOW);

  if (DEBUG) 
    led_signal();
}

void loop() {
  /* after wakeup send message */
  sendMessage();
  
  if (DEBUG) {
    Serial.print("Before sleep state: ");
    Serial.println(digitalRead(button));
    Serial.println("Entering Sleep mode");
  }

  /* Send system to sleep */
  gotoSleep();
}

/* Visual sign of system setup */
void led_signal() {
  digitalWrite(led_pin, LOW);
  
  for (int i=0; i< 3; i++) {
    Serial.println("aaaa");
    digitalWrite(led_pin, HIGH);
    delay(500);
    digitalWrite(led_pin, LOW);
    delay(500);
  }
}

/* Send Message via Radio */
void sendMessage() {
  vw_send(1, 1);
}

/* Visual sign of detection */
void wakeUp() {
  if (DEBUG) {
    Serial.print("In wakeup: ");
    Serial.println(digitalRead(button));
  }
  digitalWrite(led_pin, !digitalRead(button));
}

void gotoSleep()
{
  if (DEBUG) {
    Serial.print("button state: ");
    Serial.println(digitalRead(button));
  }
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, wakeUp, LOW);
  sleep_mode();
  sleep_disable();
  detachInterrupt(0);
}

