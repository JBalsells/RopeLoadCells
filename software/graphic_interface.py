import tkinter as tk
from tkinter import messagebox

# Crear la ventana principal
root = tk.Tk()
root.title("Interfaz con 2 botones")
root.geometry("300x200")  # Tamaño de la ventana

# Función que se ejecuta cuando se presiona el primer botón
def accion_boton_1():
    messagebox.showinfo("Botón 1", "Has presionado el Botón 1")

# Función que se ejecuta cuando se presiona el segundo botón
def accion_boton_2():
    messagebox.showinfo("Botón 2", "Has presionado el Botón 2")

# Crear los botones
boton1 = tk.Button(root, text="Botón 1", command=accion_boton_1)
boton2 = tk.Button(root, text="Botón 2", command=accion_boton_2)

# Colocar los botones en la ventana
boton1.pack(pady=20)
boton2.pack(pady=20)

# Ejecutar el bucle principal de la interfaz gráfica
root.mainloop()
