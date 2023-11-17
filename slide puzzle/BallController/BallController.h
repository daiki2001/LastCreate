#pragma once
#include "Serial.h"
#include <array>
#include <vector>
#include "Vec.h"

#ifdef _DEBUG
#include <Sprite.h>
#endif // _DEBUG

class BallController
{
private: //定数
	static const int ACCEL_X = 0;
	static const int ACCEL_Y = 1;
	static const int ACCEL_Z = 2;
	static const int GYRO_X = 3;
	static const int GYRO_Y = 4;
	static const int GYRO_Z = 5;

	static const int accelCount = 3;
	static const int gyroCount = 3;
	static const int sensorCount = accelCount + gyroCount;
	static const int flagCount = 1;
	static const int DataCount = sensorCount + flagCount;
	static const int maxRecordCount = 100;

	static const Vec3 gravity;
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
	Vec3 oldAccel;
	Vec3 gyro;
	Vec3 oldGyro;
	bool flag;
	bool oldFlag;
	Vec3 angle;

#ifdef _DEBUG
	SpriteData debugBack;
	SpriteData pointGraph;
#endif // _DEBUG

public: //メンバ関数
	~BallController();
	static BallController* Create();
	int Update();
	void Load();
	void DrawGraph();

	void AngleUpdate();
	void AngleReset();
	bool IsBallThrow();

	Vec3 GetAccelG() const { return accel; }
	Vec3 GetOldAccelG() const { return oldAccel; }
	Vec3 GetGyroDps() const { return gyro; }
	Vec3 GetOldGyroDps() const { return oldGyro; }
	bool GetFlag() const { return flag; }
	bool GetFlagTriger() const { return flag && !oldFlag; }
	bool GetFlagReturn() const { return !flag && oldFlag; }
private:
	BallController(const char* serialDevice);
};
