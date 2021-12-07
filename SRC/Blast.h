#ifndef __BLAST_H__
#define __BLAST_H__

#include "GameUtil.h"
#include "GameObject.h"

class Blast : public GameObject
{
public:
	Blast();
	Blast(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r, int ttl);
	Blast(const Blast& b);
	virtual ~Blast(void);

	virtual void Update(int t);

	void SetTimeToLive(int ttl) { mTimeToLive = ttl; }
	int GetTimeToLive(void) { return mTimeToLive; }

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

protected:
	int mTimeToLive;
};

#endif
