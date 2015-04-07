#include "GameScene.h"
#include "Definitions.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    //for debug show the physics body of object
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity( Vect( 0, 0 ) );
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld() );
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //create and add background
    auto backgoundSprite = Sprite::create("Background.png");
    backgoundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    this->addChild(backgoundSprite);
    
    //set physics body for screen size edge
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    edgeBody->setCollisionBitmask( OBSTACLE_COLLISION_BITMASK );
    edgeBody->setContactTestBitmask( true ); 
    
    //add edge body to node and put it in the centre of screen
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    edgeNode->setPhysicsBody( edgeBody );
    
    this->addChild(edgeNode);
    
    //schedulely add pipe to scene
    this->schedule(schedule_selector( GameScene::SpawnPipe), PIPE_SPAWN_FREQUENCY * visibleSize.width);
    
    //create and add a bird
    bird = new Bird(this);
    
    // set physics contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1( GameScene::onContactBegin, this );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( contactListener, this );
    
    //set touch listener
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches( true );
    touchListener->onTouchBegan = CC_CALLBACK_2( GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority( touchListener, this );
    
    //create and add score label
    score = 0;
    
    __String *tempScore = __String::createWithFormat( "%i", score );
    
    scoreLabel = Label::createWithTTF( tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    scoreLabel->setColor( Color3B::WHITE );
    scoreLabel->setPosition( Point( visibleSize.width /2 + origin.x, visibleSize.height * 0.75 + origin.y ) );
    
    this->addChild( scoreLabel, 10000 );
    
    //schedulely call the method GameScene::update( float dt )
    this->scheduleUpdate();
    
    return true;
}

void GameScene::SpawnPipe(float dt)
{
    pipe.SpawnPipe(this);
}

bool GameScene::onContactBegin( cocos2d::PhysicsContact &contact )
{
    PhysicsBody *a = contact.getShapeA()->getBody();
    PhysicsBody *b = contact.getShapeB()->getBody();
    
    //detect bird and pipe collision
    if ( ( BIRD_COLLISOIN_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask() ) || ( BIRD_COLLISOIN_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask() ))
    {
        
        CCLOG( "Score: %i", score );
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( "Sounds/Hit.mp3" );


        auto scene = GameOverScene::createScene( score );
        Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene) );
    }
    //detect bird and point line collision
    else if ( ( BIRD_COLLISOIN_BITMASK == a->getCollisionBitmask() && POINT_COLLISION_BITMASK == b->getCollisionBitmask() ) || ( BIRD_COLLISOIN_BITMASK == b->getCollisionBitmask() && POINT_COLLISION_BITMASK == a->getCollisionBitmask() ) )
    {
        CCLOG( "Point scored" );
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect( "Sounds/Point.mp3" );
        
        //update score
        score++;

        __String *tempScore = __String::createWithFormat( "%i", score );

        scoreLabel->setString( tempScore->getCString() );
    }
    
    return true;
    
}

bool GameScene::onTouchBegan( cocos2d::Touch *touch, cocos2d::Event *event )
{
    bird->Fly();
    
    this->scheduleOnce( schedule_selector( GameScene::StopFlying ), BIRD_FLY_DURATION );
    
    return true;
}

void GameScene::StopFlying( float dt )
{
    bird->StopFlying();
}

void GameScene::update( float dt )
{
    bird->Fall();
}
