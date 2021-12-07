#include "GameWorld.h"
#include "Blast.h"
#include "BoundingSphere.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Blasts live for 20s by default. */
Blast::Blast()
	: GameObject("Blast"), mTimeToLive(20000)
{
}

/** Construct a new bullet with given position, velocity, acceleration, angle, rotation and lifespan. */
Blast::Blast(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl)
	: GameObject("Blast", p, v, a, h, r), mTimeToLive(ttl)
{
}

/** Copy constructor. */
Blast::Blast(const Blast& b)
	: GameObject(b),
	mTimeToLive(b.mTimeToLive)
{
}

/** Destructor. */
Blast::~Blast(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update blast, removing it from game world if necessary. */
void Blast::Update(int t)
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

bool Blast::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Blast::OnCollision(const GameObjectList& objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}
