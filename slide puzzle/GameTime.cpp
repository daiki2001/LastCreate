#include "GameTime.h"

GameTime::GameTime()
{
}

GameTime::~GameTime()
{
}

void GameTime::Init()
{
	number[0] = Sprite::Get()->SpriteCreate(L"Resources/number/0.png");
	number[1] = Sprite::Get()->SpriteCreate(L"Resources/number/1.png");
	number[2] = Sprite::Get()->SpriteCreate(L"Resources/number/2.png");
	number[3] = Sprite::Get()->SpriteCreate(L"Resources/number/3.png");
	number[4] = Sprite::Get()->SpriteCreate(L"Resources/number/4.png");
	number[5] = Sprite::Get()->SpriteCreate(L"Resources/number/5.png");
	number[6] = Sprite::Get()->SpriteCreate(L"Resources/number/6.png");
	number[7] = Sprite::Get()->SpriteCreate(L"Resources/number/7.png");
	number[8] = Sprite::Get()->SpriteCreate(L"Resources/number/8.png");
	number[9] = Sprite::Get()->SpriteCreate(L"Resources/number/9.png");
	ten = Sprite::Get()->SpriteCreate(L"Resources/number/ten.png");

	start = time(NULL);
	now = time(NULL);
	sceneChangeFlag = false;
}

void GameTime::Update()
{

	now = time(NULL);

	differenceTime = static_cast<int>(now) - static_cast<int>(start);


	nowTime = gameTimeMax - differenceTime;

	if (nowTime <= 0)
	{
		sceneChangeFlag = true;
	}
}

void GameTime::Draw()
{

	int minutes = nowTime / 60;
	if (minutes <= 0 || minutes > 9) { minutes = 0; }
	Sprite::Get()->Draw(number[minutes], Vec2(-3.0f, 12.0f), 90.0f, 90.0f);

	int second = nowTime % 60;
	int ten_Seconds = second / 10;
	int one_Seconds = second % 10;
	if (ten_Seconds <= 0 || minutes > 9) { ten_Seconds = 0; }
	if (one_Seconds <= 0 || minutes > 9) { one_Seconds = 0; }
	Sprite::Get()->Draw(number[ten_Seconds], Vec2(87.0f, 25.0f), 90.0f, 90.0f);
	Sprite::Get()->Draw(number[one_Seconds], Vec2(150.0f, 38.0f), 90.0f, 90.0f);
}

void GameTime::Start()
{

}
