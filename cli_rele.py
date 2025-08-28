import serial
import threading
import time

# ---------------- CONFIGURAÇÃO ----------------
porta = input("Digite a porta do Arduino (ex.: /dev/tty.usbserial-A5069RR4 ou COM3): ")
baudrate = 9600

try:
    arduino = serial.Serial(porta, baudrate, timeout=1)
    time.sleep(2)  # espera Arduino resetar
    print(f"\nConectado ao Arduino em {porta} \n")
except Exception as e:
    print("Erro ao conectar ao Arduino:", e)
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
                print("Erro na leitura:", e)
        time.sleep(0.05)

# ---------------- THREAD SERIAL ----------------
if arduino is not None:
    thread = threading.Thread(target=ler_serial, daemon=True)
    thread.start()

# ---------------- LOOP PRINCIPAL ----------------
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

