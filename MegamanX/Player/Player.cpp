#include "pch.h"
#include "Player.h"



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
	//TO-DO ����ü ����
	isShooting = false;
}

void Player::ChargeShoot()
{
	//TO-DO ������ ���� ����ü ����
	isShooting = false;
}

void Player::Init()
{
	SpriteGo::Init();
	playerAnimation.SetTarget(&sprite);

}

void Player::Reset()
{
	playerAnimation.ClearEvent();

	playerAnimation.Play("animations/Idle.csv");
	SetPlayerStatus(Status::Idle);
	SetOrigin(Origins::BC);
	SetPosition({ 0.f, 0.f });
	SetFlipX(false);

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
	h = InputMgr::GetAxisRaw(Axis::Horizontal);
	shootTimer += dt;

	if (isShootingMode) // ���� ��ݰ� ������� ���� ������� �ִ� �ð�
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
	}
	if (!isDash)
	{
		velocity.x = h * speed * dt;
	}
	if (!isCharge)
	{
		chargeTimer = 0;
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
		Shoot();
		shootTimer = 0;
		chargeTimer = 0;
		isShooting = true;
		isShootingMode = true;
	}
	if (InputMgr::GetKey(sf::Keyboard::X))
	{
		isCharge = true;
		chargeTimer += dt;
	}

	if (InputMgr::GetKeyUp(sf::Keyboard::X) && shootTimer > shootInterval)
	{
		ChargeShoot();
		isCharge = false;
		shootTimer = 0;
		isShooting = true;
		isShootingMode = true;
		sf::Color color(255, 255, 255, 255); // ���� ������ ���ƿ�
		sprite.setColor(color);
	}
	//

	

	velocity.y += gravity * dt; // �߷� ����

	position += sf::Vector2f(velocity.x, velocity.y * dt);
	SetPosition(position); // �߷��� ������ ��ġ ����

	if (position.y > 0.f)
	{
		position.y = 0.f;
		velocity.y = 0.f;

		// ���� ������, 0�� �ƴ϶� �� �浹�� ���� 
	}
	SetOrigin(Origins::BC);

	//std::cout << velocity.x << std::endl; // �׽�Ʈ �ڵ�
}




void Player::UpdateIdle(float dt)
{
	if (isShootingMode && isShooting)
	{
		playerAnimation.Play("animations/Shot.csv");
		isCantFlip = true;
		isShooting = false;
	} // ��� -> ���

	if (h != 0.f)
	{
		isCantFlip = false;
		SetPlayerStatus(Status::Run);
		playerAnimation.Play("animations/Run.csv");
	}
	 // ��� -> �޸���

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetPlayerStatus(Status::JumpingUp);
		velocity.y = -600.f;
		playerAnimation.Play("animations/JumpUp.csv");
	} // ��� -> ����

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
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

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SetPlayerStatus(Status::JumpingUp);
		velocity.y = -600.f;
		playerAnimation.Play("animations/JumpUp.csv");
	} // �޸��� -> ����

	if (InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		SetPlayerStatus(Status::Dash);
		playerAnimation.Play("animations/Dash.csv");
	} // �޸��� -> ���

	if (h == 0.f)
	{
		SetPlayerStatus(Status::Idle);
		playerAnimation.Play("animations/Idle.csv");
	}
	// �޸��� -> ���� 
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
	}//���� ��� �� ���

	if (velocity.y > 0)
	{
		SetPlayerStatus(Status::FallingDown);
		playerAnimation.Play("animations/JumpDown.csv");
	} // ���� ��� �� �ϰ�
}

void Player::UpdateFallingDown(float dt)
{

	if (isShootingMode)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/JumpDownShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // ���� �ϰ� �� ���

	if (position.y >= 0.f)  // TO-DO �浹�������� �ٲٱ�
	{
		SetPlayerStatus(Status::Landing);
		playerAnimation.Play("animations/Landing.csv");
	} // ���� �ϰ� �� ����
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
	} // ���� �ϰ� �� ���
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
	} // ��� -> ����

	if (isShootingMode)
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/DashShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // ��� -> ��û��

}

void Player::UpdateHit(float dt)
{
}

void Player::UpdateDie(float dt)
{
}

