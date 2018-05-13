//------------------------------------------------------------------------
// * @filename: LogicManager.h
// *
// * @brief: 逻辑管理
// *
// * @author: XGM
// * @date: 2017/12/07
//------------------------------------------------------------------------
#pragma once

#include "LogicHandler.h"

#include "xbase/Manager.h"
#include "xbase/Singleton.h"

#include <vector>

class LogicManager : protected Manager<std::string, LogicHandler*>, public Singleton<LogicManager>
{
public:
	class iterator;
	typedef Manager<std::string, LogicHandler*> mybase_manager;
	typedef typename mybase_manager::key_type key_type;
	typedef typename mybase_manager::value_type value_type;
	typedef std::vector<value_type> list_type;
	typedef typename list_type::iterator list_iterator;
protected:
	friend class Singleton<LogicManager>;
	LogicManager();
	virtual ~LogicManager();
public:
	//------------------------------------------------------------------------
	// 注册逻辑
	//------------------------------------------------------------------------
	virtual bool RegLogic(const key_type& key, value_type value);

	//------------------------------------------------------------------------
	// 获取逻辑
	//------------------------------------------------------------------------
	virtual value_type GetLogic(const key_type& key);

	//------------------------------------------------------------------------
	// 通过下标获取逻辑
	//------------------------------------------------------------------------
	value_type GetLogicByIndex(uint32_t index);

	//------------------------------------------------------------------------
	// 获取逻辑数量
	//------------------------------------------------------------------------
	uint32_t GetSize();

	//------------------------------------------------------------------------
	// 打印所有逻辑
	//------------------------------------------------------------------------
	virtual void PrintAll(std::ostream& out) override;

public:
	//------------------------------------------------------------------------
	// begin迭代器
	//------------------------------------------------------------------------
	iterator begin();

	//------------------------------------------------------------------------
	// end迭代器
	//------------------------------------------------------------------------
	iterator end();

protected:
	list_type m_Logics;
};

// 迭代器
class LogicManager::iterator : public std::iterator<std::input_iterator_tag, LogicHandler*>
{
public:
	typedef std::iterator<std::input_iterator_tag, LogicHandler*> mybase_type;
	typedef typename mybase_type::value_type value_type;
private:
	friend class LogicManager;

	explicit iterator(LogicManager* pMgr);

	friend bool operator==(const iterator&, const iterator&);
	friend bool operator!=(const iterator&, const iterator&);

public:
	//------------------------------------------------------------------------
	// 获取迭代器指向的值
	//------------------------------------------------------------------------
	const value_type& operator*() const;

	//------------------------------------------------------------------------
	// 前++
	//------------------------------------------------------------------------
	iterator& operator++();

	//------------------------------------------------------------------------
	// 后++
	//------------------------------------------------------------------------
	void operator++(int);

private:
	// 管理器的指针
	LogicManager* m_pMgr;
	// 迭代器位置
	LogicManager::list_iterator m_itCurrent;
};
