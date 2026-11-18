#include "Generador.h"
#include "Errores.h"

namespace tmk {

std::vector<Id> Generador::generar(const ModeloTexto& modelo) const {
    std::mt19937 rng(cfg_.semilla);
    Contexto ctx{Vocabulario::kInicio, Vocabulario::kInicio};
    if (modelo.siguiente(ctx).vacia())
        throw ContextoError("el modelo '" + modelo.nombre() + "' no tiene contexto inicial");

    std::vector<Id> pasaje;
    while (static_cast<int>(pasaje.size()) < cfg_.maxPalabras) {
        Distribucion d = modelo.siguiente(ctx);
        if (d.vacia()) break;
        Id w = d.muestrear(rng);
        if (w == Vocabulario::kFin) break;
        pasaje.push_back(w);
        ctx = Contexto{ctx.b, w};
    }
    // TODO (semana 1): límite de frases repetidas.
    return pasaje;
}

} // namespace tmk
