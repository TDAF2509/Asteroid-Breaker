#ifndef _BULLETDEMO_H_
#define _BULLETDEMO_H_

#include "GameUtil.h"
#include "GameObject.h"

class BulletDemo : public GameObject
{
public:
	BulletDemo();
	BulletDemo(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl);
	BulletDemo(const BulletDemo& b);
	virtual ~BulletDemo(void);
	virtual void Update(int t);
	void SetTimeToLive(int ttl)
	{
		mTimeToLive = ttl;
	}
	int GetTimeToLive(void)
	{
		return mTimeToLive;
	}

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

protected:
	int mTimeToLive;
};


#endif // !_BULLETDEMO_H_

