#include "GameWorld.h"
#include "BlastDemo.h"
#include "BoundingSphere.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Blass live for 2s by default. */
BlastDemo::BlastDemo() : GameObject("BlastDemo"), mTimeToLive(2000)
{
}

/** Construct a new blast with given position, velocity, acceleration, angle, rotation and lifespan. */
BlastDemo::BlastDemo(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl) : GameObject("BulletDemo", p, v, a, h, r), mTimeToLive(ttl)
{
}

/** Copy constructor. */
BlastDemo::BlastDemo(const BlastDemo& b) : GameObject(b), mTimeToLive(b.mTimeToLive)
{
}

/** Destructor. */
BlastDemo::~BlastDemo(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update bullet, removing it from game world if necessary. */
void BlastDemo::Update(int t)
{
	// Update position/velocity
	GameObject::Update(t);
	// Reduce time to live
	mTimeToLive = mTimeToLive - t;
	// Ensure time to live isn't negative
	if (mTimeToLive < 0) { mTimeToLive = 0; }
	// If time to live is zero then remove bullet from world
	if (mTimeToLive == 0) {
		if (mWorld) mWorld->FlagForRemoval(GetThisPtr());
	}

}

bool BlastDemo::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid") && o->GetType() != GameObjectType("Spaceship")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void BlastDemo::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}