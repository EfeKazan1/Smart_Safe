#include <ESP32Servo.h>
#include <Arduino.h>
#include "efe_lib.h"
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>


void Pins::begin(Servo *motor){

        pinMode(red_pin,OUTPUT);
        pinMode(blue_pin,OUTPUT);
        pinMode(green_pin,OUTPUT);
        pinMode(buzzer_pin,OUTPUT);
        pinMode(Btn_Artr,INPUT_PULLUP);
        pinMode(Btn_Ony,INPUT_PULLUP);
        motor->attach(servo_pin);
}

void Password::password_begin(Pins *p,Safe_State &safe_state, Safe_State next_state){

    if(digitalRead(p->getBtn_Artr())==LOW){

        anlik++;

        if(anlik>9) anlik=0;

        delay(250);

        while(digitalRead(p->getBtn_Artr()) == LOW);

    }
    if(digitalRead(p->getBtn_Ony())==LOW){

        sifre[hane]=anlik;

        hane++;

        anlik=0;

        delay(200);

        while(digitalRead(p->getBtn_Ony()) == LOW);
        
        if(hane>=4){

            if(safe_state==GIRIS){
                savePassword();
            }
            safe_state=next_state;

            kontrol=1;

            hane= 0;

        }   

    }

}

bool Password::checkwebpassword(String gelenyazi){

    if(gelenyazi.length()!=4){
        return false;
    }

    for(uint8_t i = 0; i<4 ; i++){
        int gelenrakam= gelenyazi.charAt(i) - '0';

        if(sifre[i]!=gelenrakam){
            return false;
        }
    }
    return true;

}

void Password::setwebpassword(String gelenyazi){

    if(gelenyazi.length()!=4){
        return;
    }
    for (uint8_t i = 0; i<4 ; i++){
        sifre[i]=gelenyazi.charAt(i) - '0';
    }

    savePassword();

}

uint8_t Password::password_check(Password &other){

    uint8_t sonuc = 1;

    for(uint8_t i=0; i<4; i++){

        if(sifre[i]!= other.sifre[i]){

            sonuc=2;

            break;
        }
    }
    for(uint8_t j = 0; j<4 ; j++){

        other.sifre[j]=0;

    }

    return sonuc;

}
//eski şifreyi öğrenmek için
void Password::loadPassword(){

    mypreference.begin("kasa",true); // kasa var mı varsa salt oku yoksa oluştur ve oku 

    mypreference.getBytes("sifre",sifre,4);

    mypreference.end();

}
    
void Password::savePassword(){

    mypreference.begin("kasa",false);

    mypreference.putBytes("sifre",sifre,4);

    mypreference.end();
}

void Password::saveSettings(uint8_t ayar){

    mypreference.begin("kasa",false);

    mypreference.putUChar("ayar",ayar);

    mypreference.end();
}

uint8_t Password::loadSettings(){

    mypreference.begin("kasa",true);

    uint8_t gelen= mypreference.getUChar("ayar",3); 
    //eğer ayar yoksa -ilk açılışta- 3 değerini döndürür 0000 0011 =3 ses ve led açık


    mypreference.end();

    return gelen;
}

void ses_cal(Pins *p,int tekrar,uint8_t *durum){
    if(*durum&SES_ACIK){
        for(int i=0 ; i<tekrar ; i++){
            digitalWrite(p->getBuzzer_pin(),HIGH);
            delay(200);
            digitalWrite(p->getBuzzer_pin(),LOW);
            delay(200);
        }
        
    }
    else{
        digitalWrite(p->getBuzzer_pin(),LOW);
    }
    
}

void renk_ayar(Pins *p, int r, int g, int b,uint8_t *durum){
    if(*durum&LED_ACIK){
        analogWrite(p->getRed_pin(),r);
        analogWrite(p->getGreen_pin(),g);
        analogWrite(p->getBlue_pin(),b);
    }
    else{
        analogWrite(p->getRed_pin(),0);
        analogWrite(p->getGreen_pin(),0);
        analogWrite(p->getBlue_pin(),0);
    }
   
}

void servo_kontrol(Servo *motor , Servo_State state){

    motor->write(state);

}




