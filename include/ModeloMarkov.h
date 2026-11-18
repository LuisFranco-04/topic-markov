#pragma once
// Cadena de Márkov de segundo orden: (w1, w2) -> conteos de w3.
#include <vector>
#include "ModeloTexto.h"
#include "TablaHash.h"

namespace tmk {

class ModeloMarkov : public ModeloTexto {
public:
    explicit ModeloMarkov(std::string nombre = "General");

    // Entrena con una secuencia de ids ya envuelta en <s> <s> ... </s>.
    void entrenar(const std::vector<Id>& ids);

    Distribucion siguiente(const Contexto& ctx) const override;
    std::string nombre() const override { return nombre_; }

    const Distribucion* operator[](const Contexto& ctx) const { return tabla_.buscar(ctx); }
    ModeloMarkov& operator+=(const ModeloMarkov& otro);   // fusiona conteos

    std::size_t numContextos() const { return tabla_.tamano(); }

private:
    std::string nombre_;
    TablaHash<Contexto, Distribucion, HashContexto> tabla_;
};

} // namespace tmk
