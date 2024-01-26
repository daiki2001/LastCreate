#include "KalmanFilter.h"

KalmanFilter::KalmanFilter()
{
	Reset();
}

void KalmanFilter::Update(double t, const Vec3& acc, const Vec3& gyro)
{
	using namespace Eigen;

	this->tPre = this->t;
	this->t = t;

	double dt = this->t - this->tPre;
	auto u = DeltaGyro(gyro, dt);
	auto z = GetEulerFromAccData(acc);
	DiagonalMatrix<double, SIZE_VECTOR_Z> r(1.0 * (dt * dt), 1.0 * (dt * dt));
	DiagonalMatrix<double, SIZE_VECTOR_X> q(0.0174 * (dt * dt), 0.0174 * (dt * dt), 0.0174 * (dt * dt));
	Filter(u, z, r, q);
}

void KalmanFilter::Reset()
{
	t = 0;
	tPre = t;
}

KalmanFilter::Vector3 KalmanFilter::DeltaGyro(const Vec3& gyro, double dt)
{
	Vector3 result;
	result << gyro.x,
		gyro.y,
		gyro.z;

	return result * dt;
}

KalmanFilter::Vector2 KalmanFilter::GetEulerFromAccData(const Vec3& acc)
{
	Vector2 result;

	result(0, 0) = atan(acc.y / acc.z);
	result(1, 0) = -atan(acc.x / sqrt(acc.y * acc.y + acc.z * acc.z));

	return result;
}

void KalmanFilter::Filter(const Vector3& u, const Vector2& z, const Matrix2x2& r, const Matrix3x3& q)
{
	// Predict
	auto f = CalcF(x, u);
	x = PredictX(x, u);
	auto h = CalcH();

	// Update
}

KalmanFilter::Vector3 KalmanFilter::F(const Vector3& x, const Vector3& u)
{
	Vector3 result;

	auto uX = u(0, 0);
	auto uY = u(1, 0);
	auto uZ = u(2, 0);
	auto c1 = cos(x(0, 0));
	auto s1 = sin(x(0, 0));
	auto c2 = cos(x(1, 0));
	auto s2 = sin(x(1, 0));
	auto c3 = cos(x(2, 0));
	auto s3 = sin(x(2, 0));

	result << x(0, 0) + uX + uY * s1 * s2 / c2 + uZ * c1 * s2 / c2,
		x(1, 0) + uY * c1 - uZ * s1,
		x(2, 0) + uY * s1 / c2 + uZ * c1 / c2;

	return result;
}

KalmanFilter::Vector2 KalmanFilter::H(const Vector3& x)
{
	Vector2 result;

	Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X> h;
	h << 1, 0, 0,
		0, 1, 0;
	result = h * x;

	return result;
}

KalmanFilter::Vector3 KalmanFilter::PredictX(const Vector3& x, const Vector3& u)
{
	Vector3 result = F(x, u);
	return result;
}

KalmanFilter::Matrix3x3 KalmanFilter::PredictP(const Matrix3x3& p, const Matrix3x3& f, const Matrix3x3& q)
{
	Matrix3x3 result;
	auto fT = f.transpose();
	result = f * p * fT + q;
	return result;
}

KalmanFilter::Matrix3x3 KalmanFilter::CalcF(const Vector3& x, const Vector3& u)
{
	Matrix3x3 result;

	auto uX = u(0, 0);
	auto uY = u(1, 0);
	auto uZ = u(2, 0);
	auto c1 = cos(x(0, 0));
	auto s1 = sin(x(0, 0));
	auto c2 = cos(x(1, 0));
	auto s2 = sin(x(1, 0));
	auto c3 = cos(x(2, 0));
	auto s3 = sin(x(2, 0));

	result << 1 + uY * c1 * s2 / c2 - uZ * s1 * s2 / c2, uY* s1 / (c2 * c2) + uZ * c1 / (c2 * c2), 0,
		-uY * s1 - uZ * c1, 1, 0,
		uY* c1 / c2 - uZ * s1 / c2, uY* s1* s2 / (c2 * c2) + uZ * c1 * s2 / (c2 * c2), 1;
	return result;
}

Eigen::Matrix<double, 2, 3> KalmanFilter::CalcH()
{
	Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X> result;
	result << 1, 0, 0,
		0, 1, 0;
	return result;
}

KalmanFilter::Vector2 KalmanFilter::UpdateYRes(const Vector2& z, const Vector3& x)
{
	Vector2 result = z - H(x);
	return result;
}

KalmanFilter::Matrix2x2 KalmanFilter::UpdateS(const Matrix3x3& p, const Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X>& h, const Matrix2x2& r)
{
	Matrix2x2 result;
	auto hT = h.transpose();
	result = h * p * hT + r;
	return result;
}

Eigen::Matrix<double, 3, 2> KalmanFilter::UpdateK(const Matrix3x3& p,const Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X>& h, const Matrix2x2& s)
{
	Eigen::Matrix<double, SIZE_VECTOR_X, SIZE_VECTOR_Z> result;
	auto hT = h.transpose();
	auto sInv = s.inverse();
	result = p * hT * sInv;
	return result;
}

KalmanFilter::Vector3 KalmanFilter::UpdateX(const Vector3& x, const Vector2& yRes,const Eigen::Matrix<double, SIZE_VECTOR_X, SIZE_VECTOR_Z>& k)
{
	Vector3 result;
	result = x + k * yRes;
	return result;
}

KalmanFilter::Matrix3x3 KalmanFilter::Update_P(const Matrix3x3& p, const Eigen::Matrix<double, SIZE_VECTOR_Z, SIZE_VECTOR_X>& h, const Eigen::Matrix<double, SIZE_VECTOR_X, SIZE_VECTOR_Z>& k)
{
	Matrix3x3 result;
	auto i = KalmanFilter::Matrix3x3::Identity();
	result = (i - k * h) * p;
	return result;
}
