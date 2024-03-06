#pragma once
#include "Scene.h"


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

public:
	SceneGame(SceneIds id);
	~SceneGame() override = default;

	void SetStatus(Status newStatus);


	void Init() override;
	void Release() override;

	void Enter() override;
	void Exit() override;


	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);


	void Draw(sf::RenderWindow& window);

};

