#include <M5Stack.h>
#include <Mailer.h>
#include <SimpleDHT.h>
#include "MyConfig.h"

Mailer mail(smtp_username, smtp_password, smtp_from_address, smtp_port,
            smtp_hostname);

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

  log_i("Connecting to %s", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_pass);
  while (WiFi.status() != WL_CONNECTED) {
    log_i("wait...");
    delay(1000);
  }
  log_i("WiFi connected");
  log_i("IP Address: %s", WiFi.localIP().toString().c_str());
  mail.send(to_address, subject, content);
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
  delay(5000);  // more than 150
}
