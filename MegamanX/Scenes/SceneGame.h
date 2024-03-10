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

	SpriteGo* background = nullptr;
	SpriteGo* maptest = nullptr;
	SpriteGo* map = nullptr;

	sf::Image collisionMapImage;
	sf::Texture collisionMapTexture;
	sf::Sprite collisionMapSprite;

	SpriteGo* mapHitBox = nullptr;  //충돌박스 표시

	void SetStatus(Status newStatus);


	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;


	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);


	void Draw(sf::RenderWindow& window);

	sf::Vector2f PlayerBoundsWorldToLocal(sf::Vector2f playerBounds);

};

