#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;



inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

inline vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    stringstream ss(str);
    string temp;
    while (getline(ss, temp, delim)) {
        if (!temp.empty())
            tokens.push_back(temp);
    }
    return tokens;
}

#endif // UTILS_H
