#include "Distribucion.h"
#include <algorithm>

namespace tmk {

void Distribucion::agregar(Id siguiente, double peso) {
    auto it = std::lower_bound(entradas_.begin(), entradas_.end(), siguiente,
                               [](const std::pair<Id, double>& e, Id id) { return e.first < id; });
    if (it != entradas_.end() && it->first == siguiente)
        it->second += peso;
    else
        entradas_.insert(it, {siguiente, peso});
    total_ += peso;
}

Distribucion Distribucion::normalizada() const {
    Distribucion d;
    if (total_ <= 0.0) return d;
    d.entradas_ = entradas_;
    for (auto& e : d.entradas_) e.second /= total_;
    d.total_ = 1.0;
    return d;
}

Id Distribucion::muestrear(std::mt19937& rng) const {
    std::uniform_real_distribution<double> u(0.0, total_);
    double r = u(rng), acum = 0.0;
    for (const auto& e : entradas_) {
        acum += e.second;
        if (r < acum) return e.first;
    }
    return entradas_.back().first;
}

Distribucion& Distribucion::operator+=(const Distribucion& otra) {
    for (const auto& e : otra.entradas_) agregar(e.first, e.second);
    return *this;
}

Distribucion operator*(Distribucion d, double w) {
    for (auto& e : d.entradas_) e.second *= w;
    d.total_ *= w;
    return d;
}

std::ostream& operator<<(std::ostream& os, const Distribucion& d) {
    os << "{";
    bool primero = true;
    for (const auto& e : d.entradas_) {
        os << (primero ? "" : ", ") << e.first << ":" << e.second;
        primero = false;
    }
    return os << "} total=" << d.total_;
}

} // namespace tmk
