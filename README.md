# cli_rele
# Projeto: Controle de Relé via CLI e Arduino

Este projeto permite ligar e desligar um relé conectado ao **Arduino**, utilizando **comandos via terminal**.  
A comunicação é feita pelo **Python (Serial)** no PC, e também pode ser acessada remotamente via **SSH** pelo celular dentro da mesma rede local.

---

## Funcionalidades

- Envio de comandos pelo terminal para ligar/desligar o relé.
- Agendamento de horários para ligar/desligar automaticamente.
- Monitoramento de temperatura ambiente via sensor.
- Consulta de status: hora atual, horários agendados, últimos comandos e temperatura.

---

## Estrutura do Projeto

cli_rele
└── cli_rele.ino # Código principal do Arduino
└── cli_rele.py # Script em Python para comunicação Serial
├── docs/
│ └── diagrama.png # Diagrama de funcionamento
│ └── cli_rele.png # Ilustração do terminal em funcionamento
└── README.md # Documentação do projeto


---

## Hardware Utilizado

- Arduino (UNO, Nano ou similar)
- Relé de 5V
- Sensor de Temperatura NTC e resistor 10k
- Led e resistor 1k
- Cabos jumpers
- PC conectado ao Arduino via USB
- Celular na mesma LAN para acesso remoto (SSH)

---

## Como Usar

1. **Carregar o código Arduino**:
   - Abra o arquivo `cli_rele.ino` na IDE Arduino.
   - Compile e faça upload para a placa.

2. **Rodar o script Python**:
   - Conecte o Arduino ao PC.
   - No terminal, execute:
     ```bash
     python3 cli_rele.py
     ```

3. **Enviar comandos via terminal**:
   - Exemplos:
     ```
     ligar
     desligar
     verificar
     agendar 14:30 ligar
     agendar 15:00 desligar
     ```

4. **Acesso remoto**:
   - Conecte-se ao PC via SSH pelo celular.
   - Rode o script Python dentro da sessão SSH.
   - Controle o relé de qualquer lugar dentro da LAN.


---

## Possíveis melhorias

- Histórico de logs salvo em arquivo.
- Interface web simples para envio de comandos.
- Integração futura com ESP32 (para eliminar necessidade do PC).

---

## 📄 Licença

Este projeto é totalmente livre para estudo, modificação e compartilhamento.  
Autor: **Erwin de Mattos** (2025)

---
