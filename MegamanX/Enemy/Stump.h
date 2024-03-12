#pragma once
#include "SpriteGo.h"
class SceneGame;

class Stump : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	Animator stumpAnimation;
	sf::FloatRect stumpBounds;

	sf::Color collisionColor = { 255 , 0 , 0 }; 
	sf::Color sideCollisionColor = { 0 , 255 , 0 }; 

public:


	Stump(const std::string& name = "");
	~Stump() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

};

