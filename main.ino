#include <IRremote.h>

//Atribuicao dos estados possiveis do alarme
enum AlarmeState {S_DESARMADO = 1, S_ARMADO, S_ALARME};
AlarmeState estado = S_DESARMADO;

//Pino do receptor IR e buzzer
const int RECV_PIN = 10;
const int pinBuzzer = 11;

//Pinagem dos LEDs e do sensor
const int pinSensor = 3;
const int ledVerde = 4;
const int ledAmarelo = 5;
const int ledVermelho = 6;

float armazenavalor;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  // Configuracao dos pinos
  pinMode(pinSensor, INPUT);
  pinMode(RECV_PIN, INPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  
  Serial.begin(9600);
  irrecv.enableIRIn(); //Inicializa o receptor IR
}

void loop() {
  //Printa os valores lidos para referencia
  if (irrecv.decode(&results)) {
    Serial.print("Valor lido: ");
    Serial.println(results.value, HEX);
    armazenavalor = (results.value);
  }
  switch(estado){
    case S_DESARMADO:
      if(results.value == 0xAF548B7 || results.value == 0xFF10EF){ //Se botao 7 for pressionado, muda o estado para armado
        estado = S_ARMADO;
      }
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, LOW);
    irrecv.resume(); //Le o próximo valor
    break;
    
    case S_ARMADO:
      if(digitalRead(pinSensor)){
        estado = S_ALARME;
      }
      if(results.value == 0xAF5A857 || results.value == 0xFF18E7){ //Se botao 5 for pressionado, muda o estado para desarmado
        estado = S_DESARMADO;
      }
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVermelho, LOW);
    irrecv.resume();
    break;
    
    case S_ALARME:
    tone(pinBuzzer, 500, 100);
    digitalWrite(pinBuzzer, HIGH);
    if(results.value == 0xAF5A857 || results.value == 0xFF18E7){
      estado = S_DESARMADO;
    }
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVermelho, HIGH);
    irrecv.resume();
    break;
  }
  //irrecv.resume(); //Le o próximo valor
}
