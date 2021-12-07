#include <stdlib.h>
#include "GameUtil.h"
#include "Shield.h"
#include "BoundingShape.h"

Shield::Shield(void) : GameObject("Shield") {

	mAngle = rand() % 360;
	mRotation = rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 4.0 * cos(DEG2RAD * mAngle);
	mVelocity.y = 4.0 * sin(DEG2RAD * mAngle);
	mVelocity.z = 0.0;
}

Shield::~Shield(void) {}

void Shield::Render(void) {

	GameObject::Render();

}

bool Shield::CollisionTest(shared_ptr<GameObject> o) {

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

void Shield::OnCollision(const GameObjectList& objects) {

	mWorld->FlagForRemoval(GetThisPtr());

}