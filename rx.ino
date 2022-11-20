#include <SoftwareSerial.h>
#include <HX711_ADC.h>

//pins:
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin
const int fire_mosfet = 10;

//HX711 instance:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

//SoftwareSerial instance
SoftwareSerial Xbee(4,5); // RX, TX


int fire_state = 0;

//TODO: get pressure transducer and the load cell readings
void setup(){
    pinMode(fire_mosfet , OUTPUT);

    LoadCell.begin();

    // ! calibration value to be changed
    float calibrationVal = 696.0;
    unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
    LoadCell.start(stabilizingtime, _tare);
    if (LoadCell.getTareTimeoutFlag()) {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1);
    }
    else {
        LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
        Serial.println("Startup is complete");
    }

}


void loop(){

    Xbee.write("connected");

    if(Xbee.available() && fire_state == 0){
        if(Xbee.read() == "fire"){
            fire_state = 1;
            digitalWrite(fire_mosfet,HIGH);
            Serial.println("fired");
            delay(1000);
            digitalWrite(fire_mosfet,LOW);
        }
    }

    if(fire_state == 1){
        //TODO: send data back 
    }



}

void getLoadCellVal(){

}