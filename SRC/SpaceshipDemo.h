#ifndef _SPACESHIPDEMO_H_
#define _SPACESHIPDEMO_H_

#include "GameUtil.h"
#include "GameObject.h"
#include "Shape.h"

class SpaceshipDemo : public GameObject
{
public:
	SpaceshipDemo();
	SpaceshipDemo(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	SpaceshipDemo(const SpaceshipDemo& s);
	virtual ~SpaceshipDemo(void);
	virtual void Update(int t);
	virtual void Render(void);
	virtual void Thrust(float t);
	virtual void Rotate(float r);
	virtual void Shoot(void);
	virtual void ShootBlast(void);
	void SetSpaceshipDemoShape(shared_ptr<Shape> spaceship_shape)
	{
		mSpaceshipDemoShape = spaceship_shape;
	}
	void SetThrusterDemoShape(shared_ptr<Shape> thruster_shape) 
	{ 
		mThrusterDemoShape = thruster_shape; 
	}
	void SetBulletDemoShape(shared_ptr<Shape> bullet_shape) 
	{
		mBulletDemoShape = bullet_shape; 
	}

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

	bool mShieldOnDemo;
	bool mBlastReadyDemo;
	
private:
	float mThrustDemo;

	shared_ptr<Shape> mSpaceshipDemoShape;
	shared_ptr<Shape> mThrusterDemoShape;
	shared_ptr<Shape> mBulletDemoShape;
};

#endif // !_SPACESHIPDEMO_H_

