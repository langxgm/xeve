//------------------------------------------------------------------------
// * @filename: Random.h
// *
// * @brief: 随机数模板
// *
// * @author: XGM
// * @date: 2017/03/01
//------------------------------------------------------------------------
#pragma once

#include <random>

template<typename T>
struct Distribution
{
	typedef std::uniform_int_distribution<T> type;
};

template<>
struct Distribution<float>
{
	typedef std::uniform_real_distribution<float> type;
};

template<>
struct Distribution<double>
{
	typedef std::uniform_real_distribution<double> type;
};

template<typename T = int>
class Random
{
public:
	typedef T result_type;
	typedef typename Distribution<result_type>::type distribution_type;
	typedef typename distribution_type::param_type dist_param_type;
public:
	explicit Random(unsigned int seed)
		:m_random_engine(seed)
	{
	}

	//------------------------------------------------------------------------
	// 设置随机种子
	//------------------------------------------------------------------------
	void seed(unsigned int seed)
	{
		m_random_engine.seed(seed);
	}

	//------------------------------------------------------------------------
	// 设置范围[start,end]
	//------------------------------------------------------------------------
	void range(result_type start, result_type end)
	{
		dist_param_type p(start, end);
		m_distribution.param(p);
	}

	//------------------------------------------------------------------------
	// 随机值
	// @return int:[start,end], float:[start,end)
	//------------------------------------------------------------------------
	result_type next()
	{
		return m_distribution(m_random_engine);
	}

	//------------------------------------------------------------------------
	// 随机值
	// @return int:[start,end], float:[start,end)
	//------------------------------------------------------------------------
	result_type rand(result_type start, result_type end)
	{
		dist_param_type p(start, end);
		if (!(m_distribution.param() == p))
		{
			range(start, end);
		}
		return next();
	}

private:
	std::default_random_engine m_random_engine;
	distribution_type m_distribution;
};
