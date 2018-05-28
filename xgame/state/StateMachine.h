//------------------------------------------------------------------------
// * @filename: StateMachine.h
// *
// * @brief: 状态机
// *
// * @author: XGM
// * @date: 2017/01/23
//------------------------------------------------------------------------
#pragma once

#include "State.h"
#include <assert.h>

template<class entity_type>
class StateMachine
{
public:
	StateMachine(entity_type* pOwner)
		:m_pOwner(pOwner)
	{

	}

	~StateMachine()
	{

	}

	State<entity_type>* GetCurrentState() const { return m_pCurrentState; }
	void SetCurrentState(State<entity_type>* pState) { m_pCurrentState = pState; }

	State<entity_type>* GetPreviousState() const { return m_pPreviousState; }
	void SetPreviousState(State<entity_type>* pState) { m_pPreviousState = pState; }

	State<entity_type>* GetGlobalState() const { return m_pGlobalState; }
	void SetGlobalState(State<entity_type>* pState) { m_pGlobalState = pState; }

	//------------------------------------------------------------------------
	// 更新状态内容
	//------------------------------------------------------------------------
	void Update() const
	{
		if (m_pGlobalState)
		{
			m_pGlobalState->Execute(m_pOwner);
		}

		if (m_pCurrentState)
		{
			m_pCurrentState->Execute(m_pOwner);
		}
	}

	//------------------------------------------------------------------------
	// 处理消息
	// @return 返回消息是否被成功的处理了
	//------------------------------------------------------------------------
	bool HandleMessage(const Message* pMsg)
	{
		if (m_pCurrentState)
		{
			if (m_pCurrentState->OnMessage(m_pOwner, pMsg))
			{
				return true;
			}
		}

		if (m_pGlobalState)
		{
			if (m_pGlobalState->OnMessage(m_pOwner, pMsg))
			{
				return true;
			}
		}

		return false;
	}

	//------------------------------------------------------------------------
	// 改变状态
	//------------------------------------------------------------------------
	void ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState && "[StateMachine::ChangeState] pNewState == nullptr");
		// 保留上一个状态的记录
		m_pPreviousState = m_pCurrentState;
		// 调用现有状态的退出方法
		m_pCurrentState->Exit(m_pOwner);
		// 改变状态
		m_pCurrentState = pNewState;
		// 调用新状态的进入方法
		m_pCurrentState->Enter(m_pOwner);
	}

	//------------------------------------------------------------------------
	// 回到前一个状态
	//------------------------------------------------------------------------
	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	//------------------------------------------------------------------------
	// 在某个状态中
	//------------------------------------------------------------------------
	bool IsInState(State<entity_type>* pState)
	{
		return (m_pCurrentState == pState);
	}

private:
	// 拥有这个实例的指针
	entity_type* m_pOwner = nullptr;

	// 当前状态
	State<entity_type>* m_pCurrentState = nullptr;
	// 上一个状态的记录
	State<entity_type>* m_pPreviousState = nullptr;
	// 全局状态
	State<entity_type>* m_pGlobalState = nullptr;
};
