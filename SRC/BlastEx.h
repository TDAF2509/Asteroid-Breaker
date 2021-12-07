#ifndef __BLASTEX_H__
#define __BLASTEX_H__

#include "GameUtil.h"
#include "GameObject.h"

class BlastEx : public GameObject
{
public:
	BlastEx();
	BlastEx(GLVector3f p, GLVector3f v, GLfloat h, GLfloat r);
	BlastEx(const BlastEx& e);
	virtual ~BlastEx(void);

	virtual void Update(int t);
	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
};

#endif

