#pragma once

class GameObject;

class Scene
{
public:
	enum Layers
	{
		None = 0,
		World = 1,
		Ui = 2,
		BackGround = 3,
		EveryThing = 0xfffffff,
	};
protected:
	SceneIds id;
	std::list<GameObject*> gameObjects;
	std::list<GameObject*> backGroundObjects;
	std::list<GameObject*> uiGameObjects;

	std::list<GameObject*> resortingGameObjects;
	std::list<GameObject*> removeGameObjects;

	sf::View backgroundView;
	sf::View worldView;
	sf::View uiView;


	ResourceMgr<sf::Texture>& texResMgr;
	ResourceMgr<sf::Font>& fontResMgr;
	ResourceMgr<sf::SoundBuffer>& soundResMgr;
public:
	Scene(SceneIds id);

	virtual ~Scene() = default;


	sf::Vector2f ScreenToWorld(sf::Vector2i screenPos);
	sf::Vector2i WorldToScreen(sf::Vector2f worldPos);

	sf::Vector2f ScreenToUi(sf::Vector2i screenPos);
	sf::Vector2i WorldToUi(sf::Vector2f worldPos);


	virtual void Init();
	virtual void Release();

	virtual void Enter();
	virtual void Exit() {};

	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void FixedUpdate(float dt);

	virtual void Draw(sf::RenderWindow& window);

	virtual GameObject* FindGo(const std::string& name, Layers layer = Layers::EveryThing);
	virtual int FindGoAll(const std::string& name, std::list<GameObject*>& list, Layers layer = Layers::EveryThing);

	virtual GameObject* AddGo(GameObject* obj, Layers layer = Layers::World);
	virtual void ResortGo(GameObject* obj);
	virtual void RemoveGo(GameObject* obj);

	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;
};

