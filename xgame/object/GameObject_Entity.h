//------------------------------------------------------------------------
// * @filename: GameObject_Entity.h
// *
// * @brief: 游戏实体
// *
// * @author: XGM
// * @date: 2017/01/10
//------------------------------------------------------------------------
#pragma once

#include "xgame/message/MessageHandler.h"

#include "GameObject.h"

struct Message;

class GameObject_Entity : public GameObject, public MessageHandler
{
public:
};
