#include <SoftwareSerial.h>

SoftwareSerial Xbee(4,5); // RX, TX

int fire_switch = 10;

int fire_state = 0;

//TODO: get pressure transducer and the load cell readings
void setup(){
    pinMode(fire_switch , OUTPUT);

}


void loop(){

    Xbee.write("connected");

    if(Xbee.available() && fire_state == 0){
        if(Xbee.read() == "fire"){
            fire_state = 1;
            digitalWrite(fire_switch,HIGH);
            Serial.println("fired");
            delay(5000);
            digitalWrite(fire_switch,LOW);
        }
    }


    if(fire_state == 1){
        //TODO: send data back 
    }



}