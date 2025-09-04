#include "follow.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

/**
 * compute()
 * Calcula los conjuntos FOLLOW para todos los no terminales de la gramática.
 * Algoritmo iterativo hasta que los conjuntos FOLLOW dejen de cambiar.
 */

// Helper: reconoce epsilon con alias comunes.
static inline bool is_eps(const string& s) {
    return s == "''" || s == "ε" || s == "EPS" || s == "epsilon";
}

// Helper: terminal escrito como 'a'
static inline bool is_quoted_terminal(const string& x) {
    return x.size() >= 2 && x.front() == '\'' && x.back() == '\'';
}

void Follow::compute() {
    // 1. Inicializar conjuntos FOLLOW vacíos para cada no terminal
    for (const string& nt : grammar.nonTerminals) {
        followSets[nt] = set<string>();
    }

    // =============================
    // Regla 1 base: El símbolo inicial contiene el EOF '$'
    // =============================
    if (!grammar.initialState.empty()) {
        followSets[grammar.initialState].insert("$");
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

            string left = trim(line.substr(0, pos));   // LHS: A
            string right = trim(line.substr(pos + 2)); // RHS: α
            vector<string> alternatives = split(right, '|');

            for (auto& alt : alternatives) {
                // Normalizar símbolos de la alternativa
                vector<string> raw = split(trim(alt), ' ');
                vector<string> symbols;
                symbols.reserve(raw.size());
                for (auto &tok : raw) {
                    string t = trim(tok);
                    if (!t.empty()) symbols.push_back(t);
                }

                // ========= PASO A: tu bucle original (Regla 1) =========
                // FOLLOW(B) += FIRST(nextSym) \ {ε} cuando hay al menos 1 símbolo a la derecha
                for (size_t i = 0; i < symbols.size(); i++) {
                    string A = trim(symbols[i]);
                    if (A.empty() || !grammar.nonTerminals.count(A)) continue;

                    if (i + 1 < symbols.size()) {
                        string nextSym = trim(symbols[i + 1]);
                        set<string> firstNext;

                        if (is_quoted_terminal(nextSym)) {
                            // Terminal explícito 'a'
                            firstNext.insert(nextSym.substr(1, nextSym.size() - 2));
                        } else if (grammar.nonTerminals.count(nextSym)) {
                            // No terminal
                            // (si aún no está calculado, at() lanzaría; asumimos FIRST listo)
                            firstNext = first.firstSets.at(nextSym);
                        } else {
                            // Terminal implícito (sin comillas)
                            if (!is_eps(nextSym)) firstNext.insert(nextSym);
                        }

                        // Insertar FIRST(nextSym) en FOLLOW(A), ignorando ε
                        size_t before = followSets[A].size();
                        for (const string& s : firstNext) {
                            if (!is_eps(s)) followSets[A].insert(s);
                        }
                        if (followSets[A].size() > before) changed = true;
                    }
                }

                // ========= PASO B: NUEVO bucle de derecha a izquierda (Regla 2) =========
                set<string> trailer = followSets[left]; // FOLLOW(A) al inicio
                for (int i = (int)symbols.size() - 1; i >= 0; --i) {
                    const string Xi = trim(symbols[(size_t)i]);
                    if (Xi.empty()) continue;

                    if (grammar.nonTerminals.count(Xi)) {
                        // Xi es no terminal: FOLLOW(Xi) += trailer
                        size_t beforeF = followSets[Xi].size();
                        followSets[Xi].insert(trailer.begin(), trailer.end());
                        if (followSets[Xi].size() > beforeF) changed = true;

                        // Actualizar trailer según FIRST(Xi)
                        set<string> FX;
                        auto itFX = first.firstSets.find(Xi);
                        if (itFX != first.firstSets.end()) {
                            FX = itFX->second;
                        }
                        bool nullable = (FX.count("''") || FX.count("ε") ||
                                         FX.count("EPS") || FX.count("epsilon"));

                        // Quitar ε de FX
                        set<string> FX_noeps;
                        for (const auto& s : FX) if (!is_eps(s)) FX_noeps.insert(s);

                        if (nullable) {
                            // trailer = trailer ∪ FIRST(Xi)\{ε}
                            trailer.insert(FX_noeps.begin(), FX_noeps.end());
                        } else {
                            // trailer = FIRST(Xi)\{ε}
                            trailer = FX_noeps;
                        }
                    } else if (is_eps(Xi)) {
                        // Xi = ε: no afecta (mantener trailer)
                        continue;
                    } else {
                        // Xi es terminal (explícito o implícito): trailer = { Xi }
                        string term = Xi;
                        if (is_quoted_terminal(Xi)) term = Xi.substr(1, Xi.size() - 2);
                        trailer.clear();
                        trailer.insert(term);
                    }
                }
            }
        }
    }
}

/**
 * print()
 * Muestra en consola los conjuntos FOLLOW de cada no terminal
 */
void Follow::print() {
    for (auto& p : followSets) {
        cout << "Follow(" << p.first << ") = { ";
        size_t count = 0;
        for (auto& s : p.second) {
            cout << s;
            count++;
            if (count < p.second.size()) cout << ", ";
        }
        cout << " }" << endl;
    }
}
