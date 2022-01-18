#include <ECE3.h>
#include <limits.h>

//#include <ECE3_LCD7.h>

unsigned short sensor_values[8]; // right -> left, 0 -> 7
// const byte ERR_WEIGHTS[4] = { 14, 13, 12, 8 };

const int L_NSLP=31; // nslp ==> awake & ready for PWM
const int L_DIR=29;
const int L_PWM=40;
const int LED_1 = 57;
const int LED_2 = 58;

const int R_NSLP = 11;
const int R_DIR = 30;
const int R_PWM = 39;

const int LED_RF = 41;


  
const float K_p = 0.03;

const float K_d = 0.42;
const unsigned char BASE_SPD = 160;
const unsigned short DONUT_THRESHOLD = 4000;

// TODO: CHANGE TO CALIBRATE SENSOR
/* ----- Sensor calibration data ------ */
const short SENSOR_MAX_DATA[8] = {1926, 1921, 1994, 1465, 1495, 1903, 1880, 1788};
const short SENSOR_MIN_DATA[8] = {574, 579, 506, 551, 497, 597, 620, 712};
short SENSOR_SUM; // For donuting
bool DONUT_CONDITION = false;
bool DONUT_COMPLETE = false;
bool CAR_STOP = false;
short stop_count = 0;


short prev_err = SHRT_MIN;

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
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  
  ECE3_Init();

// set the data rate in bits/second for serial data transmission
  Serial.begin(9600); 
  delay(2000); //Wait 2 seconds before starting 
  
}



/* Fuse sensor program complte */

short fuse_sensors() {
  SENSOR_SUM = 0;
  ECE3_read_IR(sensor_values);
  int result[8];
  for (int i = 0; i < 8; i++) {
    
    if (sensor_values[i] == 0)
    {
      result[i] = 0;
    }
    else
    {
      result[i] = ((sensor_values[i] - SENSOR_MIN_DATA[i]) * 1000 / SENSOR_MAX_DATA[i]); 
    }
    SENSOR_SUM += result[i];

    //Serial.print(result[i]);
    //Serial.print('\t');
  }

  short fused = ((-15)*result[0] - 14*result[1] - 12*result[2] - 8 * result[3] + 8* result[4] + 12*result[5] + 14*result[6] + 15*result[7]) / 8;
  return fused;
  

}

void check_donut()
{
  if (SENSOR_SUM > DONUT_THRESHOLD) {
    // digitalWrite(LED_1, HIGH);

    if (DONUT_CONDITION) {

    // Two donut signals, invoke donut
      if (!DONUT_COMPLETE) {
          execute_donut();
      } else {
        if (stop_count > 300)  {

          
          analogWrite(L_PWM,0);
          analogWrite(R_PWM,0);
          exit(0);
          
        }
        

      }
    
    }
    else {
      DONUT_CONDITION = true;
    }
  }
  else {
    // digitalWrite(LED_1, LOW);

    DONUT_CONDITION = false;

  }
}

void execute_donut()
{
  digitalWrite(R_DIR,HIGH);

  analogWrite(L_PWM,90);
  analogWrite(R_PWM,90);

  delay(570);
  digitalWrite(R_DIR,LOW);
  analogWrite(L_PWM,0);
  analogWrite(R_PWM,0);
  DONUT_CONDITION = false;

  DONUT_COMPLETE = true;
  digitalWrite(LED_2, HIGH);
}


void loop() {

  // TODO: Change this to test different K_P values

  // TODO: Change this to test different base speed
  


  short sensor_fusion = fuse_sensors();
  
  float d = 0;
  if (prev_err != SHRT_MIN) {
    float err_deriv = prev_err - sensor_fusion;
    d = K_d * err_deriv;
  }
  
  float p = sensor_fusion * K_p;
  float control = p - d;

  analogWrite(L_PWM, BASE_SPD - control);
  analogWrite(R_PWM, BASE_SPD + control);

  check_donut();
  if (stop_count <= 300 && DONUT_COMPLETE) {
    stop_count ++;
  }
  
  prev_err = sensor_fusion;
}
