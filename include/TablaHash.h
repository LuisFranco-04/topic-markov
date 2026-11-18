#pragma once
// TablaHash<K, V, Hash>: tabla hash genérica propia con encadenamiento.
// Cubetas en new[]/delete[], regla de tres, rehash al pasar factor de carga 0.75.
#include <cstddef>
#include <functional>
#include <utility>

namespace tmk {

template <typename K, typename V, typename Hash = std::hash<K>>
class TablaHash {
    struct Nodo {
        K clave;
        V valor;
        Nodo* sig;
        Nodo(const K& k, const V& v, Nodo* s) : clave(k), valor(v), sig(s) {}
    };

public:
    explicit TablaHash(std::size_t capacidad = 16)
        : cubetas_(nullptr), capacidad_(capacidad < 1 ? 1 : capacidad), tam_(0) {
        cubetas_ = new Nodo*[capacidad_]();
    }

    ~TablaHash() { liberar(); }

    TablaHash(const TablaHash& otra)
        : cubetas_(nullptr), capacidad_(otra.capacidad_), tam_(0) {
        cubetas_ = new Nodo*[capacidad_]();
        otra.paraCada([this](const K& k, const V& v) { insertar(k, v); });
    }

    TablaHash& operator=(const TablaHash& otra) {
        if (this != &otra) {
            TablaHash copia(otra);   // copy-and-swap
            intercambiar(copia);
        }
        return *this;
    }

    // Inserta o sobrescribe.
    void insertar(const K& clave, const V& valor) { (*this)[clave] = valor; }

    // Devuelve referencia al valor; lo crea con V{} si no existe.
    V& operator[](const K& clave) {
        Nodo* n = buscarNodo(clave);
        if (n) return n->valor;
        if (static_cast<double>(tam_ + 1) / capacidad_ > kFactorMax) rehash(capacidad_ * 2);
        std::size_t i = indice(clave, capacidad_);
        cubetas_[i] = new Nodo(clave, V{}, cubetas_[i]);
        ++tam_;
        return cubetas_[i]->valor;
    }

    V* buscar(const K& clave) {
        Nodo* n = buscarNodo(clave);
        return n ? &n->valor : nullptr;
    }
    const V* buscar(const K& clave) const {
        const Nodo* n = buscarNodo(clave);
        return n ? &n->valor : nullptr;
    }

    bool contiene(const K& clave) const { return buscarNodo(clave) != nullptr; }
    std::size_t tamano() const { return tam_; }
    std::size_t capacidad() const { return capacidad_; }
    bool vacia() const { return tam_ == 0; }

    // Recorre todos los pares (orden no especificado).
    template <typename F>
    void paraCada(F f) const {
        for (std::size_t i = 0; i < capacidad_; ++i)
            for (const Nodo* n = cubetas_[i]; n; n = n->sig) f(n->clave, n->valor);
    }
    template <typename F>
    void paraCadaMut(F f) {
        for (std::size_t i = 0; i < capacidad_; ++i)
            for (Nodo* n = cubetas_[i]; n; n = n->sig) f(n->clave, n->valor);
    }

    void intercambiar(TablaHash& o) noexcept {
        std::swap(cubetas_, o.cubetas_);
        std::swap(capacidad_, o.capacidad_);
        std::swap(tam_, o.tam_);
    }

private:
    static constexpr double kFactorMax = 0.75;

    std::size_t indice(const K& k, std::size_t cap) const { return Hash{}(k) % cap; }

    Nodo* buscarNodo(const K& k) const {
        for (Nodo* n = cubetas_[indice(k, capacidad_)]; n; n = n->sig)
            if (n->clave == k) return n;
        return nullptr;
    }

    void rehash(std::size_t nuevaCap) {
        Nodo** nuevas = new Nodo*[nuevaCap]();
        for (std::size_t i = 0; i < capacidad_; ++i) {
            Nodo* n = cubetas_[i];
            while (n) {
                Nodo* sig = n->sig;
                std::size_t j = indice(n->clave, nuevaCap);
                n->sig = nuevas[j];
                nuevas[j] = n;
                n = sig;
            }
        }
        delete[] cubetas_;
        cubetas_ = nuevas;
        capacidad_ = nuevaCap;
    }

    void liberar() {
        if (!cubetas_) return;
        for (std::size_t i = 0; i < capacidad_; ++i) {
            Nodo* n = cubetas_[i];
            while (n) {
                Nodo* sig = n->sig;
                delete n;
                n = sig;
            }
        }
        delete[] cubetas_;
        cubetas_ = nullptr;
        tam_ = 0;
    }

    Nodo** cubetas_;
    std::size_t capacidad_;
    std::size_t tam_;
};

} // namespace tmk
