#include <iostream>
#include <vector>

using namespace std;

class PageTable {
private:
    struct PageEntry {
        int FrameId;
        int PageId;
        bool DirtyBit;
        int PinCount;
        int LastUsed;

        PageEntry(int frameId, int pageId) : FrameId(frameId), PageId(pageId), DirtyBit(false), PinCount(0), LastUsed(0) {}
    };

    vector<PageEntry> entries;

public:
    // Constructor
    PageTable() {}

    // Destructor
    ~PageTable() {}

    // Mostrar la tabla 
    void mostrarTabla() {
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tLast Used" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t" 
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t" 
                 << entries[i].LastUsed << endl;
        }
        cout << endl;
    }

    // Verificar si una página dada existe en la tabla de páginas
    bool verificarExistenciaDePagina(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                return true;
            }
        }
        return false;
    }

    // Obtener el número de marco asociado a una página dada
    int getNumFrame(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                return entries[i].FrameId;
            }
        }
        return -1; // Si la página no se encuentra
    }

    // Disminuir el contador de pines (PinCount) de una página dada
    void descontarPinCount(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].PinCount--;
                break;
            }
        }
    }

    // Aumentar el contador de pines (PinCount) de una página dada
    void aumentarPinCount(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].PinCount++;
                break;
            }
        }
    }

    // Cambiar el bit de suciedad (DirtyBit) de una página dada entre 0 y 1
    void cambiarDirty(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].DirtyBit = !entries[i].DirtyBit;
                break;
            }
        }
    }

    void incrementarLastUsed(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                entries[i].LastUsed++;
                break;
            }
        }
    }

    // Decrementar el valor de LastUsed de una página dada
    void decrementarLastUsed(int numPagina) {
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PageId == numPagina) {
                if (entries[i].LastUsed > 0) {
                    entries[i].LastUsed--;
                }
                break;
            }
        }
    }

    // Actualizar los datos en la tabla de páginas cuando se solicita una página
    void actualizarDatos(int numPaginaActualizar, int numFilaFrameId) {
        entries.push_back(PageEntry(numFilaFrameId, numPaginaActualizar));
        aumentarPinCount(numPaginaActualizar); // Incrementar el contador de pines
    }
};

int main() {
    // Crear una instancia de la tabla de páginas
    PageTable pageTable;

    // Mostrar la tabla de páginas inicialmente
    cout << "Tabla de páginas inicial:" << endl;
    pageTable.mostrarTabla();

    // Agregar algunas páginas (esto es solo un ejemplo, puedes ajustar los números según tus necesidades)
    pageTable.actualizarDatos(1, 0);
    pageTable.actualizarDatos(2, 1);
    pageTable.actualizarDatos(3, 2);

    // Mostrar la tabla de páginas después de agregar páginas
    cout << "\nTabla de páginas después de agregar páginas:" << endl;
    pageTable.mostrarTabla();

    // Simular el acceso a una página existente
    int paginaExistente = 2;
    cout << "\nAccediendo a la página " << paginaExistente << ":" << endl;
    pageTable.aumentarPinCount(paginaExistente); // Aquí debería ser aumentarPinCountDePagina en lugar de aumentarLastUsedDePagina
    pageTable.mostrarTabla();

    // Simular el acceso a una página inexistente
    int paginaInexistente = 4;
    cout << "\nIntentando acceder a la página " << paginaInexistente << " (inexistente):" << endl;
    pageTable.actualizarDatos(paginaInexistente, 3); // Agregamos la página 4 en el marco 3
    pageTable.mostrarTabla();

    return 0;
}
