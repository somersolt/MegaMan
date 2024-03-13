#pragma once
#include "Scene.h"

class Player;
class SpriteGo;

class SceneGame : public Scene
{
public:
	enum class Status
	{
		Playing,
		Pause,
	};

protected:

	Status currentStatus;
	Player* player = nullptr;
public:
	SceneGame(SceneIds id);
	~SceneGame() override = default;

	std::list<GameObject*> enemyList;

	SpriteGo* background = nullptr;
	SpriteGo* maptest = nullptr;
	SpriteGo* map = nullptr;

	sf::Image collisionMapImage;
	sf::Texture collisionMapTexture;
	sf::Sprite collisionMapSprite;

	SpriteGo* mapHitBox = nullptr;  //충돌박스 표시

	const std::list<GameObject*>& GetEnemyList() const { return enemyList; }

	void SetStatus(Status newStatus);
	sf::Vector2f GetPlayerPostion();

	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;


	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);


	void SetPixelToMap(int a, int b,int x, int y, sf::Color c);

	void Draw(sf::RenderWindow& window);

	sf::Vector2f PlayerBoundsWorldToMapLocal(sf::Vector2f playerBounds);

};

