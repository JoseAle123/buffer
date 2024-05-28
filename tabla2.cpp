#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct PageTableEntry {
    int PageId;
    int FrameId;
    int DirtyBit;
    int PinCount;
    int LastUsed;
};

class PageTable {
private:
    vector<PageTableEntry> entries;

public:
    PageTable(const vector<PageTableEntry>& initialEntries) : entries(initialEntries) {}

    void reemplazarPagina(int newPageId) {
        int minLastUsed = INT_MAX;
        int pageIndexToReplace = -1;

        // Encontrar la página con el menor LastUsed y PinCount igual a 0
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].PinCount == 0 && entries[i].LastUsed < minLastUsed) {
                minLastUsed = entries[i].LastUsed;
                pageIndexToReplace = i;
            }
        }

        // Si no se encontró una página con PinCount igual a 0, buscar la próxima página con el siguiente menor LastUsed
        if (pageIndexToReplace == -1) {
            int minLastUsedWithZeroPinCount = INT_MAX;
            for (size_t i = 0; i < entries.size(); ++i) {
                if (entries[i].PinCount == 0 && entries[i].LastUsed < minLastUsedWithZeroPinCount) {
                    minLastUsedWithZeroPinCount = entries[i].LastUsed;
                    pageIndexToReplace = i;
                }
            }
        }

        // Si se encontró una página para reemplazar, reemplazarla con la nueva página
        if (pageIndexToReplace != -1) {
            // Incrementar LastUsed en 1 solo para el marco de página específico
            entries[pageIndexToReplace].LastUsed++;

            entries[pageIndexToReplace].PageId = newPageId;
            entries[pageIndexToReplace].DirtyBit = 0; // Podrías querer resetear DirtyBit también, dependiendo de tu implementación
            cout << "Se reemplazó la página en el frame " << entries[pageIndexToReplace].FrameId << endl;
        } else {
            cout << "No se puede reemplazar la página, todas las páginas están fijadas (PinCount > 0)." << endl;
        }
    }

    void mostrarTabla() {
        cout << "Frame id\tPageId\tDirtyBit\tPin Count\tLast Used" << endl;
        for (size_t i = 0; i < entries.size(); ++i) {
            cout << entries[i].FrameId << "\t\t" << entries[i].PageId << "\t\t"
                 << entries[i].DirtyBit << "\t\t" << entries[i].PinCount << "\t\t"
                 << entries[i].LastUsed << endl;
        }
        cout << endl;
    }
};

int main() {
    vector<PageTableEntry> initialEntries = {
        {1, 10, 0, 1, 19},
        {2, 11, 0, 0, 2},
        {3, 12, 0, 1, 10}
    };

    PageTable pageTable(initialEntries);

    pageTable.reemplazarPagina(15);

    pageTable.mostrarTabla();

    return 0;
}
