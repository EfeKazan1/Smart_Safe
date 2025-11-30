#include <Arduino.h>
#include "efe_lib.h"
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27,16,2);

Safe_State kasa_durumu = GIRIS;

Pins pinler(27,26,25,23,18,32,33,19,5,21,22);

Servo_State servo_durumu = LOCKED;

Servo myservo;

uint8_t durum = 0;

Time time1;

Time time2;

Password asil;

Password other;

void setup(){

    Serial.begin(115200);

    lcd.init();

    lcd.backlight();

    pinler.pin_ata(&myservo); 
    
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
    if(digitalRead(pinler.getled_pin())==LOW){
        durum^=LED_ACIK;// 0000 0000
        lcd.setCursor(0,0);
        if(durum & LED_ACIK) {
            lcd.print("LED ACIK");
        }
        else{
            lcd.print("LED KAPALI");

        }   
        delay(1000);
        lcd.clear();
        
    }
    if(digitalRead(pinler.getSes_pin())==LOW){
        durum^=SES_ACIK; //0000 0000
        if(durum & SES_ACIK) {
            lcd.print("SES ACIK");
        }
        else{
            lcd.print("SES KAPALI");

        }   
        delay(1000);
        lcd.clear();
    }

    switch (kasa_durumu)
    {
    case GIRIS:
        lcd.setCursor(0,0);
        lcd.print("Sifrenizi Olusturunuz");
        asil.sifre_ata(&pinler, &myservo, kasa_durumu,YENI_GIRIS, LOCKED, &lcd);
        
        break;

    case YENI_GIRIS:
        lcd.setCursor(0,0);
        lcd.print("Sifrenizi Giriniz");
        other.sifre_ata(&pinler, &myservo, kasa_durumu,BEKLEME, LOCKED, &lcd);
        break;

    case BEKLEME:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sifre Girildi");
        renk_ayar(&pinler,255,255,0,&durum);
        delay(1000);
        if(asil.sifre_kontrol(other)==1){
            kasa_durumu=ACIK;
        }
        else{
            kasa_durumu=KILITLI;
        }
        lcd.clear();
        break;

    case ACIK:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Sifre Dogru");
        servo_kontrol(&myservo,OPEN);
        renk_ayar(&pinler,0,255,0,&durum);
        ses_cal(&pinler,1,&durum);
        delay(5000);
        kasa_durumu=YENI_GIRIS;
        servo_kontrol(&myservo,LOCKED);
        delay(2000);
        lcd.clear();
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