#include <Arduino.h>
#include "efe_lib.h"
#include <ESP32Servo.h>

Safe_State kasa_durumu = GIRIS;

Pins pinler= {27,26,25,23,22,32,33};

Servo_State servo_durumu = LOCKED;

Servo myservo;

uint8_t durum = 0;

void setup(){

    Serial.begin(115200);

    pin_ata(&pinler,&myservo);//pinler atandı pinMode 
    
    servo_kontrol(&myservo,LOCKED);

    renk_ayar(&pinler,0,0,255);

}

void loop(){

    switch (kasa_durumu)
    {
    case GIRIS:
        sifre_ata(&pinler, &myservo, kasa_durumu, LOCKED);//
        break;
    
    case BEKLEME:
        renk_ayar(&pinler,255,255,0);
        Kasa_Durum(&pinler,&myservo , kasa_durumu ,servo_durumu, &durum);
        break;
    case ACIK:
        Serial.println("Sifre Dogru");
        servo_kontrol(&myservo,OPEN);
        renk_ayar(&pinler,0,255,0);
        delay(5000);
        kasa_durumu=BEKLEME;
        break;
    case KILITLI:
        Serial.println("Sifre Yanlis");
        servo_kontrol(&myservo,LOCKED);
        renk_ayar(&pinler,255,0,0);
        delay(5000);
        kasa_durumu=BEKLEME;
        break;
    default:
        break;
    }
    


}