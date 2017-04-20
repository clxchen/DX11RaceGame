
#pragma once
#include "GameStandard.h"

namespace IAlgorithm {
	static void NormalizeVelocityDirection(Velocity & v){
		// ��λ�����������ע�⿪�������ֵ������������Ҫ����ԭ�����ٶȷ���
		float fSquareSum = v.Direction.x*v.Direction.x + v.Direction.y*v.Direction.y + v.Direction.z*v.Direction.z;
		v.Direction.x = (v.Direction.x < 0) ? -sqrt(v.Direction.x*v.Direction.x / fSquareSum) : sqrt(v.Direction.x*v.Direction.x / fSquareSum);
		v.Direction.y = (v.Direction.y < 0) ? -sqrt(v.Direction.y*v.Direction.y / fSquareSum) : sqrt(v.Direction.y*v.Direction.y / fSquareSum); 
		v.Direction.z = (v.Direction.z < 0) ? -sqrt(v.Direction.z*v.Direction.z / fSquareSum) : sqrt(v.Direction.z*v.Direction.z / fSquareSum);
	}
	static float ComputeTwoVectorAngle(const XMVECTOR & v1, const XMVECTOR & v2){ // �������������нǻ���ֵ
			XMVECTOR dot = XMVector3Dot(v1, v2);
			float fDot = XMVectorGetX(dot); // ��������˵�ֵ

			XMVECTOR len1 = XMVector3Length(v1);
			XMVECTOR len2 = XMVector3Length(v2);
			float fLen1 = XMVectorGetX(len1);
			float fLen2 = XMVectorGetX(len2);
			float fLenMul = fLen1 * fLen2; // ������ģ�Ļ�

			float cosV = static_cast<float>(fDot / fLenMul); // �н�cosֵ
			float angle = acosf(cosV);						 // arccos�õ���ת�Ƕȣ������ƣ����˽Ƕ���0-Pi��Χ��
			return angle;
		}
};
