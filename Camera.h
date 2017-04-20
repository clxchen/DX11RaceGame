#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "GameStandard.h"

class Camera
{
	enum TransformFlag{
		None = 0,
		Rotate = (1 << 0),
		Translate = (1 << 1)
	};
public:
	Camera();
	void Pitch(float angle);
	void Yaw(float angle);
	void init();
	void SetEye(const XMVECTOR& position);
	void SetUp(const XMVECTOR& postion);
	void SetAt(const XMVECTOR& postion);
	const XMVECTOR& GetEye();
	const XMVECTOR& GetAt();
	const XMVECTOR& GetUp();

	void MoveForwardBy(float value);
	void MoveRightBy(float value);
	void MoveUpBy(float value);

	void SetView( XMFLOAT3 carPos, float angle);
	void SwitchView();
	
	void Apply();

	const XMFLOAT4X4& GetView();
private:
	float height;
	float distance;
	UINT flag;
	float rotateX;
	float rotateY;
	float rotateZ;
	 bool viewType;

	XMVECTOR move;
	XMFLOAT4X4 cameraRotation;
	XMVECTOR eye;
	XMVECTOR up;
	XMVECTOR at;
	XMFLOAT4X4 view;
};



#endif