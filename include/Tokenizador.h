#pragma once
// Normaliza a minúsculas y separa por espacios y puntuación básica.
// Conserva '.', '!' y '?' como tokens de fin de oración.
#include <string>
#include <vector>

namespace tmk {

class Tokenizador {
public:
    std::vector<std::string> tokenizar(const std::string& texto) const;
};

} // namespace tmk
