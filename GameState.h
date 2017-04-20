
#pragma once
#include "GameStandard.h"

class GameState {
private:
	//-----成员变量-----
	int   nGameState;   // 当前游戏状态

public:
	//-----成员函数（公有）-----
	GameState();
	~GameState();
	void  ResetState();							    // 重置游戏状态
	void  StateChange(const int & state);		    // 改变游戏状态
	int   GetState();							    // 获取当前游戏状态
};
//-----函数实现（内联）-----
inline		 GameState::GameState() {
	ResetState();
}
inline		 GameState::~GameState() {}
inline void	 GameState::ResetState() {
	nGameState = state_building;

}
inline void  GameState::StateChange(const int & state) {
	nGameState = state;
}
inline int   GameState::GetState() {
	return nGameState;
}