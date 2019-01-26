#include <Arduino.h>
#include <math.h>

#define MICROSTEPS 64

int motorA1 = 11;
int motorA2 = 12;

int motorB1 = 8;
int motorB2 = 7;

int enablePinA = 10;
int enablePinB = 6;

int fullStepsPerSecond;

unsigned long delayMicroS;

int sinMap[256];
int cosMap[256];

unsigned long previousMicros = 0;

int oneDirectionMSteps = 180 * MICROSTEPS;

bool directionCw = true;

int currentMStep = 0;

void setup()
{
        pinMode(motorA1, OUTPUT);
        pinMode(motorA2, OUTPUT);
        pinMode(motorB1, OUTPUT);
        pinMode(motorB2, OUTPUT);

        pinMode(enablePinA, OUTPUT);
        pinMode(enablePinB, OUTPUT);

        fullStepsPerSecond = 50;

        // this line is for Leonardo's, it delays the serial interface
        // until the terminal window is opened
        while (!Serial);
        Serial.begin(9600);

        for (int i = 0; i < MICROSTEPS * 4; i++)
        {
                sinMap[i] = round(sin(M_PI * i / (2 * MICROSTEPS)) * 255);
                Serial.println(sinMap[i]);
                cosMap[i] = round(cos(M_PI * i / (2 * MICROSTEPS)) * 255);
        }

        delayMicroS = pow(10, 6) / (fullStepsPerSecond * MICROSTEPS);
}

void makeStep(int microstep)
{
        microstep = microstep % (MICROSTEPS * 4);

        int coilA = sinMap[microstep];
        int coilB = cosMap[microstep];

        analogWrite(enablePinA, abs(coilA));
        analogWrite(enablePinB, abs(coilB));

        digitalWrite(motorA1, coilA > 0 ? HIGH : LOW);
        digitalWrite(motorA2, coilA >= 0 ? LOW : HIGH);

        digitalWrite(motorB1, coilB > 0 ? HIGH : LOW);
        digitalWrite(motorB2, coilB >= 0 ? LOW : HIGH);
}

void loop()
{
        unsigned long currentMicros = micros();

        if (currentMicros - previousMicros >= delayMicroS)
        {
                previousMicros = currentMicros;

                if (currentMStep >= oneDirectionMSteps)
                {
                        directionCw = false;
                }
                else if (currentMStep < 0)
                {
                        directionCw = true;
                }

                makeStep(directionCw ? ++currentMStep : --currentMStep);
        }
}
