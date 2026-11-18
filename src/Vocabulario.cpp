#include "Vocabulario.h"
#include "Errores.h"

namespace tmk {

Vocabulario::Vocabulario() {
    idDe("<s>");
    idDe("</s>");
}

Id Vocabulario::idDe(const std::string& palabra) {
    if (const Id* id = ids_.buscar(palabra)) return *id;
    Id nuevo = static_cast<Id>(palabras_.size());
    ids_.insertar(palabra, nuevo);
    palabras_.push_back(palabra);
    return nuevo;
}

Id Vocabulario::buscar(const std::string& palabra) const {
    const Id* id = ids_.buscar(palabra);
    return id ? *id : -1;
}

const std::string& Vocabulario::palabra(Id id) const {
    if (id < 0 || static_cast<std::size_t>(id) >= palabras_.size())
        throw MarkovError("Id fuera de rango: " + std::to_string(id));
    return palabras_[id];
}

} // namespace tmk
