#ifndef _CORE_REFERENCE_COUNTING_PTR_H_
#define _CORE_REFERENCE_COUNTING_PTR_H_

#include <map>
#include <assert.h>

template <typename T>
class RefCountPointer
{
public:
    explicit RefCountPointer(T* type_pointer);
	RefCountPointer();
    ~RefCountPointer();

    RefCountPointer(const RefCountPointer<T>& rhs);
    T& operator*() const;
    T* operator->();
    RefCountPointer<T>& operator=(const RefCountPointer<T>& rhs);
	RefCountPointer<T>& operator=(T* rhs);
	bool operator==(const RefCountPointer<T>& rhs);
	bool operator==(const T* rhs);

	int NotNull() const;

    operator void*() const;

protected:
    T* pointer;
    static std::map<T*, int> reference_counts;
    void AddReference();
    void ReleaseReference();
};

template <typename T>
std::map<T*, int> RefCountPointer<T>::reference_counts;

template <typename T>
RefCountPointer<T>::RefCountPointer(T* type_pointer)
{
    pointer = type_pointer;
    AddReference();
}

template <typename T>
int RefCountPointer<T>::NotNull() const
{
	return (int) pointer;
}

template <typename T>
RefCountPointer<T>::RefCountPointer()
{
    pointer = 0;
	AddReference();
}

template <typename T>
RefCountPointer<T>::~RefCountPointer()
{
	if (pointer) ReleaseReference();
}

template <typename T>
T& RefCountPointer<T>::operator*() const
{
    return (*pointer);
}

template <typename T>
T* RefCountPointer<T>::operator->()
{
    return pointer;
}

template <typename T>
RefCountPointer<T>& RefCountPointer<T>::operator=(const RefCountPointer<T>& rhs)
{
    if (this == &rhs)
    {
        return (*this);
    }
    ReleaseReference();
    this->pointer = rhs.pointer;
    AddReference();
    return (*this);
}

template <typename T>
void RefCountPointer<T>::AddReference()
{
	if (!pointer) return;
    if (reference_counts.find(pointer) == reference_counts.end())
    {
        reference_counts[pointer] = 1;
    }
    else
    {
        reference_counts[pointer]++;
    }
}

template <typename T>
void RefCountPointer<T>::ReleaseReference()
{
	if (!pointer) return;
    if (reference_counts.find(pointer) == reference_counts.end())
    {
        assert(0);
    }
    else
    {
        reference_counts[pointer]--;
        if (reference_counts[pointer] <= 0)
        {
            reference_counts.erase(pointer);
            delete pointer;
        }
    }
}

template<typename T>
RefCountPointer<T>::operator void*() const
{
    return pointer;
}

template<typename T>
RefCountPointer<T>& RefCountPointer<T>::operator=(T* rhs)
{
	if (this->pointer == &rhs)
    {
        return (*this);
    }
    ReleaseReference();
    this->pointer = rhs;
    AddReference();
    return (*this);
}

template<typename T>
bool RefCountPointer<T>::operator==(const RefCountPointer<T>& rhs)
{
	return pointer == rhs.pointer;
}

template<typename T>
bool RefCountPointer<T>::operator==(const T* rhs)
{
	return pointer = rhs;
}

#endif
