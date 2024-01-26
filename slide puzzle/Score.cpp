#include "Score.h"
#include"LoadCSV.h"
#include "WriteCSV.h"
#include <iostream>
#include <string>
using namespace std;
Score::Score()
{
}

Score::~Score()
{
}

void Score::Init()
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
	score += 1;
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
		Sprite::Get()->Draw(number[num], Vec2(1155.0f, 18.0f) - Vec2(50.0f, 0.0f) * static_cast<float>(i), 90.0f, 90.0f);
	}
}

void Score::ResultSceneDraw() {
	if (loadFlag) {
		RankingLoad();  // ランキングの読みこみと最新の状態にする
		RankingWrite(); // CSVにランキングを書き込み
		loadFlag = false;
	}
	
	// 1位
	int score1st_ = topScore[0];
	int index1st = 1;
	while (score1st_ > 9) {
		index1st++;
		score1st_ = score1st_ / 10;
	}
	for (int i = 0; i < index1st; i++) {
		int m = 10;
		for (int j = 0; j < i; j++) {
			m = m * 10;
		}

		int num = topScore[0] % m / (m / 10);
		if (num > 10 || num < 0) {
			break;
		}
		Sprite::Get()->Draw(
		    number[num], Vec2(670.0f, 370.0f) - Vec2(60.0f, 0.0f) * static_cast<float>(i), 64.0f,
		    64.0f);
	}
	// 2位
	int score2nd_ = topScore[1];
	int index2nd = 1;
	while (score2nd_ > 9) {
		index2nd++;
		score2nd_ = score2nd_ / 10;
	}
	for (int i = 0; i < index2nd; i++) {
		int m = 10;
		for (int j = 0; j < i; j++) {
			m = m * 10;
		}

		int num = topScore[1] % m / (m / 10);
		if (num > 10 || num < 0) {
			break;
		}
		Sprite::Get()->Draw(
		    number[num], Vec2(670.0f, 430.0f) - Vec2(60.0f, 0.0f) * static_cast<float>(i), 64.0f,
		    64.0f);
	}

	// 3位
	int score3rd_ = topScore[2];
	int index3rd = 1;
	while (score3rd_ > 9) {
		index3rd++;
		score3rd_ = score3rd_ / 10;
	}
	for (int i = 0; i < index3rd; i++) {
		int m = 10;
		for (int j = 0; j < i; j++) {
			m = m * 10;
		}

		int num = topScore[2] % m / (m / 10);
		if (num > 10 || num < 0) {
			break;
		}
		Sprite::Get()->Draw(
		    number[num], Vec2(670.0f, 490.0f) - Vec2(60.0f, 0.0f) * static_cast<float>(i), 64.0f,
		    64.0f);
	}

	//今回の点数
	int scoreNow_ = score;
	int indexNow = 1;
	while (scoreNow_ > 9) {
		indexNow++;
		scoreNow_ = scoreNow_ / 10;
	}
	for (int i = 0; i < indexNow; i++) {
		int m = 10;
		for (int j = 0; j < i; j++) {
			m = m * 10;
		}

		int num = score % m / (m / 10);
		if (num > 10 || num < 0) {
			break;
		}
		Sprite::Get()->Draw(
			number[num], Vec2(670.0f, 200.0f) - Vec2(60.0f, 0.0f) * static_cast<float>(i), 64.0f,
			64.0f);
	}
}

void Score::RankingLoad() {
	LoadCSV1D(topScore, 3, "Resources/rank/rank.csv");

	if (topScore[0] < score) {
		topScore[2] = topScore[1];
		topScore[1] = topScore[0];
		topScore[0] = score;
	} else if (topScore[1] < score) {
		topScore[2] = topScore[1];
		topScore[1] = score;
	} else if (topScore[2] < score) {
		topScore[2] = score;
	}
}

void Score::RankingWrite() {
	const std::string& firstPlace = std::to_string(topScore[0]);
	const std::string& secondPlace = std::to_string(topScore[1]);
	const std::string& thirdPlace = std::to_string(topScore[2]);
	const std::string& comma = ",";
	const std::string& ranking = firstPlace + comma + secondPlace + comma + thirdPlace;
	WriteCSV(ranking, 3, "Resources/rank/rank.csv");
}
