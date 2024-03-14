#pragma once
#include "SpriteGo.h"

class SceneGame;
class Player;
class Stump;

class Tree : public SpriteGo
{
private:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;
	Stump* stump = nullptr;
	sf::FloatRect treebounds;
	sf::Vector2f boundingBoxWorldPos;
	sf::Vector2f boundingBoxLocalPos;
	sf::Color collisionColor = { 255 , 0 , 0 };
	sf::Color sideCollisionColor = { 0 , 255 , 0 };
	int stumpCount = 3;
	int Hp = 2;
	int GrowUpCount = 0;
	bool isGrow = true;
	bool fallDown = false;
	float GrowTimer = 0;

	std::string stumpName = "";
	int treeNumber = 0;

public:

	Tree(const std::string& name = "");
	~Tree() override = default;

	void Init() override;
	void Reset() override;
	void Release() override;

	void Update(float dt) override;

	void SetStump(std::string name) { stumpName = name; }
	std::string GetStump() { return stumpName; }
	void SetTreeNumber(int n) { treeNumber = n; }
	void SetFallDown(bool f) { fallDown = f; }
	void OnDamage(int d);

	int startX;
	int endX;
	int startY;
	int endY;

	void GrowUp();
	void FallDown();
	void Boom();

};

