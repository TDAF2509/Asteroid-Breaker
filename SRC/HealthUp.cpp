#include <stdlib.h>
#include "GameUtil.h"
#include "GameWorld.h"
#include "HealthUp.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"

HealthUp::HealthUp(void) : GameObject("HealthUp") {
	mAngle = rand() % 360;
	mRotation = 0;// rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0;
	mVelocity.x = 10 * cos(DEG2RAD * mAngle);
	mVelocity.y = 10 * sin(DEG2RAD * mAngle);
	mVelocity.z = 0;
}

HealthUp::~HealthUp(void) {

}

void HealthUp::Render(void) {

	GameObject::Render();
	
}

bool HealthUp::CollisionTest(shared_ptr<GameObject> o) {

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

void HealthUp::OnCollision(const GameObjectList& objects) {

	mWorld->FlagForRemoval(GetThisPtr());
}