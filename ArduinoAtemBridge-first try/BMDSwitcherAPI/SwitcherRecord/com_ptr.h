/* -LICENSE-START-
 ** Copyright (c) 2019 Blackmagic Design
 **
 ** Permission is hereby granted, free of charge, to any person or organization
 ** obtaining a copy of the software and accompanying documentation covered by
 ** this license (the "Software") to use, reproduce, display, distribute,
 ** execute, and transmit the Software, and to prepare derivative works of the
 ** Software, and to permit third-parties to whom the Software is furnished to
 ** do so, all subject to the following:
 **
 ** The copyright notices in the Software and this entire statement, including
 ** the above license grant, this restriction and the following disclaimer,
 ** must be included in all copies of the Software, in whole or in part, and
 ** all derivative works of the Software, unless such copies or derivative
 ** works are solely in the form of machine-executable object code generated by
 ** a source language processor.
 **
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 ** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 ** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 ** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 ** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 ** DEALINGS IN THE SOFTWARE.
 ** -LICENSE-END-
 */

#pragma once

#include <cstddef>
#include <utility>
#ifdef _WIN32
#include <guiddef.h>
#include <winerror.h>
#elif defined(__APPLE__)
#include <CoreFoundation/CFPlugInCOM.h>
#else
#include "LinuxCOM.h"
#endif

template<typename T>
class com_ptr
{
	template<typename U>
		friend class com_ptr;

public:
	constexpr com_ptr();
	constexpr com_ptr(std::nullptr_t);
	explicit com_ptr(T* ptr);
	com_ptr(const com_ptr<T>& other);
	com_ptr(com_ptr<T>&& other);
	
	template<typename U>
	com_ptr(REFIID iid, com_ptr<U> other);

	~com_ptr();

	com_ptr<T>& operator=(std::nullptr_t);
	com_ptr<T>& operator=(T* ptr);
	com_ptr<T>& operator=(const com_ptr<T>& other);
	com_ptr<T>& operator=(com_ptr<T>&& other);

	T* get() const;
	T** releaseAndGetAddressOf();

	const T* operator->() const;
	T* operator->();
	const T& operator*() const;
	T& operator*();

	explicit operator bool() const;

	bool operator==(const com_ptr<T>& other) const;
	bool operator<(const com_ptr<T>& other) const;

private:
	void release();

	T* m_ptr;
};

template<typename T>
constexpr com_ptr<T>::com_ptr() :
	m_ptr(nullptr)
{ }

template<typename T>
constexpr com_ptr<T>::com_ptr(std::nullptr_t) :
	m_ptr(nullptr)
{ }

template<typename T>
com_ptr<T>::com_ptr(T* ptr) :
	m_ptr(ptr)
{
	if (m_ptr)
		m_ptr->AddRef();
}

template<typename T>
com_ptr<T>::com_ptr(const com_ptr<T>& other) :
	m_ptr(other.m_ptr)
{
	if (m_ptr)
		m_ptr->AddRef();
}

template<typename T>
com_ptr<T>::com_ptr(com_ptr<T>&& other) :
	m_ptr(other.m_ptr)
{
	other.m_ptr = nullptr;
}

template<typename T>
template<typename U>
com_ptr<T>::com_ptr(REFIID iid, com_ptr<U> other)
{
	if (other.m_ptr)
	{
		if (other.m_ptr->QueryInterface(iid, (void**)&m_ptr) != S_OK)
			m_ptr = nullptr;
	}
	else
	{
		m_ptr = nullptr;
	}
}

template<typename T>
com_ptr<T>::~com_ptr()
{
	release();
}

template<typename T>
com_ptr<T>& com_ptr<T>::operator=(std::nullptr_t)
{
	release();
	m_ptr = nullptr;
	return *this;
}

template<typename T>
com_ptr<T>& com_ptr<T>::operator=(T* ptr)
{
	if (ptr)
		ptr->AddRef();
	release();
	m_ptr = ptr;
	return *this;
}

template<typename T>
com_ptr<T>& com_ptr<T>::operator=(const com_ptr<T>& other)
{
	return (*this = other.m_ptr);
}

template<typename T>
com_ptr<T>& com_ptr<T>::operator=(com_ptr<T>&& other)
{
	release();
	m_ptr = other.m_ptr;
	other.m_ptr = nullptr;
	return *this;
}

template<typename T>
T* com_ptr<T>::get() const
{
	return m_ptr;
}

template<typename T>
T** com_ptr<T>::releaseAndGetAddressOf()
{
	release();
	return &m_ptr;
}

template<typename T>
const T* com_ptr<T>::operator->() const
{
	return m_ptr;
}

template<typename T>
T* com_ptr<T>::operator->()
{
	return m_ptr;
}

template<typename T>
const T& com_ptr<T>::operator*() const
{
	return *m_ptr;
}

template<typename T>
T& com_ptr<T>::operator*()
{
	return *m_ptr;
}

template<typename T>
com_ptr<T>::operator bool() const
{
	return m_ptr != nullptr;
}

template<typename T>
void com_ptr<T>::release()
{
	if (m_ptr)
		m_ptr->Release();
}

template<typename T>
bool com_ptr<T>::operator==(const com_ptr<T>& other) const
{
	return m_ptr == other.m_ptr;
}

template<typename T>
bool com_ptr<T>::operator<(const com_ptr<T>& other) const
{
	return m_ptr < other.m_ptr;
}

template<class T, class... Args>
com_ptr<T> make_com_ptr(Args&&... args)
{
	com_ptr<T> temp(new T(args...));
	// com_ptr takes ownership of reference count, so release reference count added by raw pointer constructor
	temp->Release();
	return std::move(temp);
}
