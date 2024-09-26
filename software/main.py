import sys
import time
import serial
import tkinter as tk
from tkinter import ttk

if len(sys.argv) > 1:
    port = sys.argv[1]
    baudrate = sys.argv[2]
    print(f"USB port: {port}")
else:
    print("Port number not found.")

ser = serial.Serial(f"/dev/tty{str(port)}", baudrate, timeout=1)

# Verifica si el puerto está abierto
if ser.is_open:
    print(f"Serial port USB{port} is opened.")

try:
    while True:
        data = ser.readline().decode('utf-8').strip()
        if data:
            print(f"Recibido: {data}")

        #command = "d"
        #ser.write(command.encode('utf-8'))



except KeyboardInterrupt:
    print("Lectura interrumpida por el usuario.")

finally:
    ser.close()
    print("Puerto serial cerrado.")
