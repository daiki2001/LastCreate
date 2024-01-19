#pragma once
#include <array>
#include <vector>
#include "../Eigen/Core"
#include "../Eigen/LU"

template <class T>
class KalmanFilter
{
public: //定数
	static const int IMU_DATA = 6;
	const double DT = 0.01;
	const double GRAV_ACC = 9.80665;
	const std::vector<double> INIT_P = { 0.0001, 0.0001, 0.0001, 0.0001 };
	const std::vector<double> INIT_Q = { 0.01, 0.01, 0.01, 0.01 };
	const std::vector<double> INIT_R = { 1.0, 1.0, 10.0 };
	const std::vector<double> INIT_X = { 0.0, 0.0, 0.0, 1.0 };
	static const int SIZE_VECTOR_X = 4;
	static const int SIZE_VECTOR_Y = 3;
private: //メンバ変数
	Eigen::Matrix<T, SIZE_VECTOR_X, 1> xHat;		// Estimated value of the state valiable
	Eigen::Matrix<T, SIZE_VECTOR_X, 1> xHatMinus;	// Advance estimate of the state valiable
	Eigen::Matrix<T, SIZE_VECTOR_Y, 1> y;			// Observation value
	Eigen::Matrix<T, SIZE_VECTOR_X, 1> f;			// System Matrix
	Eigen::Matrix<T, SIZE_VECTOR_Y, 1> h;			// Transformation matrix from state valiable to observation value
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_X> A;			// Linearized f
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_X> AT;			// Transposition of A
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_Y> C;			// Liniarized h
	Eigen::Matrix<T, SIZE_VECTOR_Y, SIZE_VECTOR_X> CT;			// Transposition of C
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_Y> g;			// Kalman gain
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_X> P;			// Covariance matrix
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_X> PMinus;		// Advance covariance matrix
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_X> Q;			// Variance matrix of w
	Eigen::Matrix<T, SIZE_VECTOR_Y, SIZE_VECTOR_Y> R;			// Variance matrix of v
	Eigen::Matrix<T, SIZE_VECTOR_X, SIZE_VECTOR_X> I;			// Unit Matrix

public: //メンバ関数
	KalmanFilter();
	~KalmanFilter() = default;

	void Reset();

	Eigen::Matrix<T, SIZE_VECTOR_X, 1> getXHat() const { return xHat; };

	void prediction(const std::array<T, IMU_DATA>& imu);
	void filter(const std::array<T, IMU_DATA>& imu);

	void normXHat();

	Eigen::Matrix<T, 3, 1> QuaternionToEuler(const Eigen::Matrix<T, 4, 1>& q);
};

template<class T>
inline KalmanFilter<T>::KalmanFilter()
{
	xHat << INIT_X[0],
			INIT_X[1],
			INIT_X[2],
			INIT_X[3];

	P << INIT_P[0], 0.0, 0.0, 0.0,
		 0.0, INIT_P[1], 0.0, 0.0,
		 0.0, 0.0, INIT_P[2], 0.0,
		 0.0, 0.0, 0.0, INIT_P[3];

	Q << INIT_Q[0], 0.0, 0.0, 0.0,
		 0.0, INIT_Q[1], 0.0, 0.0,
		 0.0, 0.0, INIT_Q[2], 0.0,
		 0.0, 0.0, 0.0, INIT_Q[3];

	R << INIT_R[0], 0.0, 0.0,
		 0.0, INIT_R[1], 0.0,
		 0.0, 0.0, INIT_R[2];

	I << 1.0, 0.0, 0.0, 0.0,
		 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 0.0, 1.0;

	// fill other matrix 0
	xHatMinus.setZero();
	y.setZero();
	f.setZero();
	h.setZero();
	PMinus.setZero();
	g.setZero();
	A.setZero();
	AT.setZero();
	C.setZero();
	CT.setZero();
}

template<class T>
inline void KalmanFilter<T>::Reset()
{
	xHat << INIT_X[0],
			INIT_X[1],
			INIT_X[2],
			INIT_X[3];

	P << INIT_P[0], 0.0, 0.0, 0.0,
		 0.0, INIT_P[1], 0.0, 0.0,
		 0.0, 0.0, INIT_P[2], 0.0,
		 0.0, 0.0, 0.0, INIT_P[3];

	Q << INIT_Q[0], 0.0, 0.0, 0.0,
		 0.0, INIT_Q[1], 0.0, 0.0,
		 0.0, 0.0, INIT_Q[2], 0.0,
		 0.0, 0.0, 0.0, INIT_Q[3];

	R << INIT_R[0], 0.0, 0.0,
		 0.0, INIT_R[1], 0.0,
		 0.0, 0.0, INIT_R[2];

	I << 1.0, 0.0, 0.0, 0.0,
		 0.0, 1.0, 0.0, 0.0,
		 0.0, 0.0, 1.0, 0.0,
		 0.0, 0.0, 0.0, 1.0;

	// fill other matrix 0
	xHatMinus.setZero();
	y.setZero();
	f.setZero();
	h.setZero();
	PMinus.setZero();
	g.setZero();
	A.setZero();
	AT.setZero();
	C.setZero();
	CT.setZero();
}

template<class T>
inline void KalmanFilter<T>::prediction(const std::array<T, IMU_DATA>& imu)
{
	std::vector<T> w, q;
	w.push_back(static_cast<T>(0));
	for (int i = 3; i < 6; ++i)
		w.push_back(imu[i]); // w[1]~w[3]

	A << 1.0, 0.5 * DT * w[3], -0.5 * DT * w[2], 0.5 * DT * w[1],
		-0.5 * DT * w[3], 1.0, 0.5 * DT * w[1], 0.5 * DT * w[2],
		0.5 * DT * w[2], -0.5 * DT * w[1], 1.0, 0.5 * DT * w[3],
		-0.5 * DT * w[1], -0.5 * DT * w[2], -0.5 * DT * w[3], 1.0;

	f = A * xHat;

	xHatMinus = f;

	q.push_back(static_cast<T>(0));
	for (int i = 0; i < 4; ++i)
		q.push_back(xHatMinus[i]); // q[1]~q[4]

	CT << 2 * q[3] * GRAV_ACC, -2 * q[4] * GRAV_ACC, 2 * q[1] * GRAV_ACC, -2 * q[2] * GRAV_ACC,
		2 * q[4] * GRAV_ACC, 2 * q[3] * GRAV_ACC, 2 * q[2] * GRAV_ACC, 2 * q[1] * GRAV_ACC,
		-2 * q[1] * GRAV_ACC, -2 * q[2] * GRAV_ACC, 2 * q[3] * GRAV_ACC, 2 * q[4] * GRAV_ACC;

	PMinus = A * P * A.transpose() + Q;
}

template<class T>
void KalmanFilter<T>::filter(const std::array<T, IMU_DATA>&imu)
{
	std::vector<T> a, q;
	a.push_back(static_cast<T>(0));
	for (int i = 0; i < 3; ++i)
		a.push_back(imu[i]); // a[1]~a[3]
	q.push_back(static_cast<T>(0));
	for (int i = 0; i < 4; ++i)
		q.push_back(xHatMinus[i]); // q[1]~q[4]

	C = CT.transpose();

	g = PMinus * C * (CT * PMinus * C + R).inverse();

	y << a[1],
		 a[2],
		 a[3];

	h << 2 * (q[3] * q[1] - q[2] * q[4]) * GRAV_ACC,
		2 * (q[2] * q[3] - q[1] * q[4]) * GRAV_ACC,
		(q[3] * q[3] - q[1] * q[1] - q[2] * q[2] + q[4] * q[4])* GRAV_ACC;

	xHat = xHatMinus + g * (y - h);

	P = (I - g * CT) * PMinus;

	//std::cout << "C" << std::endl << C << std::endl << std::endl;
	//std::cout << "xHatMinus" << std::endl << xHatMinus << std::endl << std::endl;
	//std::cout << "PMinus" << std::endl << PMinus << std::endl << std::endl;
	//std::cout << "g" << std::endl << g << std::endl << std::endl;
	//std::cout << "CT" << std::endl << CT << std::endl << std::endl;
	//std::cout << "I - g*CT" << std::endl << I - g * CT << std::endl << std::endl;
	//std::cout << "y" << std::endl << y << std::endl << std::endl;
	//std::cout << "h" << std::endl << h << std::endl << std::endl;
	//std::cout << "y-h" << std::endl << y - h << std::endl << std::endl;
	//std::cout << "xHat" << std::endl << xHat << std::endl << std::endl;
	//std::cout << "P" << std::endl << P << std::endl << std::endl;
}

template<class T>
void KalmanFilter<T>::normXHat()
{
	T norm;
	norm = sqrt(xHat(0, 0) * xHat(0, 0) + xHat(1, 0) * xHat(1, 0) + xHat(2, 0) * xHat(2, 0) + xHat(3, 0) * xHat(3, 0));
	xHat /= norm;
}

template<class T>
inline Eigen::Matrix<T, 3, 1> KalmanFilter<T>::QuaternionToEuler(const Eigen::Matrix<T, 4, 1>& q)
{
	Eigen::Matrix<T, 3, 1> result;

	T q0q0 = q[0] * q[0];
	T q0q1 = q[0] * q[1];
	T q0q2 = q[0] * q[2];
	T q0q3 = q[0] * q[3];
	T q1q1 = q[1] * q[1];
	T q1q2 = q[1] * q[2];
	T q1q3 = q[1] * q[3];
	T q2q2 = q[2] * q[2];
	T q2q3 = q[2] * q[3];
	T q3q3 = q[3] * q[3];

	T roll = atan2(2.0 * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3);
	T pitch = asin(2.0 * (q0q2 - q1q3));
	T yaw = atan2(2.0 * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);

	result << roll, pitch, yaw;
	return result;
}
