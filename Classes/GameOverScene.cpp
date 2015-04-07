#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "Definitions.h"

USING_NS_CC;

//global variable score
static unsigned int score;

Scene* GameOverScene::createScene( unsigned int tempScore )
{
    score = tempScore;
    
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
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

    //create menu item
    auto retryItem = MenuItemImage::create( "Retry Button.png", "Retry Button Clicked.png", CC_CALLBACK_1( GameOverScene::GoToGameScene, this) );
    retryItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height /4 * 3 ) );
    
    auto mainMenuItem = MenuItemImage::create( "Menu Button.png", "Menu Button Clicked.png", CC_CALLBACK_1( GameOverScene::GoToMainMenuScene, this) );
    mainMenuItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height /4 ) );

    //crete menu
    auto menu = Menu::create( retryItem, mainMenuItem, NULL );
    menu->setPosition( Point::ZERO );
    
    this->addChild( menu );
    
    //create tiny database and store highscore
    UserDefault *def = UserDefault::getInstance( );

    auto highScore = def->getIntegerForKey( "HIGHSCORE", 0 );
    
    if ( score > highScore )
    {
        highScore = score;
        
        def->setIntegerForKey("HIGHSCORE", highScore);
    }
    
    def->flush();
    
    //create score label
    __String *tempScore = __String::createWithFormat( "%i", score );
    auto currentScore = Label::createWithTTF( tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    currentScore->setPosition( Point( visibleSize.width * 0.25 +origin.x, visibleSize.height / 2 + origin.y ));
    
    this->addChild( currentScore );
    
    //create high score label
    __String *tempHighScore = __String::createWithFormat("%i", highScore);
    
    auto highScoreLabel = Label::createWithTTF( tempHighScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height * SCORE_FONT_SIZE );
    
    highScoreLabel->setColor( Color3B::YELLOW );
    highScoreLabel->setPosition( Point( visibleSize.width * 0.75 +origin.x, visibleSize.height / 2 + origin.y ));
    
    this->addChild( highScoreLabel );
    
    
    return true;
}

void GameOverScene::GoToMainMenuScene( cocos2d::Ref *sender )
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene) );
}

void GameOverScene::GoToGameScene( cocos2d::Ref *sender )
{
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene( TransitionFade::create( TRANSITION_TIME, scene) );
}

