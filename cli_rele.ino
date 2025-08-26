#define rele 3

// Relógio simples (hora e minuto)
int hora = 0;
int minuto = 0;
unsigned long ultimoMillis = 0;

// Agenda
int horaAgendada = -1;
int minutoAgendado = -1;
bool alarmeDisparado = false;

String comando = "";

void setup() {
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);

  Serial.begin(9600);
  delay(200);

  Serial.println("=== CLI do Relé ===");
  Serial.println("-------------------------");
  Serial.println("Comandos disponíveis:");
  Serial.println("relogio   -> Ajustar hora (formato HHMM)");
  Serial.println("agendar   -> Ajustar agenda do rele (formato HHMM)");
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

  // --- Verifica agendamento ---
  if (hora == horaAgendada && minuto == minutoAgendado && !alarmeDisparado) {
    digitalWrite(rele, HIGH);
    Serial.println(">> Rele AGENDADO foi LIGADO!");
    alarmeDisparado = true;
  }

  // Reinicia flag para próximo dia
  if (hora != horaAgendada || minuto != minutoAgendado) {
    alarmeDisparado = false;
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
      Serial.println("Rele LIGADO manualmente!");
    }
    else if (comando.equalsIgnoreCase("desligar")) {
      digitalWrite(rele, LOW);
      Serial.println("Rele DESLIGADO manualmente!");
    }
    else if (comando.equalsIgnoreCase("relogio")) {
      ajustarRelogio();
    }
    else if (comando.equalsIgnoreCase("agendar")) {
      ajustarAgenda();
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
  Serial.println("Digite a hora no formato HHMM (exemplo 1345 para 13:45): ");
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

// Ajusta agenda
void ajustarAgenda() {
  Serial.println("Digite o horario para ligar o rele no formato HHMM (exemplo 0815 para 08:15): ");
  limparBufferSerial();
  while (Serial.available() == 0);
  int entrada = Serial.parseInt();
  limparBufferSerial();

  horaAgendada = entrada / 100;
  minutoAgendado = entrada % 100;

  alarmeDisparado = false; // zera flag
  Serial.print("Agenda configurada para: ");
  mostraHora(horaAgendada, minutoAgendado);
  Serial.println();
}

// Verifica hora atual e agendada
void verificarStatus() {
  Serial.print("Hora atual: ");
  mostraHora(hora, minuto);
  Serial.println();

  if (horaAgendada >= 0 && minutoAgendado >= 0) {
    Serial.print("Hora agendada: ");
    mostraHora(horaAgendada, minutoAgendado);
    Serial.println();
  } else {
    Serial.println("Nenhuma agenda configurada.");
  }
}
