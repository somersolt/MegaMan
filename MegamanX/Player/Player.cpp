#include "pch.h"
#include "Player.h"
#include "SceneGame.h"
#include "Buster.h"
#include <algorithm>


Player::Player(const std::string& name) : SpriteGo(name)
{
}


void Player::SetPlayerStatus(Status status)
{
	preStatus = currentStatus;
	currentStatus = status;
}

void Player::Shoot()
{
	Buster* buster = new Buster("buster");
	buster->Init();
	buster->Reset();
	buster->SetPosition({ position.x, position.y - 16 });
	buster->Fire({1.f,0.f}, 600, 1 ,side);
	sceneGame->AddGo(buster);
	isShooting = false;
}

void Player::ChargeShoot()
{
	Buster* chargeBuster = new Buster("chargeBuster");
	chargeBuster->Init();
	chargeBuster->Reset();
	chargeBuster->SetPosition({ position.x, position.y - 30 });
	chargeBuster->ChargeFire({ 1.f,0.f }, 600, 5, side);
	sceneGame->AddGo(chargeBuster);
	isShooting = false;
}

void Player::Init()
{
	SpriteGo::Init();
	playerAnimation.SetTarget(&sprite);
	playerEffectAnimation.SetTarget(&effect);
}

void Player::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	//게임 씬 동기화

	playerAnimation.ClearEvent();
	playerEffectAnimation.ClearEvent();
	playerAnimation.Play("animations/Idle.csv");
	SetPlayerStatus(Status::Idle);
	SetOrigin(Origins::BC);
	SetPosition({ 1000.f, 0.f });
	SetFlipX(false);
	// 플레이어 애니메이션 세팅

	playerHitBox.setSize({ 30.f, 35.f });
	playerHitBox.setOrigin({ playerHitBox.getLocalBounds().width / 2, playerHitBox.getLocalBounds().height });
	playerHitBox.setFillColor(sf::Color::Transparent);
	playerHitBox.setPosition(position);
	playerBounds = playerHitBox.getGlobalBounds();



	std::function<void()> ToIdle = std::bind(&Player::IdleAnimation, this);
	playerAnimation.AddEvent("animations/Landing.csv", 2, ToIdle);
	playerAnimation.AddEvent("animations/LandingShot.csv", 2, ToIdle);
	playerAnimation.AddEvent("animations/Dash.csv", 3, ToIdle);
	playerAnimation.AddEvent("animations/Shot.csv", 6, ToIdle);
	std::function<void()> ToShot = std::bind(&Player::ShotAnimation, this);
	playerAnimation.AddEvent("animations/DashShot.csv", 3, ToShot);
}

void Player::IdleAnimation()
{
	playerAnimation.Play("animations/Idle.csv");
	SetPlayerStatus(Status::Idle);
	speed = 200;
	isDash = false;
	isCantFlip = false;
}

void Player::ShotAnimation()
{
	playerAnimation.Play("animations/Shot.csv");
	SetPlayerStatus(Status::Idle);
	speed = 200;
	isDash = false;
	isCantFlip = false;
}



void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	playerAnimation.Update(dt);
	playerEffectAnimation.Update(dt);

	///  히트박스 테스트 코드

	if (InputMgr::GetKeyDown(sf::Keyboard::T))
	{
		playerHitBox.setFillColor(sf::Color::Transparent);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		playerHitBox.setFillColor(sf::Color::Blue);
	}
	/// 히트박스 테스트 코드


	h = InputMgr::GetAxisRaw(Axis::Horizontal);
	shootTimer += dt;
	effect.setPosition(position);
	effect.setOrigin({ effect.getLocalBounds().width / 2 , effect.getLocalBounds().height - 30});
	playerHitBox.setPosition(position);

	if (isShootingMode) // 실제 사격과 관계없이 총을 꺼내들고 있는 시간
	{
		ShootingModeTimer += dt;

		if (ShootingModeTimer > ShootingModeInterval)
		{
			isShootingMode = false;
			ShootingModeTimer = 0;
		}
	}



	switch (currentStatus)
	{
	case Player::None:
		break;
	case Player::Idle:
		UpdateIdle(dt);
		break;
	case Player::Run:
		UpdateRun(dt);
		break;
	case Player::JumpingUp:
		UpdateJumpingUp(dt);
		break;
	case Player::FallingDown:
		UpdateFallingDown(dt);
		break;
	case Player::Landing:
		UpdateLanding(dt);
		break;
	case Player::Dash:
		UpdateDash(dt);
		break;
	case Player::Hit:
		UpdateHit(dt);
		break;
	case Player::Die:
		UpdateDie(dt);
		break;
	default:
		break;
	}

	if (h != 0.f && !isCantFlip)
	{
		SetFlipX(h < 0);
		if (h < 0)
		{
			side = Sides::Left;
		}
		else if (h > 0)
		{
			side = Sides::Right;
		}
	}
	if (!isDash)
	{
		velocity.x = h * speed * dt;
	}
	if (!isCharge)
	{
		chargeTimer = 0;
		chargeEffectMode = false;
	}
	if (isCharge && !chargeEffectMode && chargeTimer > 0.5f)
	{
		playerEffectAnimation.Play("animations/Charge.csv");
		chargeEffectMode = true;
	}

	if (1.f > chargeTimer && chargeTimer > 0.5f)
	{
		sf::Color color(255, 255, 100, 255); 
		sprite.setColor(color);
	}
	if (chargeTimer > 1.f)
	{
		sf::Color color(255, 255, 255, 255);
		sprite.setColor(color);
		chargeTimer = 0;
	}


	// 사격
	if (InputMgr::GetKeyDown(sf::Keyboard::X) && shootTimer > shootInterval)
	{
		Shoot();
		chargeTimer = 0;
		isShooting = true;
		isShootingMode = true;
	}
	if (InputMgr::GetKey(sf::Keyboard::X))
	{
		isCharge = true;
		chargeTimer += dt;
	}

	if (InputMgr::GetKeyUp(sf::Keyboard::X))
	{
		if (!chargeEffectMode)
		{
			Shoot();
		}
		if (chargeEffectMode)
		{
			ChargeShoot();
		}
		isCharge = false;
		shootTimer = 0;
		isShooting = true;
		isShootingMode = true;
		sf::Color color(255, 255, 255, 255); // 원래 색으로 돌아옴
		sprite.setColor(color);
	}
	



	if (InputMgr::GetKeyDown(sf::Keyboard::Z))
	{
		SetPosition({ 0,0 });
	}

	if (!isGrounded)
	{
		velocity.y += gravity * dt;
	}

	playerBounds.left = playerHitBox.getPosition().x - playerHitBox.getSize().x / 2;
	playerBounds.top = playerHitBox.getPosition().y - playerHitBox.getSize().y / 2;
	playerBounds.width = playerHitBox.getSize().x;
	playerBounds.height = playerHitBox.getSize().y;


	position += sf::Vector2f(velocity.x, velocity.y * dt);
	SetPosition(position); // 중력을 포함한 위치 적용

	SetOrigin(Origins::BC);

	//std::cout << velocity.x << std::endl; // 테스트 코드
}




void Player::UpdateIdle(float dt)
{
	if (isShootingMode && isShooting)
	{
		playerAnimation.Play("animations/Shot.csv");
		isCantFlip = true;
		isShooting = false;
	} // 대기 -> 사격

	if (h != 0.f)
	{
		isCantFlip = false;
		SetPlayerStatus(Status::Run);
		playerAnimation.Play("animations/Run.csv");
	}
	 // 대기 -> 달리기

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetPlayerStatus(Status::JumpingUp);
		velocity.y = -600.f;
		playerAnimation.Play("animations/JumpUp.csv");
	} // 대기 -> 점프

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
	} // 대기 -> 대시

}

void Player::UpdateRun(float dt)
{

	if (isShootingMode)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		if (i == 9)
		{
			i = 0;
		}
		playerAnimation.Play("animations/RunShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // 달리기 -> 사격

	if (!isShootingMode)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		if (i == 9)
		{
			i = 0;
		}
		playerAnimation.Play("animations/Run.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // 달리기 사격 -> 달리기

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetPlayerStatus(Status::JumpingUp);
		velocity.y = -600.f;
		playerAnimation.Play("animations/JumpUp.csv");
	} // 달리기 -> 점프

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
	} // 달리기 -> 대시

	if (h == 0.f)
	{
		SetPlayerStatus(Status::Idle);
		playerAnimation.Play("animations/Idle.csv");
	}
	// 달리기 -> 서기 
}

void Player::UpdateJumpingUp(float dt)
{

	if (isShootingMode)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		if (i < 2)
		{
			playerAnimation.Play("animations/JumpUpShot.csv", true, true);
			playerAnimation.SetCurrentFrame(i);
		}
		else
		{
			playerAnimation.Play("animations/JumpUpShot.csv", true, true);
			playerAnimation.SetCurrentFrame(2);
		}
	}//점프 상승 중 사격

	if (velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
	} // 점프 상승 중 하강
}

void Player::UpdateFallingDown(float dt)
{

	if (isShootingMode)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/JumpDownShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 점프 하강 중 사격

	if (position.y >= 0.f)  // TO-DO 충돌판정으로 바꾸기
	{
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
	} // 점프 하강 중 착지
}

void Player::UpdateLanding(float dt)
{
	h = 0;
	velocity.x /= 2;
	isCantFlip = true;
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/LandingShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 점프 하강 중 사격
}

void Player::UpdateDash(float dt)
{
	if (!isFlipX)
	{
		velocity.x = 1;
	}
	else if (isFlipX)
	{
		velocity.x = -1;
	}
	isCantFlip = true;
	isDash = true;


	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetPlayerStatus(Status::JumpingUp);
		velocity.y = -600.f;
		playerAnimation.Play("animations/JumpUp.csv");
	} // 대시 -> 점프

	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/DashShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 대시 -> 대시사격

}

void Player::UpdateHit(float dt)
{
}

void Player::UpdateDie(float dt)
{
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	if (chargeEffectMode)
	{
		window.draw(effect);
	}
	window.draw(playerHitBox);

}



void Player::LateUpdate(float dt)
{

}


void Player::FixedUpdate(float dt)
{

	//sf::Sprite collisionMap = sceneGame->collisionMapSprite;
	//sf::Image MapImage = collisionMap.getTexture()->copyToImage();

	//for (int x = playerBounds.left; x < playerBounds.left + playerBounds.width; ++x)
	//{
	//	int y = playerBounds.top + playerBounds.height;
	//	sf::Color pixelColor = MapImage.getPixel(x, y);
	//	std::cout << x << "/" << y << std::endl;
	//	if (pixelColor == collisionColor)
	//	{
	//		//std::cout << "in!!" << std::endl;
	//		velocity.y = 0;
	//		isColliding = true;
	//	}
	//	if (pixelColor != collisionColor)
	//	{
	//		//std::cout << "out!!" << std::endl;
	//		isColliding = false;
	//	}
	//}
	//isGrounded = isColliding;

	sf::Vector2f boundingBoxWorldPos = playerBounds.getPosition();
	sf::Image MapImage = sceneGame->collisionMapSprite.getTexture()->copyToImage();
	sf::Vector2f boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToLocal(boundingBoxWorldPos);

	int startX = std::max(0, static_cast<int>(boundingBoxLocalPos.x));
	int endX = std::min(static_cast<int>(MapImage.getSize().x),
		static_cast<int>(boundingBoxLocalPos.x + 30));
	int bottomY = static_cast<int>(boundingBoxLocalPos.y + 35);
	if (startX >= 0 && bottomY >= 0 && bottomY <= 165)
	{
		for (int x = startX; x < endX; ++x)
		{
			sf::Color pixelColor = MapImage.getPixel(x, bottomY);
			std::cout << x << "/" << bottomY << std::endl;
			if (pixelColor == collisionColor)
			{
				//std::cout << "in!!" << std::endl;
				velocity.y = 0;
				isColliding = true;
			}
			if (pixelColor != collisionColor)
			{
				//std::cout << "out!!" << std::endl;
				isColliding = false;
			}
		}
		isGrounded = isColliding;
	}
}

