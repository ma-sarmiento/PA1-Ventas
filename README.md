# PA1-Ventas

Proyecto académico desarrollado para la materia de **Programación Avanzada** (2023-30).  
El sistema permite gestionar las ventas de una tienda de electrodomésticos utilizando archivos **binarios** para el almacenamiento persistente de la información.

---

## 📌 Funcionalidades

- Registrar nuevos productos.
- Consultar productos disponibles por referencia.
- Generar reporte general de inventario.
- Registrar ventas y calcular el total por cliente.
- Almacenar y recuperar información utilizando archivos binarios.

---

## 🛠️ Tecnologías y herramientas

- Lenguaje: **C++**
- Manejo de archivos binarios (`fstream`, modo `binary`)
- IDE recomendado: **CLion**, **Visual Studio Code** o **Code::Blocks**
- Sistema de control de versiones: **Git + GitHub**

---

## 📁 Estructura del proyecto
```plaintext
📦 ElectroVentasPA1/
├── .gitignore
├── CMakeLists.txt
├── main.cpp
├── README.md
├── productos.txt         
├── clientes.txt          

```

## 🧪 Estado del Proyecto


Este código fue funcional y aprobado en su momento. Fue revisado para su publicación en GitHub con fines académicos y de portafolio personal.  

---



## 📁 Archivos incluidos

-`main.cpp`: Código fuente principal.

-`funciones.cpp`: Funciones auxiliares.

-`README.md`: Este documento.

-`.gitignore`: Archivos y carpetas ignoradas por Git para mantener limpio el repositorio.

---

## 🚀 Cómo ejecutar el proyecto

1. **Clona este repositorio**:

   ```bash
   git clone https://github.com/ma-sarmiento/PA1-Ventas.git
   cd PA1-Ventas
Requiere tener instalado un compilador compatible con C++, como g++ (MinGW para Windows).

2. Compila el archivo fuente con un compilador C++:
   
   ```bash
   g++ -std=c++11 main.cpp -o ElectroVentas

3. Ejecuta el programa:
   
   ```bash
   ElectroVentas.exe en Windows
   ./ElectroVentas en linux/MacOs

  💡 También puedes abrirlo directamente desde CLion o cualquier otro IDE de tu preferencia para C++ y compilar con un solo clic.

---
   
>  Nota: Por razones de derechos académicos, el enunciado original del proyecto **no será publicado en este repositorio**.
