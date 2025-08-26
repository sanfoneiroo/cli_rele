import serial
import threading
import time

# ---------------- CONFIGURAÇÃO ----------------
BAUDRATE = 9600

# Pergunta a porta ao usuário
porta = input("Digite a porta do Arduino (ex.: /dev/tty.usbserial-A5069RR4 no Mac/Linux ou COM3 no Windows): ")

try:
    arduino = serial.Serial(porta, BAUDRATE, timeout=1)
    time.sleep(2)  # espera Arduino resetar
    print(f"✅ Conectado ao Arduino na porta {porta}")
except Exception as e:
    print("❌ Erro ao conectar ao Arduino:", e)
    arduino = None

# ---------------- FUNÇÃO DE LEITURA ----------------
def ler_serial():
    """Lê mensagens do Arduino e imprime no terminal"""
    while True:
        if arduino is not None and arduino.in_waiting > 0:
            try:
                linha = arduino.readline().decode(errors="ignore").strip()
                if linha:
                    print(f"\n[Arduino] {linha}")
            except Exception as e:
                print("⚠️ Erro na leitura:", e)
        time.sleep(0.05)

# ---------------- THREAD SERIAL ----------------
if arduino is not None:
    thread = threading.Thread(target=ler_serial, daemon=True)
    thread.start()

# ---------------- LOOP PRINCIPAL ----------------
print("\n=== Controle do Relé Py-Ino ===\n")
print("Comandos disponíveis:")
print("  relogio   -> Ajustar relógio (formato HHMM)")
print("  agendar   -> Ajustar agenda do relé (formato HHMM)")
print("  ligar     -> Liga o relé manualmente")
print("  desligar  -> Desliga o relé manualmente")
print("  verificar -> Mostra hora atual e hora agendada")
print("===============================================\n")

try:
    while True:
        if arduino is not None:
            comando = input("> ")
            if comando.strip():
                arduino.write((comando + "\n").encode())
        else:
            time.sleep(1)
except KeyboardInterrupt:
    print("\nSaindo...")
    if arduino is not None:
        arduino.close()
