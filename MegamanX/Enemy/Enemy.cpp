#include "pch.h"
#include "Enemy.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "EnemyShot.h"
#include "Player/Player.h"

Enemy::Enemy(const std::string& name, sf::Image& mapImage) : SpriteGo(name), MapImage(mapImage)
{
}

void Enemy::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/rabbit1.png";
}

void Enemy::Reset()
{

	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();
	isGrounded = true;
	SetOrigin(Origins::BC);

	EnemyHitBox.setSize({ 27.f, 31.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
	// 애니메이션 세팅

}


void Enemy::OnDamage(int damage)
{
	Hp -= damage;
}

void Enemy::LateUpdate(float dt)
{
	if (Utils::Distance(player->GetPosition(), position) < 200)
	{
		isFight = true;
	}
	if (!isFight)
	{
		return;
	}
	SpriteGo::LateUpdate(dt);
	EnemyAnimation.Update(dt);

	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	if (!isCantFlip)
		SetFlipX(h > 0 ? true : false);

	velocity.x = h * speed * dt * 0.5;
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
		SpriteGoEffect* boom = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom);
		boom->SetOrigin({ EnemyHitBox.getGlobalBounds().width / 2, EnemyHitBox.getGlobalBounds().height });
		boom->SetPosition(position);
		boom->Init();
		boom->Reset();
		Release();
		sceneGame->RemoveGo(this);
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


void Enemy::FixedUpdate(float dt)
{

	if (GetGlobalBounds().intersects(player->GetPlayerBounds()) && player->GetDamageTimer() < 0.f && player->GetCurrentStatus() != Player::Status::Die)
	{
		player->SetDamageTimer(0);
		player->OnDamage(1);
	}
}

void Enemy::CheckRightCollision()
{
	sf::Color MiddleRightPixel = MapImage.getPixel(endX, startY + (EnemyHitBox.getSize().y / 2));
	if (MiddleRightPixel == sideCollisionColor)
	{
		isMiddleRightColliding = true;

		int temp = 0;
		while (MiddleRightPixel == sideCollisionColor)
		{
			temp++;
			MiddleRightPixel = MapImage.getPixel(endX - temp, startY + (EnemyHitBox.getSize().y / 2));
		}
		rollBackMiddleRight = temp;
		if (isJump)
		{
			isMiddleRightColliding = false;
		}
	}
	else
	{
		isMiddleRightColliding = false;
	}
}
void Enemy::CheckLeftCollision()
{
	sf::Color MiddleLeftPixel = MapImage.getPixel(startX, startY + (EnemyHitBox.getSize().y / 2));
	if (MiddleLeftPixel == sideCollisionColor)
	{
		isMiddleLeftColliding = true;

		int temp = 0;
		while (MiddleLeftPixel == sideCollisionColor)
		{
			temp++;
			MiddleLeftPixel = MapImage.getPixel(startX + temp, startY + (EnemyHitBox.getSize().y / 2));
		}
		rollBackMiddleLeft = temp;
		if (isJump)
		{
			isMiddleLeftColliding = false;
		}
	}
	else
	{
		isMiddleLeftColliding = false;
	}
}
void Enemy::CheckBottomCollision()
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

	sf::Color BottomCenterPixel = MapImage.getPixel(startX + (EnemyHitBox.getSize().x / 2), endY);
	if (BottomCenterPixel == collisionColor)
	{
		isBottomCenterColliding = true;

		int temp = 0;
		while (BottomCenterPixel == collisionColor)
		{
			temp++;
			BottomCenterPixel = MapImage.getPixel(startX + (EnemyHitBox.getSize().x / 2), endY - temp);
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
		rollBackSideSlope = std::max(temp, rollBackSideSlope);
	}
	else
	{
		isBottomRightSlopeColliding = false;
	}

	isBottomColliding = ((isBottomLeftColliding || isBottomRightColliding) || isBottomCenterColliding);
	if (!isJump)
		isGrounded = isBottomColliding;
	isBottomSlopeColliding = (isBottomLeftSlopeColliding || isBottomRightSlopeColliding);
}
void Enemy::CheckSlopeCollision()

{
	sf::Color SlopePixel = MapImage.getPixel(startX + (EnemyHitBox.getSize().x / 2), endY - 1);
	if (SlopePixel == slopeCollisionColor)
	{
		isSlopeColliding = true;
		int temp = 0;
		while (SlopePixel == slopeCollisionColor)
		{
			temp++;
			SlopePixel = MapImage.getPixel(startX + (EnemyHitBox.getSize().x / 2), endY - 1 - temp);
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
