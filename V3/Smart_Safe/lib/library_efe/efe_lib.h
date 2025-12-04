#ifndef EFE_LIB_H
#define EFE_LIB_H

#include <ESP32Servo.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>


#define SES_ACIK (1<<0)
#define LED_ACIK (1<<1)

extern uint8_t durum;

typedef enum{
    GIRIS,
    YENI_GIRIS,
    KONTROL,
    ACIK,
    KILITLI

}Safe_State;

typedef enum {

    OPEN=90,
    LOCKED=0

}Servo_State;


class Pins{

private:
    const uint8_t red_pin;
    const uint8_t green_pin;
    const uint8_t blue_pin;
    const uint8_t Btn_Artr;
    const uint8_t Btn_Ony;
    const uint8_t servo_pin;
    const uint8_t buzzer_pin;
    const uint8_t ses_pin;
    const uint8_t led_pin;
    const uint8_t SDA;
    const uint8_t SCL;

public:
    Pins(const uint8_t red_pin, const uint8_t green_pin, const uint8_t blue_pin, const uint8_t Btn_Artr, const uint8_t Btn_Ony, const uint8_t servo_pin , const uint8_t buzzer_pin , const uint8_t ses_pin , const uint8_t led_pin , const uint8_t SDA , const uint8_t SCL):red_pin(red_pin), green_pin(green_pin), blue_pin(blue_pin), Btn_Artr(Btn_Artr), Btn_Ony(Btn_Ony), servo_pin(servo_pin), buzzer_pin(buzzer_pin) ,ses_pin(ses_pin), led_pin(led_pin), SDA(SDA), SCL(SCL){
    }
    uint8_t getRed_pin() { return red_pin;}
    uint8_t getGreen_pin() { return green_pin;}
    uint8_t getBlue_pin() { return blue_pin;}
    uint8_t getBtn_Artr() { return Btn_Artr;}
    uint8_t getBtn_Ony() { return Btn_Ony;}
    uint8_t getBuzzer_pin() { return buzzer_pin;}
    uint8_t getSes_pin() { return ses_pin;}
    uint8_t getled_pin() { return led_pin;}

    void begin(Servo *motor);

};

class Password{
private:

    uint8_t kontrol;

    uint8_t sifre[4];

    uint8_t anlik;

    uint8_t hane;

    Preferences mypreference;
 
public:
    
    Password(){
        anlik = 0;

        hane = 0;

        kontrol = 0;

        for(uint8_t i = 0; i<4; i++){
            sifre[i]=0;//diziyi sıfırlamak için
        }
    }

    void password_begin(Pins *p,Safe_State &safe_state,Safe_State next_state);

    uint8_t password_check(Password &other);

    void loadPassword();
    
    void savePassword();

    void saveSettings(uint8_t ayar);

    uint8_t loadSettings();

};

void ses_cal(Pins *p,int tekrar, uint8_t *durum);
    
void renk_ayar(Pins *p, int r, int g, int b,uint8_t *durum);
  
void servo_kontrol(Servo *motor , Servo_State state);


#endif
