#include "LectorDatos.h"
#include <fstream>
#include "Errores.h"

namespace tmk {

Categoria categoriaDesdeTexto(const std::string& s) {
    if (s == "World") return Categoria::World;
    if (s == "Sports") return Categoria::Sports;
    if (s == "Business") return Categoria::Business;
    if (s == "SciTech" || s == "Sci/Tech") return Categoria::SciTech;
    if (s == "General") return Categoria::General;
    throw CategoriaError(s);
}

const char* nombreCategoria(Categoria c) {
    switch (c) {
        case Categoria::World: return "World";
        case Categoria::Sports: return "Sports";
        case Categoria::Business: return "Business";
        case Categoria::SciTech: return "SciTech";
        case Categoria::General: return "General";
    }
    return "?";
}

std::vector<Ejemplo> LectorDatos::leer(const std::string& ruta) {
    std::ifstream in(ruta);
    if (!in) throw DatosError("no se puede abrir '" + ruta + "'");

    std::vector<Ejemplo> ejemplos;
    invalidas_ = 0;
    std::string linea;
    while (std::getline(in, linea)) {
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;
        auto tab = linea.find('\t');
        if (tab == std::string::npos || tab + 1 >= linea.size()) {
            ++invalidas_;
            continue;
        }
        try {
            Categoria c = categoriaDesdeTexto(linea.substr(0, tab));
            if (c == Categoria::General) { ++invalidas_; continue; }
            ejemplos.push_back({c, linea.substr(tab + 1)});
        } catch (const CategoriaError&) {
            ++invalidas_;
        }
    }
    if (ejemplos.empty()) throw DatosError("'" + ruta + "' no contiene ejemplos validos");
    return ejemplos;
}

} // namespace tmk
