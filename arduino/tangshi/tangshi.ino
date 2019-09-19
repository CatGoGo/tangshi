
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>

WiFiManager wifiManager;
HTTPClient http;
const unsigned long HTTP_TIMEOUT = 5000;
int buttonState = 0;
String response;
int button_pin = D4;//触发按钮

char const *ap_name = "TangshiAP";//ap name

boolean debug_mode = true;//debug?
String debug_wifi_name = "chinaunion";
String debug_wifi_pass = "chinaunion";


//由于tts语音模块需要ansi编码。故，每次串口输出都需经过转码。可用python3的.encode("ansi")来实现

void ap() {
  wifiManager.autoConnect(ap_name);
}

void debug() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(debug_wifi_name, debug_wifi_pass);
  WiFi.setAutoConnect(true);
}

void conn() {
  if (debug_mode) {
    debug();
  } else {
    ap();
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}


void setup() {

  //  Serial.swap();//调用映射方法GPIO1(TX),GPIO3(RX)   不调用，同样可用，因为是 TX0/RX0。调用后，arduino监控台无输出。
  pinMode(button_pin, INPUT_PULLUP);
  conn();
  Serial.begin(9600);
  Serial.println("\xcd\xf8\xc2\xe7\xc1\xac\xbd\xd3\xb3\xc9\xb9\xa6");//网络连接成功
  http.begin("http://192.168.137.1:5000/text/ansi");
  http.setTimeout(HTTP_TIMEOUT);
  http.setUserAgent("esp8266");//用户代理版本
  //  http.setAuthorization("esp8266","123456");//用户校验信息
}

void loop() {
  buttonState = digitalRead(button_pin);
  if (buttonState == LOW) {
    Serial.printf("\xd5\xfd\xd4\xda\xc7\xeb\xc7\xf3 API");//正在请求API
    int httpCode = http.GET();
    Serial.println(httpCode);
    if (httpCode == HTTP_CODE_OK) {
      response = http.getString();
      Serial.println(response);//输出内容
    } else {
      Serial.printf("API \xc1\xac\xbd\xd3\xca\xa7\xb0\xdc");//API连接失败
    }
    http.end();
    delay(5000);
  }

}
