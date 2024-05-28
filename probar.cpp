BufferManager(size_t num_frames) : bufferPool(num_frames) {
    try {
        if (!fs::exists("Bloques") || !fs::is_directory("Bloques")) {
            std::cerr << "El directorio 'Bloques' no existe o no es un directorio válido." << std::endl;
            return;
        }

        int archivos_procesados = 0;

        for (const auto& entry : fs::directory_iterator("Bloques")) {
            if (archivos_procesados >= 16) // Salir después de procesar 16 archivos
                break;

            if (entry.is_regular_file() && entry.path().extension() == ".txt") { // Verificar la extensión del archivo
                std::string filename = entry.path().filename().stem().string();
                int numero_pagina = std::stoi(filename.substr(7));
                archivo_a_numero[filename] = numero_pagina;
                archivos_procesados++;
            }
        }

        // Mostrar los archivos y sus números de página
        for (const auto& pair : archivo_a_numero) {
            std::cout << "Archivo: " << pair.first << ", Numero de pagina: " << pair.second << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
