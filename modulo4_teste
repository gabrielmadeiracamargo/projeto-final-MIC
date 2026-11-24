
#include <Adafruit_LiquidCrystal.h>
#include <avr/interrupt.h>

// ==========================================
// CONFIGURACOES
// ==========================================

const unsigned int TEMPO_TOTAL = 10 * 60;
const uint8_t TOTAL_CONTAS_FASE1 = 5;

// ==========================================
// VARIAVEIS
// ==========================================

Adafruit_LiquidCrystal lcd(0);

unsigned int tempoRestante = TEMPO_TOTAL;

uint8_t fase = 1;
uint8_t contasCorretas = 0;

uint8_t numeroA = 0;
uint8_t numeroB = 0;
uint16_t resultado = 0;

uint16_t resposta = 0;
bool digitando = false;
bool jogoEncerrado = false;

volatile uint8_t segundosPendentes = 0;

// ==========================================
// TIMER1 - CONTAGEM DO TEMPO
// ==========================================

ISR(TIMER1_COMPA_vect) {
  if (segundosPendentes < 255) {
    segundosPendentes++;
  }
}

void iniciarTimer() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 15624;

  // CTC, prescaler 1024
  TCCR1B = (1 << WGM12) |
           (1 << CS12) | (1 << CS10);

  TIMSK1 |= (1 << OCIE1A);
}

// ==========================================
// TIMER2 - BUZZER NO D8
// ==========================================

ISR(TIMER2_COMPA_vect) {
  PINB = (1 << PB0);
}

void tocarNota(uint16_t frequencia,
               unsigned int duracao) {

  if (frequencia == 0) {
    delay(duracao);
    return;
  }

  uint16_t comparacao =
      (F_CPU / (2UL * 128UL * frequencia)) - 1;

  uint8_t estado = SREG;
  cli();

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  OCR2A = (uint8_t)comparacao;

  TCCR2A = (1 << WGM21);
  TIMSK2 |= (1 << OCIE2A);

  // Prescaler 128
  TCCR2B = (1 << CS22) | (1 << CS20);

  SREG = estado;

  delay(duracao);

  TCCR2B = 0;
  TIMSK2 &= ~(1 << OCIE2A);
  PORTB &= ~(1 << PB0);
}

void somAcerto() {
  tocarNota(523, 100);
  delay(30);
  tocarNota(659, 100);
  delay(30);
  tocarNota(784, 180);
}

void somErro() {
  tocarNota(392, 160);
  delay(50);
  tocarNota(262, 240);
}

// ==========================================
// ADC - SEMENTE ALEATORIA
// ==========================================

uint16_t lerSemente() {
  ADMUX = (1 << REFS0) | 1;

  ADCSRA = (1 << ADEN) |
           (1 << ADPS2) |
           (1 << ADPS1) |
           (1 << ADPS0);

  ADCSRA |= (1 << ADSC);

  while (ADCSRA & (1 << ADSC)) {
  }

  return ADC;
}

// ==========================================
// DISPLAY
// ==========================================

void mostrarTempo() {
  lcd.setCursor(0, 0);
  lcd.print("Tempo:          ");

  lcd.setCursor(7, 0);

  unsigned int minutos = tempoRestante / 60;
  unsigned int segundos = tempoRestante % 60;

  lcd.print(minutos);
  lcd.print(":");

  if (segundos < 10) {
    lcd.print("0");
  }

  lcd.print(segundos);
}

void mostrarConta() {
  lcd.setCursor(0, 1);
  lcd.print("                ");

  lcd.setCursor(0, 1);
  lcd.print(numeroA);
  lcd.print("+");
  lcd.print(numeroB);
  lcd.print("=?");

  lcd.setCursor(11, 1);
  lcd.print(contasCorretas);
  lcd.print("/");
  lcd.print(TOTAL_CONTAS_FASE1);
}

// ==========================================
// FASE 1 - CONTAS MATEMATICAS
// ==========================================

void gerarConta() {
  numeroA = random(100);
  numeroB = random(100);

  resultado = numeroA + numeroB;

  mostrarConta();

  Serial.println();
  Serial.print("Fase 1 - Conta ");
  Serial.print(contasCorretas + 1);
  Serial.print("/");
  Serial.println(TOTAL_CONTAS_FASE1);

  Serial.print(numeroA);
  Serial.print(" + ");
  Serial.print(numeroB);
  Serial.println(" = ?");
}

void iniciarFase1() {
  fase = 1;
  contasCorretas = 0;

  Serial.println("FASE 1: MATEMATICA");

  gerarConta();
}

void checarConta() {
  if (resposta == resultado) {
    Serial.println("Correto!");

    somAcerto();
    contasCorretas++;

    if (contasCorretas >= TOTAL_CONTAS_FASE1) {
      iniciarFase2();
    } else {
      gerarConta();
    }
  }
  else {
    Serial.println("Incorreto! -10 segundos");

    somErro();

    if (tempoRestante >= 10) {
      tempoRestante -= 10;
    } else {
      tempoRestante = 0;
    }

    mostrarTempo();

    if (tempoRestante == 0) {
      encerrarJogo();
    }
  }
}

// ==========================================
// FASE 2 - FUTURA IMPLEMENTACAO
// ==========================================

void iniciarFase2() {
  fase = 2;

  Serial.println();
  Serial.println("FASE 1 CONCLUIDA!");
  Serial.println("Iniciando fase 2...");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fase 1 completa");
  lcd.setCursor(0, 1);
  lcd.print("Proxima: Fase 2");

  // Implementar a logica da fase 2 aqui
}

// ==========================================
// TEMPO E FIM DE JOGO
// ==========================================

void encerrarJogo() {
  if (jogoEncerrado) {
    return;
  }

  jogoEncerrado = true;

  Serial.println("TEMPO ESGOTADO!");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMPO ESGOTADO");
  lcd.setCursor(0, 1);
  lcd.print("Fim de jogo!");
}

void atualizarTempo() {
  uint8_t quantidade;

  uint8_t estado = SREG;
  cli();

  quantidade = segundosPendentes;
  segundosPendentes = 0;

  SREG = estado;

  if (jogoEncerrado) {
    return;
  }

  if (quantidade >= tempoRestante) {
    tempoRestante = 0;
  } else {
    tempoRestante -= quantidade;
  }

  if (tempoRestante == 0) {
    encerrarJogo();
  } else if (fase == 1) {
    mostrarTempo();
  }
}

// ==========================================
// SETUP
// ==========================================

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setBacklight(1);

  // Buzzer D8
  DDRB |= (1 << PB0);
  PORTB &= ~(1 << PB0);

  randomSeed(lerSemente());

  iniciarTimer();

  mostrarTempo();
  iniciarFase1();
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================

void loop() {
  if (segundosPendentes > 0) {
    atualizarTempo();
  }

  if (jogoEncerrado) {
    return;
  }

  // Entrada serial apenas na fase 1
  if (fase == 1) {
    while (Serial.available() > 0) {
      char entrada = Serial.read();

      if (entrada >= '0' && entrada <= '9') {
        if (resposta <= 6553) {
          resposta = resposta * 10 +
                    (entrada - '0');
          digitando = true;
        }
      }

      else if (entrada == '\n' ||
               entrada == '\r') {

        if (digitando) {
          Serial.print("Resposta: ");
          Serial.println(resposta);

          checarConta();

          resposta = 0;
          digitando = false;

          break;
        }
      }
    }
  }
}
