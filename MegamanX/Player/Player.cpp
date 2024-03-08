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
	textureId = "graphics/xsheet.png";

}

void Player::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	playerAnimation.ClearEvent();
	playerEffectAnimation.ClearEvent();
	playerAnimation.Play("animations/Idle.csv");
	SetPlayerStatus(Status::Idle);
	SetOrigin(Origins::BC);
	SetPosition({ 1250,0 });
	SetFlipX(false);
	// 플레이어 애니메이션 세팅

	playerHitBox.setSize({ 30.f, 35.f });
	playerHitBox.setOrigin({ playerHitBox.getLocalBounds().width / 2, playerHitBox.getLocalBounds().height });
	playerHitBox.setFillColor(sf::Color::Transparent);
	playerHitBox.setPosition(position);

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
	playerBounds = playerHitBox.getGlobalBounds();

	if (isShootingMode) // 실제 사격과 관계없이 총을 꺼내들고 있는 시간
	{
		ShootingModeTimer += dt;

		if (ShootingModeTimer > ShootingModeInterval)
		{
			isShootingMode = false;
			ShootingModeTimer = 0;
		}
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
	case Player::Climbing:
		UpdateClimbing(dt);
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


	if (InputMgr::GetKeyDown(sf::Keyboard::Z))
	{
		velocity.x = 0;
		velocity.y = 0;
		SetPosition({ 1250,0 });
		isGrounded = false;
	}
}




void Player::UpdateIdle(float dt)
{
	if (isShootingMode && isShooting)
	{
		playerAnimation.Play("animations/Shot.csv");
		isCantFlip = true;
		isShooting = false;

	} // 대기 -> 사격

	if (!isGrounded && !isColliding && velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isColliding = false;
		isGrounded = false;
		velocity.y = -600.f;
		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;

	} // 대기 -> 점프

	if (h != 0.f)
	{
		isCantFlip = false;
		SetPlayerStatus(Status::Run);
		playerAnimation.Play("animations/Run.csv");
		return;
	}
	 // 대기 -> 달리기


	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		isGrounded = false;
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
		return;

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

	if (!isGrounded && !isColliding && velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		velocity.y = -600.f;
		isGrounded = false;
		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;

	} // 달리기 -> 점프

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
		return;

	} // 달리기 -> 대시

	if (h == 0.f)
	{
		SetPlayerStatus(Status::Idle);
		playerAnimation.Play("animations/Idle.csv");
		return;
	}
	// 달리기 -> 서기 
}

void Player::UpdateJumpingUp(float dt)
{
	isGrounded = false;

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
		return;

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

	if (isGrounded && velocity.y >= 0)
	{
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
		return;

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
		velocity.x = 0.5f;
	}
	else if (isFlipX)
	{
		velocity.x = -0.5f;
	}
	isCantFlip = true;
	isDash = true;


	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isColliding = false;
		isGrounded = false;
		velocity.y = -600.f;

		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;

	} // 대시 -> 점프

	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/DashShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 대시 -> 대시사격

}

void Player::UpdateClimbing(float dt)
{

}


void Player::UpdateHit(float dt)
{
}

void Player::UpdateDie(float dt)
{
}


void Player::LateUpdate(float dt)
{
	std::cout << position.y << "/" << preposition.y << std::endl;

	preposition = position;
	position += sf::Vector2f(velocity.x, velocity.y * dt);
	playerHitBox.setPosition(position);
	playerBounds = playerHitBox.getGlobalBounds();

	boundingBoxWorldPos = playerBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToLocal(boundingBoxWorldPos);

	startX = std::max(0, static_cast<int>(boundingBoxLocalPos.x));
	endX = std::min(static_cast<int>(MapImage.getSize().x - 30),
		static_cast<int>(boundingBoxLocalPos.x + 30));
	startY = std::max(0, static_cast<int>(boundingBoxLocalPos.y));
	endY = std::min(static_cast<int>(MapImage.getSize().y - 35),
		static_cast<int>(boundingBoxLocalPos.y + 35));

	if (startX > 0 && endX < 350 && startY > 0 && endY < 200)  // TO-DO 맵의 로컬좌표 크기임. 큰맵으로 만들면 크게 *오류방지*
	{
		CheckBottomCollision();
		CheckRightCollision();

	}


	if (isPreMiddleRightColliding && isMiddleRightColliding && side == Sides::Right) // 우측 보정
	{
		position.x -= rollBackMiddleRight;
		rollBackMiddleRight = 0;
		SetPosition(position);
		playerHitBox.setPosition(position);
		SetOrigin(Origins::BC);
	}

	if(PreColliding && isColliding && PreGrounded && isGrounded) // 하단 보정
	{
		position.y -= std::max(rollBackBottomLeft, rollBackBottomRight);
		rollBackBottomLeft = 0;
		rollBackBottomRight = 0;
		SetPosition(position); // 중력을 포함한 위치 적용
		playerHitBox.setPosition(position);
		SetOrigin(Origins::BC);

	}


	isPreMiddleRightColliding = isMiddleRightColliding;
	PreColliding = isColliding;
	PreGrounded = isGrounded;
	SetPosition(position); // 중력을 포함한 위치 적용
	SetOrigin(Origins::BC);

}


void Player::FixedUpdate(float dt)
{
	if (!isGrounded)
	{
		velocity.y += gravity * dt;
	}
}

void Player::CheckBottomCollision()
{

	sf::Color BottomLeftPixel = MapImage.getPixel(startX + 1, endY);
	if (BottomLeftPixel == collisionColor)
	{
		isBottomLeftColliding = true;

		int temp = 0;
		while (BottomLeftPixel == collisionColor)
		{
			temp++;
			BottomLeftPixel = MapImage.getPixel(startX + 1, endY - temp);
		}
		rollBackBottomLeft = temp;

	}
	else
	{
		isBottomLeftColliding = false;
		rollBackBottomLeft = 0;
	}

	sf::Color BottomRightPixel = MapImage.getPixel(endX - 1, endY);
	if (BottomRightPixel == collisionColor)
	{
		isBottomRightColliding = true;
		int temp = 0;
		while (BottomRightPixel == collisionColor)
		{
			temp++;
			BottomRightPixel = MapImage.getPixel(endX - 1, endY - temp);
		}
		rollBackBottomRight = temp;
	}
	else
	{
		isBottomRightColliding = false;
		rollBackBottomRight = 0;
	}
	isColliding = (isBottomLeftColliding || isBottomRightColliding);
	isGrounded = isColliding;
}

void Player::CheckRightCollision()
{
	sf::Color MiddleRightPixel = MapImage.getPixel(endX, endY / 2);
	if (MiddleRightPixel == sideCollisionColor)
	{
		isMiddleRightColliding = true;

		int temp = 0;
		while (MiddleRightPixel == sideCollisionColor)
		{
			temp++;
			MiddleRightPixel = MapImage.getPixel(endX - temp, endY / 2);
		}
		rollBackMiddleRight = temp;

		return;
	}
	isMiddleRightColliding = false;
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