#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

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
struct Cliente {
    char tipoId;              // 'C' o 'E'
    long long documento;      // número de identificación
    char nombre[30];
    char apellido[30];
    char direccion[50];
};

// Declaración
void registrarProducto();
void consultarProducto();
void generarReporte();
void registrarVenta();
bool actualizarProducto(const Producto&);
void cargarProductosDesdeArchivo();
void cargarClientesDesdeArchivo();
void registrarCliente();
void buscarProductoPorNombre();
void buscarClientePorNombreApellido();

int main() {
    int opcion;
    bool continuar = true;

    while (continuar) {
        cout << "\n--- SISTEMA DE VENTAS ---\n";
        cout << "1. Registrar producto\n";
        cout << "2. Cargar productos desde archivo\n";
        cout << "3. Consultar producto\n";
        cout << "4. Buscar producto por nombre\n";
        cout << "5. Registrar cliente\n";
        cout << "6. Cargar clientes desde archivo\n";
        cout << "7. Buscar cliente por nombre/apellido\n";
        cout << "8. Generar reporte\n";
        cout << "9. Registrar venta\n";
        cout << "10. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarProducto();
                break;
            case 2:
                cargarProductosDesdeArchivo();
                break;
            case 3:
                consultarProducto();
                break;
            case 4:
                buscarProductoPorNombre();
                break;
            case 5:
                registrarCliente();
                break;
            case 6:
                cargarClientesDesdeArchivo();
                break;
            case 7:
                buscarClientePorNombreApellido();
                break;
            case 8:
                generarReporte();
                break;
            case 9:
                registrarVenta();
                break;
            case 10:
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
    cin.ignore();  // limpiar buffer

    // Verificar si ya existe
    ifstream archivoExistente("productos.dat", ios::binary);
    Producto temp;
    while (archivoExistente.read(reinterpret_cast<char*>(&temp), sizeof(Producto))) {
        if (temp.codigo == p.codigo) {
            cout << "El producto con código " << p.codigo << " ya existe.\n";
            archivoExistente.close();
            return;
        }
    }
    archivoExistente.close();

    cout << "Nombre: ";
    cin.getline(p.nombre, 30);

    cout << "Cantidad en inventario: ";
    cin >> p.cantidad;

    cout << "Valor unitario: ";
    cin >> p.valor;

    // Guardar en productos.dat (binario)
    ofstream archivoBin("productos.dat", ios::binary | ios::app);
    if (!archivoBin) {
        cerr << "Error al abrir productos.dat\n";
        return;
    }
    archivoBin.write(reinterpret_cast<char*>(&p), sizeof(Producto));
    archivoBin.close();

    // Guardar en productos.txt (texto plano)
    ofstream archivoTxt("productos.txt", ios::app);
    if (!archivoTxt) {
        cerr << "Error al abrir productos.txt\n";
        return;
    }
    archivoTxt << p.codigo << ";" << p.nombre << ";" << fixed << setprecision(0) << p.valor << ";" << p.cantidad << "\n";
    archivoTxt.close();

    cout << "Producto registrado exitosamente en productos.dat y productos.txt\n";
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
    if (!archivo) {
        cerr << "Error al abrir el archivo de productos.\n";
        return;
    }

    cout << "\n--- REPORTE DE INVENTARIO ---\n";
    cout << left << setw(10) << "Codigo"
         << setw(30) << "Nombre"
         << setw(12) << "Cantidad"
         << setw(15) << "Valor Unitario"
         << setw(15) << "Valor Total" << endl;
    cout << string(82, '-') << endl;

    int totalProductos = 0;
    float totalInventario = 0;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        float valorTotal = p.cantidad * p.valor;
        totalProductos++;
        totalInventario += valorTotal;

        cout << left << setw(10) << p.codigo
             << setw(30) << p.nombre
             << setw(12) << p.cantidad
             << "$" << setw(14) << fixed << setprecision(0) << p.valor
             << "$" << setw(14) << fixed << setprecision(2) << valorTotal << endl;
    }

    archivo.close();

    cout << string(82, '-') << endl;
    cout << "Total de productos: " << totalProductos << endl;
    cout << "Valor total del inventario: $" << fixed << setprecision(2) << totalInventario << endl;
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
void cargarProductosDesdeArchivo() {
    ifstream archivoTxt("productos.txt");
    ofstream archivoBin("productos.dat", ios::binary | ios::trunc);  // trunc para sobrescribir

    if (!archivoTxt || !archivoBin) {
        cerr << "Error al abrir productos.txt o productos.dat.\n";
        return;
    }

    Producto p;
    string linea;

    while (getline(archivoTxt, linea)) {
        // Separar por ";"
        size_t pos1 = linea.find(';');
        size_t pos2 = linea.find(';', pos1 + 1);
        size_t pos3 = linea.find(';', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) {
            cerr << "Línea mal formateada: " << linea << endl;
            continue;
        }

        string codigoStr = linea.substr(0, pos1);
        string nombreStr = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string valorStr = linea.substr(pos2 + 1, pos3 - pos2 - 1);
        string cantidadStr = linea.substr(pos3 + 1);

        p.codigo = stoi(codigoStr);
        strncpy(p.nombre, nombreStr.c_str(), sizeof(p.nombre));
        p.nombre[sizeof(p.nombre) - 1] = '\0';
        p.valor = stof(valorStr);
        p.cantidad = stoi(cantidadStr);

        archivoBin.write(reinterpret_cast<char*>(&p), sizeof(Producto));
    }

    archivoTxt.close();
    archivoBin.close();

    cout << "Productos cargados exitosamente desde productos.txt\n";
}
void cargarClientesDesdeArchivo() {
    ifstream archivoTxt("clientes.txt");
    ofstream archivoBin("clientes.dat", ios::binary | ios::trunc);

    if (!archivoTxt || !archivoBin) {
        cerr << "Error al abrir clientes.txt o clientes.dat.\n";
        return;
    }

    Cliente c;
    string linea;

    while (getline(archivoTxt, linea)) {
        cout << "Leyendo: " << linea << endl;  // DEBUG para identificar errores

        size_t pos1 = linea.find(';');
        size_t pos2 = linea.find(';', pos1 + 1);
        size_t pos3 = linea.find(';', pos2 + 1);
        size_t pos4 = linea.find(';', pos3 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos || pos4 == string::npos) {
            cerr << "Linea mal formateada: " << linea << endl;
            continue;
        }

        string tipoIdStr = linea.substr(0, pos1);
        string documentoStr = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string nombreStr = linea.substr(pos2 + 1, pos3 - pos2 - 1);
        string apellidoStr = linea.substr(pos3 + 1, pos4 - pos3 - 1);
        string direccionStr = linea.substr(pos4 + 1);

        c.tipoId = tipoIdStr.empty() ? ' ' : tipoIdStr[0];  // Prevención si cadena vacía

        try {
            c.documento = stoll(documentoStr);
        } catch (const out_of_range& e) {
            cerr << "Error: documento fuera de rango. Línea: " << linea << endl;
            continue;
        } catch (const invalid_argument& e) {
            cerr << "Error: argumento invalido para documento. Línea: " << linea << endl;
            continue;
        }

        strncpy(c.nombre, nombreStr.c_str(), sizeof(c.nombre));
        c.nombre[sizeof(c.nombre) - 1] = '\0';

        strncpy(c.apellido, apellidoStr.c_str(), sizeof(c.apellido));
        c.apellido[sizeof(c.apellido) - 1] = '\0';

        strncpy(c.direccion, direccionStr.c_str(), sizeof(c.direccion));
        c.direccion[sizeof(c.direccion) - 1] = '\0';

        archivoBin.write(reinterpret_cast<char*>(&c), sizeof(Cliente));
    }

    archivoTxt.close();
    archivoBin.close();

    cout << "Clientes cargados exitosamente desde clientes.txt\n";
}
void registrarCliente() {
    Cliente c;
    cout << "\n--- Registro de Cliente ---\n";

    cout << "Tipo de identificacion (C o E): ";
    cin >> c.tipoId;

    cout << "Numero de documento: ";
    cin >> c.documento;

    // Verificar si ya existe el cliente
    ifstream archivoBin("clientes.dat", ios::binary);
    if (archivoBin) {
        Cliente temp;
        while (archivoBin.read(reinterpret_cast<char*>(&temp), sizeof(Cliente))) {
            if (temp.documento == c.documento) {
                cout << "El cliente con ese documento ya existe.\n";
                archivoBin.close();
                return;
            }
        }
        archivoBin.close();
    }

    cin.ignore();
    cout << "Nombre: ";
    cin.getline(c.nombre, 30);

    cout << "Apellido: ";
    cin.getline(c.apellido, 30);

    cout << "Direccion: ";
    cin.getline(c.direccion, 50);

    // Guardar en archivo binario
    ofstream archivoBinOut("clientes.dat", ios::binary | ios::app);
    if (!archivoBinOut) {
        cerr << "Error al abrir clientes.dat para escritura.\n";
        return;
    }
    archivoBinOut.write(reinterpret_cast<char*>(&c), sizeof(Cliente));
    archivoBinOut.close();

    // Guardar en archivo de texto
    ofstream archivoTxt("clientes.txt", ios::app);
    if (!archivoTxt) {
        cerr << "Error al abrir clientes.txt para escritura.\n";
        return;
    }
    archivoTxt << c.tipoId << ';'
               << c.documento << ';'
               << c.nombre << ';'
               << c.apellido << ';'
               << c.direccion << '\n';
    archivoTxt.close();

    cout << "Cliente registrado exitosamente.\n";
}
void buscarProductoPorNombre() {
    char criterio[30];
    Producto p;
    bool encontrado = false;

    cout << "\n--- Buscar Producto por Nombre ---\n";
    cout << "Ingrese parte del nombre a buscar: ";
    cin.ignore();
    cin.getline(criterio, 30);

    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) {
        cerr << "Error al abrir productos.dat.\n";
        return;
    }

    cout << left << setw(10) << "Codigo"
         << setw(30) << "Nombre"
         << setw(15) << "Precio Unitario" << endl;
    cout << string(55, '-') << endl;

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (strstr(p.nombre, criterio) != nullptr) {
            cout << left << setw(10) << p.codigo
                 << setw(30) << p.nombre
                 << "$" << fixed << setprecision(0) << p.valor << endl;
            encontrado = true;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "No se encontraron productos que coincidan con \"" << criterio << "\".\n";
    }
}
// Función para buscar clientes por subcadena en nombre o apellido
void buscarClientePorNombreApellido() {
    ifstream archivo("clientes.dat", ios::binary);
    if (!archivo) {
        cerr << "Error al abrir clientes.dat.\n";
        return;
    }

    Cliente c;
    string termino;
    bool encontrado = false;

    cin.ignore(); // Limpiar buffer
    cout << "\nBuscar cliente\n";
    cout << "Ingrese nombre o apellido (o parte): ";
    getline(cin, termino);

    // Convertir término a minúsculas para comparación insensible a mayúsculas
    for (char &ch : termino) ch = tolower(ch);

    cout << "\nResultados encontrados:\n";

    while (archivo.read(reinterpret_cast<char*>(&c), sizeof(Cliente))) {
        string nombreCompleto = string(c.nombre) + " " + string(c.apellido);
        string nombreMinuscula = nombreCompleto;
        for (char &ch : nombreMinuscula) ch = tolower(ch);

        if (nombreMinuscula.find(termino) != string::npos) {
            cout << "---------------------------\n";
            cout << "Tipo ID: " << c.tipoId << endl;
            cout << "Documento: " << c.documento << endl;
            cout << "Nombre: " << c.nombre << endl;
            cout << "Apellido: " << c.apellido << endl;
            cout << "Direccion: " << c.direccion << endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron coincidencias.\n";
    }

    archivo.close();
}
