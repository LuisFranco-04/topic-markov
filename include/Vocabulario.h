#pragma once
// Vocabulario: convierte palabras a ids enteros y viceversa.
#include <string>
#include <vector>
#include "TablaHash.h"

namespace tmk {

using Id = int;

class Vocabulario {
public:
    static constexpr Id kInicio = 0;  // "<s>"
    static constexpr Id kFin = 1;     // "</s>"

    Vocabulario();

    Id idDe(const std::string& palabra);              // la agrega si no existe
    Id buscar(const std::string& palabra) const;      // -1 si no existe
    const std::string& palabra(Id id) const;
    std::size_t tamano() const { return palabras_.size(); }

private:
    TablaHash<std::string, Id> ids_;
    std::vector<std::string> palabras_;
};

} // namespace tmk
