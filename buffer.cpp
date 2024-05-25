#include <iostream>
#include <fstream>
#include <vector>

#define PAGE_SIZE 4096 // Tamaño de página por defecto

struct PageFrame {
    int frameNum;
    int pageNum;
    bool dirtyFlag;
    int fixCount;
    char *data;

    PageFrame(int frameNum) : frameNum(frameNum), pageNum(-1), dirtyFlag(false), fixCount(0), data(new char[PAGE_SIZE]()) {}
};

struct BufferPool {
    std::vector<PageFrame*> frames;
    int numPages;
    int occupiedCount;
    int numRead;
    int numWrite;

    BufferPool(int numPages) : numPages(numPages), occupiedCount(0), numRead(0), numWrite(0) {
        for (int i = 0; i < numPages; ++i) {
            frames.push_back(new PageFrame(i));
        }
    }

    ~BufferPool() {
        for (auto frame : frames) {
            delete[] frame->data;
            delete frame;
        }
    }
};

// Función para simular la lectura de datos de una página desde un archivo
void readData(BufferPool& bufferPool, const char* fileName, int pageNum) {
    if (pageNum >= 0 && pageNum < bufferPool.numPages) {
        // Incrementar el contador de lecturas
        bufferPool.numRead++;

        // Abrir el archivo
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo." << std::endl;
            return;
        }

        // Leer los datos de la página
        std::string line;
        for (int i = 0; i <= pageNum; ++i) {
            if (!std::getline(file, line)) {
                std::cerr << "Error: No se encontró la página solicitada en el archivo." << std::endl;
                return;
            }
        }

        // Simular la lectura de datos de la página
        std::cout << "Leyendo datos de la página " << pageNum << " en el archivo " << fileName << ": " << line << std::endl;
    } else {
        std::cerr << "Error: Número de página inválido." << std::endl;
    }
}

int main() {
    const char* pageFileName1 = "data1.txt";
    const char* pageFileName2 = "data2.txt";
    int numPages = 5;

    // Inicializar el buffer pool
    BufferPool bufferPool(numPages);

    // Ejemplo de uso: leer datos de la página 2 del primer archivo
    readData(bufferPool, pageFileName1, 2);

    // Ejemplo de uso: leer datos de la página 3 del segundo archivo
    readData(bufferPool, pageFileName2, 3);

    return 0;
}
