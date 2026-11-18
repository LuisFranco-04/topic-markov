#pragma once
// ModeloTexto: interfaz abstracta. El Generador solo conoce esta clase,
// así que modelo de tema, General y mezcla usan el mismo código.
#include <functional>
#include <string>
#include "Distribucion.h"

namespace tmk {

struct Contexto {
    Id a, b;
    bool operator==(const Contexto& o) const { return a == o.a && b == o.b; }
};

struct HashContexto {
    std::size_t operator()(const Contexto& c) const {
        return std::hash<long long>{}((static_cast<long long>(c.a) << 32) ^ static_cast<unsigned>(c.b));
    }
};

class ModeloTexto {
public:
    virtual ~ModeloTexto() = default;
    // Distribución normalizada de la siguiente palabra; vacía si no conoce el contexto.
    virtual Distribucion siguiente(const Contexto& ctx) const = 0;
    virtual std::string nombre() const = 0;
};

} // namespace tmk
