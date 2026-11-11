#include <avr/io.h>
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// MCP23008 com endereco padrao 0x20
Adafruit_LiquidCrystal lcd(0);

unsigned int tempoRestante, tempoTotal = 10*60; // 10 minutos em segundos
unsigned int segundos = 0;
uint8_t fase = 0;
uint8_t numeroA, numeroB = 0; //dois operandos do desafio da conta
uint8_t resultado = 0;

const char teclas[4][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

void setup() {
  Serial.begin(9600);
  Wire.begin();

  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  
  // D2 a D5: linhas como saidas
  DDRD |= 0b00111100;

  // D6 e D7: colunas como entradas
  DDRD &= ~0b11000000;

  // D8 e D9: colunas como entradas
  DDRB &= ~0b00000011;

  // Pull-ups nas colunas
  PORTD |= 0b11000000;
  PORTB |= 0b00000011;

  // Linhas inicialmente em HIGH
  PORTD |= 0b00111100;
  
  tempoRestante = tempoTotal;
  randomSeed(analogRead(1)); //deixar um pino vazio
}

void loop() {
  delay(1000);
  tempoRestante--;

  lcd.setCursor(11, 0);
  lcd.print(tempoRestante);
  lcd.print(" s");
  gerarConta();
}

void gerarConta() {
 	numeroA=random(100);
  	numeroB=random(100);
  	resultado = numeroA+numeroB;
  	Serial.print(numeroA);
    Serial.print(" + ");
    Serial.println(numeroB);

    lcd.setCursor(0, 1);
    lcd.print("                "); // Limpa a linha
    lcd.setCursor(0, 1);
    lcd.print(numeroA);
    lcd.print(" + ");
    lcd.print(numeroB);
  	lcd.print("? ");
  	lcd.setCursor(0, 0);
  	lcd.print("Calcule:");
}

void checarConta