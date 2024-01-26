#pragma once
#include <array>
#include <vector>
#include "Vec.h"
#include "../Eigen/Core"
#include "../Eigen/LU"

enum EulerOrder
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
};

class KalmanFilter
{
public: //定数
	static const int IMU_DATA = 6;
	const double DT = 0.01;
	const double GRAV_ACC = 9.80665;
	const std::vector<double> INIT_X = { 0.0, 0.0, 0.0, 1.0 };
	const std::vector<double> INIT_P = { 0.0001, 0.0001, 0.0001, 0.0001 };
	const std::vector<double> INIT_Q = { 0.01, 0.01, 0.01, 0.01 };
	const std::vector<double> INIT_R = { 1.0, 1.0, 10.0 };
	static const int SIZE_VECTOR_X = 3;
	static const int SIZE_VECTOR_Z = 2;

private: //エイリアス
	using Vector2 = Eigen::Matrix<double, SIZE_VECTOR_Z, 1>;
	using Vector3 = Eigen::Matrix<double, SIZE_VECTOR_X, 1>;
	using Matrix2x2 = Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_Z>;
	using Matrix3x3 = Eigen::Matrix<double, SIZE_VECTOR_X, SIZE_VECTOR_X>;

private: //メンバ変数
	double t; //時間
	double tPre; //前の時間
	Vector3 x; //姿勢の推定値
	Vector3 p; //ノイズの推定値

public: //メンバ関数
	KalmanFilter();
	~KalmanFilter() = default;

	// 更新
	void Update(double t, const Vec3& acc, const Vec3& gyro);
	// リセット
	void Reset();

	// 
	Vector3 GetX() { return x; }
private:
	// 角速度の推定
	Vector3 DeltaGyro(const Vec3& gyro, double dt);
	// 加速度による姿勢推定
	Vector2 GetEulerFromAccData(const Vec3& acc);

	// 拡張カルマンフィルター
	void Filter(const Vector3& u, const Vector2& z, const Matrix2x2& r, const Matrix3x3& q);
	Vector3 F(const Vector3& x, const Vector3& u);
	Vector2 H(const Vector3& x);
	Vector3 PredictX(const Vector3& x, const Vector3& u);
	Matrix3x3 PredictP(const Matrix3x3& p, const Matrix3x3& f, const Matrix3x3& q);
	Matrix3x3 CalcF(const Vector3& x, const Vector3& u);
	Eigen::Matrix<double, 2, 3> CalcH();
	Vector2 UpdateYRes(const Vector2& z, const Vector3& x);
	Matrix2x2 UpdateS(const Matrix3x3& p, const Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X>& h, const Matrix2x2& r);
	Eigen::Matrix<double, 3, 2> UpdateK(const Matrix3x3& p, const Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X>& h, const Matrix2x2& s);
	Vector3 UpdateX(const Vector3& x, const Vector2& yRes, const Eigen::Matrix<double, SIZE_VECTOR_X, SIZE_VECTOR_Z>& k);
	Matrix3x3 Update_P(const Matrix3x3& p, const Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X>& h, const Eigen::Matrix<double, SIZE_VECTOR_X, SIZE_VECTOR_Z>& k);
};
