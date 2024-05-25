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

// Clase BufferPool para manejar los frames   l
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

    size_t buscarFrameVacio() const {
        for (size_t i = 0; i < frames.size(); ++i) {
            if (frames[i].estaVacio()) {
                return i;
            }
        }
        return frames.size(); // No hay frames vacíos
    }

    size_t getNumFrames() const {
        return frames.size();
    }

    Frame* obtenerFrame(size_t numero_frame) {
        if (numero_frame >= 0 && numero_frame < frames.size()) {
            return &frames[numero_frame];
        } else {
            std::cout << "Número de frame inválido." << std::endl;
            return nullptr;
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


class BufferManager {
private:
    BufferPool bufferPool;
    PageTable pageTable;
    std::unordered_map<std::string, int> archivo_a_numero;

public:
    BufferManager(size_t num_frames) : bufferPool(num_frames) {}

    void cargarPaginaEnBufferPool(size_t frame_num, const std::string& nombre_archivo) {
        // Verificar si el archivo ya tiene un número asignado
        int numPagina;
        auto it = archivo_a_numero.find(nombre_archivo);
        if (it != archivo_a_numero.end()) {
            // Si ya existe, obtener el número de página asociado
            numPagina = it->second;
        } else {
            // Si no existe, asignar un nuevo número de página
            numPagina = archivo_a_numero.size() + 1; // Comenzar desde 1
            archivo_a_numero[nombre_archivo] = numPagina;
        }

        // Verificar si el frame está vacío antes de cargar la página
        if (bufferPool.obtenerFrame(frame_num)->estaVacio()) {
            // Cargar la página en el buffer pool
            bufferPool.cargarPaginaAlFrame(frame_num, nombre_archivo);

            // Actualizar la tabla de páginas
            pageTable.actualizarDatos(numPagina, frame_num);
        } else {
            std::cout << "El frame " << frame_num << " ya está ocupado. No se puede cargar la página." << std::endl;
        }
    }

    void consultarPagina(int numPagina) {
        // Verificar si la página existe en la tabla de páginas
        if (pageTable.verificarExistenciaDePagina(numPagina)) {
            // Incrementar el contador de PinCount
            pageTable.aumentarPinCount(numPagina);
            std::cout << "El contador de PinCount para la página " << numPagina << " ha sido incrementado." << std::endl;
        } else {
            std::cout << "La página " << numPagina << " no se encuentra en la tabla de páginas." << std::endl;
        }
    }


    void mostrarContenidoFrame(int numFrame) {
        bufferPool.mostrarContenidoFrame(numFrame);
    }

    void mostrarTablaDePaginas() {
        pageTable.mostrarTabla();
    }
};

int main() {
    BufferManager* bufferManager = nullptr;

    int opcion = 0;

    while (opcion != 6) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Crear BufferPool\n";
        std::cout << "2. Insertar pagina en BufferPool\n";
        std::cout << "3. Consultar página en BufferPool\n";
        std::cout << "4. Mostrar contenido de un frame\n";
        std::cout << "5. Mostrar tabla de páginas\n";
        std::cout << "6. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1: {
                size_t num_frames;
                std::cout << "Ingrese el número de frames para el BufferPool: ";
                std::cin >> num_frames;
                bufferManager = new BufferManager(num_frames);
                std::cout << "BufferPool creado con " << num_frames << " frames." << std::endl;
                break;
            }
            case 2: {
                if (!bufferManager) {
                    std::cout << "Primero debe crear el BufferPool." << std::endl;
                } else {
                    size_t frame_num;
                    std::string nombre_archivo;
                    std::cout << "Ingrese el número de frame: ";
                    std::cin >> frame_num;
                    std::cout << "Ingrese el nombre del archivo: ";
                    std::cin >> nombre_archivo;
                    bufferManager->cargarPaginaEnBufferPool(frame_num, nombre_archivo);
                }
                break;
            }

            case 3: {
                int pagina;
                cout << "ingrese numero de pagina: "; cin >> pagina;
                bufferManager->consultarPagina(pagina);
                break;
            }
            
            case 4: {
                if (!bufferManager) {
                    std::cout << "Primero debe crear el BufferPool." << std::endl;
                } else {
                    int numFrame;
                    std::cout << "Ingrese el número de frame para mostrar su contenido: ";
                    std::cin >> numFrame;
                    bufferManager->mostrarContenidoFrame(numFrame);
                }
                break;
            }
            case 5: {
                if (!bufferManager) {
                    std::cout << "Primero debe crear el BufferPool." << std::endl;
                } else {
                    bufferManager->mostrarTablaDePaginas();
                }
                break;
            }
            case 6: {
                std::cout << "Saliendo..." << std::endl;
                delete bufferManager;
                break;
            }
            default: {
                std::cout << "Opción no válida, por favor intente de nuevo." << std::endl;
            }
        }
    }

    return 0;
}
