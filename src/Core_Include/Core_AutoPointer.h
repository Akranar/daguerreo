#ifndef _CORE_AUTO_PTR_H_
#define _CORE_AUTO_PTR_H_

template <typename T>
class AutoPointer
{
public:
    AutoPointer(T* type_pointer);
	AutoPointer();
    ~AutoPointer();

    AutoPointer(const AutoPointer<T>& rhs);
    T& operator*() const;
    T* operator->();
    AutoPointer<T>& operator=(const AutoPointer<T>& rhs);
	AutoPointer<T>& operator=(T* rhs);
	bool operator==(const AutoPointer<T>& rhs);
	bool operator==(const T* rhs);

	int NotNull() const;

    operator void*() const;

protected:
    T* pointer;
};

template <typename T>
AutoPointer<T>::AutoPointer(T* type_pointer = 0)
{
    pointer = type_pointer;
}

template <typename T>
int AutoPointer<T>::NotNull() const
{
	return (int) pointer;
}

template <typename T>
AutoPointer<T>::AutoPointer()
{
    pointer = 0;
}

template <typename T>
AutoPointer<T>::~AutoPointer()
{
	if (pointer) delete pointer;
}

template <typename T>
T& AutoPointer<T>::operator*() const
{
    return (*pointer);
}

template <typename T>
T* AutoPointer<T>::operator->()
{
    return pointer;
}

template <typename T>
AutoPointer<T>& AutoPointer<T>::operator=(const AutoPointer<T>& rhs)
{
    if (this == &rhs)
    {
        return (*this);
    }
    if (pointer) delete pointer;
    this->pointer = rhs.pointer;
    return (*this);
}

template <typename T>
AutoPointer<T>& AutoPointer<T>::operator=(T* rhs)
{
	if (pointer) delete pointer;
	this->pointer = rhs;
	return (*this);
}

template<typename T>
AutoPointer<T>::operator void*() const
{
    return pointer;
}

template<typename T>
bool AutoPointer<T>::operator==(const AutoPointer<T>& rhs)
{
	return pointer == rhs.pointer;
}

template<typename T>
bool AutoPointer<T>::operator==(const T* rhs)
{
	return pointer == rhs;
}

#endif
