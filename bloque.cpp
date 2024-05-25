#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

class Bloque {
private:
    string rutaArchivo;
    ofstream archivo;
    size_t tamano_actual;
    size_t tamano_maximo;

public:
    Bloque(const string &nombreCarpeta, int numero, size_t tamano_maximo)
        : tamano_actual(0), tamano_maximo(tamano_maximo) {
        rutaArchivo = nombreCarpeta + "/bloque_" + to_string(numero) + ".txt";
        archivo.open(rutaArchivo, ios::out | ios::app);
        cout << "Creando archivo: " << rutaArchivo << endl;
    }

    ~Bloque() {
        if (archivo.is_open()) {
            archivo.close();
        }
    }

    bool puedeAgregar(const string &linea) {
        return (tamano_actual + linea.size() + 1 <= tamano_maximo);
    }

    void agregarLinea(const string &linea) {
        archivo << linea << "\n";
        tamano_actual += linea.size() + 1;
    }
};

class ManejoBloques {
private:
    Bloque** bloques;
    int num_bloques;
    size_t tamano_bloque;

public:
    ManejoBloques(const string &nombreCarpeta, int num_bloques, size_t tamano_bloque)
        : num_bloques(num_bloques), tamano_bloque(tamano_bloque) {
        mkdir(nombreCarpeta.c_str(), 0777);
        bloques = new Bloque*[num_bloques];
        for (int i = 0; i < num_bloques; ++i) {
            bloques[i] = new Bloque(nombreCarpeta, i + 1, tamano_bloque);
        }
    }

    ~ManejoBloques() {
        for (int i = 0; i < num_bloques; ++i) {
            delete bloques[i];
        }
        delete[] bloques;
    }

    void ingresarDatos(const string &archivoTxt) {
        ifstream archivo(archivoTxt);
        string linea;
        int bloque_actual = 0;

        while (getline(archivo, linea)) {
            while (!bloques[bloque_actual]->puedeAgregar(linea)) {
                bloque_actual = (bloque_actual + 1) % num_bloques;
            }
            bloques[bloque_actual]->agregarLinea(linea);
        }
    }
};

int main() {
    ManejoBloques manejoBloques("Bloques", 16, 256);
    manejoBloques.ingresarDatos("titanic.csv");
    return 0;
}
