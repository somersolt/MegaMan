#pragma once
#include "Enemy.h"
class Wheeler : public Enemy
{
    private:

		float skillTimer = 0;
		bool onSkill = false;
public:
	Wheeler(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset(int g);
	void Update(float dt) override;

	void Down();
};

