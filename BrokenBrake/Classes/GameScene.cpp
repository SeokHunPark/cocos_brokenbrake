#include "GameScene.h"
#include "Defines.h"

Scene* Game::createScene()
{
	auto scene = Scene::create();
	auto layer = Game::create();
	scene->addChild(layer);
	return scene;
}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	log("Game::init()");

	// Touch event
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	auto positionListener = EventListenerTouchAllAtOnce::create();
	positionListener->onTouchesBegan = CC_CALLBACK_2(Game::onTouchesBegan, this);
	positionListener->onTouchesMoved = CC_CALLBACK_2(Game::onTouchesMoved, this);
	positionListener->onTouchesCancelled = CC_CALLBACK_2(Game::onTouchesCancelled, this);
	positionListener->onTouchesEnded = CC_CALLBACK_2(Game::onTouchesEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(positionListener, this);

	_beginTouchPosition = Vec2(0, 0);
	_endTouchPosition = Vec2(0, 0);

	// Initialize
	initBackground();
	initPlayerCar();
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Explosion.plist");
	BlendFunc bfExplosion = { GL_ONE, GL_ONE };

	auto spriteBatchNodeExplosion = SpriteBatchNode::create("Explosion.png");
	spriteBatchNodeExplosion->setBlendFunc(bfExplosion);
	addChild(spriteBatchNodeExplosion, ZORDER_SPRITE_BATCH_NODE_EXPLOSION, TAG_SPRITE_BATCH_NODE_EXPLOSION);

	// Schedule
	schedule(schedule_selector(Game::addAICar), 0.5f);
	
	return true;
}

void Game::onTouchesBegan(const std::vector<Touch*> &touches, Event* event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++)
	{
		Vec2 location = (*iter)->getLocation();
		_beginTouchPosition = location;
	}
}

void Game::onTouchesMoved(const std::vector<Touch*>&touches, Event* unused_event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++)
	{
		Vec2 location = (*iter)->getLocation();
		_endTouchPosition = location;

		Vec2 distanceVector = _endTouchPosition - _beginTouchPosition;
		movePlayerCar(distanceVector.x);

		_beginTouchPosition = location;
	}
}

void Game::onTouchesCancelled(const std::vector<Touch*>&touches, Event* unused_event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++)
	{
		Vec2 location = (*iter)->getLocation();

		_beginTouchPosition = Vec2(0, 0);
		_endTouchPosition = Vec2(0, 0);
	}
}

void Game::onTouchesEnded(const std::vector<Touch*>&touches, Event *unused_event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++)
	{
		Vec2 location = (*iter)->getLocation();

		//addExplosion(location, 0.2, 0.03);
		_beginTouchPosition = Vec2(0, 0);
		_endTouchPosition = Vec2(0, 0);
	}
}

void Game::selfRemover(Node* sender)
{
	sender->removeFromParentAndCleanup(true);
}

void Game::vehicleRemover(Node* sender)
{
	Vehicle* vehicle = NULL;
	for (std::vector<Vehicle*>::iterator iter = _vectorAIVehicle.begin(); iter != _vectorAIVehicle.end(); iter++)
	{
		vehicle = (Vehicle*)*iter;
		if (vehicle->body == sender) 
		{
			_vectorAIVehicle.erase(iter);
			break;
		}
		vehicle = NULL;
	}

	if (NULL != vehicle)
	{
		vehicle->release();
		delete vehicle;
	}
}

void Game::initBackground()
{
	Size winSize = Director::getInstance()->getWinSize();

	auto layerRoad = Layer::create();
	addChild(layerRoad);
	
	auto spriteRoad_0 = Sprite::create("road.png");
	spriteRoad_0->setAnchorPoint(Vec2::ZERO);
	spriteRoad_0->setScaleX(winSize.width / spriteRoad_0->getContentSize().width);
	spriteRoad_0->setScaleY(winSize.height / spriteRoad_0->getContentSize().height);
	spriteRoad_0->setPosition(Vec2::ZERO);
	layerRoad->addChild(spriteRoad_0);

	auto spriteRoad_1 = Sprite::create("road.png");
	spriteRoad_1->setAnchorPoint(Vec2::ZERO);
	spriteRoad_1->setScaleX(winSize.width / spriteRoad_1->getContentSize().width);
	spriteRoad_1->setScaleY(winSize.height / spriteRoad_1->getContentSize().height);
	spriteRoad_1->setPosition(Vec2(0, winSize.height));
	layerRoad->addChild(spriteRoad_1);

	auto action_0 = MoveBy::create(6.0, Vec2(0, -winSize.height));
	auto action_1 = Place::create(Vec2::ZERO);
	auto action_2 = Sequence::create(action_0, action_1, NULL);
	auto action_3 = RepeatForever::create(action_2);
	layerRoad->runAction(action_3);
}

void Game::initPlayerCar()
{
	Size winSize = Director::getInstance()->getWinSize();

	auto spritePlayerCar = Sprite::create("dokdoship.png");
	spritePlayerCar->setPosition(Vec2(winSize.width * 0.5, winSize.height * 0.2));
	addChild(spritePlayerCar, ZORDER_SPRITE_PLAYERCAR, TAG_SPRITE_PLAYERCAR);
}

void Game::movePlayerCar(float distance)
{
	Size winSize = Director::getInstance()->getWinSize();
	
	float factor = 1.0;
	/*auto spritePlayerCar = (Sprite*)getChildByTag(TAG_SPRITE_PLAYERCAR);
	Vec2 originPositon = spritePlayerCar->getPosition();
	spritePlayerCar->setPosition(Vec2(originPositon.x + (factor * distance), originPositon.y));*/

	auto action_0 = MoveBy::create(0.1f, Vec2(distance * factor, 0));
	auto spritePlayerCar = (Sprite*)getChildByTag(TAG_SPRITE_PLAYERCAR);
	if (spritePlayerCar)
		spritePlayerCar->runAction(action_0);
}

void Game::addAIVehicle(Vehicle* vehicle)
{
	_vectorAIVehicle.push_back(vehicle);
}

void Game::addAICar(float dt)
{
	Size winSize = Director::getInstance()->getWinSize();

	auto spriteAICar = Sprite::create("destroyer.png");
	float xPos = (winSize.width * 0.25) + winSize.width * (rand() % 6) * 0.1;
	float yPos = winSize.height;
	spriteAICar->setPosition(Vec2(xPos, yPos));

	addChild(spriteAICar, ZORDER_SPRITE_AICAR);

	auto moveBy = MoveBy::create(1.0f, Vec2(0, -winSize.height));

	auto callFunc = CallFuncN::create(CC_CALLBACK_1(Game::selfRemover, this));
	auto sequence = Sequence::create(moveBy, callFunc, NULL);

	spriteAICar->runAction(sequence);
}

void Game::addExplosion(Vec2 pt, float wRatio, float delayPerUnit)
{
	Size winSize = Director::getInstance()->getWinSize();

	auto spriteExplosion = Sprite::createWithSpriteFrameName("explosion_effect_00.png");
	spriteExplosion->setPosition(pt);

	spriteExplosion->setScale((winSize.width * wRatio) / spriteExplosion->getContentSize().width);
	getChildByTag(TAG_SPRITE_BATCH_NODE_EXPLOSION)->addChild(spriteExplosion);

	auto frameCache = SpriteFrameCache::getInstance();
	auto animation = Animation::create();
	char szFile[64] = { 0, };
	for (int i = 0; i < 36; i++)
	{
		sprintf(szFile, "explosion_effect_%02d.png", i);
		animation->addSpriteFrame(frameCache->getSpriteFrameByName(szFile));
	}
	animation->setDelayPerUnit(delayPerUnit);

	auto animate = Animate::create(animation);
	auto callfunc = CallFuncN::create(CC_CALLBACK_1(Game::selfRemover, this));
	auto sequence = Sequence::create(animate, callfunc, NULL);
	spriteExplosion->runAction(sequence);
}