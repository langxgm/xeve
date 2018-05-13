//------------------------------------------------------------------------
// * @filename: LoadAverage.h
// *
// * @brief: 负载平均值模板
// *
// * @author: XGM
// * @date: 2017/09/28
//------------------------------------------------------------------------
#pragma once

#include <list>

template<class _Value_t>
class LoadAverage
{
public:
	typedef _Value_t value_type;
public:
	LoadAverage(uint32_t nMaxSize)
		: m_nMaxSize(nMaxSize)
		, m_nAccumulate(0)
	{
	}

	//------------------------------------------------------------------------
	// 加入新值
	//------------------------------------------------------------------------
	void PushValue(const value_type& v)
	{
		if (m_listValues.size() == m_nMaxSize)
		{
			m_nAccumulate -= m_listValues.back();
			m_listValues.pop_back();
		}

		m_nAccumulate += v;
		m_listValues.push_front(v);
	}

	//------------------------------------------------------------------------
	// 获得平均值
	//------------------------------------------------------------------------
	value_type GetAverageValue()
	{
		if (m_listValues.size() == 0)
		{
			return value_type(0);
		}
		return m_nAccumulate / m_listValues.size();
	}

private:
	// 数量限制
	uint32_t m_nMaxSize;
	// 数据列表
	std::list<value_type> m_listValues;
	// 累积
	value_type m_nAccumulate;
};
