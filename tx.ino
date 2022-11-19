#include <SoftwareSerial.h>

SoftwareSerial Xbee(4,5); // RX, TX

//led pin out
int safety_led = 10;
int arm_led = 11;
int fire_led = 12;
int comm_led = 13;

//switches pin in
int arm_switch = 14;
int fire_switch = 15;

//keeping track of states
int arm_state = 0;
int fire_state = 0;
int comm_state = 0;



void setup(){

    Serial.begin(9600);
    while(!Serial);

    Xbee.begin(9600);

    pinMode(safety_led,OUTPUT);
    pinMode(arm_led,OUTPUT);
    pinMode(fire_led,OUTPUT);
    pinMode(comm_led,OUTPUT);

    pinMode(arm_switch,INPUT );
    pinMode(fire_switch,INPUT );
   
   //safety led on as soon as arduino turns on at safety key enable
    digitalWrite(safety_led,1);

}


void loop(){
    
    //calling readXbee() function 
    if(Xbee.available()){
        readXbee();
    }
    digitalWrite(comm_led,comm_state);
    

    //function to perform when arm is enabled
    if(digitalRead(arm_switch) == HIGH && !fire_state ){
        arm_state = 1;
        Serial.println("armed");
    }
    else if(digitalRead(arm_switch) == LOW)
        arm_state =0; 
    digitalWrite(arm_led,arm_state);

    
    //functions to perform when fire is enabled
    if(digitalRead(fire_switch)==1 && arm_state=1){
        fire_state=1;
        Xbee.write("fire");
        Serial.println("launched");
    }
    else
        fire_state =0;
    digitalWrite(fire_led,fire_state);

}

void readXbee(){

    comm_state=0;

    //when fire signal is not sent check if receiving "connected" message
    if(!fire_state &&  Xbee.read()=="connected"){
        comm_state = 1;
        Serial.println("connected");
    }

    //when motor is fired start receiving data back
    if(fire_state){
        comm_state=1;
        //TODO:store data incoming from Xbee somehow
    }
}