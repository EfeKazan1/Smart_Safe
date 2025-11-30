#include <ESP32Servo.h>
#include <Arduino.h>
#include "efe_lib.h"

int kontrol = 0;
int hane = 0;


void pin_ata(Pins *p, Servo *motor){

    pinMode(p->red_pin,OUTPUT);
    pinMode(p->blue_pin,OUTPUT);
    pinMode(p->green_pin,OUTPUT);
    pinMode(p->buzzer_pin,OUTPUT);
    pinMode(p->Btn_Artr,INPUT_PULLUP);
    pinMode(p->Btn_Ony,INPUT_PULLUP);
    motor->attach(p->servo_pin);
}

void ses_cal(Pins *p,int tekrar){
    for(int i=0 ; i<tekrar ; i++){

        digitalWrite(p->buzzer_pin,HIGH);
        delay(1000);
        digitalWrite(p->buzzer_pin,LOW);
        delay(500);
    }
}

void renk_ayar(Pins *p, int r, int g, int b){

    analogWrite(p->red_pin,r);
    analogWrite(p->green_pin,g);
    analogWrite(p->blue_pin,b);
}

void servo_kontrol(Servo *motor , Servo_State state){

    motor->write(state);

}


void sifre_ata(Pins *p, Servo *motor , Safe_State state ,Servo_State servo_state){
    
    if(digitalRead(p->Btn_Artr)==LOW){

        kontrol++;

        if(kontrol>9){
            kontrol=0;
        }
        
        delay(250);
        while(digitalRead(p->Btn_Artr) == LOW);

    }
    
    if(digitalRead(p->Btn_Ony)==LOW){
        Password *newNode = (struct Password*)malloc(sizeof(struct Password));
        newNode->digit=kontrol;
        if(head==NULL){
            head=tail=newNode;
            newNode->prev=NULL;
            newNode->next=NULL;
        }
        else{
            tail->next=newNode;
            newNode->prev=tail;
            tail=newNode;
            newNode->next=NULL;

        }
        Serial.print("Sayi:");
        Serial.println(kontrol);
        kontrol=0;
        hane++;
        delay(200);
        while(digitalRead(p->Btn_Ony) == LOW);

        if(hane==4){
            renk_ayar(p,0,0,255);
            ses_cal(p,2);

            motor->write(servo_state);

            state = BEKLEME;

            hane= 0;

        }

    }

}

int sifre_kontrol (Pins *p){


    if(digitalRead(p->Btn_Artr)==LOW){

        kontrol++;

        if(kontrol>9){
            kontrol=0;
        }
        
        delay(250);
        while(digitalRead(p->Btn_Artr) == LOW);

    }
    
    if(digitalRead(p->Btn_Ony)==LOW){
        Password *eleman = (struct Password*)malloc(sizeof(struct Password));
        eleman->digit=kontrol;
        if(headtried==NULL){
            headtried=tailtried=eleman;
            eleman->prev=NULL;
            eleman->next=NULL;
        }
        else{
            tailtried->next=eleman;
            eleman->prev=tailtried;
            tailtried=eleman;
            eleman->next=NULL;

        }
        Serial.print("Sayi:");
        Serial.println(kontrol);
        kontrol=0;
        hane++;
        delay(200);
        while(digitalRead(p->Btn_Ony) == LOW);

        if(hane==4){
            int sonuc= 1;
            Password *temp1=head;
            Password *temp2=headtried;
            Password *temp3=headtried;
            Password *yedek=headtried;

            while(temp1!=NULL && temp2!=NULL ){
                 if(temp1->digit != temp2->digit){
                    while(temp3!=NULL){
                        yedek=temp3;
                        temp3=temp3->next;
                        free(yedek);
                        
                    }
                    hane= 0;
                    sonuc=2;
                    break;
                }
                temp1= temp1->next;
                temp2=temp2->next;

            }
            while(temp3!=NULL){
                        yedek=temp3;
                        temp3=temp3->next;
                        free(yedek);
                        
                    }

            hane= 0;
            headtried=NULL;
            tailtried=NULL;
            
            return sonuc;

        }

    }
    return 0;
}
void Kasa_Durum(Pins *p, Servo *motor , Safe_State state ,Servo_State servo_state, uint8_t *durum){

    if(sifre_kontrol(p)==1){
        state=ACIK;
        motor->write(OPEN);
        renk_ayar(p,0,255,0);
        ses_cal(p,1);
    }
    else if(sifre_kontrol(p)==2){
        state=KILITLI;
        motor->write(LOCKED);
        renk_ayar(p,255,0,0);
        ses_cal(p,2);
    }


}




