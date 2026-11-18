#pragma once
// Mezcla fija de dos expertos: P = w * P_tema + (1 - w) * P_general.
// Un contexto que un modelo no conoce aporta probabilidad cero.
#include "ModeloTexto.h"

namespace tmk {

class ModeloMezcla : public ModeloTexto {
public:
    ModeloMezcla(const ModeloTexto& tema, const ModeloTexto& general, double pesoTema = 0.8);

    Distribucion siguiente(const Contexto& ctx) const override;
    std::string nombre() const override;

private:
    const ModeloTexto* tema_;      // no es dueño
    const ModeloTexto* general_;   // no es dueño
    double pesoTema_;
};

} // namespace tmk
