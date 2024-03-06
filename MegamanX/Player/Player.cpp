#include "pch.h"
#include "Player.h"



Player::Player(const std::string& name) : SpriteGo(name)
{
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
	SetOrigin(Origins::BC);
	SetPosition({ 0.f, 0.f });
	SetFlipX(false);
	isGrounded = true;

	std::function<void()> ToIdle = std::bind(&Player::IdleAnimation, this);
	playerAnimation.AddEvent("animations/Landing.csv", 1, ToIdle);
	playerAnimation.AddEvent("animations/LandingShot.csv", 1, ToIdle);
	playerAnimation.AddEvent("animations/Dash.csv", 3, ToIdle);
	playerAnimation.AddEvent("animations/DashShot.csv", 3, ToIdle);
	playerAnimation.AddEvent("animations/Shot.csv", 3, ToIdle);

}

void Player::IdleAnimation()
{
	playerAnimation.Play("animations/Idle.csv");
	speed = 200;
	isDash = false;
	isCantFlip = false;
	isShooting = false;
}



void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	playerAnimation.Update(dt);
	h = InputMgr::GetAxisRaw(Axis::Horizontal);
	
	if (!isDash)
	{
		velocity.x = h * speed * dt;
	}

	shootTimer += dt;

	if (InputMgr::GetKeyDown(sf::Keyboard::X))
	{
		shootTimer = 0;
		isShooting = true;
	}
	if (shootTimer > 1.f)
	{
		shootTimer = 0;
		isShooting = false;
	}


	if (h != 0.f && !isCantFlip)
	{
		SetFlipX(h < 0);
	}

	if (playerAnimation.GetCurrentClipId() == "animations/Idle.csv")
	{
		if (h != 0.f)
		{
			playerAnimation.Play("animations/Run.csv");
		}
	} // ������ �� -> �޸���
	else if (playerAnimation.GetCurrentClipId() == "animations/Run.csv")
	{
		if (h == 0.f)
		{
			playerAnimation.Play("animations/Idle.csv");
		}
	} // �޸��� �� -> ���� 


	if (playerAnimation.GetCurrentClipId() == "animations/Idle.csv" && InputMgr::GetKeyDown(sf::Keyboard::C))
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
		playerAnimation.Play("animations/Dash.csv");
	} // ������ �� -> ���
	else if (playerAnimation.GetCurrentClipId() == "animations/Run.csv" && InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		if (!isFlipX)
		{
			velocity.x = 1;
		}
		else if (isFlipX)
		{
			velocity.x = -1;
		}
		speed = 400;
		isCantFlip = true;
		isDash = true;
		playerAnimation.Play("animations/Dash.csv");
	} // �޸��� �� -> ���


	if (playerAnimation.GetCurrentClipId() == "animations/Idle.csv" && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		isCantFlip = false;

		playerAnimation.Play("animations/JumpUp.csv");
		velocity.y = -600.f;
	} // ����

	else if (velocity.y > 0 && playerAnimation.GetCurrentClipId() == "animations/JumpUp.csv")
	{
		if(!isJumping)
		playerAnimation.Play("animations/JumpDown.csv");
		isJumping = true;
	} // ���� ��� �� �ϰ�

	if (velocity.y > 0 && playerAnimation.GetCurrentClipId() == "animations/JumpUpShot.csv")
	{
		if (!isJumping)
			playerAnimation.Play("animations/JumpDownShot.csv");
		isJumping = true;
	} // ���� ��� ��� �� �ϰ�

	if (playerAnimation.GetCurrentClipId() == "animations/JumpDown.csv" && isGrounded)
	{
		playerAnimation.Play("animations/Landing.csv");
		isJumping = false;
		isCantFlip = true;
	} // ���� �ϰ� �� ����

	if (playerAnimation.GetCurrentClipId() == "animations/JumpDownShot.csv" && isGrounded)
	{
		playerAnimation.Play("animations/LandingShot.csv");
		isJumping = false;
		isCantFlip = true;
	} // ���� �ϰ� ��� �� ����


	if (playerAnimation.GetCurrentClipId() == "animations/Idle.csv" && isShooting)
	{
			playerAnimation.Play("animations/Shot.csv");
			isCantFlip = true;
	} // ������ �� ���

	if (playerAnimation.GetCurrentClipId() == "animations/Shot.csv" && InputMgr::GetKeyDown(sf::Keyboard::X))
	{
		shootTimer = 0;
		isShooting = true;
		playerAnimation.Play("animations/Shot.csv");
		isCantFlip = true;
	}// ����


	if (playerAnimation.GetCurrentClipId() == "animations/Shot.csv" && h != 0.f)
	{
		playerAnimation.Play("animations/RunShot.csv");
	}// ��� -> �޸��� ���

	if (playerAnimation.GetCurrentClipId() == "animations/Shot.csv" && InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		if (!isFlipX)
		{
			velocity.x = 1;
		}
		else if (isFlipX)
		{
			velocity.x = -1;
		}
		speed = 400;
		isCantFlip = true;
		isDash = true;
		playerAnimation.Play("animations/DashShot.csv");
	}// ��� -> ��� ���

	if (playerAnimation.GetCurrentClipId() == "animations/Shot.csv" && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		isCantFlip = false;
		isShooting = true;

		playerAnimation.Play("animations/JumpUpShot.csv");
		velocity.y = -600.f;
	}// �������



	if (playerAnimation.GetCurrentClipId() == "animations/Run.csv" && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		isCantFlip = false;

		playerAnimation.Play("animations/JumpUp.csv");
		velocity.y = -600.f;
	} // �޸��� -> ����



	if (playerAnimation.GetCurrentClipId() == "animations/Run.csv" && isShooting)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		if (i == 9)
		{
			i = 0;
		}
		playerAnimation.Play("animations/RunShot.csv",true,true);
		playerAnimation.SetCurrentFrame(i);

	} // �޸��� -> �޸��� ���


	else if (playerAnimation.GetCurrentClipId() == "animations/RunShot.csv")
	{
		isCantFlip = false;
		if (h == 0.f)
		{
			playerAnimation.Play("animations/Idle.csv");
		}
	} // �޸��� ��� �� -> ���� 

	if (playerAnimation.GetCurrentClipId() == "animations/RunShot.csv" && !isShooting)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		if (i == 9)
		{
			i = 0;
		}
		playerAnimation.Play("animations/Run.csv", true, true);
		playerAnimation.SetCurrentFrame(i);


	} // �޸��� ��� �� -> �޸���


	else if (playerAnimation.GetCurrentClipId() == "animations/RunShot.csv" && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		isCantFlip = false;
		isShooting = true;

		playerAnimation.Play("animations/JumpUpShot.csv");
		velocity.y = -600.f;
	} // �޸��� ��� �� -> ����

	else if (playerAnimation.GetCurrentClipId() == "animations/RunShot.csv" && InputMgr::GetKeyDown(sf::Keyboard::C))
	{
		if (!isFlipX)
		{
			velocity.x = 1;
		}
		else if (isFlipX)
		{
			velocity.x = -1;
		}
		speed = 400;
		isDash = true;
		isCantFlip = true;
		playerAnimation.Play("animations/DashShot.csv");
	} // �޸��� ��� �� -> ���


	if (playerAnimation.GetCurrentClipId() == "animations/JumpUp.csv" && isShooting)
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

	if (playerAnimation.GetCurrentClipId() == "animations/JumpDown.csv" && isShooting)
	{
		isCantFlip = false;
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/JumpDownShot.csv",true,true);
		playerAnimation.SetCurrentFrame(i);
		
	} // ���� �ϰ� �� ���



	if (playerAnimation.GetCurrentClipId() == "animations/Dash.csv" && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		isCantFlip = false;

		playerAnimation.Play("animations/JumpUp.csv");
		isDash = false;
		velocity.y = -600.f;
	} // ��� -> ����

	if (playerAnimation.GetCurrentClipId() == "animations/Dash.csv" && InputMgr::GetKeyDown(sf::Keyboard::X))
	{
		int i = playerAnimation.GetCurrentFrame();
		playerAnimation.Play("animations/DashShot.csv", true, true);
		playerAnimation.SetCurrentFrame(i);
	} // ��� -> ��û��

	if (playerAnimation.GetCurrentClipId() == "animations/DashShot.csv" && InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		isCantFlip = false;

		playerAnimation.Play("animations/JumpUp.csv");
		isDash = false;
		velocity.y = -600.f;
	} // ��û�� -> ����




	velocity.y += gravity * dt; // �߷� ����

	position += sf::Vector2f(velocity.x, velocity.y * dt);
	SetPosition(position); // �߷��� ������ ��ġ ����

	if (position.y > 0.f)
	{
		isGrounded = true;
		position.y = 0.f;
		velocity.y = 0.f;

		// ���� ������, 0�� �ƴ϶� �� �浹�� ���� 
	}
	SetOrigin(Origins::BC);

	std::cout << velocity.x << std::endl;
}

