//------------------------------------------------------------------------
// * @filename: Singleton.h
// *
// * @brief: 单例模板
// *
// * @author: XGM
// * @date: 2017/02/09
//------------------------------------------------------------------------
#pragma once

#include "NonCopyable.h"

#include <type_traits>
#include <stdexcept>

template<typename T>
class Singleton : public NonCopyable
{
public:
	typedef T value_type;
	typedef Singleton<T> mytype;
protected:
	Singleton() {}
	virtual ~Singleton() {}
public:
	//------------------------------------------------------------------------
	// 初始化实例
	//------------------------------------------------------------------------
	template<typename...Args>
	static value_type* InitInstance(Args&&... args)
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new value_type(std::forward<Args>(args)...);
		}
		return m_pInstance;
	}

	//------------------------------------------------------------------------
	// 获得实例
	//------------------------------------------------------------------------
	static value_type* GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			throw std::logic_error("the instance is not init");
		}
		return m_pInstance;
	}

	//------------------------------------------------------------------------
	// 获得实例
	//------------------------------------------------------------------------
	inline static value_type* Me()
	{
		return GetInstance();
	}

	//------------------------------------------------------------------------
	// 销毁实例
	//------------------------------------------------------------------------
	static void DestroyInstance()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

private:
	static value_type* m_pInstance;
};

template<typename T> T* Singleton<T>::m_pInstance = nullptr;
