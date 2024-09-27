#include "matrix4calc.h"
#include <cmath>
#include "../../MathExtension/mathExtension.h"
#include <cassert>

Matrix4x4 Add(const Matrix4x4& _m1, const Matrix4x4& _m2)
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.m[i][j] = _m1.m[i][j] + _m2.m[i][j];

	return result;
}

Matrix4x4 Subtract(const Matrix4x4& _m1, const Matrix4x4& _m2)
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.m[i][j] = _m1.m[i][j] - _m2.m[i][j];

	return result;
}

Matrix4x4 Multiply(const Matrix4x4& _m1, const Matrix4x4& _m2)
{
	Matrix4x4 result{};

	for (int off = 0; off < 4; off++)
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				result.m[off][i] += _m1.m[off][j] * _m2.m[j][i];

	return result;
}

Matrix4x4 Inverse(const Matrix4x4& _m)
{
	Matrix4x4 result{};

#pragma region 行列式の計算
	float _a_ =
		_m.m[0][0] * _m.m[1][1] * _m.m[2][2] * _m.m[3][3] +
		_m.m[0][0] * _m.m[1][2] * _m.m[2][3] * _m.m[3][1] +
		_m.m[0][0] * _m.m[1][3] * _m.m[2][1] * _m.m[3][2] -
		_m.m[0][0] * _m.m[1][3] * _m.m[2][2] * _m.m[3][1] -
		_m.m[0][0] * _m.m[1][2] * _m.m[2][1] * _m.m[3][3] -
		_m.m[0][0] * _m.m[1][1] * _m.m[2][3] * _m.m[3][2] -
		_m.m[0][1] * _m.m[1][0] * _m.m[2][2] * _m.m[3][3] -
		_m.m[0][2] * _m.m[1][0] * _m.m[2][3] * _m.m[3][1] -
		_m.m[0][3] * _m.m[1][0] * _m.m[2][1] * _m.m[3][2] +
		_m.m[0][3] * _m.m[1][0] * _m.m[2][2] * _m.m[3][1] +
		_m.m[0][2] * _m.m[1][0] * _m.m[2][1] * _m.m[3][3] +
		_m.m[0][1] * _m.m[1][0] * _m.m[2][3] * _m.m[3][2] +
		_m.m[0][1] * _m.m[1][2] * _m.m[2][0] * _m.m[3][3] +
		_m.m[0][2] * _m.m[1][3] * _m.m[2][0] * _m.m[3][1] +
		_m.m[0][3] * _m.m[1][1] * _m.m[2][0] * _m.m[3][2] -
		_m.m[0][3] * _m.m[1][2] * _m.m[2][0] * _m.m[3][1] -
		_m.m[0][2] * _m.m[1][1] * _m.m[2][0] * _m.m[3][3] -
		_m.m[0][1] * _m.m[1][3] * _m.m[2][0] * _m.m[3][2] -
		_m.m[0][1] * _m.m[1][2] * _m.m[2][3] * _m.m[3][0] -
		_m.m[0][2] * _m.m[1][3] * _m.m[2][1] * _m.m[3][0] -
		_m.m[0][3] * _m.m[1][1] * _m.m[2][2] * _m.m[3][0] +
		_m.m[0][3] * _m.m[1][2] * _m.m[2][1] * _m.m[3][0] +
		_m.m[0][2] * _m.m[1][1] * _m.m[2][3] * _m.m[3][0] +
		_m.m[0][1] * _m.m[1][3] * _m.m[2][2] * _m.m[3][0];
#pragma endregion

#pragma region 各成分の計算
	// 一行目
	result.m[0][0] =
		_m.m[1][1] * _m.m[2][2] * _m.m[3][3] +
		_m.m[1][2] * _m.m[2][3] * _m.m[3][1] +
		_m.m[1][3] * _m.m[2][1] * _m.m[3][2] -
		_m.m[1][3] * _m.m[2][2] * _m.m[3][1] -
		_m.m[1][2] * _m.m[2][1] * _m.m[3][3] -
		_m.m[1][1] * _m.m[2][3] * _m.m[3][2];
	result.m[0][1] =
		-_m.m[0][1] * _m.m[2][2] * _m.m[3][3] -
		_m.m[0][2] * _m.m[2][3] * _m.m[3][1] -
		_m.m[0][3] * _m.m[2][1] * _m.m[3][2] +
		_m.m[0][3] * _m.m[2][2] * _m.m[3][1] +
		_m.m[0][2] * _m.m[2][1] * _m.m[3][3] +
		_m.m[0][1] * _m.m[2][3] * _m.m[3][2];
	result.m[0][2] =
		_m.m[0][1] * _m.m[1][2] * _m.m[3][3] +
		_m.m[0][2] * _m.m[1][3] * _m.m[3][1] +
		_m.m[0][3] * _m.m[1][1] * _m.m[3][2] -
		_m.m[0][3] * _m.m[1][2] * _m.m[3][1] -
		_m.m[0][2] * _m.m[1][1] * _m.m[3][3] -
		_m.m[0][1] * _m.m[1][3] * _m.m[3][2];
	result.m[0][3] =
		-_m.m[0][1] * _m.m[1][2] * _m.m[2][3] -
		_m.m[0][2] * _m.m[1][3] * _m.m[2][1] -
		_m.m[0][3] * _m.m[1][1] * _m.m[2][2] +
		_m.m[0][3] * _m.m[1][2] * _m.m[2][1] +
		_m.m[0][2] * _m.m[1][1] * _m.m[2][3] +
		_m.m[0][1] * _m.m[1][3] * _m.m[2][2];
	// 2行目
	result.m[1][0] =
		-_m.m[1][0] * _m.m[2][2] * _m.m[3][3] -
		_m.m[1][2] * _m.m[2][3] * _m.m[3][0] -
		_m.m[1][3] * _m.m[2][0] * _m.m[3][2] +
		_m.m[1][3] * _m.m[2][2] * _m.m[3][0] +
		_m.m[1][2] * _m.m[2][0] * _m.m[3][3] +
		_m.m[1][0] * _m.m[2][3] * _m.m[3][2];
	result.m[1][1] =
		_m.m[0][0] * _m.m[2][2] * _m.m[3][3] +
		_m.m[0][2] * _m.m[2][3] * _m.m[3][0] +
		_m.m[0][3] * _m.m[2][0] * _m.m[3][2] -
		_m.m[0][3] * _m.m[2][2] * _m.m[3][0] -
		_m.m[0][2] * _m.m[2][0] * _m.m[3][3] -
		_m.m[0][0] * _m.m[2][3] * _m.m[3][2];
	result.m[1][2] =
		-_m.m[0][0] * _m.m[1][2] * _m.m[3][3] -
		_m.m[0][2] * _m.m[1][3] * _m.m[3][0] -
		_m.m[0][3] * _m.m[1][0] * _m.m[3][2] +
		_m.m[0][3] * _m.m[1][2] * _m.m[3][0] +
		_m.m[0][2] * _m.m[1][0] * _m.m[3][3] +
		_m.m[0][0] * _m.m[1][3] * _m.m[3][2];
	result.m[1][3] =
		_m.m[0][0] * _m.m[1][2] * _m.m[2][3] +
		_m.m[0][2] * _m.m[1][3] * _m.m[2][0] +
		_m.m[0][3] * _m.m[1][0] * _m.m[2][2] -
		_m.m[0][3] * _m.m[1][2] * _m.m[2][0] -
		_m.m[0][2] * _m.m[1][0] * _m.m[2][3] -
		_m.m[0][0] * _m.m[1][3] * _m.m[2][2];
	// 3行目
	result.m[2][0] =
		_m.m[1][0] * _m.m[2][1] * _m.m[3][3] +
		_m.m[1][1] * _m.m[2][3] * _m.m[3][0] +
		_m.m[1][3] * _m.m[2][0] * _m.m[3][1] -
		_m.m[1][3] * _m.m[2][1] * _m.m[3][0] -
		_m.m[1][1] * _m.m[2][0] * _m.m[3][3] -
		_m.m[1][0] * _m.m[2][3] * _m.m[3][1];
	result.m[2][1] = 
		-_m.m[0][0] * _m.m[2][1] * _m.m[3][3] -
		_m.m[0][1] * _m.m[2][3] * _m.m[3][0] -
		_m.m[0][3] * _m.m[2][0] * _m.m[3][1] +
		_m.m[0][3] * _m.m[2][1] * _m.m[3][0] +
		_m.m[0][1] * _m.m[2][0] * _m.m[3][3] +
		_m.m[0][0] * _m.m[2][3] * _m.m[3][1];
	result.m[2][2] =
		_m.m[0][0] * _m.m[1][1] * _m.m[3][3] +
		_m.m[0][1] * _m.m[1][3] * _m.m[3][0] +
		_m.m[0][3] * _m.m[1][0] * _m.m[3][1] -
		_m.m[0][3] * _m.m[1][1] * _m.m[3][0] -
		_m.m[0][1] * _m.m[1][0] * _m.m[3][3] -
		_m.m[0][0] * _m.m[1][3] * _m.m[3][1];
	result.m[2][3] =
		-_m.m[0][0] * _m.m[1][1] * _m.m[2][3] -
		_m.m[0][1] * _m.m[1][3] * _m.m[2][0] -
		_m.m[0][3] * _m.m[1][0] * _m.m[2][1] +
		_m.m[0][3] * _m.m[1][1] * _m.m[2][0] +
		_m.m[0][1] * _m.m[1][0] * _m.m[2][3] +
		_m.m[0][0] * _m.m[1][3] * _m.m[2][1];
	// 4行目
	result.m[3][0] =
		-_m.m[1][0] * _m.m[2][1] * _m.m[3][2] -
		_m.m[1][1] * _m.m[2][2] * _m.m[3][0] -
		_m.m[1][2] * _m.m[2][0] * _m.m[3][1] +
		_m.m[1][2] * _m.m[2][1] * _m.m[3][0] +
		_m.m[1][1] * _m.m[2][0] * _m.m[3][2] +
		_m.m[1][0] * _m.m[2][2] * _m.m[3][1];
	result.m[3][1] =
		_m.m[0][0] * _m.m[2][1] * _m.m[3][2] +
		_m.m[0][1] * _m.m[2][2] * _m.m[3][0] +
		_m.m[0][2] * _m.m[2][0] * _m.m[3][1] -
		_m.m[0][2] * _m.m[2][1] * _m.m[3][0] -
		_m.m[0][1] * _m.m[2][0] * _m.m[3][2] -
		_m.m[0][0] * _m.m[2][2] * _m.m[3][1];
	result.m[3][2] =
		-_m.m[0][0] * _m.m[1][1] * _m.m[3][2] -
		_m.m[0][1] * _m.m[1][2] * _m.m[3][0] -
		_m.m[0][2] * _m.m[1][0] * _m.m[3][1] +
		_m.m[0][2] * _m.m[1][1] * _m.m[3][0] +
		_m.m[0][1] * _m.m[1][0] * _m.m[3][2] +
		_m.m[0][0] * _m.m[1][2] * _m.m[3][1];
	result.m[3][3] =
		_m.m[0][0] * _m.m[1][1] * _m.m[2][2] +
		_m.m[0][1] * _m.m[1][2] * _m.m[2][0] +
		_m.m[0][2] * _m.m[1][0] * _m.m[2][1] -
		_m.m[0][2] * _m.m[1][1] * _m.m[2][0] -
		_m.m[0][1] * _m.m[1][0] * _m.m[2][2] -
		_m.m[0][0] * _m.m[1][2] * _m.m[2][1];

#pragma endregion

#pragma region 各成分 / 行列式
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] /= _a_;
		}
#pragma endregion

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& _m)
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			result.m[i][j] = _m.m[j][i];

	return result;
}

Matrix4x4 MakeIdentity4x4()
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
		result.m[i][i] = 1;

	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& _translate)
{
	//
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++) result.m[i][i] = 1.0f;

	result.m[3][0] += _translate.x;
	result.m[3][1] += _translate.y;
	result.m[3][2] += _translate.z;

	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& _scale)
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
	{
		if (i == 3)
		{
			result.m[i][i] = 1.0f;
			break;
		}
		result.m[i][i] = *(&_scale.x + i);
	}

	return result;
}

Matrix4x4 MakeRotateXMatrix(float _radian)
{
	Matrix4x4 result{};

	result.m[0][0] = 1.0f;
	result.m[1][1] = std::cosf(_radian);
	result.m[1][2] = std::sinf(_radian); 
	result.m[2][1] = -std::sinf(_radian);
	result.m[2][2] = std::cosf(_radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateYMatrix(float _radian)
{
	Matrix4x4 result{};
	
	result.m[0][0] = std::cosf(_radian);
	result.m[0][2] = -std::sinf(_radian);
	result.m[1][1] = 1.0f;
	result.m[2][0] = std::sinf(_radian);
	result.m[2][2] = std::cosf(_radian);
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateZMatrix(float _radian)
{
	Matrix4x4 result{};

	for (int i = 3; i > 1; --i)
	{
		result.m[i][i] = 1.0f;
	}
	result.m[0][0] = std::cos(_radian);
	result.m[1][0] = -std::sin(_radian);
	result.m[0][1] = std::sin(_radian);
	result.m[1][1] = std::cos(_radian);

	return result;
}

Matrix4x4 MakeAffineMatrix(
	const Vector3& _scale, 
	const Vector3& _rotate, 
	const Vector3& _translate
)
{
	Matrix4x4 result{};
	Matrix4x4 rotateMatrix = Multiply(
		MakeRotateXMatrix(_rotate.x),
		Multiply(
			MakeRotateYMatrix(_rotate.y),
			MakeRotateZMatrix(_rotate.z)
		)
	);

	Matrix4x4 scaleMatrix{};
	scaleMatrix.m[0][0] = _scale.x;
	scaleMatrix.m[1][1] = _scale.y;
	scaleMatrix.m[2][2] = _scale.z;
	scaleMatrix.m[3][3] = 1.0f;
	
	//for (int i = 0; i < 3; i++)
	//{
	//	result.m[0][i] = _scale.x * rotateMatrix.m[0][i];
	//	result.m[1][i] = _scale.y * rotateMatrix.m[1][i];
	//	result.m[2][i] = _scale.z * rotateMatrix.m[2][i];
	//}

	result = Multiply(scaleMatrix, rotateMatrix);

	result.m[3][0] = _translate.x;
	result.m[3][1] = _translate.y;
	result.m[3][2] = _translate.z;

	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float _fovY, float _aspectRatio, float _nearClip, float _farClip)
{
	Matrix4x4 result{};
	result.m[0][0] = 1.0f / _aspectRatio * cotanf(_fovY / 2.0f);
	result.m[1][1] = cotanf(_fovY / 2.0f);
	result.m[2][2] = _farClip / (_farClip - _nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -_nearClip * _farClip / (_farClip - _nearClip);

	return result;
}

Matrix4x4 MakeOrthographicMatrix(float _left, float _top, float _right, float _bottom, float _nearClip, float _farClip)
{
	Matrix4x4 result{};
	result.m[0][0] = 2.0f / (_right - _left);
	result.m[1][1] = 2.0f / (_top - _bottom);
	result.m[2][2] = 1.0f / (_farClip - _nearClip);

	result.m[3][0] = (_left + _right) / (_left - _right);
	result.m[3][1] = (_top + _bottom) / (_bottom - _top);
	result.m[3][2] = _nearClip / (_nearClip - _farClip);

	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeViewportMatrix(float _left, float _top, float _width, float _height, float _minDepth, float _maxDepth)
{
	assert(_minDepth <= _maxDepth);
	Matrix4x4 result{};
	result.m[0][0] = _width / 2.0f;
	result.m[1][1] = -_height / 2.0f;
	result.m[2][2] = _maxDepth - _minDepth;

	result.m[3][0] = _left + _width / 2.0f;
	result.m[3][1] = _top + _height / 2.0f;
	result.m[3][2] = _minDepth;
	
	result.m[3][3] = 1.0f;

	return result;
}
