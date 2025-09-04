#include "grammar.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

/**
 * Cargar gramática desde archivo
 * - Omite comentarios (# ...)
 * - Detecta terminales y no terminales
 * - Guarda todas las reglas en formato string
 */

bool Grammar::loadFromFile(const string& filename) {
    ifstream in(filename);  // Abrir el archivo de gramática
    if (!in.is_open()) {
        cerr << "Error al abrir archivo: " << filename << endl;
        return false;  // Retorna false si no se puede abrir
    }

    vector<string> rhsSymbols; // Almacena todos los símbolos que aparecen en RHS

    string line;
    while (getline(in, line)) {  // Leer línea por línea
        line = trim(line);        // Eliminar espacios al inicio y final
        if (line.empty() || line[0] == '#') continue;  // Ignorar líneas vacías o comentarios

        rules.push_back(line);    // Guardar la regla completa

        size_t pos = line.find("->");  // Separar LHS y RHS
        if (pos == string::npos) {
            cerr << "Regla inválida: " << line << endl;
            continue;
        }

        string left = trim(line.substr(0, pos));  // Símbolo del lado izquierdo (LHS)
        if (nonTerminals.empty()) initialState = left; // El primer no terminal es el inicial
        nonTerminals.insert(left);  // Agregar a la lista de no terminales

        string right = trim(line.substr(pos + 2));  // Parte derecha de la regla
        vector<string> alternatives = split(right, '|'); // Separar alternativas por '|'

        // Procesar cada alternativa
        for (auto alt : alternatives) {
            alt = trim(alt);
            if (alt.empty() || alt == "''" || alt == "ε") continue; // Ignorar vacío

            vector<string> symbols = split(alt, ' '); // Separar símbolos por espacios
            for (auto sym : symbols) {
                sym = trim(sym);
                if (!sym.empty() && sym != "''" && sym != "ε") {
                    rhsSymbols.push_back(sym); // Guardar símbolo de RHS
                }
            }
        }
    }

    // Determinar terminales: símbolos en RHS que no están en no terminales
    for (auto s : rhsSymbols) {
        if (nonTerminals.count(s) == 0) {
            terminals.insert(s);
        }
    }

    terminals.insert("$"); // Agregar símbolo de fin de entrada (EOF)
    in.close();            // Cerrar archivo
    return true;           // Éxito al cargar la gramática
}

/**
 * Imprimir la gramática cargada
 */
void Grammar::print() {
    cout << "Estado inicial: " << initialState << "\n";

    // Imprimir no terminales
    cout << "No terminales: ";
    for (auto nt : nonTerminals) cout << nt << " ";
    cout << "\n";

    // Imprimir terminales
    cout << "Terminales: ";
    for (auto t : terminals) cout << t << " ";
    cout << "\n";

    // Imprimir todas las reglas
    cout << "Reglas:\n";
    for (auto r : rules) cout << "  " << r << "\n";
}
