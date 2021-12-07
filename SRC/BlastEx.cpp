#include "GameWorld.h"
#include "BoundingSphere.h"
#include "BlastEx.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. */
BlastEx::BlastEx() : GameObject("BlastEx") {}

/** Construct a new Blast explosion with given position, velocity, angle and rotation. */
BlastEx::BlastEx(GLVector3f p, GLVector3f v, GLfloat h, GLfloat r)
	: GameObject("BlastEx", p, v, GLVector3f(), h, r) {}

/** Copy constructor. */
BlastEx::BlastEx(const BlastEx& e) : GameObject(e) {}

/** Destructor. */
BlastEx::~BlastEx(void) {}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update explosion, removing it from game world if necessary. */
void BlastEx::Update(int t)
{
	// Update position/velocity
	GameObject::Update(t);

	if (mSprite.get() == NULL) return;

	// If sprite has finished animation remove explosion from world
	if (!mSprite->IsAnimating()) {
		if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
	}
}

bool BlastEx::CollisionTest(shared_ptr<GameObject> o)
{
	if (GetType() == o->GetType())
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
	if (o->GetType() == GameObjectType("Spaceship") || o->GetType() == GameObjectType("SpaceshipDemo") || o->GetType() == GameObjectType("Bullet") || o->GetType() == GameObjectType("HealthUp") || o->GetType() == GameObjectType("Explosion") || o->GetType() == GameObjectType("BulletDemo") || o->GetType() == GameObjectType("BlastPowerUp") || o->GetType() == GameObjectType("Shield") || o->GetType() == GameObjectType("Blast"))
	{
		return false;
	}
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void BlastEx::OnCollision(const GameObjectList& objects)
{

	mWorld->FlagForRemoval(GetThisPtr());

}
