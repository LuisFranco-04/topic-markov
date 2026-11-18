#pragma once
// Lee el archivo preparado: una línea por ejemplo, "categoria<TAB>texto".
#include <string>
#include <vector>

namespace tmk {

enum class Categoria { World = 0, Sports = 1, Business = 2, SciTech = 3, General = 4 };
constexpr int kNumTemas = 4;

Categoria categoriaDesdeTexto(const std::string& s);   // lanza CategoriaError
const char* nombreCategoria(Categoria c);

struct Ejemplo {
    Categoria categoria;
    std::string texto;
};

class LectorDatos {
public:
    // Lanza DatosError si el archivo no existe. Las líneas mal formadas se cuentan y se saltan.
    std::vector<Ejemplo> leer(const std::string& ruta);
    std::size_t lineasInvalidas() const { return invalidas_; }

private:
    std::size_t invalidas_ = 0;
};

} // namespace tmk
