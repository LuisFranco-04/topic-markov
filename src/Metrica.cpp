#include "Metrica.h"
#include "ModeloTexto.h"
#include "TablaHash.h"

namespace tmk {

double tasaBigramasRepetidos(const std::vector<Id>& p) {
    if (p.size() < 2) return 0.0;
    TablaHash<Contexto, int, HashContexto> vistos;
    std::size_t repetidos = 0, total = p.size() - 1;
    for (std::size_t i = 1; i < p.size(); ++i) {
        int& n = vistos[Contexto{p[i - 1], p[i]}];
        if (n > 0) ++repetidos;
        ++n;
    }
    return static_cast<double>(repetidos) / total;
}

} // namespace tmk
