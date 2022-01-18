#include <ECE3.h>
#include <limits.h>

//#include <ECE3_LCD7.h>

unsigned short sensor_values[8]; // right -> left, 0 -> 7
// const byte ERR_WEIGHTS[4] = { 8, 4, 2, 1 };

const int L_NSLP=31; // nslp ==> awake & ready for PWM
const int L_DIR=29;
const int L_PWM=40;
const int LED_1 = 57;
const int LED_2 = 58;

const int R_NSLP = 11;
const int R_DIR = 30;
const int R_PWM = 39;

const int LED_RF = 41;

const int M_1 = 1926;
const int M_2 = 1921;
const int M_3 = 1994;
const int M_4 = 1465;
const int M_5 = 1495;
const int M_6 = 1903;
const int M_7 = 1880;
const int M_8 = 1788;



// TODO: CHANGE THIS
/* ----- Sensor calibration data ------ */
const int SENSOR_MAX_DATA[8] = {1926, 1921, 1994, 1465, 1495, 1903, 1880, 1788};
const int SENSOR_MIN_DATA[8] = {574, 579, 506, 551, 497, 597, 620, 712};
int SENSOR_SUM; // For donuting
bool DONUT_CONDITION = false;

///////////////////////////////////
void setup() {
// put your setup code here, to run once:
  pinMode(L_NSLP,OUTPUT);
  pinMode(L_DIR,OUTPUT);
  pinMode(L_PWM,OUTPUT);

  pinMode(R_NSLP,OUTPUT);
  pinMode(R_DIR,OUTPUT);
  pinMode(R_PWM,OUTPUT);

  digitalWrite(L_DIR,LOW);
  digitalWrite(L_NSLP,HIGH);
  
  digitalWrite(R_DIR,LOW);
  digitalWrite(R_NSLP,HIGH);

  pinMode(LED_RF, OUTPUT);
  
  ECE3_Init();

// set the data rate in bits/second for serial data transmission
  Serial.begin(9600); 
  Serial.println("Hello!");
  delay(2000); //Wait 2 seconds before starting 
  
}

void print_values() {
  for (unsigned char i = 0; i < 8; ++i) {
    Serial.print(sensor_values[i]);
    Serial.print("\t");
  }
  Serial.println();
}

/* Fuse sensor program complte */


void execute_donut()
{
  digitalWrite(R_DIR,HIGH);

  //TODO: EDIT PWM
  analogWrite(L_PWM,90);
  analogWrite(R_PWM,90);

  //TODO: EDIT delay
  delay(590);
  digitalWrite(R_DIR,LOW);
  analogWrite(L_PWM,0);
  analogWrite(R_PWM,0);
  exit(0);
}

void loop() {



  execute_donut();
  


    
}
