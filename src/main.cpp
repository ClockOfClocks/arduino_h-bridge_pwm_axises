#include <Arduino.h>
#include <math.h>;


int motorA1 = 11;
int motorA2 = 12;

int motorB1 = 8;
int motorB2 = 7;

int enablePinA = 10;
int enablePinB = 6;

int fullStepsPerSecond;

#define MICROSTEPS 64

void setup()
{
        pinMode(motorA1, OUTPUT);
        pinMode(motorA2, OUTPUT);
        pinMode(motorB1, OUTPUT);
        pinMode(motorB2, OUTPUT);

        pinMode(enablePinA, OUTPUT);
        pinMode(enablePinB, OUTPUT);

        fullStepsPerSecond = 30;

        // this line is for Leonardo's, it delays the serial interface
        // until the terminal window is opened
        while (!Serial);
        Serial.begin(9600);

        analogWrite(enablePinA, 255);
        analogWrite(enablePinB, 255);
}
void makeStep(int microstep){
        microstep = microstep % (MICROSTEPS * 4);

        int coilA = ceil(sin(M_PI * microstep / (2 * MICROSTEPS)) * 255);
        int coilB = ceil(cos(M_PI * microstep / (2 * MICROSTEPS)) * 255);


        analogWrite(enablePinA, abs(coilA));
        analogWrite(enablePinB, abs(coilB));

        uint8_t motorA1Value;
        uint8_t motorA2Value;
        uint8_t motorB1Value;
        uint8_t motorB2Value;

        if(coilA > 2) {
                motorA1Value = HIGH;
                motorA2Value = LOW;
        }else if(coilA < -2) {
                motorA1Value = LOW;
                motorA2Value = HIGH;
        }else{
                motorA1Value = LOW;
                motorA2Value = LOW;
        }

        if(coilB > 2) {
                motorB1Value = HIGH;
                motorB2Value = LOW;
        }else if (coilB < -2) {
                motorB1Value= LOW;
                motorB2Value = HIGH;
        }else{
                motorB1Value = LOW;
                motorB2Value = LOW;
        }

        digitalWrite(motorA1, motorA1Value);
        digitalWrite(motorA2, motorA2Value);
        digitalWrite(motorB1, motorB1Value);
        digitalWrite(motorB2, motorB2Value);

        // Serial.print(coilA);
        // Serial.print(" | ");
        // Serial.print(coilB);
        //
        // Serial.print(" | ");
        // Serial.print(motorA1Value == HIGH ? "HIGH" : "LOW");
        // Serial.print(" | ");
        // Serial.print(motorA2Value == HIGH ? "HIGH" : "LOW");
        // Serial.print(" | ");
        // Serial.print(motorB1Value == HIGH ? "HIGH" : "LOW");
        // Serial.print(" | ");
        // Serial.print(motorB2Value == HIGH ? "HIGH" : "LOW");
        //
        // Serial.print("\n");
}


void loop()
{
        int s = 180 * MICROSTEPS;
        uint64_t delayMicroS = pow(10, 6) / (fullStepsPerSecond * MICROSTEPS);
        //delayMicroS = pow(10, 6);

        for (int i = 0; i < s; i++) {
                makeStep(i);
                delayMicroseconds(delayMicroS);
        }

        delay(4000);

        for (int i = s; i >= 0; i--) {
                makeStep(i);
                delayMicroseconds(delayMicroS);
        }

        delay(4000);
}
