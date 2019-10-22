#include <M5Stack.h>
#include <SimpleDHT.h>

// ref: http://shikarunochi.matrix.jp/?p=2586
// 抵抗は https://www.switch-science.com/catalog/818/　を使っていたら、いらない

// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
// int pinDHT11 = SCL;
int pinDHT11 = G5;
SimpleDHT11 dht11(pinDHT11);

void setup() {
  M5.begin();
  M5.Power.begin();

  M5.Lcd.setTextSize(4);
}

void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  M5.Lcd.clearDisplay();
  M5.Lcd.setCursor(0, 0);

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) !=
      SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    M5.Lcd.print("Read DHT11 failed, err=");
    M5.Lcd.println(err);
    delay(1000);
    return;
  }

  // LCD display
  M5.Lcd.print((int)temperature);
  M5.Lcd.print(" *C, ");
  M5.Lcd.print((int)humidity);
  M5.Lcd.println(" H");

  Serial.print("Sample OK: ");
  Serial.print((int)temperature);
  Serial.print(" *C, ");
  Serial.print((int)humidity);
  Serial.println(" H");

  // DHT11 sampling rate is 1HZ.
  delay(1500);
}
