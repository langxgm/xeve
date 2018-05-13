//------------------------------------------------------------------------
// * @filename: Defer.h
// *
// * @brief: 延迟回调类(析构回调)
// *
// * @author: XGM
// * @date: 2017/12/12
//------------------------------------------------------------------------
#pragma once

#include <functional>

template<class _F_t>
class Defer
{
public:
	virtual ~Defer()
	{
		if (m_func)
		{
			m_func();
		}
	}

	void operator << (const std::function<_F_t>& func)
	{
		m_func = func;
	}

protected:
	std::function<_F_t> m_func;
};

#define defer Defer<void()>()<<
