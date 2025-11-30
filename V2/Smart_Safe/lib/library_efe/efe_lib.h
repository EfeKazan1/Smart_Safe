#ifndef EFE_LIB_H
#define EFE_LIB_H

#include <ESP32Servo.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

#define SES_ACIK (1<<0)
#define LED_ACIK (1<<1)

extern uint8_t durum;

typedef enum{
    GIRIS,
    YENI_GIRIS,
    BEKLEME,
    ACIK,
    KILITLI

}Safe_State;

typedef enum {

    OPEN=90,
    LOCKED=0

}Servo_State;


class Pins{

private:
    int red_pin;
    int green_pin;
    int blue_pin;
    int Btn_Artr;
    int Btn_Ony;
    int servo_pin;
    int buzzer_pin;
    int ses_pin;
    int led_pin;
    int SDA;
    int SCL;

public:
    Pins(int red_pin, int green_pin, int blue_pin, int Btn_Artr, int Btn_Ony, int servo_pin , int buzzer_pin , int ses_pin , int led_pin , int SDA , int SCL):red_pin(red_pin), green_pin(green_pin), blue_pin(blue_pin), Btn_Artr(Btn_Artr), Btn_Ony(Btn_Ony), servo_pin(servo_pin), buzzer_pin(buzzer_pin) ,ses_pin(ses_pin), led_pin(led_pin), SDA(SDA), SCL(SCL){
    }
    int getRed_pin() { return red_pin;}
    int getGreen_pin() { return green_pin;}
    int getBlue_pin() { return blue_pin;}
    int getBtn_Artr() { return Btn_Artr;}
    int getBtn_Ony() { return Btn_Ony;}
    int getBuzzer_pin() { return buzzer_pin;}
    int getSes_pin() { return ses_pin;}
    int getled_pin() { return led_pin;}

    void pin_ata(Servo *motor);

};

class Password{
private:
    int digit;
    Password *next=nullptr;
    Password *prev=nullptr;
    Password* head=nullptr;
    Password* tail=nullptr;
    Password* temp1=nullptr;
    Password* temp2=nullptr;
    Password* yedek=nullptr;

    
public:
    int kontrol=0;
    int hane=0;

    int getDigit() {return digit;}
    Password* getNext(){ return next;}
    Password* getPrev(){ return prev;}

    void setDigit(int a) {digit=a;}
    void setNext(Password *n) {next=n;}
    void setPrev(Password *p) {prev=p;}
    void setHead(Password *h) { head=h;}
    void setTail(Password *t) { tail=t;}

    void sifre_ata(Pins *p, Servo *motor , Safe_State &state ,Safe_State sonraki_state,Servo_State servo_state,LiquidCrystal_I2C *lcd);

    int sifre_kontrol(Password &other);
};

void ses_cal(Pins *p,int tekrar, uint8_t *durum);
    
void renk_ayar(Pins *p, int r, int g, int b,uint8_t *durum);
  
void servo_kontrol(Servo *motor , Servo_State state);


#endif
