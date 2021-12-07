#include "GameUtil.h"
#include "GameWorld.h"
#include "BulletDemo.h"
#include "BlastDemo.h"
#include "SpaceshipDemo.h"
#include "BoundingSphere.h"
#include "Animation.h"
#include "AnimationManager.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
SpaceshipDemo::SpaceshipDemo(): GameObject("SpaceshipDemo"), mThrustDemo(0)
{
	mShieldOnDemo = false;
	mBlastReadyDemo = false;
	Animation* blast_anim = AnimationManager::GetInstance().CreateAnimationFromFile("blast", 200, 1032, 200, 172, "blast.png");
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
SpaceshipDemo::SpaceshipDemo(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrustDemo(0)
{
}

/** Copy constructor. */
SpaceshipDemo::SpaceshipDemo(const SpaceshipDemo& s) : GameObject(s), mThrustDemo(0)
{
}

/** Destructor. */
SpaceshipDemo::~SpaceshipDemo(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void SpaceshipDemo::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void SpaceshipDemo::Render(void)
{

	if (mSpaceshipDemoShape.get() != NULL)
	{
		mSpaceshipDemoShape->Render();
	}

	// If ship is thrusting
	if ((mThrustDemo > 0) && (mThrusterDemoShape.get() != NULL))
	{
		mThrusterDemoShape->Render();
	}

	GameObject::Render();

	if (mShieldOnDemo)
	{
		glScalef(7, 6, 8);
		// Disabling lighting so the colours are visible
		glDisable(GL_LIGHTING);
		glColor3f(0, 0, 1);
		// Start drawing lines
		glBegin(GL_LINE_LOOP);
		// Add vertices to draw the shape of the shield around the
		// The spaceship in the shape of an octagon
		glVertex3f(-7, -7, 0.0);
		glVertex3f(-10, 0, 0.0);
		glVertex3f(-7, 7, 0.0);
		glVertex3f(0, 10, 0.0);
		glVertex3f(7, 7, 0.0);
		glVertex3f(10, 0, 0.0);
		glVertex3f(7, -7, 0.0);
		glVertex3f(0, -10, 0.0);
		// Finish drawing lines
		glEnd();
		// Enable lighting
		glEnable(GL_LIGHTING);
	}
}

/** Fire the rockets. */
void SpaceshipDemo::Thrust(float t)
{
	mThrustDemo = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrustDemo * cos(DEG2RAD * mAngle);
	mAcceleration.y = mThrustDemo * sin(DEG2RAD * mAngle);
}

/** Set the rotation. */
void SpaceshipDemo::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void SpaceshipDemo::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceshipDemo_heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
	spaceshipDemo_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceshipDemo_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceshipDemo_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> BulletDemo(new BulletDemo(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	BulletDemo->SetBoundingShape(make_shared<BoundingSphere>(BulletDemo->GetThisPtr(), 2.0f));
	BulletDemo->SetShape(mBulletDemoShape);
	// Add the new bullet to the game world
	mWorld->AddObject(BulletDemo);

}

/** Shoot a Blast. */
void SpaceshipDemo::ShootBlast(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f blast_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float blast_speed = 90;
	// Construct a vector for the bullet's velocity
	GLVector3f blast_velocity = mVelocity + spaceship_heading * blast_speed;
	// Construct a new bullet
	shared_ptr<GameObject> blast
	(new BlastDemo(blast_position, blast_velocity, mAcceleration, mAngle, 0, 20000));
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("blast");
	shared_ptr<Sprite> blast_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	blast_sprite->SetLoopAnimation(true);
	blast->SetBoundingShape(make_shared<BoundingSphere>(blast->GetThisPtr(), 10.0f));
	blast->SetShape(mBulletDemoShape);
	blast->SetSprite(blast_sprite);
	blast->SetScale(0.1f);
	// Add the new bullet to the game world
	mWorld->AddObject(blast);

}

bool SpaceshipDemo::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid") && o->GetType() != GameObjectType("Bullet")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void SpaceshipDemo::OnCollision(const GameObjectList& objects)
{
	if (mShieldOnDemo == true)
	{
		mShieldOnDemo = false;
	}
	else
	{
		mWorld->FlagForRemoval(GetThisPtr());
		mShieldOnDemo = true;
	}
}