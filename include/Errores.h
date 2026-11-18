#pragma once
// Jerarquía de excepciones propia del proyecto.
#include <stdexcept>
#include <string>

namespace tmk {

class MarkovError : public std::runtime_error {
public:
    explicit MarkovError(const std::string& msg) : std::runtime_error(msg) {}
};

// Archivo de datos inexistente o línea mal formada.
class DatosError : public MarkovError {
public:
    explicit DatosError(const std::string& msg) : MarkovError("Datos: " + msg) {}
};

// Categoría que no es World, Sports, Business, SciTech ni General.
class CategoriaError : public MarkovError {
public:
    explicit CategoriaError(const std::string& cat)
        : MarkovError("Categoria desconocida: " + cat) {}
};

// No existe un contexto inicial válido para generar.
class ContextoError : public MarkovError {
public:
    explicit ContextoError(const std::string& msg) : MarkovError("Contexto: " + msg) {}
};

// Archivo .tmk con número mágico, versión o tamaño incorrecto.
class ModeloCorruptoError : public MarkovError {
public:
    explicit ModeloCorruptoError(const std::string& msg)
        : MarkovError("Modelo corrupto: " + msg) {}
};

} // namespace tmk
