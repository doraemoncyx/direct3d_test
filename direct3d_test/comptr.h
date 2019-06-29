#pragma once

#include <memory>

template <typename  T>
struct ComDeleter
{
	void operator()(T* p)
	{
		if (p)
		{
			p->Release();
		}
	}
};

template <typename T>
class ComPtr : public std::unique_ptr<T, ComDeleter<T>>
{

};