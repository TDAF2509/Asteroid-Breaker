#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Blast.h"
#include "Spaceship.h"
#include "BoundingSphere.h"
#include "Animation.h"
#include "AnimationManager.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship(): GameObject("Spaceship"), mThrust(0)
{

	mShieldOn = false;
	mBlastReady = false;
	Animation* blast_anim = AnimationManager::GetInstance().CreateAnimationFromFile("blast", 200,1032,200,172, "blast.png");

}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();

	if (mShieldOn)
	{
		glScalef(7, 6, 8);
		// Disabling lighting so the colours are visible
		glDisable(GL_LIGHTING);
		// Blue colour
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
void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);

}

/** Shoot a Blast. */
void Spaceship::ShootBlast(void)
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
	(new Blast(blast_position, blast_velocity, mAcceleration, mAngle, 0, 20000));
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("blast");
	shared_ptr<Sprite> blast_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	blast_sprite->SetLoopAnimation(true);
	blast->SetBoundingShape(make_shared<BoundingSphere>(blast->GetThisPtr(), 10.0f));
	blast->SetShape(mBulletShape);
	blast->SetSprite(blast_sprite);
	blast->SetScale(0.1f);
	// Add the new bullet to the game world
	mWorld->AddObject(blast);

}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	if (mShieldOn == true)
	{
		mShieldOn = false;
	}
	else
	{
		mWorld->FlagForRemoval(GetThisPtr());
		mShieldOn = true;
	}
	
}