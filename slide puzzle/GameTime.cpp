#include "GameTime.h"

GameTime::GameTime()
{
}

GameTime::~GameTime()
{
}

void GameTime::Init()
{
	number[0] = Sprite::Get()->SpriteCreate(L"Resources/number/combo0.png");
	number[1] = Sprite::Get()->SpriteCreate(L"Resources/number/combo1.png");
	number[2] = Sprite::Get()->SpriteCreate(L"Resources/number/combo2.png");
	number[3] = Sprite::Get()->SpriteCreate(L"Resources/number/combo3.png");
	number[4] = Sprite::Get()->SpriteCreate(L"Resources/number/combo4.png");
	number[5] = Sprite::Get()->SpriteCreate(L"Resources/number/combo5.png");
	number[6] = Sprite::Get()->SpriteCreate(L"Resources/number/combo6.png");
	number[7] = Sprite::Get()->SpriteCreate(L"Resources/number/combo7.png");
	number[8] = Sprite::Get()->SpriteCreate(L"Resources/number/combo8.png");
	number[9] = Sprite::Get()->SpriteCreate(L"Resources/number/combo9.png");
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
	Sprite::Get()->Draw(number[minutes], Vec2(0.0f, 20.0f), 40.0f, 40.0f);
	Sprite::Get()->Draw(ten, Vec2(30.0f, 20.0f), 40.0f, 40.0f);

	int second = nowTime % 60;
	int ten_Seconds = second / 10;
	int one_Seconds = second % 10;
	if (ten_Seconds <= 0 || minutes > 9) { ten_Seconds = 0; }
	if (one_Seconds <= 0 || minutes > 9) { one_Seconds = 0; }
	Sprite::Get()->Draw(number[ten_Seconds], Vec2(60.0f, 20.0f), 40.0f, 40.0f);
	Sprite::Get()->Draw(number[one_Seconds], Vec2(90.0f, 20.0f), 40.0f, 40.0f);
}

void GameTime::Start()
{

}
