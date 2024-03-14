#pragma once
#include "Enemy.h"

class Tree;

class Lumberjack :
    public Enemy
{
private:

	float skillTimer = 0;
	bool onSkill = false;
	Tree* tree = nullptr;
	std::string lumberjackStump = "";
public:
	Lumberjack(const std::string& name, sf::Image& mapImage, std::string lumberjackStump);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Swing();
	void ToIdle();
};

