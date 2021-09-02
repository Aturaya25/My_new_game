#include "StartupScene.h"
#include "LevelScene.h"

USING_NS_CC;

Scene* Startup::createScene()
{
    return Startup::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in StartupScene.cpp\n");
}

bool Startup::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Breakout Demo", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        this->addChild(label, 1);
    }

    auto sprite = Sprite::create("cocos2dx.png");
    if (sprite == nullptr)
    {
        problemLoading("'cocos2dx.png'");
    }
    else
    {
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        this->addChild(sprite, 0);
    }

	scheduleOnce(schedule_selector(Startup::openLevelCallback), 3.0f);

    return true;
}

void Startup::openLevelCallback(float dt)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, Level::createScene(), Color3B(0, 255, 255)));
}
