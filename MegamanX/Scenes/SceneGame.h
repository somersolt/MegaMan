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
	sf::FloatRect viewBounds;
	sf::Vector2f viewSize = { 284, 244 };
	sf::Color collisionColor = { 255 , 0 , 0 }; // 충돌 색깔 - 빨간색

	sf::Vector2f preWorldViewCenter;
	sf::Vector2f worldViewCenter;
	sf::Vector2f worldViewMoment;
	sf::Vector2f backgroundViewMoment;
	sf::Vector2f backgroundViewCenter;

	float viewXMax;
	float viewXMin;
	float viewYMax;
	float viewYMin;

	int rollBackTop = 0;
	bool isTopColliding = false;
	bool isTopViewMax = false;

	int rollBackBottom = 0;
	bool isBottomColliding = false;
	bool isBottomViewMax = false;

	int rollBackRight = 0;
	bool isRightColliding = false;
	bool isRightViewMax = false;

	int rollBackLeft = 0;
	bool isLeftColliding = false;
	bool isLeftViewMax = false;

	int startX;
	int endX;
	int startY;
	int endY;

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
	sf::FloatRect GetViewBounds() const { return viewBounds; }
	bool isStart = true;
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

