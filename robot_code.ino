//CODE FOR SELF-BALANCING ROBOT

#include <Adafruit_MPU6050.h> // MPU6050 library
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define PI 3.141592
#define del 500

Adafruit_MPU6050 mpu;
float i=0;
double d=0;
float alpha=0;

//Declaring motor driver variables 

const int PR1 = 5;      //  arduino  pin 5 to l298  pin IN4
const int PR2 = 6;      //  arduino  pin 6 to l298  pin IN3
const int PL1 = 7;      //  arduino  pin 7 to l298  pin IN1
const int PL2 = 8;      //  arduino  pin 8 to l298  pin IN2
const int PWMR= 9;      //  arduino  pin 9 to l298  pin ENB
const int PWML= 10;     //  arduino  pin 10 to l298  pin ENA

// PID control coefficients

const float kp=3500;
const float ki=50;
const float kd=600;

int aa=0;
int bb=0;
float pwm=0;

void setup(void) {
  pinMode(PR1,OUTPUT); //Setting driver output pins
  pinMode(PR2,OUTPUT);
  pinMode(PL1,OUTPUT);
  pinMode(PL2,OUTPUT);
  
  Serial.begin(115200);
  while (!Serial)
    delay(10); 

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  float e=a.acceleration.y;
  float theta = asin(e/10);
  theta=theta*180/PI;
  d=(theta-alpha)/del;
  d=d*1000;
  alpha=theta;
  Serial.print("theta ");
  Serial.print(theta);
  Serial.println("");
  i=i+(theta*del/1000);
  Serial.print("d :");
  Serial.print(d);
  Serial.println("");
  Serial.print("i ");
  Serial.print(i);
  Serial.println("");
  pwm = ( theta * kp ) + ( i*ki  ) + ( d * kd ) ; // P I D
  if(pwm < 0){    
            aa = 1;
            bb = 0;
               
           // digitalWrite(13, 0);
          }
          if(pwm > 0){    
            aa = 0;
            bb= 1;          
            //digitalWrite(13, 1);                                  
          }
   pwm  = abs(pwm);                  
          if ( pwm < 0) pwm = 0;
          if ( pwm > 255) pwm = 255;

 if(abs(theta) > 0.5){
              analogWrite(PWMR, pwm);
              digitalWrite(PR1, aa);
              digitalWrite(PR2 ,bb);
             
              analogWrite(PWML ,pwm);
              digitalWrite(PL1 ,aa);
              digitalWrite(PL2 ,bb);
              }
           else{
              analogWrite(PWMR , 0);
              analogWrite(PWML , 0);
              i = 0;
              delay(del);
           }
         
}
