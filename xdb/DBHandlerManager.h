//------------------------------------------------------------------------
// * @filename: DBHandlerManager.h
// *
// * @brief: 数据库处理者的管理
// *
// * @author: XGM
// * @date: 2017/10/13
//------------------------------------------------------------------------
#pragma once

#include "xbase/Manager.h"
#include "xbase/Singleton.h"

#include "xdb/DBHandler.h"

class DBHandlerManager : public Manager<std::string, DBHandler*>, public Singleton<DBHandlerManager>
{
public:
	class iterator;
protected:
	friend class Singleton<DBHandlerManager>;
	DBHandlerManager() {}
	virtual ~DBHandlerManager() {}
public:
	//------------------------------------------------------------------------
	// 清除引用
	//------------------------------------------------------------------------
	void ClearRef();

	//------------------------------------------------------------------------
	// begin迭代器
	//------------------------------------------------------------------------
	iterator begin();

	//------------------------------------------------------------------------
	// end迭代器
	//------------------------------------------------------------------------
	iterator end();
};

// 迭代器
class DBHandlerManager::iterator : public std::iterator<std::input_iterator_tag, DBHandler*>
{
public:
	typedef std::iterator<std::input_iterator_tag, DBHandler*> mybase_type;
	typedef typename mybase_type::value_type value_type;
private:
	friend class DBHandlerManager;

	explicit iterator(DBHandlerManager* pMgr);

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
	DBHandlerManager* m_pMgr;
	// 迭代器位置
	DBHandlerManager::map_iterator m_itCurrent;
};
