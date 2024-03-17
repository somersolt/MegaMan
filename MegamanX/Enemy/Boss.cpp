#include "pch.h"
#include "Boss.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "EnemyShot.h"
#include "BossShot.h"
#include "Player/Player.h"
#include "IceStatue.h"

Boss::Boss(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Boss::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/boss/boss18.png";
}

void Boss::Reset()
{
	SpriteGo::Reset();
	SetTexture(textureId);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	bossCurrentStatus = BossStatus::Appear;
	SetOrigin(Origins::BC);
	speed = 0;
	Hp = 32;

	EnemyHitBox.setSize({ 38.f, 36.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);


	std::function<void()> ToShot = std::bind(&Boss::Shot, this);
	std::function<void()> ToBreath = std::bind(&Boss::IceBreath, this);
	std::function<void()> ToIdle = std::bind(&Boss::ToIdle, this);

	EnemyAnimation.AddEvent("animations/boss/Shot.csv", 3,
		ToShot);
	EnemyAnimation.AddEvent("animations/boss/Shot.csv", 9,
		ToShot);
	EnemyAnimation.AddEvent("animations/boss/Shot.csv", 15,
		ToShot);
	EnemyAnimation.AddEvent("animations/boss/Shot.csv", 16,
		ToIdle);
	EnemyAnimation.AddEvent("animations/boss/Appear.csv", 17,
		[=]() {sceneGame->SetAppearAnimation(true); });
	EnemyAnimation.AddEvent("animations/boss/Sliding.csv", 5,
		[=]() {speed = 500; });
	EnemyAnimation.AddEvent("animations/boss/Sliding.csv", 13,
		[=]() {speed = 200; });
	EnemyAnimation.AddEvent("animations/boss/Sliding.csv", 16,
		[=]() {speed = 50; });
	EnemyAnimation.AddEvent("animations/boss/Sliding.csv", 18,
		ToIdle);
	for (int i = 0; i < 12; i++)
	{
		EnemyAnimation.AddEvent("animations/boss/Breath.csv", 4 + i * 2,
			ToBreath);
	}
	EnemyAnimation.AddEvent("animations/boss/Breath.csv", 28,
		ToIdle);
	EnemyAnimation.AddEvent("animations/boss/Hang.csv", 9, [=]() {
		gravity = 1000;
		EnemyAnimation.Stop();
		SetBossStatus(BossStatus::FallingDown);
		});
}

void Boss::Update(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::P))
	{
		SummonIceStatue();
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::O))
	{
		Hang();
		sceneGame->BlizzardEffect();
	}
	SpriteGo::Update(dt);
	skillTimer += dt;
	hitTimer += dt;
	if (bossCurrentStatus != BossStatus::Sliding && bossCurrentStatus != BossStatus::JumpingUp && bossCurrentStatus != BossStatus::FallingDown
		&& bossCurrentStatus != BossStatus::Hanging
		) {
		if (playerPos.x - position.x > 0)
		{
			h = 1;
		}
		else
		{
			h = -1;
		}
	}

	if (h == -1)
	{
		side = Sides::Left;
	}
	if (h == 1)
	{
		side = Sides::Right;
	}

	skillTimer += dt;
	playerPos = player->GetPosition();

	switch (bossCurrentStatus)
	{
	case Boss::Appear:
		UpdateAppear(dt);
		break;
	case Boss::Idle:
		UpdateIdle(dt);
		break;
	case Boss::JumpingUp:
		UpdateJumpingUp(dt);
		break;
	case Boss::FallingDown:
		UpdateFallingDown(dt);
		break;
	case Boss::Sliding:
		UpdateSliding(dt);
		break;
	case Boss::Breath:
		UpdateBreath(dt);
		break;
	case Boss::Hanging:
		UpdateHanging(dt);
		break;
	case Boss::Hit:
		UpdateHit(dt);
		break;
	case Boss::Die:
		UpdateDie(dt);
		break;
	default:
		break;
	}
}

void Boss::UpdateAppear(float dt)
{
	player->SetWait(true);
	if (isGrounded && !isappear)
	{
		EnemyAnimation.Play("animations/boss/Appear.csv");
		isappear = true;
	}
}

void Boss::UpdateIdle(float dt)
{
	isCantFlip = false;
	wind = false;
	int skill = -1;
	if (skillTimer > 2 && !onSkill)
	{
		skill = Utils::RandomRange(0, 12);
	}

	switch (skill)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		EnemyAnimation.Play("animations/boss/Shot.csv");
		skillTimer = 0;
		break;
	case 4:
	case 5:
		Jump();
		skillTimer = 0;
		break;
	case 6:
		ToIdle();
		skillTimer = 0;
		break;
	case 7:
	case 8:

		EnemyAnimation.Play("animations/boss/Sliding.csv");
		skillTimer = 0;
		onSkill = true;
		SetBossStatus(BossStatus::Sliding);
		break;
	case 9:
	case 10:
		EnemyAnimation.Play("animations/boss/Breath.csv");
		skillTimer = 0;
		onSkill = true;
		SummonIceStatue();
		SetBossStatus(BossStatus::Breath);
		break;
	case 11:
		Hang();
		break;
	default:
		break;
	}
}

void Boss::UpdateJumpingUp(float dt)
{
	isJump = false;
	isSlopeColliding = false;
	if (velocity.y >= 0)
	{
		SetBossStatus(BossStatus::FallingDown);
		SetTexture("graphics/boss/boss18.png");
		return;
	} // 점프 상승 중 하강
}
void Boss::UpdateFallingDown(float dt)
{
	SetTexture("graphics/boss/boss18.png", true);
	if (isGrounded)
	{
		ToIdle();
		speed = 0;
	}
}
void Boss::UpdateSliding(float dt)
{
	isCantFlip = true;
	isSuperArmor = true;

	if (isMiddleLeftColliding)
	{
		side = Sides::Right;
		SetFlipX(true);
		position.x += 2.f;
		h = 1;
	}
	if (isMiddleRightColliding)
	{
		side = Sides::Left;
		SetFlipX(false);
		position.x -= 2.f;
		h = -1;
	}
}
void Boss::UpdateBreath(float dt)
{
	isSuperArmor = true;
}
void Boss::UpdateHanging(float dt)
{
	if (7823 - position.x > 0)
	{
		h = 1;
	}
	if (7823 - position.x == 0)
	{
		h = 0;
	}
	if (7823 - position.x < 0)
	{
		h = -1;
	}
	isJump = false;
	isSlopeColliding = false;
	if (Utils::Distance({ 7823,85 }, position) < 20 && !hanged)
	{
		SetPosition({ 7823, 85 });
		hanged = true;
		wind = true;
		velocity.y = 0;
		gravity = 0;
		player->SetWind(true);
		sceneGame->BlizzardEffect();
		EnemyAnimation.Play("animations/boss/Hang.csv");
		return;
	}

	if (velocity.y >= 0 && !hanged)
	{
		EnemyAnimation.Stop();
		SetTexture("graphics/boss/boss18.png", true);
		if (isGrounded && skillTimer > 2)
		{
			ToIdle();
			return;
		}
	}

}

void Boss::UpdateHit(float dt)
{
	sprite.setColor({ 255,255,255,150 });
	speed = 0;

	if (velocity.y >= 0)
	{
		isJump = false;
	}

	if (hitTimer > 0.7f)
	{
		ToIdle();
	}
}

void Boss::UpdateDie(float dt)
{
	player->SetWait(true);
	speed = 0;
	sprite.setColor({ 255,255,255,255 });
	EnemyAnimation.Stop();
	SetTexture("graphics/boss/boss19.png", true);
	boomTimer += dt;
	dieTimer += dt;
	if (boomTimer > 0.3f && dieTimer < 5.f)
	{
		boomTimer = 0;
		SpriteGoEffect* boom = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom);
		boom->SetOrigin({ EnemyHitBox.getGlobalBounds().width / 2, EnemyHitBox.getGlobalBounds().height });
		sf::Vector2f randomPosition = position + Utils::RandomInUnitCircle() * 30.f;
		boom->SetPosition(randomPosition);
		boom->Init();
		boom->Reset();
	}
	if (dieTimer > 5.f)
	{
		SetActive(false);
		player->SetPlayerStatus(Player::Status::Victory);
	}
}

void Boss::OnDamage(int damage)
{
	if (hitTimer < 1.f || bossCurrentStatus == BossStatus::Die)
	{
		return;
	}

	Hp -= damage;
	hitTimer = 0;

	if (damage < 3 || isSuperArmor)
	{
		return;
	}

	gravity = 1000;
	if (side == Sides::Right)
	{
		velocity.x = -40;
		velocity.y = -80;
		if (!isSlopeColliding)
			isJump = true;
		isGrounded = false;
	}
	if (side == Sides::Left)
	{
		velocity.x = 40;
		velocity.y = -80;
		if (!isSlopeColliding)
			isJump = true;
		isGrounded = false;
	}
	SetBossStatus(BossStatus::Hit);
	EnemyAnimation.Stop();
	SetTexture("graphics/boss/boss19.png", true);
}

void Boss::LateUpdate(float dt)
{
	SpriteGo::LateUpdate(dt);
	EnemyAnimation.Update(dt);

	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	if (!isCantFlip)
		SetFlipX(h > 0 ? true : false);

	velocity.x = h * speed * dt * 0.5;

	if (bossCurrentStatus == BossStatus::Hit)
	{
		float pushForce = 40 * exp(-5 * hitTimer);
		velocity.x -= h * pushForce * dt;
	}
	if (bossCurrentStatus == BossStatus::Hanging)
	{
		float distanceToHook = 7823 - position.x;
		float reductionFactor = exp(-0.1f * std::abs(distanceToHook));
		velocity.x -= h * reductionFactor * dt;
	}

	velocity.x = Utils::Clamp(velocity.x, -100, 100);
	if (isGrounded)
	{
		velocity.y = 0;
	}

	if (!isGrounded && !isSlopeGrounded)
	{
		velocity.y += gravity * dt;
	}
	if (Hp <= 0)
	{
		sceneGame->EnemyClear();
		SetBossStatus(BossStatus::Die);
	}

	if (velocity.y > 800.f)
	{
		velocity.y = 800.f;
	}
	if (velocity.y > 0.f)
	{
		isJump = false;
	}

	if (isMiddleRightColliding)
	{
		velocity.x = std::min(0.f, velocity.x);
	}
	if (isMiddleLeftColliding)
	{
		velocity.x = std::max(0.f, velocity.x);
	}

	position += sf::Vector2f(velocity.x, velocity.y * dt);

	boundingBoxWorldPos = EnemyBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);

	startX = std::max(0, static_cast<int>(boundingBoxLocalPos.x));
	endX = std::min(static_cast<int>(MapImage.getSize().x),
		static_cast<int>(boundingBoxLocalPos.x + EnemyHitBox.getSize().x));
	startY = std::max(0, static_cast<int>(boundingBoxLocalPos.y));
	endY = std::min(static_cast<int>(MapImage.getSize().y),
		static_cast<int>(boundingBoxLocalPos.y + EnemyHitBox.getSize().y));


	CheckRightCollision();
	CheckLeftCollision();
	CheckBottomCollision();
	CheckSlopeCollision();


	if (isMiddleRightColliding) // 우측 보정
	{
		position.x -= rollBackMiddleRight - 1;
		rollBackMiddleRight = 0;
	}
	if (isMiddleLeftColliding) // 좌측 보정
	{
		position.x += rollBackMiddleLeft - 1;
		rollBackMiddleLeft = 0;
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


	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	SetPosition(position); // 중력을 포함한 위치 적용
	SetOrigin(Origins::BC);
}

void Boss::FixedUpdate(float dt)
{
	if (GetGlobalBounds().intersects(player->GetPlayerBounds()) && player->GetDamageTimer() < 0.f && player->GetCurrentStatus() != Player::Status::Die && bossCurrentStatus != BossStatus::Die)
	{
		player->SetDamageTimer(0);
		player->OnDamage(1);
	}
}

void Boss::ToIdle()
{
	speed = 0;
	isSuperArmor = false;
	hanged = false;
	onSkill = false;
	wind = false;
	player->SetWind(false);
	sprite.setColor({ 255,255,255,255 });
	SetBossStatus(BossStatus::Idle);
	EnemyAnimation.Play("animations/boss/Idle.csv");
}

void Boss::Shot()
{
	isCantFlip = true;
	onSkill = true;
	BossShot* iceShot = new BossShot("enemy", sceneGame->collisionMapImage);
	iceShot->Init();
	if (side == Sides::Left)
	{
		iceShot->Reset(-1);
	}
	if (side == Sides::Right)
	{
		iceShot->Reset(1);
	}
	iceShot->SetOrigin(Origins::BC);
	iceShot->SetPosition({ position.x, position.y - 20 });
	sceneGame->AddGo(iceShot);

}

void Boss::Jump()
{
	onSkill = true;
	isCantFlip = true;
	speed = 300;
	isJump = true;
	isGrounded = false;
	isSlopeGrounded = false;
	position.y -= rollBackSideSlope + 3;
	rollBackSideSlope = 0;
	position.y -= 2;
	velocity.y = -450.f;
	SetBossStatus(BossStatus::JumpingUp);
	EnemyAnimation.Play("animations/boss/JumpUp.csv");
	return;
}

void Boss::IceBreath()
{
	isSuperArmor = true;
	onSkill = true;
	EnemyShot* iceBreat = new EnemyShot("iceBreat");
	iceBreat->Init("animations/boss/BreathBullet.csv", "graphics/boss/breathe.png");
	iceBreat->Reset();

	if (side == Sides::Left)
	{
		iceBreat->EnemyFire({ -1,0 }, 100, 1, -1);
		iceBreat->SetPosition({ position.x - 10, position.y - 20 });
	}
	if (side == Sides::Right)
	{
		iceBreat->EnemyFire({ 1,0 }, 100, 1, -1);
		iceBreat->SetPosition({ position.x + 10, position.y - 20 });
	}
	sceneGame->AddGo(iceBreat);
}

void Boss::SummonIceStatue()
{
	IceStatue* iceStatue = new IceStatue("enemy", sceneGame->collisionMapImage);
	iceStatue->Init();
	if (side == Sides::Left)
	{
		iceStatue->Reset(-1);
	}
	if (side == Sides::Right)
	{
		iceStatue->Reset(1);
	}
	iceStatue->SetOrigin(Origins::BC);
	if (side == Sides::Left)
	{
		iceStatue->SetPosition({ position.x - 30 , position.y - 30 });
	}
	if (side == Sides::Right)
	{
		iceStatue->SetPosition({ position.x + 30 , position.y - 30 });
	}
	sceneGame->AddGo(iceStatue);

	IceStatue* iceStatue2 = new IceStatue("enemy", sceneGame->collisionMapImage);
	iceStatue2->Init();
	if (side == Sides::Left)
	{
		iceStatue2->Reset(-1);
	}
	if (side == Sides::Right)
	{
		iceStatue2->Reset(1);
	}
	iceStatue2->SetOrigin(Origins::BC);
	if (side == Sides::Left)
	{
		iceStatue2->SetPosition({ position.x - 60 , position.y - 30 });
	}
	if (side == Sides::Right)
	{
		iceStatue2->SetPosition({ position.x + 60 , position.y - 30 });
	}
	sceneGame->AddGo(iceStatue2);


}

void Boss::Hang()
{
	onSkill = true;
	skillTimer = 0;
	isCantFlip = true;
	speed = 400;
	isJump = true;
	isGrounded = false;
	isSlopeGrounded = false;
	position.y -= rollBackSideSlope + 3;
	rollBackSideSlope = 0;
	position.y -= 2;
	velocity.y = -500;
	if (playerPos.x - position.x < 0)
	{
		windSide = Sides::Left;
	}
	if (playerPos.x - position.x > 0)
	{
		windSide = Sides::Right;
	}
	player->SetWindSide(windSide);
	SetBossStatus(BossStatus::Hanging);
	EnemyAnimation.Play("animations/boss/JumpUp.csv");
	return;
}
