//şimdi struct yapısı olcak pinler için enum olucak hatta bi tane de menu olsun bitwise olcak şifre için struct

#ifndef EFE_LIB_H
#define EFE_LIB_H

#include <ESP32Servo.h>
#include <Arduino.h>

#define SES_ACIK (1<<0)
#define LED_ACIK (1<<1)
#define CHECK (1<<2)

extern uint8_t durum;

typedef struct {

    int red_pin;
    int green_pin;
    int blue_pin;
    int Btn_Artr;
    int Btn_Ony;
    int servo_pin;
    int buzzer_pin;

}Pins;

typedef struct Password{

    int digit;
    struct Password *next;
    struct Password *prev;
   

}Password;

extern Password *head;
extern Password *tail;
extern Password *headtried;
extern Password *tailtried;


typedef enum{
    GIRIS,
    BEKLEME,
    ACIK,
    KILITLI

}Safe_State;

typedef enum {

    OPEN=90,
    LOCKED=0

}Servo_State;

void pin_ata(Pins *p, Servo *motor);

void ses_cal(Pins *p,int tekrar);
    
void renk_ayar(Pins *p, int r, int g, int b);
  
void servo_kontrol(Servo *motor , Servo_State state);

void sifre_ata(Pins *p, Servo *motor , Safe_State state ,Servo_State servo_state);
    
int sifre_kontrol (Pins *p);

void Kasa_Durum(Pins *p, Servo *motor , Safe_State state ,Servo_State servo_state, uint8_t *durum);

    
#endif
