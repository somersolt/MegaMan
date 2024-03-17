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
		bee2,
		snowThrower,
		bat,
		ostrich,
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
	bool isBossRoom = false;
	bool isWatingRoom = false;
	bool BossAppear = false;

	float resetTimer = 0.f;

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
	bool isBossRoomView = false;

	bool cameraWork = false;
	float cameraWorkTimer = 0.f;
	int cameraWorkCount = 0;
	int waitingRoomCameraWorkCount = 129;
	int bossRoomCameraWorkCount = 257;

	SpriteGo* playerLife = nullptr;
	SpriteGo* bossLife = nullptr;
	SpriteGo* pauseMenu = nullptr;

	SpriteGo* background = nullptr;
	SpriteGo* maptest = nullptr;
	SpriteGo* map = nullptr;

	sf::Image collisionMapImage;
	sf::Texture collisionMapTexture;
	sf::Sprite collisionMapSprite;

	sf::Image waitingCollisionMapImage;
	sf::Image bossCollisionMapImage;

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
	void CreateLumberjack(std::string name, sf::Vector2f pos);
	void BlizzardEffect();


	void EnemyClear();
	void EnemySummon();

	void BossClear();
	void BossSummon();
	void Test();
};

