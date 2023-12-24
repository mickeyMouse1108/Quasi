#pragma once
#include <string>
#include <fstream>

namespace stdu {
    std::string readfile(const std::string& fname) {
        std::ifstream in { fname, std::ios::in | std::ios::binary };
        if (in) {
            std::string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return contents;
        }
        return "";
    }
}