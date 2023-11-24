#include "BallController.h"
#include "Ini.h"
#include <string>

const char BallController::Data::header[4] = { 'D', 'A', 'T', 'F' };

const int BallController::Data::GetPacketSize()
{
	int result = 0;
	result += sizeof(header);
	result += sizeof(int16_t) * sensorCount;
	result += sizeof(uint8_t);
	return result;
}

const Vec3 BallController::gravity = { 1, 0, 0 };
BallController::BallController(const char* serialDevice) :
	accel{},
	oldAccel(accel),
	gyro{},
	oldGyro(gyro),
	flag(false),
	oldFlag(false),
	angle(Vec3(0, 0, 0))
{
	this->serial = Serial::create(serialDevice);
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
	oldAccel = accel;
	oldGyro = gyro;
	oldFlag = flag;
	char* p = buf;
	const int packetSize = Data::GetPacketSize();
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

		accel = Vec3(records.back().cencor[ACCEL_X],
					 records.back().cencor[ACCEL_Y],
					 records.back().cencor[ACCEL_Z]);
		gyro = Vec3(records.back().cencor[GYRO_X],
					records.back().cencor[GYRO_Y],
					records.back().cencor[GYRO_Z]);
		flag = records.back().flags[0];
	}

	const int tailSize = (int)(buf + contentSize - p);
	memmove(buf, p, tailSize);
	contentSize = tailSize;

	AngleUpdate();

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
	if (accel.length() < 1)
	{
		return;
	}

	angle += gyro;

	if (angle.x < -180)
	{
		angle.x += 360;
	}
	else if (angle.x > +180)
	{
		angle.x -= 360;
	}
	if (angle.y < -180)
	{
		angle.y += 360;
	}
	else if (angle.y > +180)
	{
		angle.y -= 360;
	}
	if (angle.z < -180)
	{
		angle.z += 360;
	}
	else if (angle.z > +180)
	{
		angle.z -= 360;
	}
}

void BallController::AngleReset()
{
	angle = Vec3(0, 0, 0);
}

bool BallController::IsBallThrow()
{
	bool result = GetFlagReturn() && gyro.y < -10;
	AngleReset();
	return result;
}
