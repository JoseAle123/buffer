#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Clase Pagina para manejar el contenido de cada página
class Pagina {
private:
    std::string ruta;
    std::vector<std::string> contenido;

public:
    // Constructor con argumentos para cargar el contenido de un archivo
    Pagina(const std::string& nombre_archivo, const std::string& directorio = "Bloques/") {
        ruta = directorio + nombre_archivo;
        cargarContenido();
    }

    // Constructor sin argumentos para manejar la inicialización de una instancia vacía
    Pagina() {}

    void cargarContenido() {
        std::ifstream archivo(ruta);
        std::string linea;
        if (archivo.is_open()) {
            while (getline(archivo, linea)) {
                contenido.push_back(linea);
            }
            archivo.close();
        } else {
            contenido.push_back("El archivo no fue encontrado.");
        }
    }

    std::string mostrarContenido() const {
        std::string contenido_str;
        for (const auto& linea : contenido) {
            contenido_str += linea + "\n";
        }
        return contenido_str;
    }
};

// Clase Frame para manejar cada frame del buffer pool
class Frame {
private:
    Pagina pagina; // Solo una página por frame

public:
    bool estaVacio() const {
        return pagina.mostrarContenido().empty();
    }

    void agregarPagina(const std::string& nombre_archivo) {
        if (estaVacio()) {
            pagina = Pagina(nombre_archivo);
        } else {
            std::cout << "El frame ya tiene una página cargada. No se puede agregar más páginas." << std::endl;
        }
    }

    void mostrarPagina() {
        if (!estaVacio()) {
            std::cout << pagina.mostrarContenido();
        } else {
            std::cout << "El frame está vacío." << std::endl;
        }
    }
};

// Clase BufferPool para manejar los frames
class BufferPool {
private:
    std::vector<Frame> frames;

public:
    BufferPool(size_t num_frames) {
        frames.reserve(num_frames);
        for (size_t i = 0; i < num_frames; ++i) {
            frames.emplace_back();
        }
    }

    void cargarPaginaAlFrame(size_t numero_frame, const std::string& nombre_archivo) {
        if (numero_frame >= 0 && numero_frame < frames.size()) {
            frames[numero_frame].agregarPagina(nombre_archivo);
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }

    void mostrarContenidoFrame(int numero_frame) {
        if (numero_frame >= 0 && numero_frame < frames.size()) {
            std::cout << "Contenido del Frame " << numero_frame << ":" << std::endl;
            frames[numero_frame].mostrarPagina();
        } else {
            std::cout << "Número de frame inválido." << std::endl;
        }
    }
};


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
    size_t numeroFrames;
    std::cout << "Ingrese el número de frames: ";
    std::cin >> numeroFrames;
    int opcion;
    BufferPool bufferPool(numeroFrames);

    while (opcion != 3) {
        size_t frame;
        std::string nombrePagina;
        std::cout << "Ingrese el número de frame (0-" << numeroFrames - 1 << ") donde cargar la página (o -1 para salir): ";
        std::cin >> frame;
        if (frame == -1) break;

        std::cout << "Ingrese el nombre del archivo de la página: ";
        std::cin >> nombrePagina;

        bufferPool.cargarPaginaAlFrame(frame, nombrePagina);

        cout << "Escriba 3 para salir: "; 
        cin >> opcion;
    }

    int frameMostrar;
    std::cout << "Ingrese el número de frame que desea mostrar: ";
    std::cin >> frameMostrar;
    bufferPool.mostrarContenidoFrame(frameMostrar);

    return 0;
}
