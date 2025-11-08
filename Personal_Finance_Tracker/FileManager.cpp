#include "FileManager.h"
#include <fstream>
#include <iostream>

namespace FileManager {

    bool saveCSV(const std::string& filename, const std::vector<Transaction>& list) {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Error: cannot open " << filename << " for writing.\n";
            return false;
        }
        // header
        if (!list.empty()) {
            out << "date,category,description,amount,type\n";
        }
        else {
            // still write an empty file with header to be nice
            out << "date,category,description,amount,type\n";
        }
        for (const auto& t : list) out << t.toCSV(false) << "\n";
        return true;
    }

    std::vector<Transaction> loadCSV(const std::string& filename) {
        std::vector<Transaction> res;
        std::ifstream in(filename);
        if (!in) {
            // Not fatal; return empty
            return res;
        }
        std::string line;
        bool first = true;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            if (first) {
                // If looks like header, skip
                first = false;
                if (line.size() >= 4 && line.rfind("date,", 0) == 0) continue;
            }
            res.push_back(Transaction::fromCSV(line, false));
        }
        return res;
    }

}
