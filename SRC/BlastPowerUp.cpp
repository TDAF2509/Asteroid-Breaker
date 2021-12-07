#include <stdlib.h>
#include "GameUtil.h"
#include "BlastPowerUp.h"
#include "BoundingShape.h"

BlastPowerUp::BlastPowerUp(void) : GameObject("BlastPowerUp") {

	mAngle = rand() % 360;
	mRotation = rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 4.0 * cos(DEG2RAD * mAngle);
	mVelocity.y = 4.0 * sin(DEG2RAD * mAngle);
	mVelocity.z = 0.0;
}

BlastPowerUp::~BlastPowerUp(void) {}

void BlastPowerUp::Render(void) {

	GameObject::Render();

}

bool BlastPowerUp::CollisionTest(shared_ptr<GameObject> o) {

	if (o->GetType() == GameObjectType("Asteroid") || o->GetType() == GameObjectType("Bullet") || o->GetType() == GameObjectType("HealthUp") || o->GetType() == GameObjectType("Explosion") || o->GetType() == GameObjectType("BulletDemo") || o->GetType() == GameObjectType("BlastPowerUp") || o->GetType() == GameObjectType("Shield") || o->GetType() == GameObjectType("Blast"))
	{
		return false;
	}

	if (mBoundingShape.get() == NULL)
	{
		return false;
	}

	if (o->GetBoundingShape().get() == NULL)
	{
		return false;
	}

	return mBoundingShape->CollisionTest(o->GetBoundingShape());

}

void BlastPowerUp::OnCollision(const GameObjectList& objects) {

	mWorld->FlagForRemoval(GetThisPtr());

}