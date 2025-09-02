#include <Thermistor.h>
#include <NTC_Thermistor.h>

#define rele 3
#define led 4
#define PINO_TERMISTOR A3
#define RESISTOR 10000        // resistor de referência (10k)
#define TERMISTOR_NOMINAL 10000 // valor nominal do termistor (10k a 25 °C)
#define TEMPERATURA_NOMINAL 25  // temperatura nominal (25 °C)
#define B_COEFICIENTE 3950      // coeficiente Beta do termistor

// cria o objeto para ler o termistor
NTC_Thermistor temp(PINO_TERMISTOR, RESISTOR, TERMISTOR_NOMINAL, TEMPERATURA_NOMINAL, B_COEFICIENTE);

// Relógio simples (hora e minuto)
int hora = 0;
int minuto = 0;
unsigned long ultimoMillis = 0;

// Agenda ligar
int horaLigar = -1;
int minutoLigar = -1;
bool ligarDisparado = false;

// Agenda desligar
int horaDesligar = -1;
int minutoDesligar = -1;
bool desligarDisparado = false;

String comando = "";

void setup() {
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);

  Serial.begin(9600);
  delay(200);

  Serial.println("=== CLI do Relé ===");
  Serial.println("Comandos disponíveis:");
  Serial.println("relogio   -> Ajustar hora (formato HHMM)");
  Serial.println("acordar   -> Ajustar horário de ligar o rele (formato HHMM)");
  Serial.println("dormir    -> Ajustar horário de desligar o rele (formato HHMM)");
  Serial.println("ligar     -> Ligar rele manual");
  Serial.println("desligar  -> Desligar rele manual");
  Serial.println("verificar -> Mostrar hora atual e agenda");
  Serial.println("-------------------------");
}

void loop() {
  // --- Atualiza relogio simples (1 minuto = 60000ms) ---
  if (millis() - ultimoMillis >= 60000) {
    ultimoMillis = millis();
    minuto++;
    if (minuto >= 60) {
      minuto = 0;
      hora = (hora + 1) % 24;
    }
  }

  // --- Verifica agendamento de ligar ---
  if (hora == horaLigar && minuto == minutoLigar && !ligarDisparado) {
    digitalWrite(rele, HIGH);
    digitalWrite(led, HIGH);
    Serial.println(">> Rele AGENDADO foi LIGADO!");
    ligarDisparado = true;
  }

  // Reinicia flag para próximo dia (ligar)
  if (hora != horaLigar || minuto != minutoLigar) {
    ligarDisparado = false;
  }

  // --- Verifica agendamento de desligar ---
  if (hora == horaDesligar && minuto == minutoDesligar && !desligarDisparado) {
    digitalWrite(rele, LOW);
    digitalWrite(led, LOW);
    Serial.println(">> Rele AGENDADO foi DESLIGADO!");
    desligarDisparado = true;
  }

  // Reinicia flag para próximo dia (desligar)
  if (hora != horaDesligar || minuto != minutoDesligar) {
    desligarDisparado = false;
  }

  // --- Leitura de comandos pelo terminal ---
  if (Serial.available() > 0) {
    comando = Serial.readStringUntil('\n');
    comando.trim();

    while (Serial.available() > 0) {
      Serial.read(); // limpa buffer
    }

    if (comando.equalsIgnoreCase("ligar")) {
      digitalWrite(rele, HIGH);
      digitalWrite(led, HIGH);
      Serial.println("Rele LIGADO manualmente!");
    }
    else if (comando.equalsIgnoreCase("desligar")) {
      digitalWrite(rele, LOW);
      digitalWrite(led, LOW);
      Serial.println("Rele DESLIGADO manualmente!");
    }
    else if (comando.equalsIgnoreCase("relogio")) {
      ajustarRelogio();
    }
    else if (comando.equalsIgnoreCase("acordar")) {
      ajustarAcordar();
    }
    else if (comando.equalsIgnoreCase("dormir")) {
      ajustarDormir();
    }
    else if (comando.equalsIgnoreCase("verificar")) {
      verificarStatus();
    }
    else {
      Serial.println("Comando invalido!");
    }
  }
}

// ---------------- FUNÇÕES ----------------

// Mostra hora formatada
void mostraHora(int h, int m) {
  if (h < 10) Serial.print("0");
  Serial.print(h);
  Serial.print(":");
  if (m < 10) Serial.print("0");
  Serial.print(m);
}

// Função auxiliar: limpar buffer Serial
void limparBufferSerial() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

// Ajusta relogio
void ajustarRelogio() {
  Serial.println("Digite a hora no formato HHMM: ");
  limparBufferSerial();
  while (Serial.available() == 0);
  int entrada = Serial.parseInt();
  limparBufferSerial();

  hora = entrada / 100;
  minuto = entrada % 100;

  ultimoMillis = millis(); // reseta contagem de minutos
  Serial.print("Relogio ajustado para: ");
  mostraHora(hora, minuto);
  Serial.println();
}

// Ajusta horário de ligar
void ajustarAcordar() {
  Serial.println("Digite o horario para LIGAR o rele no formato HHMM: ");
  limparBufferSerial();
  while (Serial.available() == 0);
  int entrada = Serial.parseInt();
  limparBufferSerial();

  horaLigar = entrada / 100;
  minutoLigar = entrada % 100;

  ligarDisparado = false;
  Serial.print("Agenda de LIGAR configurada para: ");
  mostraHora(horaLigar, minutoLigar);
  Serial.println();
}

// Ajusta horário de desligar
void ajustarDormir() {
  Serial.println("Digite o horario para DESLIGAR o rele no formato HHMM: ");
  limparBufferSerial();
  while (Serial.available() == 0);
  int entrada = Serial.parseInt();
  limparBufferSerial();

  horaDesligar = entrada / 100;
  minutoDesligar = entrada % 100;

  desligarDisparado = false;
  Serial.print("Agenda de DESLIGAR configurada para: ");
  mostraHora(horaDesligar, minutoDesligar);
  Serial.println();
}

// Verifica hora atual e agendada
void verificarStatus() {
  double celsius = temp.readCelsius();  // lê a temperatura em °C

  Serial.print("Hora: ");
  mostraHora(hora, minuto);
  Serial.print(" | ");

  if (horaLigar >= 0 && minutoLigar >= 0) {
    Serial.print("Acordar: ");
    mostraHora(horaLigar, minutoLigar);
  } else {
    Serial.print("Acordar: --:--");
  }
  Serial.print(" | ");

  if (horaDesligar >= 0 && minutoDesligar >= 0) {
    Serial.print("Dormir: ");
    mostraHora(horaDesligar, minutoDesligar);
  } else {
    Serial.print("Dormir: --:--");
  }
  Serial.print(" | Temp: ");
  Serial.print(celsius);
  Serial.println(" °C");  // só quebra a linha no final
}
