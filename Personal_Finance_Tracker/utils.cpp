#include "utils.h"
#include <algorithm>
#include <sstream>
#include <cctype>

namespace utils {
    std::string trim(const std::string& s) {
        size_t b = 0, e = s.size();
        while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
        while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
        return s.substr(b, e - b);
    }

    std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
        return s;
    }

    bool parseYMD(const std::string& d, int& y, int& m, int& day) {
        if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
        try {
            y = std::stoi(d.substr(0, 4));
            m = std::stoi(d.substr(5, 2));
            day = std::stoi(d.substr(8, 2));
        }
        catch (...) { return false; }
        if (m < 1 || m > 12) return false;
        if (day < 1 || day > 31) return false; // naive
        return true;
    }

    bool isValidDate(const std::string& d) {
        int y, m, day;
        return parseYMD(d, y, m, day);
    }

    bool dateLess(const std::string& a, const std::string& b) {
        return a < b; // lexicographic works with YYYY-MM-DD
    }

    bool dateInRange(const std::string& d, const std::string& start, const std::string& end) {
        return (start.empty() || start <= d) && (end.empty() || d <= end);
    }
}
