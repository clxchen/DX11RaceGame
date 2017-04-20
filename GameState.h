
#pragma once
#include "GameStandard.h"

class GameState {
private:
	//-----��Ա����-----
	int   nGameState;   // ��ǰ��Ϸ״̬

public:
	//-----��Ա���������У�-----
	GameState();
	~GameState();
	void  ResetState();							    // ������Ϸ״̬
	void  StateChange(const int & state);		    // �ı���Ϸ״̬
	int   GetState();							    // ��ȡ��ǰ��Ϸ״̬
};
//-----����ʵ�֣�������-----
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