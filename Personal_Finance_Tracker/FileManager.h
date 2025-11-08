#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Transactions.h"

namespace FileManager {
    bool saveCSV(const std::string& filename, const std::vector<Transaction>& list);
    std::vector<Transaction> loadCSV(const std::string& filename);
}

#endif
