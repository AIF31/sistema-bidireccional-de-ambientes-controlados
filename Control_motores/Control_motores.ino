#include <AFMotor.h>
#include <math.h>
#define Umax 508 
#define Umin 0 

#define T 0.1 // sampling time 
  
const double p1 = 0.000006;
const double p2 = -0.0032;
const double p3 = 0.5762;
const double p4 = 0;

AF_DCMotor motor1(1);

//Pines ultrasonico
const int Trigger = 2;   //Pin digital 2 para el Trigger del sensor (Input pin)
const int Echo = 3;   //Pin digital 3 para el Echo del sensor (Output pin)

double valorUltrasonico = 0;
double valorUltrasonico_anterior = 0;
double velocidadMotor=0;

double setpoint1 = 0.275;
double setpoint2 = 0.05;
double error;
double P, I, D, U;
double I_prec=0, U_prec=0, D_prec=0; 
boolean Saturado = false;

double Kp = 1133; 
double Ki = 50; 
double Kd = 20; 

int bandera_pos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
}


void loop() {
//Ultrasonico
double t; //tiempo que demora en llegar el eco
double d; //distancia en centimetros
digitalWrite(Trigger, HIGH);
delayMicroseconds(10);          //Enviamos un pulso de 10us
digitalWrite(Trigger, LOW);

t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
d = (t/59)/100;             //escalamos el tiempo a una distancia en cm
valorUltrasonico = d;

if(bandera_pos == 0){
  error = valorUltrasonico - setpoint1;
  if (error <= 0 ){
    motor1.run(RELEASE);
    bandera_pos = 1;
    error = valorUltrasonico-setpoint2;
    delay(6000);
  }
}else if(bandera_pos == 1){
  error = valorUltrasonico - setpoint2;
  if (error <= 0 ){
    motor1.run(RELEASE);
    bandera_pos = 2;
    //error = valorUltrasonico-setpoint2;
    delay(6000);
  }
}else if(bandera_pos == 2){
  motor1.setSpeed(200);
  motor1.run(BACKWARD);
  if(valorUltrasonico >= 0.48){
    motor1.run(RELEASE);
    bandera_pos = 0;
    delay(5000);
  }
}

delay(3);

P = Kp*error;

if ( !Saturado ){

I = I_prec + T*Ki*error;

D = (Kd/T)*(valorUltrasonico - valorUltrasonico_anterior);

U = P + I + D; }
else {
  motor1.run(RELEASE);
  }

if ( U < Umin || )  {
  U=Umin; 
  Saturado = true;
  motor1.setSpeed(100);
  motor1.run(FORWARD);
  delay(200);
}
  
else if ( U > Umax || valorUltrasonico >= 0.5) {
  U=Umax; 
  Saturado = true;
  motor1.setSpeed(100);
  motor1.run(BACKWARD);
  delay(200);
}

else  Saturado = false; 


velocidadMotor = p1*pow(U,3) + p2*pow(U,2) + p3*U + p4;

motor1.setSpeed(velocidadMotor);
motor1.run(FORWARD);

I_prec = I;
valorUltrasonico_anterior = valorUltrasonico;
D_prec = D;
}
