#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <cstdint>

namespace utils {
    std::string trim(const std::string& s);
    std::string toLower(std::string s);
    bool isValidDate(const std::string& yyyymmdd);  // very basic validation
    // Parse "YYYY-MM-DD" -> (year, month, day), returns false if invalid
    bool parseYMD(const std::string& yyyymmdd, int& y, int& m, int& d);
    // Compare dates lexicographically (YYYY-MM-DD format), return a<b
    bool dateLess(const std::string& a, const std::string& b);
    bool dateInRange(const std::string& d, const std::string& start, const std::string& end); // inclusive
}

#endif
