#include "D3DObject.h"
//#include "Algorithm.h"

const float MAX_SPEED = 80.0f;		//��������ٶ�

void D3DObject::BindFxFile(WCHAR* fxPath,ID3D11Device* device) {
	HRESULT hr = S_OK;
	ID3DBlob* pTechBlob = NULL;
	// ��fx�ļ���ȡ��ɫ�������Ϣ
	hr = D3DX11CompileFromFile(
		fxPath,				// ��Ӧ�ĸ߼���ɫ���Խű�
		NULL,			    // ��ѡ�߼����������ﲻʹ��
		NULL,			    // ��ѡ�߼����������ڴ�����ɫ���е�Include�ļ������ﲻʹ��
		NULL,               // ��Ӧ����ɫ����������������fx�ļ������Բ���ָ������ĺ���
		"fx_5_0",		    // ��Ӧ��fx��ɫ���汾
		D3DCOMPILE_ENABLE_STRICTNESS, // ʹ��Լ������ֹʹ�ù��ڵ��﷨
		NULL,						  // Effect�ļ�����ѡ����ﲻʹ��
		NULL,						  // ���̱߳��ʱ����ָ���سǱã����ﲻ�漰
		&pTechBlob,					  // ���ر���õ�������ɫ�������ڴ��ָ��
		NULL,
		NULL						  // ָ�򷵻�ֵ��ָ�룬���ﲻʹ��
		);
	ReportError(hr, L"Failed to compile fx file!");

	// ����ID3DEffect����
	hr = D3DX11CreateEffectFromMemory(
		pTechBlob->GetBufferPointer(),
		pTechBlob->GetBufferSize(),
		0,        // ��ʹ��Effect��ʶ
		device,  // �豸ָ�룬��������Effect��Դ
		&pEffect  // ���ش����õ�ID3DEffect����
		);
	ReportError(hr, L"Failed to create effect!");
}
void D3DObject::BindTexture(LPCWSTR texPath, ID3D11Device* device){
	D3DX11CreateShaderResourceViewFromFile(device, texPath, NULL, NULL, &pTexture, NULL);
}
void D3DObject::BindVertice(Vertex* vertice, const UINT & vertexNum) {
	//-----��Ա��������-----
	pVertice = vertice;
	nVertexNum = vertexNum;
	nVertexStructSize = sizeof(Vertex);
}
void D3DObject::BindIndice(unsigned int* indice, const UINT & indexNum)
{
	pIndice = indice;
	nIndexNum = indexNum;
	nIndexStructSize = sizeof(unsigned int);
}
void D3DObject::CreateInputLayout(ID3D11Device* device){
	//-----��ȡPassͨ���ṹ����ʵ��fx�ļ��ṹ����-----
	pTechnique = pEffect->GetTechniqueByName("TexTech");
	D3DX11_PASS_DESC passDesc;
	HRESULT hr = pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	ReportError(hr, L"Get Pass Failed!");

	//-----����D3D�豸��ʶ������벼��-----
	hr = device->CreateInputLayout(
		layout,							// GameStandard�����D3D11_INPUT_ELEMENT_DESC����
		nInputLayoutDescElemNum,		// D3D11_INPUT_ELEMENT_DESC�����Ԫ�ظ���
		passDesc.pIAInputSignature,		// ��ʵ��4��ͬ����������������Effect Pass�����������ʶ
		passDesc.IAInputSignatureSize,	// ��ʵ��4��ͬ����������������Effect Pass�����������ʶ�Ĵ�С
		&pVertexLayout);				// �������ɵ����벼�ֶ���
	ReportError(hr, L"����Input Layoutʧ��");
}
void D3DObject::CreateVertexBuffer(ID3D11Device* device){
	//-----���㻺��ṹ-----
	D3D11_BUFFER_DESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				  // �����д��ʽ��Ĭ��
	vertexBufferDesc.ByteWidth = nVertexStructSize * nVertexNum;  // ���û��������С
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	      // �������������󶨵����㻺��
	vertexBufferDesc.CPUAccessFlags = 0;						  // CPU���ʱ�ʶ����0��ʾû��CPU����
	vertexBufferDesc.MiscFlags = 0;							      // �������ʶ����0��ʾ��ʹ�ø���

	//-----���ڳ�ʼ������Դ������-----
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = pVertice; // ������Ҫ��ʼ�������ݣ�����Ӧ�Ķ�������

	//-----����D3D�豸��ʶ��Ķ��㻺��-----
	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &initData, &pVertexBuffer);
	ReportError(hr, L"����VertexBufferʧ��");
}
void D3DObject::CreateIndexBuffer(ID3D11Device* device) {
	//-----��������ṹ-----
	D3D11_BUFFER_DESC indexBufferDesc;
	memset(&indexBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				  // �����д��ʽ��Ĭ��
	indexBufferDesc.ByteWidth = nIndexStructSize * nIndexNum;  // ���û��������С
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	      // �������������󶨵���������
	indexBufferDesc.CPUAccessFlags = 0;						  // CPU���ʱ�ʶ����0��ʾû��CPU����
	indexBufferDesc.MiscFlags = 0;							      // �������ʶ����0��ʾ��ʹ�ø���

																  //-----���ڳ�ʼ������Դ������-----
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = pIndice; // ������Ҫ��ʼ�������ݣ�����Ӧ�Ķ�������

								 //-----����D3D�豸��ʶ�����������-----
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &initData, &pIndexBuffer);
	ReportError(hr, L"����IndexBufferʧ��");
}

void D3DObject::Draw(ID3D11DeviceContext* immediateContext, const CoordinateSystem & coord){
	// ��������ϵ���浽Effect
	pEffect->GetVariableByName("World")->AsMatrix()->SetMatrix((float*)&(coord.world));            
	pEffect->GetVariableByName("View")->AsMatrix()->SetMatrix((float*)&(coord.view));              
	pEffect->GetVariableByName("Projection")->AsMatrix()->SetMatrix((float*)&(coord.projection)); 
	pEffect->GetVariableByName("EyePosition")->AsMatrix()->SetMatrix((float*)&(coord.eyePosition));

	// ���ò��ʵ�Effect
	pEffect->GetVariableByName("Texture")->AsShaderResource()->SetResource(pTexture);

	// ���ô����õ����벼�ֵ�����ִ���豸������
	immediateContext->IASetInputLayout(pVertexLayout);
	
	// ���ô����õĶ��㻺�浽����ִ���豸������
	UINT nOffset = 0;
	immediateContext->IASetVertexBuffers(
		0,				    // �󶨵ĵ�һ�������
		1,				    // ���㻺��ĸ���������Ϊ1��
		&pVertexBuffer,	    // �����õĶ��㻺��
		&nVertexStructSize,	// ���
		&nOffset		    // �����һ��Ԫ�ص�����Ԫ�ص�ƫ����
		);

	immediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// ָ��ͼԪ����Ϊ������
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// ��Passͨ��Ӧ�õ�����ִ���豸������
	pTechnique->GetPassByIndex(0)->Apply(0, immediateContext);
	
	pTechnique = pEffect->GetTechniqueByName("TexTech");
	D3DX11_PASS_DESC passDesc;
	HRESULT hr = pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	ReportError(hr, L"Get Pass Failed!");
	// ͨ�����㻺�����
	immediateContext->DrawIndexed(nIndexNum, 0, 0);
}

float Car::CalSpeed(){
	if (carSpeed.velocity < MAX_SPEED)
		carSpeed.velocity = carSpeed.velocity + carAccele.accele;
	else if ((carSpeed.velocity>= MAX_SPEED))
		carSpeed.velocity = MAX_SPEED;

	return carSpeed.velocity;

}
void Car::CarYaw(float angle){
	
	XMMatrixRotationY(angle);

}





	
