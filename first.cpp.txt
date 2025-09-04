#include "first.h"
#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * compute()
 * Calcula los conjuntos FIRST para todos los no terminales de la gramática.
 * Algoritmo iterativo hasta que los conjuntos FIRST dejen de cambiar.
 */
void First::compute() {
    // 1. Inicializar conjuntos FIRST vacíos para cada no terminal
    for (const string& nt : grammar.nonTerminals) {
        firstSets[nt] = set<string>();
    }

    bool changed = true;

    while (changed) {
        changed = false;

        // 2. Recorrer todas las reglas de la gramática
        for (const string& r : grammar.rules) {
            string line = trim(r);
            if (line.empty()) continue;

            size_t pos = line.find("->");
            if (pos == string::npos) continue;

            string left = trim(line.substr(0, pos));   // LHS: no terminal
            string right = trim(line.substr(pos + 2)); // RHS: símbolos de derivación

            // 2.1 Separar RHS en alternativas por '|'
            vector<string> alternatives = split(right, '|');

            for (auto& alt : alternatives) {
                vector<string> symbols = split(trim(alt), ' ');
                if (symbols.empty()) continue;

                string sym = trim(symbols[0]); // Solo el primer símbolo de la alternativa

                set<string> firstSym;

                if (sym.empty()) continue;

                // =============================
                // Regla 1: FIRST(X) = {X} si X es terminal
                // =============================
                if (sym == "''") {
                    firstSym.insert("''"); // Epsilon
                }
                else if (sym.front() == '\'' && sym.back() == '\'') {
                    firstSym.insert(sym.substr(1, sym.size() - 2)); // Tomamos el terminal
                }

                // =============================
                // Regla 2: FIRST(X1) - {ε} ⊆ FIRST(X)
                // Aquí no consideramos ε, solo propagamos FIRST(X1) tal cual
                // =============================
                else if (grammar.nonTerminals.count(sym)) {
                    firstSym = firstSets[sym]; // Tomamos el FIRST del no terminal X1
                }
                // Caso: terminal implícito (sin comillas)
                else {
                    firstSym.insert(sym); // También aplica regla 1 para terminales sin comillas
                }

                // Insertar los elementos de FIRST(X1) o {X} en FIRST(LHS)
                size_t before = firstSets[left].size();
                firstSets[left].insert(firstSym.begin(), firstSym.end());
                if (firstSets[left].size() > before) changed = true;
            }
        }
    }
}



/**
 * print()
 * Muestra en consola los conjuntos FIRST de cada no terminal
 */
void First::print() {
    for (auto& p : firstSets) {
        cout << "First(" << p.first << ") = { ";
        size_t count = 0;
        for (const auto& s : p.second) {
            cout << s;
            count++;
            if (count < p.second.size()) cout << ", ";
        }
        cout << " }" << endl;
    }
}
