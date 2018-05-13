//------------------------------------------------------------------------
// * @filename: Print.h
// *
// * @brief: 打印接口
// *
// * @author: XGM
// * @date: 2017/03/15
//------------------------------------------------------------------------
#pragma once

template<class _Output_t>
class Print
{
public:
	typedef _Output_t output_type;
public:
	virtual void PrintAll(output_type& out) = 0;
};
