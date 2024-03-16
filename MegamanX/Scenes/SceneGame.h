#pragma once
#include "Scene.h"

class Player;
class Boss;
class SpriteGo;

class SceneGame : public Scene
{
public:
	enum class Status
	{
		Playing,
		Pause,
	};

	enum class monster
	{
		None = -1,
		rabbit,
		bee,
		bee2
	};

protected:

	Status currentStatus;
	Player* player = nullptr;
	Boss* boss = nullptr;
	sf::FloatRect viewBounds;
	sf::Vector2f viewSize = { 284, 244 };

	sf::Vector2f preWorldViewCenter;
	sf::Vector2f worldViewCenter;
	sf::Vector2f worldViewMoment;
	sf::Vector2f backgroundViewMoment;
	sf::Vector2f backgroundViewCenter;

	float viewXMax;
	float viewXMin;
	float viewYMax;
	float viewYMin;
	bool onBoss = false;
	bool onWatingRoom = false;


public:
	SceneGame(SceneIds id);
	~SceneGame() override = default;

	std::list<GameObject*> enemyList;
	std::vector<SpriteGo*> lifeGaugeList;
	std::vector<SpriteGo*> bossGaugeList;
	
	float bossGaugeTimer = 0.f;
	int bossHp = 0;
	int bossHPAnimationCount = 0;
	bool appearAnimation = false;
	bool bossFight = false;

	SpriteGo* playerLife = nullptr;
	SpriteGo* bossLife = nullptr;

	SpriteGo* background = nullptr;
	SpriteGo* maptest = nullptr;
	SpriteGo* map = nullptr;

	sf::Image collisionMapImage;
	sf::Texture collisionMapTexture;
	sf::Sprite collisionMapSprite;

	SpriteGo* mapHitBox = nullptr;  //충돌박스 표시

	const std::list<GameObject*>& GetEnemyList() const { return enemyList; }

	void SetStatus(Status newStatus);
	void SetAppearAnimation(bool s) { appearAnimation = s; }

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

	void CreateMonster(monster monster, sf::Vector2f pos);
	void CreateWheeler(int g , sf::Vector2f pos);
};

