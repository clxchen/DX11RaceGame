
#include "Game.h"
Game game; // ��Ϸ����

const float ACCELE = 0.01f; //����ǰ�����ٶ�
const float MAX_SPEED = 80.0f;		//��������ٶ�
const float FRICTION = 0.05f;		//ɲ��ʱ�ļ��ٶ�
const float SCROLL_FRICTION = 0.005f;//����Ȼ����ʱ�ļ��ٶ�


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
// ��Ϣѭ��������
void Run(float timeDelta){
	float ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	game.pD3DDevice->ClearView(ClearColor);
	switch (game.pGameState->GetState()){
	case state_building:
		//MessageBox(NULL, L"Still Building", L"Error", NULL);
		break;
	case state_ready:{ // ��ʱ��Ϸ�ѿ�ʼ����ҿ����ƶ�λ�ú��ӽ�
						 // �ƶ�λ�ú��ӽǵİ�����Ӧ
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
						 
						 // ���´��ڻ���
						 game.UpdateWindow(timeDelta);
	}break;
		/*
		case state_chargeUp:{
		// �ƶ�λ�ú��ӽǵİ�����Ӧ
		TransRespond(timeDelta);
		// ����������Ӧ
		if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
		game.pGameState->ChargePowerUp(0.5f);
		else{
		PlaySound(L"Ͷ��.wav", NULL, SND_ALIAS | SND_ASYNC); // ������Ч
		game.pGameState->StateChange(state_shooting);
		XMFLOAT3 dir = XMFLOAT3(game.pCamera->GetCurrAt().x - game.pCamera->GetCurrEye().x, game.pCamera->GetCurrAt().y - game.pCamera->GetCurrEye().y, game.pCamera->GetCurrAt().z - game.pCamera->GetCurrEye().z);
		game.pBall->SetVelocity(dir, game.pGameState->GetChargePower() / 5.0f);
		break;
		}
		// ��������������δ�ͷſո��������½�
		if (game.pGameState->GetChargePower() == MAX_CHARGE_POWER){
		game.pGameState->StateChange(state_chargeDown);
		break;
		}
		// ���´��ڻ���
		game.UpdateWindow(timeDelta);
		}break;
		case state_chargeDown:{
		// �ƶ�λ�ú��ӽǵİ�����Ӧ
		TransRespond(timeDelta);
		// ����������Ӧ
		if (::GetAsyncKeyState(VK_SPACE) & 0x8000f)
		game.pGameState->ChargePowerDown(0.5f);
		else{
		PlaySound(L"Ͷ��.wav", NULL, SND_ALIAS | SND_ASYNC);
		game.pGameState->StateChange(state_shooting);
		XMFLOAT3 dir = XMFLOAT3(game.pCamera->GetCurrAt().x - game.pCamera->GetCurrEye().x, game.pCamera->GetCurrAt().y - game.pCamera->GetCurrEye().y, game.pCamera->GetCurrAt().z - game.pCamera->GetCurrEye().z);
		game.pBall->SetVelocity(dir, game.pGameState->GetChargePower() / 5.0f);
		break;
		}
		// �����ۿպ�����δ�ͷſո�����������
		if (game.pGameState->GetChargePower() == MIN_CHARGE_POWER){
		game.pGameState->StateChange(state_chargeUp);
		break;
		}
		// ���´��ڻ���
		game.UpdateWindow(timeDelta);
		}break;
		case state_shooting:{
		static int nTimes = 0; // ��ֹ�㷨�ϵ�BUG������ѭ��
		nTimes++;
		if (nTimes > 1000){ // ��������㷨����ѭ��������Ļˢ��1000�κ�������ѭ��
		game.pGameState->StateChange(state_showResult);
		nTimes = 0;
		}

		// ���´��ڻ���
		game.UpdateWindow(timeDelta);
		// ��ʼ�ж���ƽ��
		game.CheckShooting();
		// ��������˽����������չʾ״̬
		if (game.nResult != result_none){
		game.pGameState->StateChange(state_showResult);
		nTimes = 0;
		}
		}break;
		*/
	case state_showResult:{
							  static bool bPlaySound = true;
							  if (bPlaySound){
								  switch (game.nRound){	// ������Ч
								  }
								  bPlaySound = false;
							  }
							  // ��¼ʱ�䣬���չʾ��ʱ��Ϊ1������
							  static float fShowTime = 0.0f;
							  fShowTime += timeDelta;
							  // ���´��ڻ���
							  game.UpdateWindow(timeDelta);
							  // չʾʱ�䵽������һЩ״̬
							  /*
							  if (fShowTime > 1.0f){
							  fShowTime = 0.0f; // ����ʱ��
							  if (game.nResult == result_goal){ game.pGameState->ScoreUp(100 * (game.GetDifficulty() + 1)); } // �ӷ�
							  // ״̬����
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
// ��Ϣѭ������
int EnterMsgLoop(void(*run)(float timeDelta))
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));
	static float fLastTime = static_cast<float>(timeGetTime()); // ��ȡ��ǰʱ�䣬��Ϊ��һ�μ�¼��ʱ��
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){ // Ϊһ����Ϣ����߳���Ϣ���У���Ϣ�ɵõ������ط�0ֵ������0
			TranslateMessage(&msg); // ����Ϣmsg�������ת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg); // ����Ϣ�����ص������е�WndProc��
		}
		else{
			float fCurrTime = static_cast<float>(timeGetTime()); // ��ȡ��ǰʱ��
			run((fCurrTime - fLastTime)*0.001f); // ������ʾ����������ʵ��ͼ�εı仯ʱ�õ�
			fLastTime = fCurrTime;
		}
	}
	return msg.wParam;
}
// �ص�����
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
// ���������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	game.Build(hInstance);  // ���Ϸ����
	EnterMsgLoop(Run);      // ������Ϸ
	return 0;
}


