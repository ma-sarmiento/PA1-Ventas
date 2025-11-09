#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// Estructura para representar un producto
struct Producto {
    int codigo;
    char nombre[30];
    int cantidad;
    float valor;
};

// Estructura para representar una venta
struct Venta {
    int codigoProducto;
    int cantidadVendida;
    float valorTotal;
};

// Declaración del prototipo
void registrarProducto();
void consultarProducto();
void generarReporte();
void registrarVenta();
bool actualizarProducto(const Producto&);


int main() {
    int opcion;
    bool continuar = true;

    while (continuar) {
        cout << "\n--- SISTEMA DE VENTAS ---\n";
        cout << "1. Registrar producto\n";
        cout << "2. Consultar producto\n";
        cout << "3. Generar reporte\n";
        cout << "4. Registrar venta\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarProducto();
                break;
            case 2:
                consultarProducto();
                break;
            case 3:
                generarReporte();
                break;
            case 4:
                registrarVenta();
                break;
            case 5:
                continuar = false;
                break;
            default:
                cout << "Opcion no válida. Intente de nuevo.\n";
        }
    }

    return 0;
}

// Función para registrar un nuevo producto
void registrarProducto() {
    Producto p;

    cout << "\n--- Registro de Producto ---\n";
    cout << "Codigo: ";
    cin >> p.codigo;
    cin.ignore(); // Limpiar buffer

    cout << "Nombre: ";
    cin.getline(p.nombre, 30);

    cout << "Cantidad en inventario: ";
    cin >> p.cantidad;

    cout << "Valor unitario: ";
    cin >> p.valor;

    // Abrir archivo binario
    ofstream archivo("productos.dat", ios::binary | ios::app);
    if (!archivo) {
        cerr << "Error al abrir el archivo de productos.\n";
        return;
    }

    archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
    archivo.close();

    cout << "Producto registrado exitosamente.\n";
}
// Función para consultar un producto por su código
void consultarProducto() {
    int codigoBuscado;
    Producto p;
    bool encontrado = false;

    cout << "\n--- Consulta de Producto ---\n";
    cout << "Ingrese el codigo del producto: ";
    cin >> codigoBuscado;

    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo de productos.\n";
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado) {
            cout << "\nProducto encontrado:\n";
            cout << "Codigo: " << p.codigo << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Cantidad disponible: " << p.cantidad << endl;
            cout << fixed << setprecision(0);  // Muestra sin decimales
            cout << "Valor unitario: $" << p.valor << endl;

            encontrado = true;
            break;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
    }
}

void generarReporte() {
    Producto p;
    ifstream archivo("productos.dat", ios::binary);

    cout << "\n--- Reporte General de Inventario ---\n";

    if (!archivo) {
        cerr << "Error al abrir el archivo de productos.\n";
        return;
    }

    bool hayDatos = false;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        hayDatos = true;
        cout << "-----------------------------\n";
        cout << "Codigo: " << p.codigo << endl;
        cout << "Nombre: " << p.nombre << endl;
        cout << "Cantidad: " << p.cantidad << endl;
        cout << fixed << setprecision(0);
        cout << "Valor unitario: $" << p.valor << endl;
    }

    archivo.close();

    if (!hayDatos) {
        cout << "No hay productos registrados.\n";
    }
}
// Reemplaza un producto existente por uno actualizado
bool actualizarProducto(const Producto& productoActualizado) {
    fstream archivo("productos.dat", ios::in | ios::out | ios::binary);
    if (!archivo) {
        cerr << "Error al abrir productos.dat para actualizar.\n";
        return false;
    }

    Producto temp;
    while (archivo.read(reinterpret_cast<char*>(&temp), sizeof(Producto))) {
        if (temp.codigo == productoActualizado.codigo) {
            // Retrocede el puntero para sobrescribir
            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<const char*>(&productoActualizado), sizeof(Producto));
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false; // No se encontró el producto
}
// Función para registrar una venta
void registrarVenta() {
    int codigo;
    int cantidadVenta;
    Producto p;
    bool encontrado = false;

    cout << "\n--- Registrar Venta ---\n";
    cout << "Ingrese el codigo del producto: ";
    cin >> codigo;

    // Abrimos archivo para lectura
    fstream archivo("productos.dat", ios::in | ios::binary);
    if (!archivo) {
        cerr << "Error al abrir productos.dat.\n";
        return;
    }

    // Buscar el producto
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo) {
            encontrado = true;
            break;
        }
    }
    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
        return;
    }

    cout << "Cantidad disponible: " << p.cantidad << endl;
    cout << "Ingrese cantidad a vender: ";
    cin >> cantidadVenta;

    if (cantidadVenta > p.cantidad) {
        cout << "No hay suficiente inventario.\n";
        return;
    }

    // Actualizar inventario
    p.cantidad -= cantidadVenta;
    if (!actualizarProducto(p)) {
        cout << "Error actualizando el producto.\n";
        return;
    }

    // Registrar venta en archivo ventas.dat
    Venta v;
    v.codigoProducto = p.codigo;
    v.cantidadVendida = cantidadVenta;
    v.valorTotal = cantidadVenta * p.valor;

    ofstream ventasFile("ventas.dat", ios::binary | ios::app);
    if (!ventasFile) {
        cerr << "Error al abrir ventas.dat.\n";
        return;
    }

    ventasFile.write(reinterpret_cast<char*>(&v), sizeof(Venta));
    ventasFile.close();

    cout << " Venta registrada. Total: $" << fixed << setprecision(2) << v.valorTotal << "\n";
}


