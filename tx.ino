/*
    TODO:change pinout acc to sch
    TODO:add SerialTransfer
    TODO:change comms connected logic
    TODO: receive structure and print onto serial for the plotter
    TODO: add func in plotter or make new one for plotting incoming data

*/

#include <SoftwareSerial.h>
#include <SerialTransfer.h>

SerialTransfer myTransfer;

// struct definition
struct _attribute_((packed)) STRUCT
{
    char msg;
    float presstrans_val;
    float loadcell_val;
}testStruct;

SoftwareSerial Xbee(4, 5); // RX, TX

// led pin out
#define safety_led 10;
#define arm_led 11;
#define fire_led 12;
#define comm_led 13;

// switches pin in
#define arm_switch 14;
#define fire_switch 15;

// keeping track of states
int arm_state = 0;
int fire_state = 0;
int comm_state = 0;

void setup()
{

    Serial.begin(9600);
    while (!Serial)
        ;

    Xbee.begin(9600);
    myTransfer.begin(Xbee);

    pinMode(safety_led, OUTPUT);
    pinMode(arm_led, OUTPUT);
    pinMode(fire_led, OUTPUT);
    pinMode(comm_led, OUTPUT);

    pinMode(arm_switch, INPUT);
    pinMode(fire_switch, INPUT);

    // safety led on as soon as arduino turns on at safety key enable
    digitalWrite(safety_led, 1);
}

void loop()
{

    // calling readXbee() function
    if (myTransfer.available())
    {
        readXbee();
    }
    digitalWrite(comm_led, comm_state);

    // function to perform when arm is enabled
    if (digitalRead(arm_switch) == HIGH && digitalRead(fire_switch) == 0)
    {
        arm_state = 1;
        Serial.println("armed");
    }
    else if (digitalRead(arm_switch) == LOW)
        arm_state = 0;

    if (arm_state == 1)
        sendArmSig();
    digitalWrite(arm_led, arm_state);

    // functions to perform when fire is enabled
    if (digitalRead(fire_switch) == 1 && arm_state == 1)
    {
        fire_state = 1;
        sendFireSig();
        Serial.println("launched");
    }
    else
    {
        fire_state = 0;
    }
    digitalWrite(fire_led, fire_state);
}

void readXbee()
{

    comm_state = 0;

    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

    recSize = myTransfer.rxObj(testStruct, recSize);
    if (testStruct.msg == 'C')
    {
        comm_state = 1;
    }
    else
    {
        comm_state = 0;
    }

    Serial.println("pressure: " + testStruct.presstrans_val + "PSI  ; " + "load cell: " + testStruct.loadcell_val + " kg");
}

void sendFireSig()
{
    // use this variable to keep track of how many
    // bytes we're stuffing in the transmit buffer
    uint16_t sendSize = 0;
    char sig = 'F';

    sendSize = myTransfer.txObj(sig, sendSize);

    ///////////////////////////////////////// Send buffer
    myTransfer.sendData(sendSize);
    delay(100);
}

void sendArmSig()
{
    // use this variable to keep track of how many
    // bytes we're stuffing in the transmit buffer
    uint16_t sendSize = 0;
    char sig = 'A';

    sendSize = myTransfer.txObj(sig, sendSize);

    ///////////////////////////////////////// Send buffer
    myTransfer.sendData(sendSize);
    delay(100);
}