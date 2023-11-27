#pragma once
#include "Enemy.h"

class BaseEnemy :
    public Enemy
{
public:

	enum MoveAct
	{
		Progress,
		Retreat,
		Right,
		Left,
	};

public:
	BaseEnemy() = default;
	~BaseEnemy() = default;
	void Init(Vec3 pos = {}, Vec3 rot = {}) override;
	void Update() override;
	void Draw() override;

private:
    void Move();

private:
	int maxTime_ = 0;
	int timer_ = 0;
	MoveAct act;
};

