#include "ModeloMarkov.h"

namespace tmk {

ModeloMarkov::ModeloMarkov(std::string nombre) : nombre_(std::move(nombre)) {}

void ModeloMarkov::entrenar(const std::vector<Id>& ids) {
    for (std::size_t i = 2; i < ids.size(); ++i)
        tabla_[Contexto{ids[i - 2], ids[i - 1]}].agregar(ids[i]);
}

Distribucion ModeloMarkov::siguiente(const Contexto& ctx) const {
    const Distribucion* d = tabla_.buscar(ctx);
    return d ? d->normalizada() : Distribucion{};
}

ModeloMarkov& ModeloMarkov::operator+=(const ModeloMarkov& otro) {
    otro.tabla_.paraCada([this](const Contexto& c, const Distribucion& d) { tabla_[c] += d; });
    return *this;
}

} // namespace tmk
