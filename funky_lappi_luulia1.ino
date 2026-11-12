// C++ code
//
void setup()
{
 DDRB = 0b00000000; //Aqui defini os pinos de AREF até 8 como 0
 PORTB = 0b00111100; //Aqui defini os regitradores internos de 13 a 10 como pull up
 DDRD = 0b00000000;
 PORTD = 0b00000100; 
 EICRA = 0b00000010; // versão boas praticasEICRA |= (1 << ISC01); // Configura o bit ISC01 como 1EICRA &= ~(1 << ISC00);
}//indentifica em borda de descida

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Wait for 1000 millisecond(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}