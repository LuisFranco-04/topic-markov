#pragma once
// Distribucion: probabilidades (o conteos) de la siguiente palabra.
// Operadores: d * w, d1 + d2 (mezcla), d1 += d2 (fusión de conteos), <<.
#include <ostream>
#include <random>
#include <utility>
#include <vector>
#include "Vocabulario.h"

namespace tmk {

class Distribucion {
public:
    void agregar(Id siguiente, double peso = 1.0);
    double total() const { return total_; }
    bool vacia() const { return entradas_.empty(); }
    const std::vector<std::pair<Id, double>>& entradas() const { return entradas_; }

    Distribucion normalizada() const;          // pesos que suman 1
    Id muestrear(std::mt19937& rng) const;     // muestreo proporcional al peso

    Distribucion& operator+=(const Distribucion& otra);
    friend Distribucion operator+(Distribucion a, const Distribucion& b) { return a += b; }
    friend Distribucion operator*(Distribucion d, double w);
    friend Distribucion operator*(double w, const Distribucion& d) { return d * w; }
    friend std::ostream& operator<<(std::ostream& os, const Distribucion& d);

private:
    std::vector<std::pair<Id, double>> entradas_;  // ordenado por Id
    double total_ = 0.0;
};

} // namespace tmk
