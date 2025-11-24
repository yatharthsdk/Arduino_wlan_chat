import socket
import threading

ARDUINO_IP = "Your_Arduino_IP" # "Your will get it from serial monitor"
ARDUINO_PORT = 5050

def receiver(sock: socket.socket):
  while True:
    try:
      data = sock.recv(1024)
    except ConnectionResetError:
      print("\n[!] Connection reset by Arduino.")
      break

    if not data:
      print("\n[!] Connection closed by Arduino.")
      break

    print("\nFrom Arduino:", data.decode().strip())
    print("You -> Arduino: ", end="", flush=True)

def main():
  print(f"Connecting to Arduino at {ARDUINO_IP}:{ARDUINO_PORT}...")
  with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((ARDUINO_IP, ARDUINO_PORT))
    print("Connected! Type messages, Ctrl+C to quit.\n")
    print("Use RSSI_ON / RSSI_OFF to control signal reporting.\n")

    t = threading.Thread(target=receiver, args=(s,), daemon=True)
    t.start()

    try:
      while True:
        msg = input("You -> Arduino: ")
        if msg:
          s.sendall((msg + "\n").encode("utf-8"))
    except KeyboardInterrupt:
      print("\n[!] Exiting chat client.")

if __name__ == "__main__":
  main()
