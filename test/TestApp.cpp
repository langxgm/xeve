
#include "TestApp.h"

#include "gtest.h"
#include "internal/gtest-port.h"

static class OnApp
{
public:
	OnApp()
	{
		TestApp::InitInstance();
		//static char arg1[] = "test";
		//static char* argv[] = { arg1 };
		//TestApp::Me()->Init(0, argv);
		//TestApp::Me()->Loop(20);
	}

	~OnApp()
	{
		TestApp::DestroyInstance();
	}
private:

} s_test_app;

TestApp::TestApp()
{

}

TestApp::~TestApp()
{

}

std::string TestApp::GetName()
{
	return "TestApp";
}

void TestApp::RunOnce()
{

}
