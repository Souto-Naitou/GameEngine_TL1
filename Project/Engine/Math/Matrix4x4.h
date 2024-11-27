// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.

/// 前方宣言
class Vector3;

#pragma once
/// <summary>
/// 4x4行列
/// </summary>
class Matrix4x4 {
public:
    float m[4][4];

    /// ====================
    /// Minus Sign
    /// ====================
    Matrix4x4 operator-() const;

    /// =====================
    /// Make Matrix
    /// =====================

    /// <summary>
    /// 単位行列を設定
    /// </summary>
    void SetIdentity();

    /// <returns>単位行列</returns>
    static Matrix4x4 Identity();

    /// <returns>X回転行列</returns>
    static Matrix4x4 RotateXMatrix(float _radian);

    /// <returns>Y回転行列</returns>
    static Matrix4x4 RotateYMatrix(float _radian);

    /// <returns>Z回転行列</returns>
    static Matrix4x4 RotateZMatrix(float _radian);

    /// <summary>
    /// アフィン変換行列
    /// </summary>
    /// <param name="_scale">拡縮倍率</param>
    /// <param name="_rotate">角度</param>
    /// <param name="_translate">座標</param>
    /// <returns></returns>
    static Matrix4x4 AffineMatrix(
        const Vector3& _scale,
        const Vector3& _rotate,
        const Vector3& _translate
    );

    /// <summary>
    /// 透視投影行列
    /// </summary>
    /// <param name="_fovY">視野角</param>
    /// <param name="_aspectRatio">アスペクト比</param>
    /// <param name="_nearClip">近クリップ</param>
    /// <param name="_farClip">遠クリップ</param>
    /// <returns></returns>
    static Matrix4x4 PerspectiveFovMatrix(
        float _fovY,
        float _aspectRatio,
        float _nearClip,
        float _farClip
    );

    /// <summary>
    /// 正射影行列
    /// </summary>
    /// <param name="_left">左</param>
    /// <param name="_top">上</param>
    /// <param name="_right">右</param>
    /// <param name="_bottom">下</param>
    /// <param name="_nearClip">近クリップ</param>
    /// <param name="_farClip">遠クリップ</param>
    /// <returns></returns>
    static Matrix4x4 OrthographicMatrix(
        float _left,
        float _top,
        float _right,
        float _bottom,
        float _nearClip,
        float _farClip
    );

    /// <summary>
    /// ビューポート変換行列
    /// </summary>
    /// <param name="_left">左</param>
    /// <param name="_top">上</param>
    /// <param name="_width">幅</param>
    /// <param name="_height">高さ</param>
    /// <param name="_minDepth">最小深度</param>
    /// <param name="_maxDepth">最大深度</param>
    /// <returns></returns>
    static Matrix4x4 ViewportMatrix(
        float _left,
        float _top,
        float _width,
        float _height,
        float _minDepth,
        float _maxDepth
    );

    /// <summary>
    /// 拡大行列
    /// </summary>
    /// <param name="_scale">拡大率</param>
    /// <returns></returns>
    static Matrix4x4 ScaleMatrix(const Vector3& _scale);

    /// <summary>
    /// 移動行列
    /// </summary>
    /// <param name="_translate">移動量</param>
    /// <returns></returns>
    static Matrix4x4 TranslateMatrix(const Vector3& _translate);

    /// <returns>逆行列</returns>
    Matrix4x4 Inverse() const;

    /// <returns>転置行列</returns>
    Matrix4x4 Transpose() const;

    Matrix4x4& operator+=(const Matrix4x4& _rm);
    Matrix4x4& operator-=(const Matrix4x4& _rm);
    Matrix4x4& operator*=(const Matrix4x4& _rm);
    Matrix4x4 operator*(const Matrix4x4& _rm) const;
    Matrix4x4 operator+(const Matrix4x4& _rm) const;
    Matrix4x4 operator-(const Matrix4x4& _rm) const;
};