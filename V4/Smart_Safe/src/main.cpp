#include <Arduino.h>
#include "efe_lib.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer sunucu(80);

TaskHandle_t task1;

TaskHandle_t task2;

LiquidCrystal_I2C lcd (0x27,16,2);

Safe_State kasa_durumu = GIRIS;

Pins pinler(27,26,25,23,18,32,33,19,5,21,22);

Servo_State servo_durumu = LOCKED;

Servo myservo;

uint8_t durum = 0;

Password asil;

Password other;

unsigned long ses_btn=0;

unsigned long renk_btn=0;

unsigned long acik_suresi=0;

const unsigned long BTN_BEKLE = 1000;

const unsigned long KASA_BEKLE = 5000;

void TaskKasa(void *pvParameters);

void TaskWiFi(void *pvParameters); //bu parametreler bir kutu ama içleri null

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html>
    <head>
    <title>AKILLI KASA v4</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { background-color: #121212; color: #ffffff; font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; }
    h2 { color: #03dac6; margin-bottom: 20px; }
    
    /* Şifre Ekranı */
    #sifreKutusu {
      font-size: 30px; padding: 15px; width: 80%; text-align: center;
      background-color: #1e1e1e; color: #03dac6; border: 2px solid #333;
      border-radius: 10px; margin-bottom: 20px; letter-spacing: 5px;
    }

    /* Tuşlar */
    .grid-container {
      display: grid; grid-template-columns: auto auto auto; gap: 10px;
      max-width: 400px; margin: auto;
    }
    .btn {
      background-color: #333; color: white; border: none;
      padding: 20px; font-size: 24px; border-radius: 10px; cursor: pointer;
      box-shadow: 0 4px #000; active: box-shadow: 0 2px #000; transform: translateY(2px);
    }
    .btn:active { background-color: #555; transform: translateY(4px); box-shadow: 0 0 #000; }
    
    /* Özel Tuşlar */
    .btn-sil { background-color: #cf6679; color: black; }
    .btn-gonder { grid-column: span 3; background-color: #03dac6; color: black; font-weight: bold; }
  </style>
</head>
<body>

  <h2>🔒 KASA KUMANDASI</h2>
  
  <form action="/kontrol" method="GET" id="kasaForm">
    <input type="password" id="sifreKutusu" name="sifre" readonly placeholder="****">
    <br>
    
    <div class="grid-container">
      <button type="button" class="btn" onclick="tus('1')">1</button>
      <button type="button" class="btn" onclick="tus('2')">2</button>
      <button type="button" class="btn" onclick="tus('3')">3</button>
      
      <button type="button" class="btn" onclick="tus('4')">4</button>
      <button type="button" class="btn" onclick="tus('5')">5</button>
      <button type="button" class="btn" onclick="tus('6')">6</button>
      
      <button type="button" class="btn" onclick="tus('7')">7</button>
      <button type="button" class="btn" onclick="tus('8')">8</button>
      <button type="button" class="btn" onclick="tus('9')">9</button>
      
      <button type="button" class="btn btn-sil" onclick="tus('C')">SIL</button>
      <button type="button" class="btn" onclick="tus('0')">0</button>
      <button type="button" class="btn" onclick="tus('R')">RESET</button> <button type="submit" class="btn btn-gonder">KILIDI AC</button>
    </div>
  </form>

  <script>
    function tus(deger) {
      var kutu = document.getElementById("sifreKutusu");
      if(deger == 'C') {
        kutu.value = "";
      } else if(deger == 'R') {
         // Reset özelliği eklenebilir
      } else {
        if(kutu.value.length < 4) { // Max 4 hane
           kutu.value += deger;
        }
      }
    }
  </script>
</body>
</html>
)rawliteral";


void handleAnaSayfa(){

    sunucu.send(200,"text/html",index_html);

}

void handleKontrol(){

    String WebSifre =sunucu.arg("sifre");

    Serial.print("Webden gelen sifre: ");

    Serial.println(WebSifre);

    if(kasa_durumu==GIRIS){

        asil.setwebpassword(WebSifre);

        kasa_durumu=YENI_GIRIS;

        sunucu.send(200, "text/html", "<h1>SIFRE OLUSTURULDU!</h1><meta http-equiv='refresh' content='2;url=/'>");

    }

    else{

        if(asil.checkwebpassword(WebSifre)){

            kasa_durumu=ACIK;

            sunucu.send(200, "text/html", "<h1>SIFRE DOGRU!</h1><meta http-equiv='refresh' content='2;url=/'>");
        }

        else{

            kasa_durumu=KILITLI;

            sunucu.send(200, "text/html", "<h1>YANLIS SIFRE!</h1><meta http-equiv='refresh' content='2;url=/'>");

        }

    }

    vTaskDelay(10);

}

void setup(){

    asil.loadPassword();

    durum=asil.loadSettings();

    Serial.begin(115200);

    lcd.init();

    lcd.backlight();

    pinler.begin(&myservo); 
    
    servo_kontrol(&myservo,LOCKED);

    renk_ayar(&pinler,0,0,255,&durum);

    durum|=SES_ACIK;//0000 0001

    durum|=LED_ACIK;//0000 0001

    lcd.setCursor(0,0);

    lcd.println("Sistem Basliyor");

    delay(2000);

    lcd.clear();

    xTaskCreatePinnedToCore(
        TaskKasa,
        "TaskKasa",
        10000,
        NULL,
        1,
        &task1,
        1
    );

    xTaskCreatePinnedToCore(
        TaskWiFi,
        "TaskWiFi",
        10000,
        NULL,
        1,
        &task2,
        0
    );

    

}

void loop(){

    vTaskDelay(1000);

}
void TaskKasa(void *pvParameters){
for(;;){

        unsigned long su_an = millis();

    if(digitalRead(pinler.getled_pin())==LOW && (su_an - renk_btn >BTN_BEKLE)){
        durum^=LED_ACIK;// 0000 0000
        lcd.setCursor(0,0);
        if(durum & LED_ACIK) {
            lcd.print("LED ACIK");
        }
        else{
            lcd.print("LED KAPALI");

        }   
        renk_btn=su_an;
        
        asil.saveSettings(durum);

    
        
    }
    if(digitalRead(pinler.getSes_pin())==LOW && (su_an - ses_btn > BTN_BEKLE)){
        durum^=SES_ACIK; //0000 0000
        if(durum & SES_ACIK) {
            lcd.print("SES ACIK");
        }
        else{
            lcd.print("SES KAPALI");

        }   
        
        ses_btn=su_an;

        asil.saveSettings(durum);

    }

    switch (kasa_durumu)
    {
    case GIRIS:
        lcd.setCursor(0,0);
        lcd.print("Sifrenizi Olusturunuz");
        asil.password_begin(&pinler,kasa_durumu,YENI_GIRIS);

        if(kasa_durumu==YENI_GIRIS){
            ses_cal(&pinler,1,&durum);
            renk_ayar(&pinler,255,0,255,&durum);
            lcd.setCursor(0,0);
            lcd.print("Sifreniz Olusturuldu");
            delay(2000);
        }
        break;

    case YENI_GIRIS:
        lcd.setCursor(0,0);
        lcd.print("Sifrenizi Giriniz");
        other.password_begin(&pinler,kasa_durumu,KONTROL);
        if(kasa_durumu==KONTROL){
            ses_cal(&pinler,1,&durum);
            renk_ayar(&pinler,255,0,255,&durum);
            lcd.setCursor(0,0);
            lcd.print("Sifreniz Kontrol Ediliyor");

        }
        break;

    case KONTROL:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sifre Kontrol Ediliyor..");
        renk_ayar(&pinler,255,255,0,&durum);
        delay(1000);
        if(asil.password_check(other)==1){
            kasa_durumu=ACIK;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Sifre Dogru");

            servo_kontrol(&myservo,OPEN);
            renk_ayar(&pinler,0,255,0,&durum);
            ses_cal(&pinler,1,&durum);

            acik_suresi=millis();

        }
        else{
            kasa_durumu=KILITLI;
        }
        lcd.clear();
        break;

    case ACIK:
        
        if(millis() - acik_suresi >KASA_BEKLE){
            kasa_durumu=YENI_GIRIS;
            servo_kontrol(&myservo,LOCKED);
            lcd.clear();
        }
        break;

    case KILITLI:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sifre Yanlis");
        servo_kontrol(&myservo,LOCKED);
        renk_ayar(&pinler,255,0,0,&durum);
        delay(3000);
        lcd.clear();
        kasa_durumu=YENI_GIRIS;
        break;
    default:
        break;
    }
    vTaskDelay(10);
    }
}


void TaskWiFi(void *pvParameters){


    WiFi.mode(WIFI_STA);

    WiFi.begin("WIFI NAME","WIFI PASSWORD");

    while(WiFi.status() != WL_CONNECTED){

        vTaskDelay(500);

        Serial.print(".");

    }

    Serial.println(WiFi.localIP()); 

    sunucu.on("/",handleAnaSayfa);

    sunucu.on("/kontrol", handleKontrol);

    sunucu.begin();

    for(;;){
        
        sunucu.handleClient();

        vTaskDelay(5);
       



    }




}


