import os
import re
import csv

# Captura:
# #include <vector>
# #include "BoardPosition.h"
include_pattern = re.compile(
    r'^\s*#include\s*[<"]([^>"]+)[>"]',
    re.MULTILINE
)

files = []

for root, dirs, filenames in os.walk("."):
    for filename in filenames:
        if filename.endswith(".cpp") or filename.endswith(".h"):
            files.append(os.path.join(root, filename))

with open("dependencias_gephi.csv", "w", newline="", encoding="utf-8") as csvfile:
    writer = csv.writer(csvfile, delimiter=";")

    writer.writerow(["Source", "Target"])

    for filepath in files:
        try:
            with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
                content = f.read()

            includes = include_pattern.findall(content)

            for include in includes:
                writer.writerow([
                    os.path.basename(filepath),
                    include
                ])

        except Exception as e:
            print(f"Error leyendo {filepath}: {e}")

print("Archivo generado: dependencias_gephi.csv")