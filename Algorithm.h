
#pragma once
#include "GameStandard.h"

namespace IAlgorithm {
	static void NormalizeVelocityDirection(Velocity & v){
		// 单位化方向分量，注意开方后的数值总是正数，需要考虑原来的速度方向
		float fSquareSum = v.Direction.x*v.Direction.x + v.Direction.y*v.Direction.y + v.Direction.z*v.Direction.z;
		v.Direction.x = (v.Direction.x < 0) ? -sqrt(v.Direction.x*v.Direction.x / fSquareSum) : sqrt(v.Direction.x*v.Direction.x / fSquareSum);
		v.Direction.y = (v.Direction.y < 0) ? -sqrt(v.Direction.y*v.Direction.y / fSquareSum) : sqrt(v.Direction.y*v.Direction.y / fSquareSum); 
		v.Direction.z = (v.Direction.z < 0) ? -sqrt(v.Direction.z*v.Direction.z / fSquareSum) : sqrt(v.Direction.z*v.Direction.z / fSquareSum);
	}
	static float ComputeTwoVectorAngle(const XMVECTOR & v1, const XMVECTOR & v2){ // 计算两个向量夹角弧度值
			XMVECTOR dot = XMVector3Dot(v1, v2);
			float fDot = XMVectorGetX(dot); // 两向量点乘的值

			XMVECTOR len1 = XMVector3Length(v1);
			XMVECTOR len2 = XMVector3Length(v2);
			float fLen1 = XMVectorGetX(len1);
			float fLen2 = XMVectorGetX(len2);
			float fLenMul = fLen1 * fLen2; // 两向量模的积

			float cosV = static_cast<float>(fDot / fLenMul); // 夹角cos值
			float angle = acosf(cosV);						 // arccos得到旋转角度（弧度制），此角度在0-Pi范围内
			return angle;
		}
};
