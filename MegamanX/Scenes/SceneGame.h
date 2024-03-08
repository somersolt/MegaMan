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

	SpriteGo* backgroundtest = nullptr;
	SpriteGo* background = nullptr;

	sf::Image collisionMapImage;
	sf::Texture collisionMapTexture;
	sf::Sprite collisionMapSprite;

	sf::RectangleShape mapHitBox;  //�浹�ڽ� ǥ��

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

