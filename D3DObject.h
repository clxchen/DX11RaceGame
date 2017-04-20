#pragma once
#include "GameStandard.h"

struct CarSpeed
{
	float velocity;
	XMFLOAT3 vDirection;
};

struct CarAccele
{
	float accele;
	XMFLOAT3 aDirection;
};

/************���ࣺD3D������************/
class D3DObject{
protected:
	//-----��Ա����-----
	Vertex*  pVertice;						 // ��������
	UINT	 nVertexNum;					 // ������Ŀ
	UINT	 nVertexStructSize;			     // ����ṹ�Ĵ�С
	unsigned int* pIndice;                   // ��������
	UINT     nIndexNum;                      // ������Ŀ
	UINT	 nIndexStructSize;               // �����ṹ�Ĵ�С
	XMFLOAT3 offset;						 // �����ԭ��������������������ϵ�е�λ�ã�Ҫλ�Ƶ���
	ID3D11InputLayout*		  pVertexLayout; // ��D3D�豸���ɵ�����ʶ��Ķ������벼�ֶ���ָ�룬��Ҫ����ʱ����󶨣��������һ�μ��ɣ��Ժ�ֱ��ȡ�����ƣ���Ȼÿ�λ���ǰ��Ҫ�ظ������ò�����
	ID3DX11Effect*		      pEffect;	     // ��Effect�ļ��󶨺���D3D�豸������Effect��Դ��ָ��
	ID3DX11EffectTechnique*   pTechnique;    // ��Effect�ļ���Technique������ָ�룬��Ҫ����ʱ���л�ȡpass�ṹ
	ID3D11Buffer*		      pVertexBuffer; // ָ��D3D�豸���ɵĶ��㻺���ָ�룬��Ҫ����ʱ����󶨣��������һ�μ��ɣ��Ժ�ֱ��ȡ�����ƣ���Ȼÿ�λ���ǰ��Ҫ�ظ������ò�����
	ID3D11Buffer*		      pIndexBuffer;  // ��������
	ID3D11ShaderResourceView* pTexture;      // ��D3D�豸���ɵ�����ʶ��Ĳ��ʶ���ָ�룬��Ҫ����ʱ����󶨣��������һ�μ��ɣ��Ժ�ֱ��ȡ�����ƣ���Ȼÿ�λ���ǰ��Ҫ�ظ������ò�����

public:
	//-----��Ա���������У�-----
	D3DObject();
	~D3DObject();

	XMFLOAT3 GetOffset();														  // ��ȡλ����
	void	 SetOffset(float offsetX, float offsetY, float offsetZ);			  // ����λ����
	void	 AppendOffset(float offsetX, float offsetY, float offsetZ);			  // ��ԭ��λ�����Ļ����ϼ����µ�λ����
	const XMMATRIX&		      GetOffsetTranslationMatrix();						  // ��ȡλ�Ʊ任����
	ID3D11InputLayout*		  GetVertexLayout();								  // ��ȡ���벼�ֶ���ָ��
	ID3D11Buffer*			  GetVertexBuffer();								  // ��ȡ���㻺��ָ��
	ID3DX11EffectTechnique*   GetTechnique();									  // ��ȡTechniqueָ��
	ID3DX11Effect*            GetEffect();	// ��ȡEffectָ��


	//-----��Ա���������⣩-----
	virtual void BindFxFile(WCHAR* fxPath, ID3D11Device* device);       // ��fx�ļ��󶨣������ð󶨵�D3D�豸������Effect��Դ����Ա����ָ��pEffectָ���������Դ
	virtual void BindTexture(LPCWSTR texPath, ID3D11Device* device);     // ������ļ��󶨣���D3D�豸���ɵ�����ʶ���������󣬳�Ա����ָ��pTextureָ���������Դ
	virtual void BindIndice(unsigned int* indice, const UINT & indexNum);// �󶨺���Ҫ���Ƶ��������У���ʵ����������������ͼ�������
	virtual void BindVertice(Vertex* vertice, const UINT & vertexNum);	// �󶨺���Ҫ���ƵĶ������У���ʵ����������������ͼ�������
	virtual void CreateInputLayout(ID3D11Device* device);			    // ������������벼�֣���D3D�豸���ɵ�����ʶ��Ķ������벼�ֶ��󣬳�Ա����ָ��pVertexLayoutָ���������
	virtual void CreateVertexBuffer(ID3D11Device* device);			    // ��������Ķ��㻺�棬��D3D�豸���ɵ�����ʶ��Ķ��㻺����󣬳�Ա����ָ��pVertexBufferָ���������
	virtual void CreateIndexBuffer(ID3D11Device* device);			    // ����������������棬��D3D�豸���ɵ�����ʶ�������������󣬳�Ա����ָ��pIndexBufferָ���������
	virtual void Draw(ID3D11DeviceContext* immediateContext,
		const CoordinateSystem & coord);						        // ��D3D�豸�ϻ��ƶ���

private:
	//-----��Ա����������ʵ�֣�-----
	void ReleasePointers(); // �ͷ�ָ��
	void Init();			// ��ʼ����Ա����

};
//-----����ʵ�֣�������-----
inline		D3DObject::D3DObject(){
	Init();
}
inline		D3DObject::~D3DObject(){
	ReleasePointers();
}
inline void D3DObject::Init(){
	//-----Ĭ��ֵ-----
	pVertice = NULL;
	nVertexNum = 0;
	nVertexStructSize = 0;
	offset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	pVertexLayout = NULL;
	pEffect = NULL;
	pTechnique = NULL;
	pVertexBuffer = NULL;
	pTexture = NULL;
}


inline void D3DObject::ReleasePointers(){
	Release(pVertexLayout);
	Release(pEffect);
	Release(pVertexBuffer);
	Release(pTexture);
}
inline XMFLOAT3	 D3DObject::GetOffset() {
	return offset;
}
inline void						 D3DObject::SetOffset(float offsetX, float offsetY, float offsetZ) {
	offset = XMFLOAT3(offsetX, offsetY, offsetZ);
}
inline void						 D3DObject::AppendOffset(float offsetX, float offsetY, float offsetZ) {
	offset.x += offsetX;
	offset.y += offsetY;
	offset.z += offsetZ;
}
inline const XMMATRIX&		     D3DObject::GetOffsetTranslationMatrix() {
	return XMMatrixTranslation(offset.x, offset.y, offset.z);
}
inline ID3D11InputLayout*		 D3DObject::GetVertexLayout() {
	return pVertexLayout;
}
inline ID3D11Buffer*			 D3DObject::GetVertexBuffer() {
	return pVertexBuffer;
}
inline ID3DX11EffectTechnique*   D3DObject::GetTechnique(){
	return pTechnique;
}
inline ID3DX11Effect*			 D3DObject::GetEffect(){
	return pEffect;
}

/************���ࣺ����************/
class Car : public D3DObject{
private:
	//-----��Ա����-----
	XMFLOAT3 carVertex[2]; //������
	CarSpeed carSpeed;		//���ٶ�
	CarAccele carAccele;		//�����ٶ�
	XMFLOAT3 carPosition;   // ��λ��
	D3DObject* NOS;			//������
	float  NOSf;		//����ֵ
	float carAngle;		//���ĽǶ�


public:
	//-----��Ա���������У�-----
	Car();
	~Car();
	void	 SetBoundingBoxVertex(XMFLOAT3 carVertex[]); // ���ó�����
	XMFLOAT3* GetBoundingBoxVertex();						//��ȡ������	

	void     SetSpeed(float v,XMFLOAT3 d);	//���ó��ٶ�
	CarSpeed   GetSpeed();					//��ȡ���ٶ�

	void  SetAccele(float a,XMFLOAT3 d);			//���ó����ٶ�
	CarAccele GetAccele();						//��ȡ�����ٶ�

	XMFLOAT3 GetPosition();					//��ȡ��λ��
	void SetPosition(XMFLOAT3 p);					//���ó���λ��

	
	void CarYaw(float angle);				//������ת�Ƕ�
	

	float CalSpeed();                       //���㳵�ٶ�
	void SetAcceleDirection(XMFLOAT3);

	void SetAngle(float angle);
	float GetAngle();

	XMFLOAT3 InitPosition();
	

	//void Draw(ID3D11DeviceContext* immediateContext, const CoordinateSystem & );
};
//-----����ʵ�֣�������-----
inline			Car::Car() : D3DObject(){
	carVertex[0] = { 0.0f, 0.0f, 0.0f };
	carVertex[1] = { 0.0f, 0.0f, 0.0f };
	carSpeed.velocity = 0;
	carSpeed.vDirection = { 0, 0, 0 };
	carAccele.accele = 0;
	carAccele.aDirection = { 0, 0, 0 };
	carPosition = { 0, 0, 0 };
}
inline void Car::SetBoundingBoxVertex(XMFLOAT3 carvertex[]){
	int i = 0;
	for (i = 0; i < 2; i++)
		carVertex[i] = carvertex[i];
}

inline XMFLOAT3* Car::GetBoundingBoxVertex()
{
	return carVertex;
}

inline			Car::~Car(){
}
inline void		Car::SetSpeed(float v,XMFLOAT3 d) {
	carSpeed.velocity = v;
	carSpeed.vDirection = d;
}
inline CarSpeed Car::GetSpeed(){
	return carSpeed;
}
inline XMFLOAT3 Car::GetPosition(){
	return carPosition;

}
inline XMFLOAT3 Car::InitPosition(){
	XMFLOAT3 position;
	position.x = (carVertex[0].x + carVertex[1].x) / 2;
	position.y = (carVertex[0].y + carVertex[1].y) / 2;
	position.z = (carVertex[0].z + carVertex[1].z) / 2;

	
	return position;
}

inline void Car::SetPosition(XMFLOAT3 p)
{
	carPosition = p;
}

inline void	Car::SetAccele(float a,XMFLOAT3 d) {
	carAccele.accele = a;
	carAccele.aDirection = d;
}

inline CarAccele Car::GetAccele(){
	return carAccele;
}
inline void Car::SetAngle(float angle){
	carAngle = angle;
}
inline float Car::GetAngle(){
	return carAngle;
}

