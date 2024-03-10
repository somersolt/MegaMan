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
	//���� �� ����ȭ

	playerAnimation.ClearEvent();
	playerEffectAnimation.ClearEvent();
	playerAnimation.Play("animations/Idle.csv");
	SetPlayerStatus(Status::Idle);
	SetOrigin(Origins::BC);
	SetPosition({ 200,500 });
	SetFlipX(false);
	side = Sides::Right;
	// �÷��̾� �ִϸ��̼� ����

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

	///  ��Ʈ�ڽ� �׽�Ʈ �ڵ�

	if (InputMgr::GetKeyDown(sf::Keyboard::T))
	{
		playerHitBox.setFillColor(sf::Color::Transparent);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		playerHitBox.setFillColor(sf::Color::Blue);
	}
	/// ��Ʈ�ڽ� �׽�Ʈ �ڵ�


	h = InputMgr::GetAxisRaw(Axis::Horizontal);

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
		velocity.x += h * speed * dt * 4;
	}
	else
	{
		velocity.x = h * speed * dt * 0.5;
	}

	velocity.x = Utils::Clamp(velocity.x, -100, 100);

	////////////////////////
	if (isShootingMode) // ���� ��ݰ� ������� ���� ������� �ִ� �ð�
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
	
	// ���
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
		sf::Color color(255, 255, 255, 255); // ���� ������ ���ƿ�
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


	if (InputMgr::GetKeyDown(sf::Keyboard::Z))
	{
		velocity.x = 0;
		velocity.y = 0;
		SetPosition({ 200,500 });
	}
}




void Player::UpdateIdle(float dt)
{
	if (isShootingMode && isShooting)
	{
		playerAnimation.Play("animations/Shot.csv");
		isCantFlip = true;
		isShooting = false;

	} // ��� -> ���

	if (!isGrounded && !isBottomColliding && velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;

	} // ��� -> ����

	if (h != 0.f)
	{
		isCantFlip = false;
		SetPlayerStatus(Status::Run);
		playerAnimation.Play("animations/Run.csv");
		return;
	}
	 // ��� -> �޸���


	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		isGrounded = false;
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
		return;

	} // ��� -> ���
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
	} // �޸��� -> ���

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

	} // �޸��� ��� -> �޸���

	if (!isGrounded && !isBottomColliding && velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;

	} // �޸��� -> ����

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
		return;

	} // �޸��� -> ���

	if (h == 0.f)
	{
		SetPlayerStatus(Status::Idle);
		playerAnimation.Play("animations/Idle.csv");
		return;
	}
	// �޸��� -> ���� 
}

void Player::UpdateJumpingUp(float dt)
{
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

	}//���� ��� �� ���
	//if ( h > 0 && isMiddleRightColliding)
	//{
	//	speed = 200;
	//	isDash = false;
	//	SetPlayerStatus(Status::Climbing);
	//	playerAnimation.Play("animations/Climbing.csv");
	//	return;
	//}//���� ��� �� ��Ÿ��
	//if (h < 0 && isMiddleLeftColliding)
	//{
	//	speed = 200;
	//	isDash = false;
	//	SetPlayerStatus(Status::Climbing);
	//	playerAnimation.Play("animations/Climbing.csv");
	//	return;
	//}//���� ��� �� ��Ÿ��

	if (velocity.y >= 0)
	{
		isDash = false;
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;

	} // ���� ��� �� �ϰ�
}

void Player::UpdateFallingDown(float dt)
{
	isCantFlip = false;
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/JumpDownShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // ���� �ϰ� �� ���

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
	} //�ϰ� �� ��Ÿ��

	if (isGrounded)
	{
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
		return;

	} // ���� �ϰ� �� ����


}

void Player::UpdateLanding(float dt)
{
	speed = 200;
	velocity.x /= 10;
	isCantFlip = false;
	if (isSlopeGrounded)
	{
		isGrounded = true;
	}
	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/LandingShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // ���� �� ���


	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{

		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
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

	} // ���� -> ����

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
		return;

	} // ���� -> ���
}

void Player::UpdateDash(float dt)
{

	speed = 400;
	isCantFlip = true;
	isDash = true;


	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
		isSlopeColliding = false;
		isCantFlip = false;
		SetPlayerStatus(Status::JumpingUp);
		playerAnimation.Play("animations/JumpUp.csv");
		return;

	} // ��� -> ����

	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/DashShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // ��� -> ��û��

}

void Player::UpdateClimbing(float dt)
{
	isCantFlip = true;
	isDash = false;
	velocity.y = Utils::Clamp(velocity.y, 0.f, 200.f);


	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/ClimbingShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // ��Ÿ�� -> ��Ÿ�� ���

	if (!isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/Climbing.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // ��Ÿ�� ��� -> ��Ÿ��


	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{

		if (isMiddleRightColliding)
		{
			position.x -= 1.f;
			velocity.x = -100.f;
			velocity.y = -400.f;
			SetPlayerStatus(Status::WallJump);
			playerAnimation.Play("animations/Walljump.csv");
			return;
		}

		if (isMiddleLeftColliding)
		{
			position.x += 1.f;
			velocity.x = 100.f;
			velocity.y = -400.f;
			SetPlayerStatus(Status::WallJump);
			playerAnimation.Play("animations/Walljump.csv");
			return;
		}

	} // ��Ÿ�� -> ������


	if (h < 1.f && isMiddleRightColliding)
	{
		position.x -= 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // ��Ÿ�� -> ����
	//////////////
	if (h == 1.f && !isMiddleRightColliding)
	{
		position.x -= 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // ��Ÿ�� -> ����
	if (h == -1.f && !isMiddleLeftColliding)
	{
		position.x -= 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // ��Ÿ�� -> ����
	//////////////
	if (h > -1.f && isMiddleLeftColliding)
	{
		position.x += 1.f;
		SetPlayerStatus(Status::FallingDown);
		sprite.setTextureRect({ 297,51,30,49 });
		playerAnimation.Play("animations/JumpDown.csv");
		return;
	} // ��Ÿ�� -> ����


	if (isGrounded && velocity.y >= 0)
	{
		isCantFlip = false;
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
		return;

	} // ��Ÿ�� -> ����
}


void Player::UpdateWallJump(float dt)
{


	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/WallJumpShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // ������ -> ������ ���

	if (!isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/WallJump.csv", true, true);
		playerAnimation.SetCurrentFrame(i);

	} // ������ ��� -> ������


	if (side == Sides::Right)
	{
		if (h < 0.f)
		{
			isCantFlip = false;
			velocity.x = h * speed * dt;//�ӵ� ����
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
			velocity.x = h * speed * dt;//�ӵ� ����
			SetPlayerStatus(Status::JumpingUp);
			playerAnimation.Play("animations/JumpUp.csv");
			return;
		}
	}

	if (isMiddleLeftColliding || isMiddleRightColliding)
	{
		SetPlayerStatus(Status::Climbing);
		velocity.x = h * speed * dt; // �ӵ� ����
		playerAnimation.Play("animations/Climbing.csv");
		return;
	}//������ �� ��Ÿ��

	if (velocity.y >= 0)
	{
		isCantFlip = false;

		velocity.x = h * speed * dt;//�ӵ� ����
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
		return;

	} // ������ �� �ϰ�


}


void Player::UpdateHit(float dt)
{
}

void Player::UpdateDie(float dt)
{
}


void Player::LateUpdate(float dt)
{
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
	playerHitBox.setPosition(position);
	playerBounds = playerHitBox.getGlobalBounds();

	boundingBoxWorldPos = playerBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToLocal(boundingBoxWorldPos);

	startX = std::max(0, static_cast<int>(boundingBoxLocalPos.x));
	endX = std::min(static_cast<int>(MapImage.getSize().x),
		static_cast<int>(boundingBoxLocalPos.x + 30));
	startY = std::max(0, static_cast<int>(boundingBoxLocalPos.y));
	endY = std::min(static_cast<int>(MapImage.getSize().y),
		static_cast<int>(boundingBoxLocalPos.y + 35));

	//if (startX > 0 && endX < 350 && startY > 0 && endY < 200)  // TO-DO ���� ������ǥ ũ����. ū������ ����� ũ�� *��������*
	{
		CheckTopCollision();
		if (!isSlopeColliding)
		{
			CheckBottomCollision();
		}
		CheckRightCollision();
		CheckLeftCollision();

		CheckSlopeCollision();
	}

	if (isMiddleRightColliding) // ���� ����
	{
		position.x -= rollBackMiddleRight - 1;
		rollBackMiddleRight = 0;
	}
	if (isMiddleLeftColliding) // ���� ����
	{
		position.x += rollBackMiddleLeft - 1;
		rollBackMiddleLeft = 0;
	}
	if (isTopColliding) // ��� ����
	{
		position.y += std::max(rollBackTopLeft, rollBackTopRight) - 1;
		rollBackTopLeft = 0;
		rollBackTopRight = 0;
	}

	if (isSlopeColliding && isGrounded ) // ��� ����
	{
 		position.y -= rollBackSlope - 1;
		position.y = std::floor(position.y);
		rollBackSlope = 0;
	}

	//if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	//{
	//	position.y -= rollBackSideSlope - 1;
	//}

	if (isBottomColliding && isGrounded) // �ϴ� ����
	{
		position.y -= std::max(rollBackBottomLeft, rollBackBottomRight) - 1;
		rollBackBottomLeft = 0;
		rollBackBottomRight = 0;
	}
	
	SetPosition(position); // �߷��� ������ ��ġ ����
	SetOrigin(Origins::BC);
}


void Player::FixedUpdate(float dt)
{
	if (!isGrounded)
	{
		velocity.y += gravity * dt;
	}
	if (isGrounded && !isSlopeGrounded)
	{
		velocity.y = 0;
	}
	if (!isGrounded && isSlopeGrounded)
	{
		velocity.y = 0;
		isGrounded = true;
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

	isBottomColliding = (isBottomLeftColliding || isBottomRightColliding);
	isGrounded = isBottomColliding;
	isBottomSlopeColliding = (isBottomLeftSlopeColliding || isBottomRightSlopeColliding);
}
void Player::CheckRightCollision()
{
	sf::Color MiddleRightPixel = MapImage.getPixel(endX, startY + 17);
	if (MiddleRightPixel == sideCollisionColor)
	{
		isMiddleRightColliding = true;

		int temp = 0;
		while (MiddleRightPixel == sideCollisionColor)
		{
			temp++;
			MiddleRightPixel = MapImage.getPixel(endX - temp, endY);
		}
		rollBackMiddleRight = temp;

		return;
	}
	isMiddleRightColliding = false;
}
void Player::CheckLeftCollision()
{
	sf::Color MiddleLeftPixel = MapImage.getPixel(startX, startY + 17);
	if (MiddleLeftPixel == sideCollisionColor)
	{
		isMiddleLeftColliding = true;

		int temp = 0;
		while (MiddleLeftPixel == sideCollisionColor)
		{
			temp++;
			MiddleLeftPixel = MapImage.getPixel(startX + temp, endY);
		}
		rollBackMiddleLeft = temp;

		return;
	}
	isMiddleLeftColliding = false;
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
	isTopColliding = (isTopLeftColliding || isTopRightColliding);
}

void Player::CheckSlopeCollision()
{
	sf::Color SlopePixel = MapImage.getPixel(startX + 15, endY);
	if (SlopePixel == slopeCollisionColor)
	{
		isSlopeColliding = true;
		int temp = 0;
		while (SlopePixel == slopeCollisionColor)
		{
			temp++;
			SlopePixel = MapImage.getPixel(startX + 15, endY - temp);
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

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	if (chargeEffectMode)
	{
		window.draw(effect);
	}
	window.draw(playerHitBox);

}