#include "BallController.h"
#include "Ini.h"
#include <string>

#ifdef _DEBUG
#include <fstream>
#endif // _DEBUG

namespace
{
const double PI = 3.14159265358979323846;
const double DEGREE = 180.0 / PI;

#ifdef _DEBUG
std::ofstream outlog;
#endif // _DEBUG

std::array<double, BallController::sensorCount> ToImuData(const Vec3& a, const Vec3& g)
{
	std::array<double, BallController::sensorCount> result = {};

	result[0] = static_cast<double>(a.x);
	result[1] = static_cast<double>(a.y);
	result[2] = static_cast<double>(a.z);
	result[3] = static_cast<double>(g.x);
	result[4] = static_cast<double>(g.y);
	result[5] = static_cast<double>(g.z);

	return result;
}
}

const char BallController::Data::header[4] = { 'D', 'A', 'T', 'F' };

const int BallController::Data::GetPacketSize()
{
	int result = 0;
	result += sizeof(header);
	result += sizeof(int16_t) * sensorCount;
	result += sizeof(uint8_t);
	return result;
}

const float BallController::deadzone = 0.25f;

BallController::BallController(const char* serialDevice) :
	accel{},
	oldAccle(accel),
	accelLPF(accel),
	gyro{},
	oldGyro(gyro),
	gyroLPF(gyro),
	angle{},
	oldAngle{},
	startAngle{},
	startAngleSum{},
	flag(false),
	oldFlag(false),
	startTime(std::chrono::system_clock::now()),
	count{},
	oldCount(count),
	kalman{},
	dataCount(0)
{
	this->serial = Serial::create(serialDevice);

#ifdef _DEBUG
	outlog.open("./data/angle_output.txt");
	outlog << std::fixed << std::setprecision(6);
#endif // _DEBUG
}

BallController::~BallController()
{
	if (this->serial) delete this->serial;
}

BallController* BallController::Create()
{
#if defined(_WIN32) || defined(_WIN64)
	const char platform[] = "Windows";
#else
	const char platform[] = "macOS";
#endif

	Ini ini = Ini("viewer.ini");
	std::string serialDevice = ini.getValue(platform, "serialDevice");
	return new BallController(serialDevice.c_str());
}

int BallController::Update() {
	int dataCount = 0;
	if (!this->serial) return dataCount;

	// データの読み込み
	static char buf[1024];
	static int contentSize = 0;
	const int receivedSize = this->serial->read(buf + contentSize, sizeof(buf) - contentSize);
	if (receivedSize == -1) return dataCount;
	contentSize += receivedSize;

	// データの記録
	oldAccle = accel;
	oldGyro = gyro;
	oldAngle = angle;
	oldFlag = flag;
	oldCount = count;
	char* p = buf;
	const int packetSize = Data::GetPacketSize();
	std::array<float, sensorCount> rawData = {};
	for (; p < buf + contentSize - packetSize; ) {
		if (memcmp(p, Data::header, sizeof(Data::header)) != 0) {
			p++;
			continue;
		}
		dataCount++;
		Data record = {};
		p += sizeof(Data::header);
		/* センサーの情報 */
		for (int i = 0; i < sensorCount; i++, p += sizeof(int16_t)) {
			const float accelSensitivity_16g = 2048;
			const float gyroSensitivity_2kdps = 16.4f;
			record.cencor[i] = *((const int16_t*)p) / (i < accelCount ? accelSensitivity_16g : gyroSensitivity_2kdps);
			rawData[i] += record.cencor[i];
		}
		/* フラグの情報 */
		for (int i = 0; i < (flagCount / 8.0f); i++, p += sizeof(uint8_t))
		{
			unsigned char flags = *p;
			int bitCount = ((flagCount / 8.0f) < 1) ? flagCount % 8 : 8;
			for (int j = 0; j < bitCount; j++)
			{
				int index = i * 8 + j;
				record.flags[index] = flags & (0x01 << j);
			}
		}
		this->records.push_back(record);
		if (this->records.size() > this->maxRecordCount) {
			this->records.erase(this->records.begin());
		}
	}

	const int tailSize = (int)(buf + contentSize - p);
	memmove(buf, p, tailSize);
	contentSize = tailSize;

	for (int i = 0; i < sensorCount; i++)
	{
		records.back().cencor[i] = rawData[i] / dataCount;
	}

	accel = Vec3(records.back().cencor[ACCEL_X],
				 records.back().cencor[ACCEL_Y],
				 records.back().cencor[ACCEL_Z]);
	gyro = Vec3(records.back().cencor[GYRO_X],
				records.back().cencor[GYRO_Y],
				records.back().cencor[GYRO_Z]);
	gyro /= static_cast<float>(DEGREE);
	flag = records.back().flags[0];

#ifdef _DEBUG
	//outlog << count.count() << ", ";
	//for (size_t i = 0; i < records.back().cencor.size(); i++)
	//{
	//	double x = (i < accelCount) ? 1.0 : DEGREE;
	//	outlog << records.back().cencor[i] / static_cast<float>(x) << ", ";
	//}
	//outlog << std::endl;
#endif // _DEBUG
	AngleUpdate();
	count = std::chrono::system_clock::now() - startTime;
#ifdef _DEBUG
	outlog << count.count() << ", " << kalman.GetX()[0] << "," << kalman.GetX()[1] << "," << kalman.GetX()[2] << std::endl;
#endif // _DEBUG

	return dataCount;
}

void BallController::Load()
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG
	static bool isLoad = false;
	if (isLoad) return;

	//スプライト画像読み込み
	debugBack = Sprite::Get()->SpriteCreate(L"Resources/black.png");
	pointGraph = Sprite::Get()->SpriteCreate(L"Resources/white1x1.png");
	isLoad = true;
}

void BallController::DrawGraph()
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	using namespace DirectX;
	int result = 0;
	int err = 0;

	const int sizeScale = 4;
	const XMFLOAT2 size = { 1280 / sizeScale, 720 / sizeScale };
	const float tickX = (float)size.x / this->maxRecordCount;
	const int tickY = 30 / sizeScale;
	const Vec4 colors[] = { Vec4(1, 0, 0, 1), Vec4(0, 1, 0, 1), Vec4(0, 0, 1, 1) };

	Sprite::Get()->Draw(debugBack, Vec2(), size.x, size.y);
	
	for (int i = 0; i < DataCount; i++)
	{
		if (i < sensorCount)
		{
			const float baseY = size.y * (i * 2 + 1) / 2 / DataCount;
			const uint16_t scale = i < this->accelCount ? 1 : 180;

			int x = 0;
			for (const auto& record : this->records) {
				const Vec2 point = { (float)((x + 1) * tickX), baseY - (float)(record.cencor[i] / scale * tickY) };
				Sprite::Get()->Draw(pointGraph, point, 10.0f / sizeScale, 10.0f / sizeScale,
									Vec2(0.5, 0.5), colors[i % 3]);
				x++;
			}
		}
		else
		{
			const float baseY = size.y * (i * 2 + 1) / 2 / DataCount;
			int flagIndex = i - sensorCount;

			int x = 0;
			for (const auto& record : this->records) {
				const Vec2 point = { (float)((x + 1) * tickX), baseY - (float)(record.flags[flagIndex] * tickY) };
				Sprite::Get()->Draw(pointGraph, point, 10.0f / sizeScale, 10.0f / sizeScale);
				x++;
			}
		}
	}

	return;
}

void BallController::AngleUpdate()
{
	if (GetFlagTriger() || abs((count - oldCount).count()) > 1.35)
	{
		AngleReset();
	}

	const Vec3 baisA = { -0.5849296877f, 0.072347369f, -0.01939517739f };
	const Vec3 baisG = { -0.8886206362f, 0.03656522179f, 0.6347734145f };
	accel -= baisA;
	gyro -= baisG;

	if (count.count() < 0.0005)
	{
		accelLPF = accel;
		gyroLPF = gyro;
	}
	else
	{
		float lpf = 0.07f;
		accelLPF = lpf * accel + (1.0f - lpf) * accelLPF;
		gyroLPF = lpf * gyro + (1.0f - lpf) * gyroLPF;
	}

	// kalman filter and normalize quaternion
	kalman.Update(count.count(), accelLPF, gyroLPF);

	if (count.count() < 1.35)
	{
		dataCount++;
		startAngleSum += angle;
		startAngle = (dataCount == 0) ? Vec3() : (startAngleSum / static_cast<float>(dataCount));
	}
}

void BallController::AngleReset()
{
#ifdef _DEBUG
	outlog << std::endl;
	outlog << std::endl;
	outlog << std::endl;
#endif // _DEBUG

	kalman.Reset();
	startAngleSum = {};
	dataCount = 0;
	startTime = std::chrono::system_clock::now();
}

bool BallController::IsForward() const
{
	bool result = false;
	result = accel.y > +deadzone;
	//Vec3 v = startAngle - angle;
	//result = v.z > +deadzone;
	return result;
}

bool BallController::IsBack() const
{
	bool result = false;
	result = accel.y < -deadzone;
	//Vec3 v = startAngle - angle;
	//result = v.z < -deadzone;
	return result;
}

bool BallController::IsLeft() const
{
	bool result = false;
	result = accel.x > +deadzone + 0.5f;
	//result = (startAngle - angle).y > +deadzone;
	return result;
}

bool BallController::IsRight() const
{
	bool result = false;
	result = accel.x < -deadzone + 0.5f;
	//result = (startAngle - angle).y < -deadzone;
	return result;
}

bool BallController::IsJamp() const
{
	static bool isJamp = false;
	bool result = false;

	if (GetFlag() || GetFlagReturn())
	{
		result = false;
	}
	else
	{
		if (accel.z > 0.0f)
		{
			if (isJamp)
			{
				result = false;
			}
			else
			{
				isJamp = true;
				result = true;
			}
		}
		else
		{
			isJamp = false;
			result = false;
		}
	}

	return result;
}

bool BallController::IsBallThrow() const
{
	bool result = GetFlagReturn() && (gyro.x < -10.0f);
	return result;
}
