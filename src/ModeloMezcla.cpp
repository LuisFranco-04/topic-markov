#include "ModeloMezcla.h"
#include <sstream>

namespace tmk {

ModeloMezcla::ModeloMezcla(const ModeloTexto& tema, const ModeloTexto& general, double pesoTema)
    : tema_(&tema), general_(&general), pesoTema_(pesoTema) {}

Distribucion ModeloMezcla::siguiente(const Contexto& ctx) const {
    // Un modelo que no conoce el contexto devuelve una distribución vacía (probabilidad 0).
    Distribucion mezcla = pesoTema_ * tema_->siguiente(ctx) + (1.0 - pesoTema_) * general_->siguiente(ctx);
    return mezcla.normalizada();
}

std::string ModeloMezcla::nombre() const {
    std::ostringstream os;
    os << "Mezcla(" << tema_->nombre() << " " << pesoTema_ << " / " << general_->nombre() << " "
       << 1.0 - pesoTema_ << ")";
    return os.str();
}

} // namespace tmk
