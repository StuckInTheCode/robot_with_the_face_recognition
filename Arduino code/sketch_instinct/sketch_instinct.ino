#include <AFMotor.h> // Подключаем библиотеку для управления двигателями
#include <Servo.h>   // Подключаем библиотеку для сервоприводом

Servo myservoHorizontal;  //поворот влево/вправо на шилде внутренние пины
Servo myservoVertical;  //поворот вверх/вниз на шилде наружние пины

AF_DCMotor motor1(1); // подключаем мотор к клеммникам M1
AF_DCMotor motor2(2); // подключаем мотор к клеммникам M1
AF_DCMotor motor3(3); // подключаем мотор к клеммникам M1
AF_DCMotor motor4(4); // подключаем мотор к клеммникам M1
////////////////////////////////поменяй тут на пины, к которым реально подключено///////////////////////////////////////
const int ikCenter = A0; // пин центрального ик-датчика
const int ikLeft = A1; // пин центрального ик-датчика
const int ikRight = A2; // пин центрального ик-датчика
const int ikBack = A3; // пин центрального ик-датчика

    
const int trigPin = A4; //пины для уз
const int echoPin = A5;

int vCenter;
int vLeft;
int vRight;
int vBack;
long distance;

String command;
String dist;
int speed = 100;

void setup() {
  myservoHorizontal.attach(9);
  myservoHorizontal.write(0); 
  myservoVertical.attach(10);
  myservoVertical.write(0); 
  // put your setup code here, to run once:

  motor1.run(RELEASE);  // останавливаем мотор
  motor1.setSpeed(speed); // задаем максимальную скорость мотора
  motor2.run(RELEASE);  // останавливаем мотор
  motor2.setSpeed(speed); // задаем максимальную скорость мотора
  motor3.run(RELEASE);  // останавливаем мотор
  motor3.setSpeed(speed); // задаем максимальную скорость мотора
  motor4.run(RELEASE);  // останавливаем мотор
  motor4.setSpeed(speed); // задаем максимальную скорость мотора  

  pinMode(trigPin, OUTPUT); // триггер - выходной пин
  pinMode(echoPin, INPUT); // эхо - входной
  
  Serial.begin(9600);
}

void loop() {
  while(Serial.available()>0 ){
     command = Serial.readString();
     dist = command.substring(1);
  }

  switch(command[0]){
      case 'v' : servoVertical(dist.toInt()); break;
      case 'h' : servoHorizontal(dist.toInt()); break;
      case 's' : stop(); break;
      case 'f' : forward(); break;
      case 'b' : back(); break;
      case 'l' : goLeft(dist.toInt()); break;
      case 'r' : goRight(dist.toInt()); break;
  }
  command = "/0";
  vCenter = digitalRead( ikCenter );
  vLeft = digitalRead( ikLeft );
  vRight = digitalRead( ikRight );;
  vBack = digitalRead( ikBack );;
  if( vCenter == LOW ) { 
     stop();
     goBack(500);
   }
  if( vCenter == HIGH && vLeft == LOW ) { 
     stop();
     goBack(500);
     goRight(200);
     forward();
   }
   if( vCenter == HIGH && vRight == LOW ) { 
     stop();
     goBack(500);
     goLeft(200);
     forward();
   }  
  if(vBack == LOW ) { 
     stop();
     forward();
   }  

    distance = getDistance(); // получаем дистанцию с датчика
}

void servoVertical(int deg){
  myservoVertical.write(deg);
}

void servoHorizontal(int deg){
  myservoHorizontal.write(deg);
}

//ехать вперед в течение времени
void goForward(int time){
  motor1.run(FORWARD);  // задаем движение вперед
  motor2.run(FORWARD);  // задаем движение вперед
  motor3.run(FORWARD);  // задаем движение вперед
  motor4.run(FORWARD);  // задаем движение вперед
  
  motor1.setSpeed(speed); // задаем скорость движения
  motor2.setSpeed(speed); // задаем скорость движения
  motor3.setSpeed(speed); // задаем скорость движения
  motor4.setSpeed(speed); // задаем скорость движения

  delay(time);

  stop();
}

//остановка
void stop(){
  motor1.run(RELEASE);  // останавливаем мотор
  motor2.run(RELEASE);  // останавливаем мотор
  motor3.run(RELEASE);  // останавливаем мотор
  motor4.run(RELEASE);  // останавливаем мотор
  delay(1000);

}

//ехать назад в течение времени
void goBack(long time){
  motor1.run(BACKWARD);  // задаем движение назад
  motor2.run(BACKWARD);  // задаем движение назад
  motor3.run(BACKWARD);  // задаем движение назад
  motor4.run(BACKWARD);  // задаем движение назад
  
  motor1.setSpeed(speed); // задаем скорость движения
  motor2.setSpeed(speed); // задаем скорость движения
  motor3.setSpeed(speed); // задаем скорость движения
  motor4.setSpeed(speed); // задаем скорость движения

  delay(time);

  stop();
}

//поворот направо в течение времени
void goRight(long time){
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);

  motor1.setSpeed(speed); // задаем скорость движения
  motor2.setSpeed(speed); // задаем скорость движения
  motor3.setSpeed(speed); // задаем скорость движения
  motor4.setSpeed(speed); // задаем скорость движения
  delay(time);

  stop();
}

//поворот налево в течение времени
void goLeft(long time){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);

  motor1.setSpeed(speed); // задаем скорость движения
  motor2.setSpeed(speed); // задаем скорость движения
  motor3.setSpeed(speed); // задаем скорость движения
  motor4.setSpeed(speed); // задаем скорость движения
  delay(time);

  stop();
}

//ехать вперед
void forward(){
  motor1.run(FORWARD);  // задаем движение вперед
  motor2.run(FORWARD);  // задаем движение вперед
  motor3.run(FORWARD);  // задаем движение вперед
  motor4.run(FORWARD);  // задаем движение вперед
  
  motor1.setSpeed(speed); // задаем скорость движения
  motor2.setSpeed(speed); // задаем скорость движения
  motor3.setSpeed(speed); // задаем скорость движения
  motor4.setSpeed(speed); // задаем скорость движения
}

//ехать назад
void back(){
  motor1.run(BACKWARD);  // задаем движение назад
  motor2.run(BACKWARD);  // задаем движение назад
  motor3.run(BACKWARD);  // задаем движение назад
  motor4.run(BACKWARD);  // задаем движение назад
  
  motor1.setSpeed(speed); // задаем скорость движения
  motor2.setSpeed(speed); // задаем скорость движения
  motor3.setSpeed(speed); // задаем скорость движения
  motor4.setSpeed(speed); // задаем скорость движения
}
//функции для ультразвукового датчика
// Определение дистанции до объекта в см
long getDistance() {
  long distacne_cm = getEchoTiming() * 1.7 * 0.01;
  return distacne_cm;
}

// Определение времени задержки
long getEchoTiming() {
  digitalWrite(trigPin, HIGH); // генерируем 10 мкс импульс запуска
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);
  // определение на пине echoPin длительности уровня HIGH, мкс:
  long duration = pulseIn(echoPin, HIGH); 
  return duration;
}
