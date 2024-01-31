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
	struct RawData {
	public: //定数
		static const char header[4];

	public: //データ
		std::array<float, sensorCount> cencor;
		std::array<bool, flagCount> flags;

	public: //関数
		static const int GetPacketSize();
	};

	struct KeepData {
	public: //定数
		static const int MAX_KEEP_COUNT = 10;

	private: //メンバ変数
		std::vector<std::array<bool, flagCount>> cencorFlags;

	public: //メンバ関数
		// コンストラクター
		KeepData() = default;
		KeepData(const KeepData&) = default;

		// 更新
		void Update(std::array<bool, flagCount > cencorFlags);

		// ゲッター
		bool GetCencorFlags(const size_t& index = 0) const;
		bool GetCencorFlagsTriger(const int count = 1, const size_t& index = 0) const;
		bool GetCencorFlagsReturn(const int count = 1, const size_t& index = 0) const;
	};

private: //メンバ変数
	Serial* serial;
	std::vector<RawData> records;

	KeepData keepData;
	Vec3 accel;
	Vec3 oldAccle;
	Vec3 accelLPF;
	Vec3 gyro;
	Vec3 oldGyro;
	Vec3 gyroLPF;
	Vec3 angle;
	Vec3 oldAngle;
	Vec3 startAngle;
	Vec3 startAngleSum;
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
	bool IsJamp() const;
	bool IsBallThrow() const;

	Vec3 GetAccelG() const { return accel; }
	Vec3 GetAccelGLPF() const { return accelLPF; }
	Vec3 GetGyroDps() const { return gyro; }
	Vec3 GetGyroDpsLPF() const { return gyroLPF; }
	bool GetFlag() const { return keepData.GetCencorFlags(); }
	bool GetFlagTriger() const { return keepData.GetCencorFlagsTriger(); }
	bool GetFlagReturn() const { return keepData.GetCencorFlagsReturn(); }
private:
	BallController(const char* serialDevice);
};
