#pragma once
#include <vector>

class Buffer
{
public:
    Buffer(/* args */);
    ~Buffer();
private:
    std::vector<char> buffer_;
};

