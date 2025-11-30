#include <ESP32Servo.h>
#include <Arduino.h>
#include "efe_lib.h"
#include <LiquidCrystal_I2C.h>

void Pins::pin_ata(Servo *motor){

        pinMode(red_pin,OUTPUT);
        pinMode(blue_pin,OUTPUT);
        pinMode(green_pin,OUTPUT);
        pinMode(buzzer_pin,OUTPUT);
        pinMode(Btn_Artr,INPUT_PULLUP);
        pinMode(Btn_Ony,INPUT_PULLUP);
        motor->attach(servo_pin);
}

void Password::sifre_ata(Pins *p, Servo *motor , Safe_State &state ,Safe_State sonraki_state,Servo_State servo_state,LiquidCrystal_I2C *lcd){
        if(digitalRead(p->getBtn_Artr())==LOW){

            kontrol++;

            if(kontrol>9){
                kontrol=0;
            }
        
            delay(250);

            while(digitalRead(p->getBtn_Artr()) == LOW);


        }
        if(digitalRead(p->getBtn_Ony())==LOW){

            Password *newNode = new Password();
        
            newNode->setDigit(kontrol);

            if(head==NULL){

                head=tail=newNode;

            }
            else{
                newNode->setPrev(tail);
            
                tail->setNext(newNode);

                tail=newNode;

            }
            kontrol=0;

            hane++;

            delay(200);

            while(digitalRead(p->getBtn_Ony()) == LOW);


            if(hane==4){
                ses_cal(p,1,&durum);

                renk_ayar(p,0,0,255,&durum);

                motor->write(servo_state);

                // state=BEKLEME;

                hane=0;

                state=sonraki_state;
            }
        }

    }
    
int Password::sifre_kontrol(Password &other){

        temp1=head;

        temp2=other.head;

        int sonuc = 1; 

        while(temp1!=nullptr && temp2!=nullptr){
            if(temp1->digit!=temp2->digit){
                temp2=other.head;
                while(temp2!=nullptr){
                    yedek=temp2;
                    temp2=temp2->getNext();
                    delete(yedek);
                }
                other.head = nullptr;
                other.tail = nullptr;
                sonuc=2;
                break;
            }
            temp1=temp1->getNext();
            temp2=temp2->getNext();
        }
        temp2=other.head;
        while(temp2!=nullptr){
            yedek=temp2;
            temp2=temp2->getNext();
            delete(yedek);
        }
        other.head = nullptr;
        other.tail = nullptr;

        return sonuc;

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




