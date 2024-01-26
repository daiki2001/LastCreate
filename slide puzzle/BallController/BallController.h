#pragma once
#include "Serial.h"
#include <array>
#include <vector>
#include "Vec.h"
#include <Sprite.h>

// カルマンフィルタ
#include <chrono>
#include "KalmanFilter.h"

class BallController
{
public: //定数
	static const int accelCount = 3;
	static const int gyroCount = 3;
	static const int sensorCount = accelCount + gyroCount;
	static const int flagCount = 1;
	static const int DataCount = sensorCount + flagCount;
	static const int maxRecordCount = 100;
private:
	static const int ACCEL_X = 0;
	static const int ACCEL_Y = 1;
	static const int ACCEL_Z = 2;
	static const int GYRO_X = 3;
	static const int GYRO_Y = 4;
	static const int GYRO_Z = 5;

	static const float deadzone;

private: //サブクラス
	struct Data {
	public: //定数
		static const char header[4];

	public: //データ
		std::array<float, sensorCount> cencor;
		std::array<bool, flagCount> flags;

	public: //関数
		static const int GetPacketSize();
	};

private: //メンバ変数
	Serial* serial;
	std::vector<Data> records;

	Vec3 accel;
	Vec3 accelLPF;
	Vec3 gyro;
	Vec3 gyroLPF;
	Vec3 angle;
	Vec3 oldAngle;
	Vec3 startAngle;
	Vec3 startAngleSum;
	bool flag;
	bool oldFlag;
	std::chrono::system_clock::time_point startTime;
	std::chrono::duration<double> count;
	std::chrono::duration<double> oldCount;
	KalmanFilter kalman;
	unsigned int dataCount;

	SpriteData debugBack;
	SpriteData pointGraph;

public: //メンバ関数
	~BallController();
	static BallController* Create();
	int Update();
	void Load();
	void DrawGraph();

	void AngleUpdate();
	void AngleReset();

	bool IsForward() const;
	bool IsBack() const;
	bool IsLeft() const;
	bool IsRight() const;
	bool IsJump() const;
	bool IsBallThrow() const;

	Vec3 GetAccelG() const { return accel; }
	Vec3 GetAccelGLPF() const { return accelLPF; }
	Vec3 GetGyroDps() const { return gyro; }
	Vec3 GetGyroDpsLPF() const { return gyroLPF; }
	bool GetFlag() const { return flag; }
	bool GetFlagTriger() const { return flag && !oldFlag; }
	bool GetFlagReturn() const { return !flag && oldFlag; }
private:
	BallController(const char* serialDevice);
};
