#include "log2txt.h"
#include <iostream>

log2txt::log2txt(const std::string& filename): filename(filename)
{
    this->file.open(filename, std::ios::out);
    if (!this->file.is_open())
    {
        std::cerr << "Error opening file " << filename << std::endl;
    }
}

log2txt::~log2txt()
{
    this->file.close();
}

void log2txt::log_n(const std::string& message)
{
    file << message << std::endl;
}

void log2txt::log(const std::string& message)
{
    file << message;
}

void log2txt::clear()
{
    file.write("",0);
}
