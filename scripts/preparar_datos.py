#!/usr/bin/env python3
"""Convierte AG News (split de entrenamiento) al formato que lee TopicMarkov.

Salida: una línea por ejemplo, "categoria<TAB>titulo. descripcion".
Toma los primeros N ejemplos de cada categoría (por defecto 2000 -> 8000 en total).
Usa --por-categoria 0 para tomar todos (120,000; solo para el benchmark).

Requiere: pip install datasets
Uso:
    python3 scripts/preparar_datos.py --salida data/ag_news_8000.tsv
    python3 scripts/preparar_datos.py --por-categoria 0 --salida data/ag_news_full.tsv
"""
import argparse

NOMBRES = ["World", "Sports", "Business", "SciTech"]  # etiquetas 0..3 en Hugging Face


def limpiar(s: str) -> str:
    return " ".join(s.replace("\t", " ").replace("\\", " ").split())


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("--salida", required=True)
    p.add_argument("--por-categoria", type=int, default=2000)
    a = p.parse_args()

    from datasets import load_dataset

    ds = load_dataset("fancyzhx/ag_news", split="train")  # nunca el split de prueba
    cuentas = [0] * len(NOMBRES)
    limite = a.por_categoria
    with open(a.salida, "w", encoding="utf-8") as f:
        for ej in ds:
            c = int(ej["label"])
            if limite and cuentas[c] >= limite:
                continue
            f.write(f"{NOMBRES[c]}\t{limpiar(ej['text'])}\n")
            cuentas[c] += 1
            if limite and all(n >= limite for n in cuentas):
                break
    print(dict(zip(NOMBRES, cuentas)), "->", a.salida)


if __name__ == "__main__":
    main()
