#pragma once
// Genera un pasaje desde cualquier ModeloTexto.
#include <random>
#include <vector>
#include "ModeloTexto.h"

namespace tmk {

struct ConfigGeneracion {
    int maxPalabras = 100;
    unsigned semilla = 42;
};

class Generador {
public:
    explicit Generador(ConfigGeneracion cfg = {}) : cfg_(cfg) {}

    // Empieza en (<s>, <s>); se detiene en </s> o al llegar a maxPalabras.
    // Lanza ContextoError si el modelo no conoce el contexto inicial.
    std::vector<Id> generar(const ModeloTexto& modelo) const;

private:
    ConfigGeneracion cfg_;
};

} // namespace tmk
