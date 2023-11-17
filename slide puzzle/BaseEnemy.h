#pragma once
#include "Enemy.h"

class BaseEnemy :
    public Enemy
{
public:
	BaseEnemy() = default;
	~BaseEnemy() = default;
	void Init(Vec3 pos = {}, Vec3 rot = {}) override;
	void Update() override;
	void Draw() override;

private:
    void Move();
};

