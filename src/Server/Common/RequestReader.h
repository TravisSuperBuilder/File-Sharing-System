#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include <iostream>

class RequestReader
{
public:
    RequestReader(std::shared_ptr<std::vector<char>> buffer): currentOffset(0),
        buffer(buffer)
    {
        if (!buffer)
        {
            throw std::runtime_error("Buffer is nullptr!");
        }
    };


    template<class T,typename std::enable_if_t<std::is_fundamental<T>::value>* = nullptr>
    T read()
    {
        if (currentOffset + sizeof(T) > buffer->size())
        {
            throw std::runtime_error("Cannot get a parameter");
        }

        std::vector<char> subbuffer(buffer->begin() + currentOffset, buffer->begin() + currentOffset + sizeof(T));
        T result = *reinterpret_cast<T*>(subbuffer.data());
        currentOffset += sizeof(T);
        return result;
    }

    template<class T, typename std::enable_if_t<std::is_same<std::vector<char>,T>::value>* = nullptr>
    std::vector<char> read()
    {
        size_t blockSize;
        blockSize = this->read<size_t>();
        if (blockSize > buffer->size() - currentOffset)
        {
            throw std::runtime_error("Wrong buffer size!");
        }

        std::vector<char> result(buffer->begin() + currentOffset, buffer->begin() + currentOffset + blockSize);
        currentOffset += blockSize;
        return result;
    }

    template<class T, typename std::enable_if_t<std::is_same<std::string,T>::value>* = nullptr>
    std::string read()
    {
        size_t blockSize;
        blockSize = this->read<size_t>();
        if (blockSize > buffer->size() - currentOffset)
            throw std::runtime_error("Wrong buffer size!");
        std::string result(buffer->begin() + currentOffset, buffer->begin() + currentOffset + blockSize);
        currentOffset += blockSize;
        return result;
    }

private:
    size_t currentOffset;
    std::shared_ptr<std::vector<char>> buffer;
};

