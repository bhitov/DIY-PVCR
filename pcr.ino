#include <math.h>
#include <PID_v1.h>

// Controls heat (lightbulb)
int RelayHOT = 4;

// Controls cooling (fan)
int RelayCOLD = 5;

// Connect PID controller
double Setpoint, Input, Output;
double P = 0.25;
double I = 2;
double D = 1;
PID myPID(&Input, &Output, &Setpoint, P, I, D, DIRECT);

// Convert thermister voltage to temp in C
double Thermister(int RawADC)
{
double Temp;
Temp = log(((10240000/RawADC) - 10000));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
Temp = Temp - 273.15; // Convert Kelvin to Celcius
return Temp;
}

void setup()
{
Serial.begin(115200);
pinMode(RelayHOT, OUTPUT);
pinMode(RelayCOLD, OUTPUT);

Input = Thermister(analogRead(0));

// target temp
Setpoint = 23.0;

myPID.SetMode(AUTOMATIC);

// PID will output something between 0-3
myPID.SetOutputLimits(0.0,3.0);

}

void loop()
{

Input = Thermister(analogRead(0));
Serial.println(Input,2); // print Celcius temp with 2 decimals reading in serial monitor

myPID.Compute();

Serial.println(Output, 3);

if (Output < 1.0) { // The temperature is too hot
  digitalWrite(RelayHOT, LOW);
  digitalWrite(RelayCOLD, HIGH);
} else if (Output < 2.0) { // The temperature is juussstt right
  digitalWrite(RelayHOT, LOW);
  digitalWrite(RelayCOLD, LOW);
} else { // The temperature is too cold
  digitalWrite(RelayHOT, HIGH);
  digitalWrite(RelayCOLD, LOW);
}  

delay(200); // loop time is 200ms

}
