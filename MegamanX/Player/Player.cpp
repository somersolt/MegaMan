#include "pch.h"
#include "Player.h"
#include "SceneGame.h"
#include "Buster.h"
#include <algorithm>

Player::Player(const std::string& name, sf::Image& mapImage) : SpriteGo(name) , MapImage(mapImage)
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

	if (GetCurrentStatus() == Status::Climbing)
	{
		buster->SetPosition({ position.x, position.y - 25 });
	}
	if (GetCurrentStatus() == Status::WallJump)
	{
		buster->SetPosition({ position.x, position.y - 30 });
	}
	if (GetCurrentStatus() == Status::JumpingUp || GetCurrentStatus() == Status::FallingDown)
	{
		buster->SetPosition({ position.x, position.y - 30});
	}

	if (GetCurrentStatus() == Status::Climbing)
	{
		buster->Fire({ 1.f,0.f }, 600, 1, oppsetSide);
	}
	else
	{
		buster->Fire({ 1.f,0.f }, 600, 1, side);
	}
	sceneGame->AddGo(buster);
	isShooting = false;
}

void Player::ChargeShoot()
{
	Buster* chargeBuster = new Buster("chargeBuster");
	chargeBuster->Init();
	chargeBuster->Reset();
	chargeBuster->SetPosition({ position.x, position.y - 30 });

	if (GetCurrentStatus() == Status::Climbing)
	{
		chargeBuster->SetPosition({ position.x, position.y - 25 });
	}
	if (GetCurrentStatus() == Status::WallJump)
	{
		chargeBuster->SetPosition({ position.x, position.y - 30 });
	}
	if (GetCurrentStatus() == Status::JumpingUp || GetCurrentStatus() == Status::FallingDown)
	{
		chargeBuster->SetPosition({ position.x, position.y - 46 });
	}
	if (GetCurrentStatus() == Status::Climbing)
	{
		chargeBuster->ChargeFire({ 1.f,0.f }, 600, 5, oppsetSide);
	}
	else
	{
		chargeBuster->ChargeFire({ 1.f,0.f }, 600, 5, side);
	}
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
	SetPosition({ 200,900 });
	isGrounded = true;
	SetFlipX(false);
	side = Sides::Right;
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
	playerAnimation.AddEvent("animations/Hit.csv", 7, ToIdle);

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

	if (currentStatus != Status::Hit) 
	{
		h = InputMgr::GetAxisRaw(Axis::Horizontal);
	}

	shootTimer += dt;
	effect.setPosition(position);
	effect.setOrigin({ effect.getLocalBounds().width / 2 , effect.getLocalBounds().height - 30 });
	playerHitBox.setPosition(position);
	playerBounds = playerHitBox.getGlobalBounds();

	if (!isCantFlip)
	{
		if (h < 0)
		{
			side = Sides::Left;
			oppsetSide = Sides::Right;
		}
		else if (h > 0)
		{
			side = Sides::Right;
			oppsetSide = Sides::Left;
		}
		SetFlipX(side == Sides::Left);
	}

	if (isDash)
	{
		if (side == Sides::Left)
		{
			h = -1;
		}
		else if (side == Sides::Right)
		{
			h = 1;
		}
	}
	if (currentStatus == Status::WallJump)
	{
		velocity.x += h * speed * dt * 4.f;
	}
	else
	{
		velocity.x = h * speed * dt * 0.5;
	}

	velocity.x = Utils::Clamp(velocity.x, -100, 100);

	////////////////////////
	if (isShootingMode) // 실제 사격과 관계없이 총을 꺼내들고 있는 시간
	{
		ShootingModeTimer += dt;

		if (ShootingModeTimer > ShootingModeInterval)
		{
			isShootingMode = false;
			ShootingModeTimer = 0;
		}
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
	if (currentStatus != Status::Hit && currentStatus != Status::Die)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::X) && shootTimer > shootInterval)
		{
			//Shoot();
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
	}

	if (isGrounded)
	{
		velocity.y = 0;
	}

	if (!isGrounded && !isSlopeGrounded)
	{
		velocity.y += gravity * dt;
	}

	if (HP <= 0 && currentStatus != Status::Die)
	{
		playerAnimation.Play("animations/Die.csv");
		SetPlayerStatus(Status::Die);
	}
	damageTimer += dt;

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
	case Player::WallJump:
		UpdateWallJump(dt);
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
}

void Player::UpdateIdle(float dt)
{
	if (isShootingMode && isShooting)
	{
		playerAnimation.Play("animations/Shot.csv");
		isCantFlip = true;
		isShooting = false;
	} // 대기 -> 사격
	if (!isGrounded && !isBottomColliding && velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isJump = true;
		isGrounded = false;
		isSlopeGrounded = false;
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
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
	} // 대기 -> 달리기
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
	if (!isGrounded && !isBottomColliding && velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isJump = true;
		isGrounded = false;
		isSlopeGrounded = false;
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
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
	}// 달리기 -> 서기 
}
void Player::UpdateJumpingUp(float dt)
{
	isJump = false;
	isSlopeColliding = false;
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
	if (velocity.y >= 0)
	{
		isDash = false;
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // 점프 상승 중 하강
}
void Player::UpdateFallingDown(float dt)
{
	isCantFlip = false;
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/JumpDownShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // 점프 하강 중 사격
	if (isMiddleLeftColliding && h < 0)
	{
		speed = 200;
		isDash = false;
		isCantFlip = true;
		SetPlayerStatus(Status::Climbing);
		playerAnimation.Play("animations/Climbing.csv");
		return;
	}
	if(isMiddleRightColliding && h > 0)
	{
		speed = 200;
		isDash = false;
		isCantFlip = true;
		SetPlayerStatus(Status::Climbing);
		playerAnimation.Play("animations/Climbing.csv");
		return;
	} //하강 중 벽타기
	if (isGrounded)
	{
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
		return;
	} // 점프 하강 중 착지
}
void Player::UpdateLanding(float dt)
{
	speed = 200;
	velocity.x /= 10;
	isCantFlip = false;
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/LandingShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 착지 중 사격
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		isJump = true;
		isGrounded = false;
		isSlopeGrounded = false;
		isDash = false;
		if (isMiddleRightColliding)
		{
			position.x -= 1;
		}
		if (isMiddleLeftColliding)
		{
			position.x += 1;
		}
		velocity.y = -400.f;
		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;
	} // 착지 -> 점프
	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
		return;
	} // 착지 -> 대시
}
void Player::UpdateDash(float dt)
{
	speed = 400;
	isCantFlip = true;
	isDash = true;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isJump = true;
		isGrounded = false;
		isSlopeGrounded = false;
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
		isSlopeColliding = false;
		isCantFlip = false;
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
	isCantFlip = true;
	isDash = false;
	velocity.y = Utils::Clamp(velocity.y, 0.f, 100.f);
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/ClimbingShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 벽타기 -> 벽타기 사격
	if (!isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/Climbing.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // 벽타기 사격 -> 벽타기
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isJump = true;
		if (isMiddleRightColliding)
		{
			position.x -= 1.f;
			velocity.x = -100.f;
			velocity.y = -400.f;
			isMiddleRightColliding = false;
			SetPlayerStatus(Status::WallJump);
			playerAnimation.Play("animations/Walljump.csv");
			return;
		}
		if (isMiddleLeftColliding)
		{
			position.x += 1.f;
			velocity.x = 100.f;
			velocity.y = -400.f;
			isMiddleLeftColliding = false;
			SetPlayerStatus(Status::WallJump);
			playerAnimation.Play("animations/Walljump.csv");
			return;
		}
	} // 벽타기 -> 벽점프
	if (h < 1.f && isMiddleRightColliding)
	{
		position.x -= 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // 벽타기 -> 낙하
	if (h == 1.f && !isMiddleRightColliding)
	{
		position.x -= 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // 벽타기 -> 낙하
	if (h == -1.f && !isMiddleLeftColliding)
	{
		position.x -= 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // 벽타기 -> 낙하
	if (h > -1.f && isMiddleLeftColliding)
	{
		position.x += 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // 벽타기 -> 낙하
	if (isGrounded && velocity.y >= 0)
	{
		isCantFlip = false;
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
		return;
	} // 벽타기 -> 착지
}
void Player::UpdateWallJump(float dt)
{
	isJump = false;
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/WallJumpShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 벽점프 -> 벽점프 사격
	if (!isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/WallJump.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // 벽점프 사격 -> 벽점프
	if (side == Sides::Right)
	{
		if (h < 0.f)
		{
			isCantFlip = false;
			velocity.x = h * speed * dt;//속도 보정
			SetPlayerStatus(Status::JumpingUp);
			playerAnimation.Play("animations/JumpUp.csv");
			return;
		}
	}
	if (side == Sides::Left)
	{
		if (h > 0.f)
		{
			isCantFlip = false;
			velocity.x = h * speed * dt;//속도 보정
			SetPlayerStatus(Status::JumpingUp);
			playerAnimation.Play("animations/JumpUp.csv");
			return;
		}
	}
	if (isMiddleLeftColliding || isMiddleRightColliding)
	{
		SetPlayerStatus(Status::Climbing);
		velocity.x = h * speed * dt; // 속도 보정
		playerAnimation.Play("animations/Climbing.csv");
		return;
	}//벽점프 중 벽타기
	if (velocity.y >= 0)
	{
		isCantFlip = false;
		velocity.x = h * speed * dt;//속도 보정
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // 벽점프 중 하강
}
void Player::UpdateHit(float dt)
{
	isCantFlip = true;
	isDash = false;
	speed = 200;
	damageTimer = 0;
	if (side == Sides::Right)
	{
		h = -1;
	}
	if (side == Sides::Left)
	{
		h = 1;
	}
	if (velocity.y >= 0)
	{
		isJump = false;
	}
}
void Player::UpdateDie(float dt)
{
	isCantFlip = true;
	velocity.x = 0;
	velocity.y = 0;
	//sceneGame->SetStatus(SceneGame::Status::Pause);
}

void Player::OnDamage(int damage)
{
	
	if (side == Sides::Right)
	{
		velocity.x -= 40;
		velocity.y = -80;
		isJump = true;
		isGrounded = false;
	}
	if (side == Sides::Left)
	{
		velocity.x += 40;
		velocity.y = -80;
		isJump = true;
		isGrounded = false;
	}
	SetPlayerStatus(Status::Hit);
	HP -= damage;
	playerAnimation.Play("animations/Hit.csv");

}


void Player::LateUpdate(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Q) && currentStatus != Status::Die)
	{
		OnDamage(1);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		velocity.x = 0;
		velocity.y = 0;
		SetPosition({ 200,900 });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F2))
	{
		velocity.x = 0;
		velocity.y = 0;
		SetPosition({ 2440,680 });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F3))
	{
		velocity.x = 0;
		velocity.y = 0;
		SetPosition({ 4678,675 });
	}

	///  히트박스 테스트 코드

	if (InputMgr::GetKeyDown(sf::Keyboard::F8))
	{
		playerHitBox.setFillColor(sf::Color::Transparent);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F7))
	{
		playerHitBox.setFillColor(sf::Color::Blue);
	}
	/// 히트박스 테스트 코드

	if (velocity.y > 800.f)
	{
		velocity.y = 800.f;
	}

	if (isMiddleRightColliding)
	{
		velocity.x = std::min(0.f, velocity.x);
	}
	if (isMiddleLeftColliding)
	{
		velocity.x = std::max(0.f, velocity.x);
	}
	if (isTopColliding)
	{
		velocity.y = std::max(0.f, velocity.y);
	}

	if (currentStatus == Status::WallJump)
	{
		position += sf::Vector2f(velocity.x * dt, velocity.y * dt);
	}
	else
	{
		position += sf::Vector2f(velocity.x, velocity.y * dt);
	}

	boundingBoxWorldPos = playerBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);

	startX = std::max(0, static_cast<int>(boundingBoxLocalPos.x));
	endX = std::min(static_cast<int>(MapImage.getSize().x),
		static_cast<int>(boundingBoxLocalPos.x + 30));
	startY = std::max(0, static_cast<int>(boundingBoxLocalPos.y));
	endY = std::min(static_cast<int>(MapImage.getSize().y),
		static_cast<int>(boundingBoxLocalPos.y + 35));

		CheckTopCollision();
		CheckBottomCollision();
		CheckRightCollision();
		CheckLeftCollision();
		CheckSlopeCollision();


	if (isMiddleRightColliding) // 우측 보정
	{
		position.x -= std::max(rollBackMiddleHighRight, rollBackMiddleLowRight) - 1;
		rollBackMiddleHighRight = 0;
		rollBackMiddleLowRight = 0;
	}
	if (isMiddleLeftColliding) // 좌측 보정
	{
		position.x += std::max(rollBackMiddleHighLeft, rollBackMiddleLowLeft) - 1;
		rollBackMiddleHighLeft = 0;
		rollBackMiddleLowLeft = 0;
	}
	if (isTopColliding) // 상단 보정
	{
		position.y += std::max(rollBackTopLeft, rollBackTopRight - 1);
		rollBackTopLeft = 0;
		rollBackTopRight = 0;
	}
	if (isSlopeColliding && isGrounded && velocity.y >= 0.f) // 경사 보정
	{
 		position.y -= rollBackSlope - 1;
		position.y = std::floor(position.y);
		rollBackSlope = 0;
	}
	if (isBottomColliding && isGrounded) // 하단 보정
	{
		rollBackBottomLeft = std::max(rollBackBottomLeft, rollBackBottomCenter);
		position.y -= std::max(rollBackBottomLeft, rollBackBottomRight) - 1;
		rollBackBottomLeft = 0;
		rollBackBottomRight = 0;
		rollBackBottomCenter = 0;
	}



	playerHitBox.setPosition(position);
	playerBounds = playerHitBox.getGlobalBounds();

	SetPosition(position); // 중력을 포함한 위치 적용
	SetOrigin(Origins::BC);
}


void Player::FixedUpdate(float dt)
{
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
	BottomLeftPixel = MapImage.getPixel(startX + 1, endY);
	if (BottomLeftPixel == slopeCollisionColor)
	{
		isBottomLeftColliding = true;
		isBottomLeftSlopeColliding = true;

		int temp = 0;
		while (BottomLeftPixel == slopeCollisionColor)
		{
			temp++;
			BottomLeftPixel = MapImage.getPixel(startX + 1, endY - temp);
		}
		rollBackSideSlope = temp;

	}
	else
	{
		isBottomLeftSlopeColliding = false;
	}

	sf::Color BottomCenterPixel = MapImage.getPixel(startX + 15, endY);
	if (BottomCenterPixel == collisionColor)
	{
		isBottomCenterColliding = true;

		int temp = 0;
		while (BottomCenterPixel == collisionColor)
		{
			temp++;
			BottomCenterPixel = MapImage.getPixel(startX + 15, endY- temp);
		}
		rollBackBottomCenter = temp;
	}
	else
	{
		isBottomCenterColliding = false;
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

	BottomRightPixel = MapImage.getPixel(endX - 1, endY);
	if (BottomRightPixel == slopeCollisionColor)
	{
		isBottomRightColliding = true;
		isBottomRightSlopeColliding = true;
		int temp = 0;
		while (BottomRightPixel == slopeCollisionColor)
		{
			temp++;
			BottomRightPixel = MapImage.getPixel(endX - 1, endY - temp);
		}
		rollBackSideSlope = std::max(temp , rollBackSideSlope);
	}
	else
	{
		isBottomRightSlopeColliding = false;
	}

	isBottomColliding = ((isBottomLeftColliding || isBottomRightColliding) || isBottomCenterColliding);
	if(!isJump)
	isGrounded = isBottomColliding;
	isBottomSlopeColliding = (isBottomLeftSlopeColliding || isBottomRightSlopeColliding);
}
void Player::CheckRightCollision()
{
	sf::Color MiddleHighRightPixel = MapImage.getPixel(endX, startY + 12);
	sf::Color MiddleLowRightPixel = MapImage.getPixel(endX, startY + 24);
	if (MiddleHighRightPixel == sideCollisionColor)
	{
		isMiddleRightHighColliding = true;

		int temp = 0;
		while (MiddleHighRightPixel == sideCollisionColor)
		{
			temp++;
			MiddleHighRightPixel = MapImage.getPixel(endX - temp, startY + 12);
		}
		rollBackMiddleHighRight = temp;
		if (isJump)
		{
			isMiddleRightHighColliding = false;
		}
	}
	else
	{
		isMiddleRightHighColliding = false;
	}

	if (MiddleLowRightPixel == sideCollisionColor)
	{
		isMiddleRightLowColliding = true;

		int temp = 0;
		while (MiddleLowRightPixel == sideCollisionColor)
		{
			temp++;
			MiddleLowRightPixel = MapImage.getPixel(endX - temp, startY + 24);
		}
		rollBackMiddleLowRight = temp;
		if (isJump)
		{
			isMiddleRightLowColliding = false;
		}
	}
	else
	{
		isMiddleRightLowColliding = false;
	}

	isMiddleRightColliding = (isMiddleRightLowColliding || isMiddleRightHighColliding);

}
void Player::CheckLeftCollision()
{
	sf::Color MiddleHighLeftPixel = MapImage.getPixel(startX, startY + 12);
	sf::Color MiddleLowLeftPixel = MapImage.getPixel(startX, startY + 24);
	if (MiddleHighLeftPixel == sideCollisionColor)
	{
		isMiddleLeftHighColliding = true;

		int temp = 0;
		while (MiddleHighLeftPixel == sideCollisionColor)
		{
			temp++;
			MiddleHighLeftPixel = MapImage.getPixel(startX + temp, startY + 12);
		}
		rollBackMiddleHighLeft = temp;
		if (isJump)
		{
			isMiddleLeftHighColliding = false;
		}
	}
	else
	{
		isMiddleLeftHighColliding = false;
	}

	if (MiddleLowLeftPixel == sideCollisionColor)
	{
		isMiddleLeftLowColliding = true;

		int temp = 0;
		while (MiddleLowLeftPixel == sideCollisionColor)
		{
			temp++;
			MiddleLowLeftPixel = MapImage.getPixel(startX + temp, startY + 24);
		}
		rollBackMiddleLowLeft = temp;
		if (isJump)
		{
			isMiddleLeftLowColliding = false;
		}
	}
	else
	{
		isMiddleLeftLowColliding = false;
	}

	isMiddleLeftColliding = (isMiddleLeftLowColliding || isMiddleLeftHighColliding);
}
void Player::CheckTopCollision()
{

	sf::Color TopLeftPixel = MapImage.getPixel(startX + 1, startY);
	if (TopLeftPixel == collisionColor)
	{
		isTopLeftColliding = true;

		int temp = 0;
		while (TopLeftPixel == collisionColor)
		{
			temp++;
			TopLeftPixel = MapImage.getPixel(startX + 1, startY + temp);
		}
		rollBackTopLeft = temp;

	}
	else
	{
		isTopLeftColliding = false;
		rollBackTopLeft = 0;
	}

	sf::Color TopRightPixel = MapImage.getPixel(endX - 1, startY);
	if (TopRightPixel == collisionColor)
	{
		isTopRightColliding = true;
		int temp = 0;
		while (TopRightPixel == collisionColor)
		{
			temp++;
			TopRightPixel = MapImage.getPixel(endX - 1, startY + temp);
		}
		rollBackTopRight = temp;
	}
	else
	{
		isTopRightColliding = false;
		rollBackTopRight = 0;
	}

	sf::Color TopCenterPixel = MapImage.getPixel(startX + 15, startY);
	if (TopCenterPixel == collisionColor)
	{
		isTopCenterColliding = true;
		int temp = 0;
		while (TopCenterPixel == collisionColor)
		{
			temp++;
			TopCenterPixel = MapImage.getPixel(startX + 15, startY + temp);
		}
		rollBackTopCenter = temp;
	}
	else
	{
		isTopCenterColliding = false;
		rollBackTopCenter = 0;
	}

	isTopColliding = ((isTopLeftColliding || isTopRightColliding) || isTopCenterColliding);
}
void Player::CheckSlopeCollision()
{
	sf::Color SlopePixel = MapImage.getPixel(startX + 15, endY - 1);
	if (SlopePixel == slopeCollisionColor)
	{
		isSlopeColliding = true;
		int temp = 0;
		while (SlopePixel == slopeCollisionColor)
		{
			temp++;
			SlopePixel = MapImage.getPixel(startX + 15, endY - 1 - temp);
		}
		rollBackSlope = temp;
	}
	else
	{
		isSlopeColliding = false;
		rollBackSlope = 0;
	}
	isSlopeGrounded = isSlopeColliding;
}

void Player::NewCollisionMap()
{
	MapImage = sceneGame->collisionMapImage;
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