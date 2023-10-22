#include <avr/sleep.h>
#include <NecEncoder.h> // https://github.com/GyverLibs/NecDecoder

#define IRLED_PIN 0
#define PWR_BUTTON_PIN 1

#define IR_ADDR 0x80
#define IR_POWER_CMD 0x0

NecEncoder enc(IRLED_PIN);

void isr() {}

void setup() {
  pinMode(PWR_BUTTON_PIN, INPUT_PULLUP);
}

void goToSleep() {
  WDTCR &= ~(1 << WDTIE); // watchdog off to fix chip insomnia

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  noInterrupts();  
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(PWR_BUTTON_PIN), isr, LOW);
  interrupts(); 
  sleep_cpu();   

  sleep_disable(); 
  delay(100);
  detachInterrupt(digitalPinToInterrupt(PWR_BUTTON_PIN));

  WDTCR |= (1 << WDTIE); // watchdog on
}

void loop() {
  goToSleep();

  enc.send(IR_ADDR, IR_POWER_CMD);
  delay(200);
}
