//------------------------------------------------------------------------
// * @filename: TestApp.h
// *
// * @brief: 测试程序
// *
// * @author: XGM
// * @date: 2017/08/17
//------------------------------------------------------------------------
#pragma once

#include "xbase/Singleton.h"
#include "xshare/Application.h"

class TestApp : public Application, public Singleton<TestApp>
{
protected:
	friend class Singleton<TestApp>;
	TestApp();
	virtual ~TestApp();
public:
	//------------------------------------------------------------------------
	// 应用名称
	//------------------------------------------------------------------------
	virtual std::string GetName() override;

	//------------------------------------------------------------------------
	// 运行一帧
	//------------------------------------------------------------------------
	virtual void RunOnce() override;

};
