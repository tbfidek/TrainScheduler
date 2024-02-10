#pragma once

#include <unordered_map>
#include <string>

class Data
{
    static Data *instance;
    std::unordered_map<std::string, std::pair<int, int>> umap;

    Data(){}

public:
    static Data *getInstance()
    {
        if (!instance)
            instance = new Data;
        return instance;
    }

    std::unordered_map<std::string, std::pair<int, int>> &getData()
    {
        return this->umap;
    }
};