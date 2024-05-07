#include <Car_Library.h>

int analogPin = A0; //가변저항 output pin 

int motorH1 = 11;
int motorH2 = 10;
int motorL1 = 8;
int motorL2 = 9;
int motorR1 = 13;
int motorR2 = 12;

int red_lock;

char message = '0';

int direc=5; // 받아온 방향 
int direction_state;
int reg=0;

int fL_trig = 3;
int fL_echo = 2;

int fR_trig = 5;
int fR_echo = 4;

int sR_trig = 7;
int sR_echo = 6;

long distance_front;
long distance_fL;
long distance_fR;
long distance_side;

int mode = 7;
int cnt = 0;
int start = 0;
int cnting = 0;
unsigned long startTime; // 시작 시간을 저장하는 변수
unsigned long currentTime; // 현재 시간을 저장하는 변수
const unsigned long delayTime = 25000; // 10초를 밀리초로 표현


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  red_lock = 1;
  pinMode(motorH1, OUTPUT);
  pinMode(motorH2, OUTPUT);
  pinMode(motorL1, OUTPUT);
  pinMode(motorL2, OUTPUT);
  pinMode(motorR1, OUTPUT);
  pinMode(motorR2, OUTPUT);

  pinMode(fL_trig, OUTPUT);
  pinMode(fL_echo, INPUT);
  pinMode(fR_trig, OUTPUT);
  pinMode(fR_echo, INPUT);
  pinMode(sR_trig, OUTPUT);
  pinMode(sR_echo, INPUT);
}

void mescheck(char message){
  if (message == 'a') direc = 1; 
  else if (message == 'b') direc = 3;
  else if (message == 'c') direc = 5;
  else if (message == 'd') direc = 7;
  else if (message == 'e') direc = 9;
  else if (message == 'f') direc = 11;
  else if (message == 'g') direc = 13;
  else if (message == 'h') direc = 15;
  else if (message == 'i') direc = 17; 
  else if (message == 'j') red_lock = 0;
  else if (message == 'k') red_lock = 1;
  else if (message == '1')
  {
    start = 0;
    startTime = millis(); 
  }
  else if (message == '2')
  {
    start = 1; 
    startTime = millis();
  }
  else direc = 0;
}

//void mescheck(char message){
//  if (message == 'z') {
//    direc = 1; //flag = 1;
//  }
//  else if (message == 'a') direc = 3;
//  else if (message == 'w') direc = 5;
//  else if (message == 'd') direc = 7;
//  else if (message == 'c') {
//    direc = 9; //flag = 1;
//  }
//  else if (message == 'r') {
//    red_lock = 0;
//    // direc = 0 ;
//  }
//  else if (message == 'g') {
//    red_lock =1;
//    // direc = 0;
//  }
//  else direc = 0;
//}
 
void check_direc(int a){
  if (a > 161) direction_state = 1; 
  else if (a > 155 and a <= 161) direction_state = 2;
  else if (a > 153 and a <= 155) direction_state = 3;
  else if (a > 151 and a <= 153) direction_state = 4;
  else if (a > 149 and a <= 151) direction_state = 5;
  else if (a > 147 and a <= 149) direction_state = 6;
  else if (a > 145 and a <= 147) direction_state = 7;
  else if (a > 143 and a <= 145) direction_state = 8;
  else if (a > 141 and a <= 143) direction_state = 9; //middle
  else if (a > 139 and a <= 141) direction_state = 10;
  else if (a > 137 and a <= 139) direction_state = 11; 
  else if (a > 135 and a <= 137) direction_state = 12;
  else if (a > 133 and a <= 135) direction_state = 13;
  else if (a > 131 and a <= 133) direction_state = 14;
  else if (a > 129 and a <= 131) direction_state = 15;
  else if (a > 127 and a <= 129) direction_state = 16;
  else if (a <= 127) direction_state = 17;
}

void check_mode(){
  
  distance_fL = ultrasonic_distance(fL_trig, fL_echo);
  distance_fR = ultrasonic_distance(fR_trig, fR_echo);

  // 초음파 값 잘 받고 있는지 확인 
  
  // Serial.print(distance_fL);
  // Serial.print("mm\n");
  // Serial.print(distance_fR);
  // Serial.print("mm\n");
  
  
  // 왼쪽 오른쪽 값 비교해서 오차가 적을 때 distance_front을 유효한 값으로 읽음 
  if (distance_fL != 0 and distance_fR != 0)
  {
    if ((distance_fL - 70 <= distance_fR) and (distance_fL + 70) >= distance_fR) {
      distance_front = (distance_fL + distance_fR)/2;  
    }
    else if ((distance_fR - 70 <= distance_fL) and (distance_fR + 70) >= distance_fL){
      distance_front = (distance_fL + distance_fR)/2;  
    }
    else distance_front = 2000;
  }
  else distance_front = 2000;
  
  // distance_side = ultrasonic_distance(sR_trig, sR_echo);

  // 초음파 값 잘 받고 있는지 확인 
  
  // Serial.print(distance_front);
  // Serial.print("mm\n");

  // Serial.print("mode: ");
  // Serial.print(mode);
  // Serial.print("\n");
  
}

void serialEvent(){
  while(Serial.available()) {
    if (mode == 0)
    {
      message = Serial.read();
    }
    else if (mode == 2) {
      message = Serial.read();
    }
    else if (mode == 4){
      message = Serial.read();
    }
    else if (mode == 5){
      message = Serial.read();
    }
    else if (mode == 7){
      message = Serial.read();
    }
  }
}

//////////////////////////////////////////////////////
/*                                                  */
/*                    main loop                     */
/*                                                  */
//////////////////////////////////////////////////////

void loop() {

  //Serial.print("mode: ");
  //Serial.print(mode);
  //Serial.print("\n");
  //check_mode();

//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 7                     */
/*                                                  */
//////////////////////////////////////////////////////


  if (mode == 7) 
  {
    currentTime = millis();
    mescheck(message);
    reg = potentiometer_Read(analogPin);
    check_direc(reg);

    motor_forward(motorL1, motorL2, 70*start);
    motor_forward(motorR1, motorR2, 70*start); 
    
    if (direc > 0)
    {
      if (direc != direction_state) 
      { 
        if (direc < direction_state)
        {
          motor_forward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
        else if (direc > direction_state)
        {
          motor_backward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
      }
      else 
      {
        motor_hold(motorH1, motorH2);
        reg = potentiometer_Read(analogPin);
        check_direc(reg);
      }
    }
    if (currentTime - startTime >= delayTime) mode = 0;
  }

//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 0                     */
/*                                                  */
//////////////////////////////////////////////////////


  else if (mode == 0) 
  {
    
    mescheck(message);
    reg = potentiometer_Read(analogPin);
    check_direc(reg);

    motor_forward(motorL1, motorL2, 50*start);
    motor_forward(motorR1, motorR2, 50*start); 
    
    if (direc > 0)
    {
      if (direc != direction_state) 
      { 
        if (direc < direction_state)
        {
          motor_forward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
        else if (direc > direction_state)
        {
          motor_backward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
      }
      else 
      {
        motor_hold(motorH1, motorH2);
        reg = potentiometer_Read(analogPin);
        check_direc(reg);
      }
    }
    check_mode();
    if (distance_front != 2000) {
      if (distance_front < 1200) mode = 1;
    }
    else mode = 0;
  }



//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 1                     */
/*                                                  */
//////////////////////////////////////////////////////


  // 왼쪽으로 핸들 꺽고 직진  
  else if (mode == 1)
  {
    // 멈추고
    motor_hold(motorL1,motorL2);
    motor_hold(motorR1,motorR2);
    delay(300);

    // 핸들 돌리기 
    motor_forward(motorH1,motorH2, 150);
    delay(1000);
    motor_hold(motorH1, motorH2);
    delay(100);
    
    // 직진 
    motor_forward(motorL1,motorL2,40);
    motor_forward(motorR1,motorR2,40);
    delay(8000);
    
    mode = 2;
   }

//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 2                     */
/*                                                  */
//////////////////////////////////////////////////////


  else if (mode == 2) 
  {
    motor_forward(motorL1, motorL2, 50);
    motor_forward(motorR1, motorR2, 50);

    mescheck(message);
    reg = potentiometer_Read(analogPin);
    check_direc(reg);
    
    if (direc > 0)
    {
      if (direc != direction_state) 
      { 
        if (direc < direction_state)
        {
          motor_forward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
        else if (direc > direction_state)
        {
          motor_backward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
      }
      else 
      {
        motor_hold(motorH1, motorH2);
        reg = potentiometer_Read(analogPin);
        check_direc(reg);
      }
    }
    /*
    distance_side = ultrasonic_distance(sR_trig, sR_echo);
    if (distance_side >= 900) mode = 3;
    else mode = 2; 
    */
  
    check_mode();
    if (distance_front != 2000) {
      if (distance_front < 1300) mode = 3;
    }
    else mode = 2;
  }


//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 3                     */
/*                                                  */
//////////////////////////////////////////////////////


  // 오른쪽으로 핸들 꺽고 직진  
  else if (mode == 3)
  {
    // 속도 줄이기 
    motor_hold(motorL1,motorL2);
    motor_hold(motorR1,motorR2);
    delay(300);

    // 핸들 돌리기 
    motor_backward(motorH1,motorH2, 200);
    delay(2000);
    motor_hold(motorH1, motorH2);
    delay(100);
    // 직진 
    motor_forward(motorL1,motorL2,30);
    motor_forward(motorR1,motorR2,30);
    delay(17000);
    
    mode = 4;
   }


//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 4                     */
/*                                                  */
//////////////////////////////////////////////////////


  else if (mode == 4 and cnt < 100) 
  {
    motor_forward(motorL1, motorL2, 45);
    motor_forward(motorR1, motorR2, 45);

    mescheck(message);
    reg = potentiometer_Read(analogPin);
    check_direc(reg);
    
    if (direc > 0)
    {
      if (direc != direction_state) 
      { 
        if (direc < direction_state)
        {
          motor_forward(motorH1, motorH2, 150);
          delay(200);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
        else if (direc > direction_state)
        {
          motor_backward(motorH1, motorH2, 150);
          delay(200);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
      }
      else 
      {
        motor_hold(motorH1, motorH2);
        reg = potentiometer_Read(analogPin);
        check_direc(reg);
      }
    }
    cnt ++;
  }

  else if (mode == 4 and cnt == 100) {
    mode = 5;
    red_lock = 1;
  }

//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 5                     */
/*                                                  */
//////////////////////////////////////////////////////


  else if (mode == 5) 
  {
    motor_forward(motorL1, motorL2, 70*red_lock*start);
    motor_forward(motorR1, motorR2, 70*red_lock*start);

    mescheck(message);
    reg = potentiometer_Read(analogPin);
    check_direc(reg);
    
    if (direc > 0)
    {
      if (direc != direction_state) 
      { 
        if (direc < direction_state)
        {
          motor_forward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
        else if (direc > direction_state)
        {
          motor_backward(motorH1, motorH2, 150);
          delay(100);
          motor_hold(motorH1, motorH2);
          reg = potentiometer_Read(analogPin);
          check_direc(reg);
        }
      }
      else 
      {
        motor_hold(motorH1, motorH2);
        reg = potentiometer_Read(analogPin);
        check_direc(reg);
      }
    }
  }

//////////////////////////////////////////////////////
/*                                                  */
/*                       mode 6                     */
/*                                                  */
//////////////////////////////////////////////////////

  else if (mode == 6)
  {
    motor_hold(motorL1, motorL2);
    motor_hold(motorR1, motorR2);
  }

}
