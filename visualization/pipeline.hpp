#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace yLab
{

class Pipeline
{
public:
    Pipeline(const std::string& vert_filepath, const std::string& frag_filepath)
    {
        auto vert_code = readFile(vert_filepath);
        auto frag_code = readFile(frag_filepath);

        std::cout << "Vertex shader code size: " << vert_code.size() << std::endl;
        std::cout << "Fragment shader code size: " << frag_code.size() << std::endl;

    }
private:
    std::vector<char> readFile(const std::string& filepath)
    {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            throw std::runtime_error{"Cannot open file: " + filepath};
        }

        size_t file_size = (size_t) file.tellg();
        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), file_size);

        file.close();
        return buffer;
    }

};

}
