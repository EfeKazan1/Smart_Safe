#include <Arduino.h>
#include "efe_lib.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>

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


void setup(){

    Serial.begin(115200);

    asil.loadPassword();

    durum=asil.loadSettings();

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

    

}

void loop(){

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
    
}