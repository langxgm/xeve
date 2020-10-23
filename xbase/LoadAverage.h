//------------------------------------------------------------------------
// * @filename: LoadAverage.h
// *
// * @brief: 负载平均值模板
// *
// * @author: XGM
// * @date: 2017/09/28
//------------------------------------------------------------------------
#pragma once

#include <vector>

template<class _Value_t>
class LoadAverage
{
public:
	typedef _Value_t value_type;
public:
	LoadAverage(uint32_t nMaxSize)
		: m_nMaxSize(nMaxSize)
		, m_nBeginIndex(0)
		, m_nEndIndex(0)
		, m_nSum(0)
	{
		m_vecValues.reserve(nMaxSize);
	}

	//------------------------------------------------------------------------
	// 加入新值
	//------------------------------------------------------------------------
	void PushValue(const value_type& v)
	{
		if (m_vecValues.size() == m_nMaxSize)
		{
			m_nSum -= m_vecValues[m_nBeginIndex];
			m_vecValues[m_nBeginIndex] = v;
			m_nSum += v;
			m_nBeginIndex = (++m_nBeginIndex % m_nMaxSize);
			m_nEndIndex = (++m_nEndIndex % m_nMaxSize);
		}
		else
		{
			m_nSum += v;
			m_vecValues.push_back(v);
			m_nBeginIndex = 0;
			m_nEndIndex = m_vecValues.size() - 1;
		}
	}

	//------------------------------------------------------------------------
	// 获得平均值
	//------------------------------------------------------------------------
	value_type GetAverageValue() const
	{
		if (m_vecValues.size() == 0)
		{
			return value_type(0);
		}
		return m_nSum / m_vecValues.size();
	}

	//------------------------------------------------------------------------
	// 获得总数
	//------------------------------------------------------------------------
	value_type GetSum() const { return m_nSum; }

	//------------------------------------------------------------------------
	// 获得列表长度
	//------------------------------------------------------------------------
	uint32_t GetSize() const { return m_vecValues.size(); }
	uint32_t GetMaxSize() const { return m_nMaxSize; }

private:
	// 数量限制
	uint32_t m_nMaxSize;
	// 数据列表
	std::vector<value_type> m_vecValues;
	// 头尾下标
	uint32_t m_nBeginIndex;
	uint32_t m_nEndIndex;
	// 求和总数
	value_type m_nSum;
};
