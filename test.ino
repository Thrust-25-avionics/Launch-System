//keeping track of states
int arm_state = 0;
int fire_state = 0;
int comm_state = 0;

void setup(){
    Serial.begin(9600);


}


void loop(){
    
    // //calling readXbee() function 
    // if(Xbee.available()){
    //     readXbee();
    // }
    // digitalWrite(comm_led,comm_state);
    

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