// TopicMarkov — generador de texto por tema con cadenas de Márkov de segundo orden.
// Uso: topicmarkov generar --datos <archivo.tsv> --cat <World|Sports|Business|SciTech> [--seed N] [--max N]
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include "Errores.h"
#include "Generador.h"
#include "LectorDatos.h"
#include "Metrica.h"
#include "ModeloMarkov.h"
#include "ModeloMezcla.h"
#include "Tokenizador.h"

using namespace tmk;

namespace {

void uso() {
    std::cerr << "Uso:\n"
                 "  topicmarkov generar --datos <archivo.tsv> --cat <World|Sports|Business|SciTech>"
                 " [--seed N] [--max N]\n"
                 "  topicmarkov entrenar | guardar | cargar | evaluar | benchmark   (pendientes)\n";
}

std::map<std::string, std::string> leerOpciones(int argc, char** argv, int desde) {
    std::map<std::string, std::string> op;
    for (int i = desde; i < argc; ++i) {
        std::string k = argv[i];
        if (k.rfind("--", 0) != 0 || i + 1 >= argc) throw MarkovError("opcion invalida: " + k);
        op[k.substr(2)] = argv[++i];
    }
    return op;
}

unsigned leerEntero(const std::map<std::string, std::string>& op, const std::string& k, unsigned def) {
    auto it = op.find(k);
    if (it == op.end()) return def;
    try {
        std::size_t usado = 0;
        unsigned long v = std::stoul(it->second, &usado);
        if (usado != it->second.size()) throw std::invalid_argument(k);
        return static_cast<unsigned>(v);
    } catch (const std::exception&) {
        throw MarkovError("--" + k + " debe ser un entero no negativo");
    }
}

std::string texto(const std::vector<Id>& ids, const Vocabulario& voc) {
    std::string s;
    for (Id id : ids) {
        const std::string& w = voc.palabra(id);
        bool punt = (w == "." || w == "!" || w == "?");
        if (!s.empty() && !punt) s += ' ';
        s += w;
    }
    return s;
}

int generar(const std::map<std::string, std::string>& op) {
    if (!op.count("datos") || !op.count("cat")) throw MarkovError("faltan --datos o --cat");
    Categoria cat = categoriaDesdeTexto(op.at("cat"));
    if (cat == Categoria::General) throw CategoriaError("General (elige un tema)");

    ConfigGeneracion cfg;
    cfg.semilla = leerEntero(op, "seed", 42);
    cfg.maxPalabras = static_cast<int>(leerEntero(op, "max", 100));

    LectorDatos lector;
    auto ejemplos = lector.leer(op.at("datos"));

    Vocabulario voc;
    Tokenizador tok;
    ModeloMarkov temas[kNumTemas] = {ModeloMarkov("World"), ModeloMarkov("Sports"),
                                     ModeloMarkov("Business"), ModeloMarkov("SciTech")};
    for (const auto& e : ejemplos) {
        std::vector<Id> ids{Vocabulario::kInicio, Vocabulario::kInicio};
        for (const auto& w : tok.tokenizar(e.texto)) ids.push_back(voc.idDe(w));
        ids.push_back(Vocabulario::kFin);
        temas[static_cast<int>(e.categoria)].entrenar(ids);
    }
    ModeloMarkov general("General");
    for (const auto& m : temas) general += m;   // General = suma de los 4 temas

    const ModeloMarkov& tema = temas[static_cast<int>(cat)];
    ModeloMezcla mezcla(tema, general, 0.8);

    std::cout << "Ejemplos: " << ejemplos.size() << " (lineas invalidas: " << lector.lineasInvalidas()
              << "), vocabulario: " << voc.tamano() << ", contextos General: " << general.numContextos()
              << ", semilla: " << cfg.semilla << "\n\n";

    Generador gen(cfg);
    const ModeloTexto* modelos[] = {&tema, &general, &mezcla};
    for (const ModeloTexto* m : modelos) {
        auto pasaje = gen.generar(*m);
        std::cout << "== " << m->nombre() << " ==\n" << texto(pasaje, voc) << "\n"
                  << "palabras: " << pasaje.size() << ", bigramas repetidos: " << std::fixed
                  << std::setprecision(3) << tasaBigramasRepetidos(pasaje) << "\n\n";
    }
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        uso();
        return 2;
    }
    try {
        std::string cmd = argv[1];
        auto op = leerOpciones(argc, argv, 2);
        if (cmd == "generar") return generar(op);
        if (cmd == "entrenar" || cmd == "guardar" || cmd == "cargar" || cmd == "evaluar" ||
            cmd == "benchmark") {
            std::cerr << "Comando '" << cmd << "' pendiente (ver plan semanal en README).\n";
            return 3;
        }
        uso();
        return 2;
    } catch (const MarkovError& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado: " << e.what() << "\n";
        return 1;
    }
}
