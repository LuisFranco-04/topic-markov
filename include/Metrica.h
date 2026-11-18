#pragma once
// Métrica automática sobre un pasaje generado.
#include <vector>
#include "Vocabulario.h"

namespace tmk {

// Bigramas repetidos / total de bigramas adyacentes (0 si el pasaje tiene menos de 2 palabras).
double tasaBigramasRepetidos(const std::vector<Id>& pasaje);

} // namespace tmk
