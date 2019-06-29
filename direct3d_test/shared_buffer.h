#pragma once
#include <memory>
#include <vector>

template <typename T>
class SharedBuffer : public std::shared_ptr<std::vector<T>>
{
};
