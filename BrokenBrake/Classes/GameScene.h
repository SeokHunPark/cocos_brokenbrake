#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Vehicle.h"

USING_NS_CC;

class Game : public cocos2d::Layer
{
public:
	virtual bool init();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(Game);

	virtual void onTouchesBegan(const std::vector<Touch*> &touches, Event* event);
	virtual void onTouchesMoved(const std::vector<Touch*>&touches, Event* unused_event);
	virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event* unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>&touches, Event *unused_event);

private:

	void selfRemover(Node* sender);
	void vehicleRemover(Node* sender);

	void initBackground();
	
	void initPlayerCar();
	void movePlayerCar(float distance);

	void addAIVehicle(Vehicle* vehicle);
	void addAICar(float dt);
	void addExplosion(Vec2 pt, float wRatio, float delayPerUnit);

	Vec2 _beginTouchPosition;
	Vec2 _endTouchPosition;

	std::vector<Vehicle*> _vectorAIVehicle;
};

#endif