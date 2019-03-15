const int trigPin = A2;
const int echoPin = A1;

#include <AFMotor.h> // Подключаем библиотеку для управления двигателями
#include <Servo.h>   // Подключаем библиотеку для сервоприводом
Servo myservo;


void setup()  {
  myservo.attach(9);
  delay(1000);
  myservo.write(0); 
  pinMode(trigPin, OUTPUT); // триггер - выходной пин
  pinMode(echoPin, INPUT); // эхо - входной
  digitalWrite(trigPin, LOW);
  Serial.begin(9600); // инициализация послед. порта
}

void loop()  { 
   
  myservo.write(180); // Поворот сервы на угол 90 градусов
 delay(2000);
  long distance = getDistance(); // получаем дистанцию с датчика
  Serial.println(distance); // выводим в последовательный порт
  delay(100);
 
  myservo.write(0); // Поворот сервы на угол 90 градусов
 delay(2000);
} 

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

