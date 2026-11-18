// Pruebas unitarias mínimas sin dependencias externas.
#include <cmath>
#include <iostream>
#include <string>
#include "Errores.h"
#include "Generador.h"
#include "Metrica.h"
#include "ModeloMarkov.h"
#include "ModeloMezcla.h"
#include "TablaHash.h"
#include "Tokenizador.h"

using namespace tmk;

static int fallas = 0;
#define CHECK(cond)                                                              \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::cerr << __FILE__ << ":" << __LINE__ << " FALLA: " #cond "\n";   \
            ++fallas;                                                            \
        }                                                                        \
    } while (0)

static void pruebaTablaHash() {
    TablaHash<std::string, int> t(2);
    for (int i = 0; i < 1000; ++i) t["k" + std::to_string(i)] = i;
    CHECK(t.tamano() == 1000);
    CHECK(t.capacidad() >= 1000 / 0.75);
    CHECK(*t.buscar("k500") == 500);
    CHECK(t.buscar("nope") == nullptr);

    TablaHash<std::string, int> copia(t);       // constructor de copia
    copia["k1"] = -1;
    CHECK(*t.buscar("k1") == 1);
    TablaHash<std::string, int> asignada;
    asignada = copia;                            // asignación
    CHECK(*asignada.buscar("k1") == -1);
    CHECK(asignada.tamano() == 1000);
}

static void pruebaTokenizador() {
    Tokenizador tok;
    auto t = tok.tokenizar("The Team WON, today!");
    CHECK(t.size() == 5);
    CHECK(t[0] == "the" && t[2] == "won" && t[4] == "!");
}

static void pruebaModeloYMezcla() {
    // (<s>,<s>) -> 2 ; (<s>,2) -> 3 x2, 4 x1 en el tema
    ModeloMarkov tema("T"), otro("O");
    tema.entrenar({0, 0, 2, 3, 1});
    tema.entrenar({0, 0, 2, 3, 1});
    tema.entrenar({0, 0, 2, 4, 1});
    otro.entrenar({0, 0, 5, 1});

    Distribucion d = tema.siguiente({0, 2});
    CHECK(std::fabs(d.total() - 1.0) < 1e-9);
    CHECK(std::fabs(d.entradas()[0].second - 2.0 / 3.0) < 1e-9);

    ModeloMarkov general("G");
    general += tema;
    general += otro;
    CHECK(general.numContextos() == tema.numContextos() + 1);  // (<s>,5) es nuevo

    // Contexto (<s>,2) solo existe en tema: la mezcla queda igual al tema.
    ModeloMezcla mezcla(tema, otro, 0.8);
    Distribucion m = mezcla.siguiente({0, 2});
    CHECK(std::fabs(m.entradas()[0].second - 2.0 / 3.0) < 1e-9);

    // Misma semilla -> mismo pasaje.
    Generador g({100, 7});
    CHECK(g.generar(general) == g.generar(general));

    bool lanzo = false;
    try {
        Generador(ConfigGeneracion{}).generar(ModeloMarkov("vacio"));
    } catch (const ContextoError&) {
        lanzo = true;
    }
    CHECK(lanzo);
}

static void pruebaMetrica() {
    CHECK(std::fabs(tasaBigramasRepetidos({1, 2, 1, 2}) - 1.0 / 3.0) < 1e-9);  // (1,2) se repite 1 de 3
    CHECK(tasaBigramasRepetidos({7}) == 0.0);
}

int main() {
    pruebaTablaHash();
    pruebaTokenizador();
    pruebaModeloYMezcla();
    pruebaMetrica();
    if (fallas == 0) std::cout << "Todas las pruebas pasaron\n";
    return fallas == 0 ? 0 : 1;
}
