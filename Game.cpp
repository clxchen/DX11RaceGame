
#include "Game.h"
Game game; // 游戏对象

const float ACCELE = 0.01f; //车的前进加速度
const float MAX_SPEED = 80.0f;		//车的最大速度
const float FRICTION = 0.05f;		//刹车时的加速度
const float SCROLL_FRICTION = 0.005f;//车自然滑动时的加速度


const XMFLOAT3 AHEAD = { 0.0f, 0.0f, 1.0f };




void TransRespond(const float & timeDelta){

	float tempangle = 0.0f;
	CarAccele ac = game.pCar->GetAccele();
		
		float speed = game.pCar->GetSpeed().velocity;
	XMFLOAT3 p = game.pCar->GetPosition();
	float angle =  game.pCar->GetAngle();

	if (::GetAsyncKeyState(VK_UP) & 0x8000f){
		game.pCar->SetAccele(ACCELE, AHEAD);
		 speed = game.pCar->CalSpeed();
		game.pCar->SetSpeed(speed, AHEAD);

	}
	if (::GetAsyncKeyState(VK_DOWN) & 0x8000f){
		
		game.pCar->SetAccele( -ACCELE, AHEAD);
		speed = game.pCar->CalSpeed();
		game.pCar->SetSpeed(speed, AHEAD);
	
	
		
	}
	if (::GetAsyncKeyState(VK_LEFT) & 0x8000f){
		if (game.pCar->GetSpeed().velocity >= 2.0f || game.pCar->GetSpeed().velocity <= -20.0f)
			tempangle = -2.0f * timeDelta;
		else
			tempangle = -100.0f*game.pCar->GetSpeed().velocity *timeDelta;
	

		
		//transX += speed*sin(angle);
		//transZ += speed*cos(angle);
	}
	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f){
		if (game.pCar->GetSpeed().velocity >=2.0f || game.pCar->GetSpeed().velocity <= -20.0f)
			tempangle = 2.0f * timeDelta;
		else
			tempangle = 100.0f*game.pCar->GetSpeed().velocity *timeDelta;
		
		
		
	}
	
	p.z = p.z + speed*timeDelta*cos(game.pCar->GetAngle());
	p.x = p.x+speed*timeDelta*sin(game.pCar->GetAngle());
	angle += tempangle;
	game.pCar->SetAngle(angle);

	game.pCar->SetPosition(p);


	game.pCamera->SetView(game.pCar->GetPosition(), tempangle);
}
// 消息循环处理函数
void Run(float timeDelta){
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	game.pD3DDevice->ClearView(ClearColor);
	switch (game.pGameState->GetState()){
	case state_building:
		//MessageBox(NULL, L"Still Building", L"Error", NULL);
		break;
	case state_ready:{ // 此时游戏已开始，玩家可以移动位置和视角
						 // 移动位置和视角的按键响应
						 TransRespond(timeDelta);
						 //MessageBox(NULL, L"Ready", L"Error", NULL);
						 if (::GetAsyncKeyState('S') & 0x8000f) {
						 game.pCamera->MoveForwardBy(-0.0001f);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('W') & 0x8000f) {
						 game.pCamera->MoveForwardBy(0.0001f);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('A') & 0x8000f) {
						 game.pCamera->MoveRightBy(-0.0001f);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('D') & 0x8000f) {
						 game.pCamera->MoveRightBy(0.0001f);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('I') & 0x8000f) {
						 game.pCamera->Pitch(0.1f * timeDelta);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('K') & 0x8000f) {
						 game.pCamera->Pitch(-0.1f * timeDelta);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('J') & 0x8000f) {
						 game.pCamera->Yaw(0.1f * timeDelta);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState('L') & 0x8000f) {
						 game.pCamera->Yaw(-0.1f * timeDelta);
						 game.pCamera->Apply();
						 break;
						 }
						 if (::GetAsyncKeyState(VK_SPACE) & 0x8000f) {

							 game.pCamera->SwitchView();
						 break;
						 }
						 
						 // 更新窗口绘制
						 game.UpdateWindow(timeDelta);
	}break;
		/*
		case state_chargeUp:{
		// 移动位置和视角的按键响应
		TransRespond(timeDelta);
		// 蓄力按键响应
		if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
		game.pGameState->ChargePowerUp(0.5f);
		else{
		PlaySound(L"投篮.wav", NULL, SND_ALIAS | SND_ASYNC); // 播放音效
		game.pGameState->StateChange(state_shooting);
		XMFLOAT3 dir = XMFLOAT3(game.pCamera->GetCurrAt().x - game.pCamera->GetCurrEye().x, game.pCamera->GetCurrAt().y - game.pCamera->GetCurrEye().y, game.pCamera->GetCurrAt().z - game.pCamera->GetCurrEye().z);
		game.pBall->SetVelocity(dir, game.pGameState->GetChargePower() / 5.0f);
		break;
		}
		// 蓄力槽满后若还未释放空格，则蓄力下降
		if (game.pGameState->GetChargePower() == MAX_CHARGE_POWER){
		game.pGameState->StateChange(state_chargeDown);
		break;
		}
		// 更新窗口绘制
		game.UpdateWindow(timeDelta);
		}break;
		case state_chargeDown:{
		// 移动位置和视角的按键响应
		TransRespond(timeDelta);
		// 蓄力按键响应
		if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
		game.pGameState->ChargePowerDown(0.5f);
		else{
		PlaySound(L"投篮.wav", NULL, SND_ALIAS | SND_ASYNC);
		game.pGameState->StateChange(state_shooting);
		XMFLOAT3 dir = XMFLOAT3(game.pCamera->GetCurrAt().x - game.pCamera->GetCurrEye().x, game.pCamera->GetCurrAt().y - game.pCamera->GetCurrEye().y, game.pCamera->GetCurrAt().z - game.pCamera->GetCurrEye().z);
		game.pBall->SetVelocity(dir, game.pGameState->GetChargePower() / 5.0f);
		break;
		}
		// 蓄力槽空后若还未释放空格，则蓄力上升
		if (game.pGameState->GetChargePower() == MIN_CHARGE_POWER){
		game.pGameState->StateChange(state_chargeUp);
		break;
		}
		// 更新窗口绘制
		game.UpdateWindow(timeDelta);
		}break;
		case state_shooting:{
		static int nTimes = 0; // 防止算法上的BUG进入死循环
		nTimes++;
		if (nTimes > 1000){ // 如果进入算法的死循环，在屏幕刷新1000次后跳出死循环
		game.pGameState->StateChange(state_showResult);
		nTimes = 0;
		}

		// 更新窗口绘制
		game.UpdateWindow(timeDelta);
		// 开始判断设计结果
		game.CheckShooting();
		// 若计算出了结果，进入结果展示状态
		if (game.nResult != result_none){
		game.pGameState->StateChange(state_showResult);
		nTimes = 0;
		}
		}break;
		*/
	case state_showResult:{
							  static bool bPlaySound = true;
							  if (bPlaySound){
								  switch (game.nRound){	// 播放音效
								  }
								  bPlaySound = false;
							  }
							  // 记录时间，结果展示的时间为1秒左右
							  static float fShowTime = 0.0f;
							  fShowTime += timeDelta;
							  // 更新窗口绘制
							  game.UpdateWindow(timeDelta);
							  // 展示时间到，重置一些状态
							  /*
							  if (fShowTime > 1.0f){
							  fShowTime = 0.0f; // 重置时间
							  if (game.nResult == result_goal){ game.pGameState->ScoreUp(100 * (game.GetDifficulty() + 1)); } // 加分
							  // 状态重置
							  game.nResult = result_none;
							  game.pGameState->ResetChargePower();
							  game.pBall->SetOffset(0.0f, 0.0f, 0.0f);
							  game.pGameState->StateChange(state_ready);
							  bPlaySound = true;
							  }
							  */
	}
	}
}
// 消息循环函数
int EnterMsgLoop(void(*run)(float timeDelta))
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));
	static float fLastTime = static_cast<float>(timeGetTime()); // 获取当前时间，作为上一次记录的时间
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){ // 为一个消息检查线程消息队列，消息可得到，返回非0值，否则0
			TranslateMessage(&msg); // 将消息msg的虚拟键转换为字符信息
			DispatchMessage(&msg); // 将消息传到回调函数中的WndProc中
		}
		else{
			float fCurrTime = static_cast<float>(timeGetTime()); // 获取当前时间
			run((fCurrTime - fLastTime)*0.001f); // 调用显示函数，后面实现图形的变化时用到
			fLastTime = fCurrTime;
		}
	}
	return msg.wParam;
}
// 回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
// 程序主入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	game.Build(hInstance);  // 搭建游戏环境
	EnterMsgLoop(Run);      // 运行游戏
	return 0;
}


