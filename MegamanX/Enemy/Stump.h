#pragma once
#include "SpriteGo.h"
class SceneGame;
class Player;

class Stump : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;
	sf::FloatRect stumpBounds;
	sf::Vector2f boundingBoxWorldPos;
	sf::Vector2f boundingBoxLocalPos;

	sf::Color collisionColor = { 255 , 0 , 0 }; 
	sf::Color sideCollisionColor = { 0 , 255 , 0 }; 
	bool firstTree = false;
	bool secondTree = false;

	bool treeChange = true;
	float stumpTimer;

public:

	Stump(const std::string& name = "");
	~Stump() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void SetTreeChange(bool s) { treeChange = s; }
	void SetFirstTree(bool t);
	void SetSecondTree(bool t);

	int startX;
	int endX;
	int startY;
	int endY;

};

