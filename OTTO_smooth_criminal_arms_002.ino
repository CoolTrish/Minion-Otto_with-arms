//#include <Otto.h>
//#include <Otto_gestures.h>
//#include <Otto_mouths.h>
//#include <Otto_sounds.h>

#include <IRremote.h>
#include <IRremoteInt.h>
#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

//----------------------------------------------------------------
//-- OTTO Dance smooth criminal
//-- Released under a GPL licencse
//-- Originally made for Zowi project remake for Otto
//-- Authors:  Javier Isabel:  javier.isabel@bq.com
//--           Juan Gonzalez (obijuan): juan.gonzalez@bq.com
//-- Tweaks for arms and IR: Jonathan Hess - December 2016
//--  Used Tolako IR remote kit -https://www.amazon.com/gp/product/B01EL7I5HC/ 
//-----------------------------------------------------------------

int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11
//SoftwareSerial BTserial(9, 10); // RX | TX

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
char myChar ;

#define N_SERVOS 6
//-- First step: Configure the pins where the servos are attached
/*
         ---------------
  RA 7==> |     O   O     | <== LA 6
        |---------------|
  YR 3==> |               | <== YL 2
         ---------------
            ||     ||
            ||     ||
  RR 5==>   -----   ------  <== RL 4
         |-----   ------|
*/
#define EEPROM_TRIM true
// Activate to take callibration data from internal memory
#define TRIM_RR 0
#define TRIM_RL 4
#define TRIM_YR  4
#define TRIM_YL -7
#define TRIM_LA 0
#define TRIM_RA 0
//OTTO.setTrims(-7,-4,-4,7);

#define PIN_RR 5
#define PIN_RL 4
#define PIN_YR 3
#define PIN_YL 2
#define PIN_LA 6
#define PIN_RA 7

#define INTERVALTIME 10.0

Oscillator servo[N_SERVOS];

void goingUp(int tempo);
void drunk (int tempo);
void noGravity(int tempo);
void kickLeft(int tempo);
void kickRight(int tempo);
void run(int steps, int T = 500);
void walk(int steps, int T = 1000);
void backyard(int steps, int T = 3000);
void backyardSlow(int steps, int T = 5000);
void turnLeft(int steps, int T = 3000);
void turnRight(int steps, int T = 3000);
void moonWalkLeft(int steps, int T = 1000);
void moonWalkRight(int steps, int T = 1000);
void crusaito(int steps, int T = 1000);
void swing(int steps, int T = 1000);
void upDown(int steps, int T = 1000);
void flapping(int steps, int T = 1000);

void setup()
{



  // Working IR Receiver Code
  Serial.begin(19200);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn(); // Start the receiver


  //    Serial.begin(9600);
  //    Serial.println("Enter AT commands:");

  // HC-06 default serial speed is 9600
  //  BTserial.begin(9600);

  servo[0].attach(PIN_RR);
  servo[1].attach(PIN_RL);
  servo[2].attach(PIN_YR);
  servo[3].attach(PIN_YL);
  servo[4].attach(PIN_LA);
  servo[5].attach(PIN_RA);

  //EEPROM.write(0,TRIM_RR);
  //EEPROM.write(1,TRIM_RL);
  //EEPROM.write(2,TRIM_YR);
  //EEPROM.write(3,TRIM_YL);

  int trim;

  if (EEPROM_TRIM) {
    for (int x = 0; x < 4; x++) {
      trim = EEPROM.read(x);
      if (trim > 128)trim = trim - 256;
      Serial.print("TRIM ");
      Serial.print(x);
      Serial.print(" en ");
      Serial.println(trim);
      servo[x].SetTrim(trim);
    }
  }
  else {
    servo[0].SetTrim(TRIM_RR);
    servo[1].SetTrim(TRIM_RL);
    servo[2].SetTrim(TRIM_YR);
    servo[3].SetTrim(TRIM_YL);
    servo[4].SetTrim(TRIM_LA);
    servo[5].SetTrim(TRIM_RA);
  }

  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
}

// TEMPO: 121 BPM
int t = 405; //495 is original
double pause = 0;

void loop()
{

  //Working IR functionality
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value
  }

  // if(Serial.available()){
  //  char init = Serial.read();
  // if (init=='X'){
  //   delay(4000); //3000 - 4500

  // dance();


  //    Serial.print();
  //    Serial.print();
  //    Serial.print();
  //   Serial.println("Hey Dudes!");
  //   Serial.flush();


  //for(int i=0;i<4;i++) servo[i].SetPosition(90);


  //    for(int i=0;i<4;i++) servo[i].SetPosition(90);
  // }
  // }
}


/*-----( Function )-----*/
void translateIR() // takes action based on IR code received

// describing Remote IR codes
{
  switch (results.value)
  {
      Serial.println(" Value to check against:");
      Serial.println(results.value, HEX);
      Serial.println("End value to check against:");

    case 0xFF629D: Serial.println(" FORWARD");
      walk(4, t * 2);
      break;

    case 0xFF22DD: Serial.println(" LEFT");
      moonWalkLeft(4, t * 2);
      break;

    case 0xFF02FD: Serial.println(" -OK-");
      dance();
      break;

    case 0xFFC23D: Serial.println(" RIGHT");
      moonWalkRight(4, t * 2);
      break;

    case 0xFFA857: Serial.println(" REVERSE");
      backyard(4, t * 2);
      break;

    case 0xFF6897: Serial.println(" 1");
      kickLeft(t);
      break;

    case 0xFF9867: Serial.println(" 2");
      kickRight(t);
      break;

    case 0xFFB04F: Serial.println(" 3");    break;
    case 0xFF30CF: Serial.println(" 4");    break;
    case 0xFF18E7: Serial.println(" 5");    break;
    case 0xFF7A85: Serial.println(" 6");    break;
    case 0xFF10EF: Serial.println(" 7");    break;
    case 0xFF38C7: Serial.println(" 8");    break;
    case 0xFF5AA5: Serial.println(" 9");    break;
    case 0xFF42BD: Serial.println(" *");    break;
    case 0xFF4AB5: Serial.println(" 0");    break;
    case 0xFF52AD: Serial.println(" #");    break;
    case 0xFFFFFFFF: Serial.println(" REPEAT"); break;

    default:
      Serial.println(" other button - " + results.value );
      // Serial.println(" other button   ");

  }// End Case

  atEase();
  delay(500); // Do not get immediate repeat


} //END translateIR

void atEase() {
  Serial.println(" At ease occurred" );
  // Reset servos / AKA: At ease dude.
  for (int i = 0; i < N_SERVOS; i++) {
    if ( i == 4 ) {
      // Serial.println( "setting servo[" + i + "] to 0" );
      servo[i].SetPosition(0);
    }
    else if ( i == 5 ) {
      // Serial.println( "setting servo[" + i + "] to 180" );
      servo[i].SetPosition(180);
    }
    else {
      //  Serial.println( "setting servo[" + i + "] to 90" );
      servo[i].SetPosition(90);
    }
  }
}


void dance() {
  primera_parte();
  segunda_parte();
  moonWalkLeft(4, t * 2);
  moonWalkRight(4, t * 2);
  moonWalkLeft(4, t * 2);
  moonWalkRight(4, t * 2);
  primera_parte();
  crusaito(4, t * 8);
  crusaito(4, t * 7);

  for (int i = 0; i < 16; i++) {
    flapping(1, t / 4);
    delay(3 * t / 4);
  }

  moonWalkRight(4, t * 2);
  moonWalkLeft(4, t * 2);
  moonWalkRight(4, t * 2);
  moonWalkLeft(4, t * 2);

  drunk(t * 4);
  drunk(t * 4);
  drunk(t * 4);
  drunk(t * 4);
  kickLeft(t);
  kickRight(t);
  drunk(t * 8);
  drunk(t * 4);
  drunk(t / 2);
  delay(t * 4);

  drunk(t / 2);

  delay(t * 4);
  walk(4, t * 2);
  backyard(4, t * 2);
  goingUp(t * 2);
  goingUp(t * 1);
  noGravity(t * 2);
  crusaito(1, t * 2);
  crusaito(1, t * 8);
  crusaito(1, t * 2);
  crusaito(1, t * 8);
  crusaito(1, t * 2);
  crusaito(1, t * 3);
  //
  delay(t);
  primera_parte();
  for (int i = 0; i < 32; i++) {
    flapping(1, t / 2);
    delay(t / 2);
  }

  // Reset servos / AKA: At ease dude.
  for (int i = 0; i < N_SERVOS; i++) {
    if ( i > 4 ) {
      servo[i].SetPosition(0);
    }
    else {
      servo[i].SetPosition(90);
    }
  }

}




////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////FUNCIONES DE CONTROL//////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void oscillate(int A[N_SERVOS], int O[N_SERVOS], int T, double phase_diff[N_SERVOS]) {
  for (int i = 0; i < N_SERVOS; i++) {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref = millis();
  for (double x = ref; x < T + ref; x = millis()) {
    for (int i = 0; i < N_SERVOS; i++) {
      servo[i].refresh();
    }
  }
}

unsigned long final_time;
unsigned long interval_time;
int oneTime;
int iteration;
float increment[N_SERVOS];
int oldPosition[] = {90, 90, 90, 90};

void moveNServos(int time, int  newPosition[]) {
  for (int i = 0; i < N_SERVOS; i++)  increment[i] = ((newPosition[i]) - oldPosition[i]) / (time / INTERVALTIME);

  final_time =  millis() + time;

  iteration = 1;
  while (millis() < final_time) { //Javi del futuro cambia esto
    interval_time = millis() + INTERVALTIME;

    oneTime = 0;
    while (millis() < interval_time) {
      if (oneTime < 1) {
        for (int i = 0; i < N_SERVOS; i++) {
          servo[i].SetPosition(oldPosition[i] + (iteration * increment[i]));
        }
        iteration++;
        oneTime++;
      }
    }
  }

  for (int i = 0; i < N_SERVOS; i++) {
    oldPosition[i] = newPosition[i];
  }
}


//////////////////////////////////////////////////////////////////////////////
////////////////////////////////PASOS DE BAILE////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void goingUp(int tempo) {

  pause = millis();
  for (int i = 0; i < 6; i++) servo[i].SetPosition(90);
  delay(tempo);
  servo[0].SetPosition(80);
  servo[1].SetPosition(100);
  delay(tempo);
  servo[0].SetPosition(70);
  servo[1].SetPosition(110);
  delay(tempo);
  servo[0].SetPosition(60);
  servo[1].SetPosition(120);
  delay(tempo);
  servo[0].SetPosition(50);
  servo[1].SetPosition(130);
  delay(tempo);
  servo[0].SetPosition(40);
  servo[1].SetPosition(140);
  delay(tempo);
  servo[0].SetPosition(30);
  servo[1].SetPosition(150);
  delay(tempo);
  servo[0].SetPosition(20);
  servo[1].SetPosition(160);
  delay(tempo);

  while (millis() < pause + 8 * t);

}

void primera_parte() {

  int move1[4] = {60, 120, 90, 90};
  int move2[4] = {90, 90, 90, 90};
  int move3[4] = {40, 140, 90, 90};

  for (int x = 0; x < 3; x++) {
    for (int i = 0; i < 3; i++) {
      lateral_fuerte(1, t / 2);
      lateral_fuerte(0, t / 4);
      lateral_fuerte(1, t / 4);
      delay(t);
    }

    pause = millis();
    for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
    moveNServos(t * 0.4, move1);
    moveNServos(t * 0.4, move2);
    while (millis() < (pause + t * 2));
  }

  for (int i = 0; i < 2; i++) {
    lateral_fuerte(1, t / 2);
    lateral_fuerte(0, t / 4);
    lateral_fuerte(1, t / 4);
    delay(t);
  }

  pause = millis();
  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  crusaito(1, t * 1.4);
  moveNServos(t * 1, move3);
  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  while (millis() < (pause + t * 4));
}

void segunda_parte() {

  int move1[4] = {90, 90, 80, 100};
  int move2[4] = {90, 90, 100, 80};
  int move3[4] = {90, 90, 80, 100};
  int move4[4] = {90, 90, 100, 80};

  int move5[4] = {40, 140, 80, 100};
  int move6[4] = {40, 140, 100, 80};
  int move7[4] = {90, 90, 80, 100};
  int move8[4] = {90, 90, 100, 80};

  int move9[4] = {40, 140, 80, 100};
  int move10[4] = {40, 140, 100, 80};
  int move11[4] = {90, 90, 80, 100};
  int move12[4] = {90, 90, 100, 80};

  for (int x = 0; x < 7; x++) {
    for (int i = 0; i < 3; i++) {
      pause = millis();
      moveNServos(t * 0.15, move1);
      moveNServos(t * 0.15, move2);
      moveNServos(t * 0.15, move3);
      moveNServos(t * 0.15, move4);
      while (millis() < (pause + t));
    }
    pause = millis();
    moveNServos(t * 0.15, move5);
    moveNServos(t * 0.15, move6);
    moveNServos(t * 0.15, move7);
    moveNServos(t * 0.15, move8);
    while (millis() < (pause + t));
  }

  for (int i = 0; i < 3; i++) {
    pause = millis();
    moveNServos(t * 0.15, move9);
    moveNServos(t * 0.15, move10);
    moveNServos(t * 0.15, move11);
    moveNServos(t * 0.15, move12);
    while (millis() < (pause + t));
  }
}

void lateral_fuerte(boolean side, int tempo) {

  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  if (side) servo[0].SetPosition(40);
  else servo[1].SetPosition(140);
  delay(tempo / 2);
  servo[0].SetPosition(90);
  servo[1].SetPosition(90);
  delay(tempo / 2);

}

void drunk (int tempo) {

  pause = millis();

  int move1[] = {60, 70, 90, 90};
  int move2[] = {110, 120, 90, 90};
  int move3[] = {60, 70, 90, 90};
  int move4[] = {110, 120, 90, 90};

  moveNServos(tempo * 0.235, move1);
  moveNServos(tempo * 0.235, move2);
  moveNServos(tempo * 0.235, move3);
  moveNServos(tempo * 0.235, move4);
  while (millis() < (pause + tempo));

}


void noGravity(int tempo) {

  int move1[4] = {120, 140, 90, 90};
  int move2[4] = {140, 140, 90, 90};
  int move3[4] = {120, 140, 90, 90};
  int move4[4] = {90, 90, 90, 90};


  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  for (int i = 0; i < N_SERVOS; i++) oldPosition[i] = 90;
  moveNServos(tempo * 2, move1);
  moveNServos(tempo * 2, move2);
  delay(tempo * 2);
  moveNServos(tempo * 2, move3);
  moveNServos(tempo * 2, move4);



}

void kickLeft(int tempo) {
  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  delay(tempo);
  servo[0].SetPosition(50); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo);
  servo[0].SetPosition(80); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(30); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(80); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(30); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(80); //pie derecho
  servo[1].SetPosition(70); //pie izquiero
  delay(tempo);
}

void kickRight(int tempo) {
  for (int i = 0; i < N_SERVOS; i++) servo[i].SetPosition(90);
  delay(tempo);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(130); //pie izquiero
  delay(tempo);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(100); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(150); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(80); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(150); //pie izquiero
  delay(tempo / 4);
  servo[0].SetPosition(110); //pie derecho
  servo[1].SetPosition(100); //pie izquiero
  delay(tempo);
}

void walk(int steps, int T) {
  int A[4] = {15, 15, 30, 30};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void run(int steps, int T) {
  int A[4] = {10, 10, 10, 10};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void backyard(int steps, int T) {
  int A[4] = {15, 15, 30, 30};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void backyardSlow(int steps, int T) {
  int A[4] = {15, 15, 30, 30};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(-90), DEG2RAD(-90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}


void turnLeft(int steps, int T) {
  int A[4] = {20, 20, 10, 30};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void turnRight(int steps, int T) {
  int A[4] = {20, 20, 30, 10};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void moonWalkRight(int steps, int T) {
  int A[6] = {25, 25, 0, 0, 25, 15};
  int O[6] = { -15 , 15, 0, 0, -15, 15};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 + 120), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void moonWalkLeft(int steps, int T) {
  int A[6] = {25, 25, 0, 0, 25, 25};
  int O[6] = { -15, 15, 0, 0, -15, 15};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 - 120), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void crusaito(int steps, int T) {
  int A[4] = {25, 25, 30, 30};
  int O[4] = { - 15, 15, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180 + 120), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void swing(int steps, int T) {
  int A[4] = {25, 25, 0, 0};
  int O[4] = { -15, 15, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(0), DEG2RAD(90), DEG2RAD(90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void upDown(int steps, int T) {
  int A[4] = {25, 25, 0, 0};
  int O[4] = { -15, 15, 0, 0};
  double phase_diff[4] = {DEG2RAD(180), DEG2RAD(0), DEG2RAD(270), DEG2RAD(270)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void flapping(int steps, int T) {
  int A[4] = {15, 15, 8, 8};
  int O[4] = { -A[0], A[1], 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(90), DEG2RAD(-90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}

void test(int steps, int T) {
  int A[4] = {15, 15, 8, 8};
  int O[4] = { -A[0] + 10, A[1] - 10, 0, 0};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(90), DEG2RAD(-90)};

  for (int i = 0; i < steps; i++)oscillate(A, O, T, phase_diff);
}
