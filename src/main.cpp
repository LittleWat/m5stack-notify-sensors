#include <M5Stack.h>
#include <Mailer.h>
#include <SimpleDHT.h>
#include <ezTime.h>

#include "MyConfig.h"

// ref: http://shikarunochi.matrix.jp/?p=2586
// 抵抗は https://www.switch-science.com/catalog/818/　を使っていたら、いらない
// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
// int pinDHT11 = SCL;
int pinDHT11 = G5;
SimpleDHT11 dht11(pinDHT11);

Mailer mail(smtp_username, smtp_password, smtp_from_address, smtp_port,
            smtp_hostname);
Timezone Tokyo;
time_t last_emailed_at;
void send_email(const String content);

void setup() {
  M5.begin();
  M5.Power.begin();

  M5.Lcd.setTextSize(3);

  log_i("Connecting to %s", wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_pass);
  waitForSync();
  log_i("WiFi connected");
  log_i("IP Address: %s", WiFi.localIP().toString().c_str());

  Tokyo.setLocation("Asia/Tokyo");
  Serial.println("Asia/Tokyo time: " + Tokyo.dateTime());
  last_emailed_at = Tokyo.now();
}

void loop() {
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

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

  // 経過時間を計算しそれが一定以上、かつ温度が閾値を超えていたらメール送信
  auto elapsed_seconds = difftime(Tokyo.now(), last_emailed_at);

  String email_content = "温度: ";
  email_content += (int)temperature;
  email_content += "℃、湿度: ";
  email_content += (int)humidity;
  email_content += "％です。\n";
  Serial.println(email_content);

  log_d("経過時間：%.1f秒", elapsed_seconds);
  if (elapsed_seconds > minimum_email_interval_seconds) {
    if (temperature > temperature_upper_limit) {
      send_email(email_content + "暑いので冷房を強くしてください。");
    } else if (temperature < temperature_lower_limit) {
      send_email(email_content + "寒いので暖房を強くしてください。");
    }
  }
  // DHT11 sampling rate is 1HZ.
  delay(sensing_interval_milliseconds);
}

void send_email(const String content) {
  mail.send(to_address, subject, content);

  M5.Lcd.println("");
  M5.Lcd.println("Emailed at ");
  M5.Lcd.println(Tokyo.dateTime());

  last_emailed_at = Tokyo.now();
}