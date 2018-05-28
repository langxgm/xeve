//------------------------------------------------------------------------
// * @filename: GameObjectAttribute.h
// *
// * @brief: 游戏对象的属性
// *
// * @author: XGM
// * @date: 2017/01/10
//------------------------------------------------------------------------
#pragma once

#include <stdint.h>

class GameObjectAttribute
{
public:
	virtual int32_t		GetIntValue(int32_t nType) = 0;
	virtual void		SetIntValue(int32_t nType, int32_t nValue) = 0;

	virtual float		GetFloatValue(int32_t nType) = 0;
	virtual void		SetFloatValue(int32_t nType, float fValue) = 0;

	virtual const char*	GetStringValue(int32_t nType) = 0;
	virtual void		SetStringValue(int32_t nType, const char* pszValue) = 0;
};

template<typename T>
struct GameObjectAttrFunc
{
	typedef T value_type;

	static value_type	Get(GameObjectAttribute* pAttr, int32_t nType);
	static void			Set(GameObjectAttribute* pAttr, int32_t nType, value_type val);
};

template<>
struct GameObjectAttrFunc<int32_t>
{
	typedef int32_t value_type;

	static value_type	Get(GameObjectAttribute* pAttr, int32_t nType)
	{
		return pAttr->GetIntValue(nType);
	}
	static void			Set(GameObjectAttribute* pAttr, int32_t nType, value_type nVal)
	{
		pAttr->SetIntValue(nType, nVal);
	}
};

template<>
struct GameObjectAttrFunc<float>
{
	typedef float value_type;

	static value_type	Get(GameObjectAttribute* pAttr, int32_t nType)
	{
		return pAttr->GetFloatValue(nType);
	}
	static void			Set(GameObjectAttribute* pAttr, int32_t nType, value_type fVal)
	{
		pAttr->SetFloatValue(nType, fVal);
	}
};

template<>
struct GameObjectAttrFunc<char*>
{
	typedef const char* value_type;

	static value_type	Get(GameObjectAttribute* pAttr, int32_t nType)
	{
		return pAttr->GetStringValue(nType);
	}
	static void			Set(GameObjectAttribute* pAttr, int32_t nType, value_type pszVal)
	{
		pAttr->SetStringValue(nType, pszVal);
	}
};
