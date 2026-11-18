# TopicMarkov

Generador de texto por tema con cadenas de Márkov de segundo orden, en C++17.
Proyecto final de **COM 0060 · Programación Avanzada** — Luis Franco.

Se entrenan cinco modelos con el dataset [AG News](https://huggingface.co/datasets/fancyzhx/ag_news):
uno por categoría (World, Sports, Business, SciTech) y uno General con todo el texto.
Para una categoría se generan tres pasajes de máximo 100 palabras: solo tema, solo General
y una mezcla fija `0.8 · P_tema + 0.2 · P_general`.

**Pregunta:** ¿los modelos entrenados por tema generan texto más relevante al tema que un único modelo general?

## Compilar

Requiere CMake ≥ 3.16 y un compilador con C++17.

```bash
cmake -S . -B build
cmake --build build -j
./build/tests
```

Con AddressSanitizer y UBSan:

```bash
cmake -S . -B build-asan -DTM_ASAN=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-asan -j
./build-asan/tests
```

## Preparar los datos

El programa en C++ lee un archivo ya preparado: una línea por ejemplo, `categoria<TAB>texto`.
La conversión se hace una sola vez con Python (fuera del alcance del programa en C++):

```bash
pip install datasets
python3 scripts/preparar_datos.py --salida data/ag_news_8000.tsv                 # 2,000 por categoría
python3 scripts/preparar_datos.py --por-categoria 0 --salida data/ag_news_full.tsv  # 120,000, para el benchmark
```

Los archivos generados no se suben al repositorio (revisa los términos del dataset antes de redistribuirlo).
`data/muestra.tsv` es un archivo pequeño escrito a mano para pruebas, con dos líneas inválidas a propósito.

## Uso

```bash
./build/topicmarkov generar --datos data/ag_news_8000.tsv --cat Sports --seed 42
./build/topicmarkov generar --datos data/muestra.tsv --cat Sports --seed 3 --max 50
```

## Estructura

```
include/
  TablaHash.h       plantilla propia TablaHash<K, V, Hash> (new[]/delete[], regla de tres, rehash)
  Vocabulario.h     palabra <-> id entero
  Tokenizador.h     minúsculas, separación por espacios y puntuación, conserva . ! ?
  LectorDatos.h     lectura del .tsv preparado, categorías
  Distribucion.h    siguiente palabra; operadores +, *, +=, <<
  ModeloTexto.h     clase abstracta (destructor virtual, siguiente() virtual pura)
  ModeloMarkov.h    cadena de segundo orden; operadores += y []
  ModeloMezcla.h    mezcla fija de dos ModeloTexto
  Generador.h       generación con semilla fija
  Metrica.h         tasaBigramasRepetidos()
  Errores.h         MarkovError -> DatosError, CategoriaError, ContextoError, ModeloCorruptoError
src/                implementaciones y main.cpp (CLI)
tests/              pruebas unitarias
scripts/            preparación de datos
docs/               anteproyecto y reporte
bench/              resultados del benchmark
```

## Estado y plan

| Periodo | Meta | Estado |
| --- | --- | --- |
| Semana 1 (17–23 nov) | TablaHash con pruebas, tokenizador, 5 modelos en un hilo, `generar` | Base lista; falta límite de frases repetidas y correr con los 8,000 ejemplos |
| Semana 2 (24–30 nov) | Mezcla 80/20, excepciones + 8 entradas inválidas, archivo `.tmk` con carga por categoría, entrenamiento con k hilos, ASan | Mezcla y excepciones base listas |
| Cierre (1–3 dic) | Benchmark 1/2/4/8 hilos, 60 pasajes evaluados (4 categorías × 3 métodos × 5), reporte, tag final | Pendiente |

## Limitaciones

El modelo solo usa las dos palabras anteriores. Imita patrones locales de las noticias, pero no sigue hechos
ni significado a lo largo del pasaje; el texto puede ser repetitivo o incoherente.
