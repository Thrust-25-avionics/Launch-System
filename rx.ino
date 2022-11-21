/*
    TODO:change pinout acc. to sch
    TODO:change hx711 calibration factor
    TODO: change pressure transducer max value
    TODO:add SerialTransfer.h xbee
*/

#include <Wire.h>
#include <SoftwareSerial.h>
#include <SerialTransfer.h>
#include <HX711_ADC.h>
#include <SPI.h>
#include <SD.h>

// pins:
#define HX711_dout 4; // mcu > HX711 dout pin
#define HX711_sck 5;  // mcu > HX711 sck pin
#define fire_mosfet 10; //mosfet
#define presstrans_pin = A0; //pressure transducer
#define buzzer 7; //buzzer

// HX711 instance:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

// SoftwareSerial instance
SoftwareSerial Xbee(4, 5); // RX, TX

//SerialTransfer instance
SerialTransfer myTransfer;

// struct definition
struct _attribute_((packed)) STRUCT
{
    char msg;
    float presstrans_val;
    float loadcell_val;
}testStruct;


#define chip_select 10; // cs for sdcard

unsigned long t = 0;
int fire_state = 0;

const int presstrans_max_PSI = 100;//max value output of press trans (to change)


// TODO: get pressure transducer and the load cell readings
void setup()
{   
    // Xbee setup:
    Serial.begin(9600);
    while (!Serial) ;
    Xbee.begin(9600);
    myTransfer.begin(Xbee);

    // fire mosfet
    pinMode(fire_mosfet, OUTPUT);

    // Loadcell setup:
    LoadCell.begin();
    // ! calibration value to be changed
    float calibrationVal = 696.0;

    unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
    boolean _tare = true;                 // set this to false if you don't want tare to be performed in the next step
    LoadCell.start(stabilizingtime, _tare);
    if (LoadCell.getTareTimeoutFlag())
    {
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
        while (1);
    }
    else
    {
        LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
        Serial.println("Startup is complete");
    }

    
    // sdcard setup:
    Serial.print("Initializing SD card...");

    // see if the card is present and can be initialized:
    if (!SD.begin(chip_select))
    {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1)
            ;
    }
    Serial.println("card initialized.");
    
}

void loop()
{

    

    if(myTransfer.available()){
        readXbee();
    }

    // TODO: send data back
    testStruct.loadcell_val = getloadcell_val();
    testStruct.presstrans_val = getpresstrans_val();

    //sending data back;
    writeXbee();

    writeSDCard() ; //write into sd card

    delay(10);
}

void writeSDCard()
{
    String dataString = millis() + "," + presstrans_val + "," + loadcell_val;
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.println(dataString);
        dataFile.close();
    
        Serial.println(dataString);// print to the serial port too:
    }
    // if the file isn't open, pop up an error:
    else
    {
        Serial.println("error opening datalog.txt");
    }
}

float getpresstrans_val()
{
    float val = analogRead(presstrans_pin);
    val =( (val - 102.4)*presstrans_max_PSI)/(921.6-102.4);
    return val;

}

float getloadcell_val()
{
    static boolean newDataReady = 0;
    const int serialPrintInterval = 0; // increase value to slow down serial print activity
    float val;

    // check for new data/start next conversion:
    if (LoadCell.update())
        newDataReady = true;

    // get smoothed value from the dataset:
    if (newDataReady)
    {
        if (millis() > t + serialPrintInterval)
        {
            val = LoadCell.getData();
            Serial.print("Load_cell output val: ");
            Serial.println(i);
            newDataReady = 0;
            t = millis();
        }
    }

    // receive command from serial terminal, send 't' to initiate tare operation:
    if (Serial.available() > 0)
    {
        char inByte = Serial.read();
        if (inByte == 't')
            LoadCell.tareNoDelay();
    }

    // check if last tare operation is complete:
    if (LoadCell.getTareStatus() == true)
    {
        Serial.println("Tare complete");
    }

    return val;
}

void readXbee(){

}