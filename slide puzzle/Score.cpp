#include "Score.h"

Score::Score()
{
}

Score::~Score()
{
}

void Score::Init()
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

	scoreBack = Sprite::Get()->SpriteCreate(L"Resources/sprite/scoreBack.png");
}

void Score::ScoreReset()
{
	score = 0;
}

void Score::Update()
{
}

void Score::PlasScore()
{
	score += 10;
}

void Score::GameSceneDraw()
{

	int score_ = score;
	int index = 1;
	while (score_ > 9)
	{
		index++;
		score_ = score_ / 10;
	}

	for (int i = 0; i < index; i++)
	{
		int m = 10;
		for (int j = 0; j < i; j++)
		{
			m = m * 10;
		}

		int num = score % m / (m / 10);
		if (num > 10 || num < 0) { break; }
		Sprite::Get()->Draw(number[num], Vec2(1240.0f, 20.0f) - Vec2(38.0f, 0.0f) * static_cast<float>(i), 40.0f, 40.0f);
	}
}

void Score::ResultSceneDraw()
{
	int score_ = score;
	int index = 1;
	while (score_ > 9)
	{
		index++;
		score_ = score_ / 10;
	}
	Sprite::Get()->Draw(scoreBack, Vec2(400.0f, 260.0f) , 500.0f, 128.0f);
	for (int i = 0; i < index; i++)
	{
		int m = 10;
		for (int j = 0; j < i; j++)
		{
			m = m * 10;
		}

		int num = score % m / (m / 10);
		if (num > 10 || num < 0) { break; }
		Sprite::Get()->Draw(number[num], Vec2(800.0f, 300.0f) - Vec2(60.0f, 0.0f) * static_cast<float>(i), 64.0f, 64.0f);
	}
}
