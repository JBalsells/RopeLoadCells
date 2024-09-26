import tkinter as tk
from tkinter import messagebox

root = tk.Tk()
root.title("Interfaz con 2 botones")
root.geometry("800x600")

def accion_boton_1():
    messagebox.showinfo("Botón 1", "Has presionado el Botón 1")

def accion_boton_2():
    messagebox.showinfo("Botón 2", "Has presionado el Botón 2")

def accion_boton_3():
    messagebox.showinfo("Botón 3", "Has presionado el Botón 2")

boton1 = tk.Button(root, text="Pause/Stop", command=accion_boton_1)
boton2 = tk.Button(root, text="Recalibrate", command=accion_boton_2)
boton3 = tk.Button(root, text="Save", command=accion_boton_3)

boton1.pack(pady=10)
boton2.pack(pady=10)
boton3.pack(pady=10)

root.mainloop()
