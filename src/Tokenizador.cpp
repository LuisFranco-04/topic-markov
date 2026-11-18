#include "Tokenizador.h"
#include <cctype>

namespace tmk {

std::vector<std::string> Tokenizador::tokenizar(const std::string& texto) const {
    std::vector<std::string> tokens;
    std::string actual;
    auto cerrar = [&]() {
        if (!actual.empty()) {
            tokens.push_back(actual);
            actual.clear();
        }
    };
    for (unsigned char c : texto) {
        if (std::isalnum(c) || c == '\'' || c >= 0x80) {
            actual.push_back(static_cast<char>(std::tolower(c)));
        } else if (c == '.' || c == '!' || c == '?') {
            cerrar();
            tokens.emplace_back(1, static_cast<char>(c));
        } else {
            cerrar();  // espacios y demás puntuación separan
        }
    }
    cerrar();
    return tokens;
}

} // namespace tmk
