#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto node = Node::create();
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    node->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    node->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("img.png");

    // add the sprite as a child to this layer
    node->addChild(sprite, 0);
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    sprite->runAction(RotateBy::create(2, 360));
    
    this->addChild(node);
    
    postProcessingToNode(node, "lut.png");
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::postProcessingToNode(cocos2d::Node *node, const std::string &lut)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888);
    renderTexture->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    node->setVisible(true);
    node->visit();
    node->setVisible(false);
    renderTexture->end();
    this->addChild(renderTexture);
    CC_ASSERT(node != renderTexture->getParent());
    addShaderToNode(renderTexture->getSprite(), lut);
    
    this->schedule([=](float){
        renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        node->setVisible(true);
        node->visit();
        node->setVisible(false);
        renderTexture->end();
    }, "rendertexture");

}

void HelloWorld::addShaderToNode(cocos2d::Node* node, const std::string& lut)
{
    if(node){
        auto texCache = Director::getInstance()->getTextureCache();
        auto lutTex = texCache->addImage(lut);
        Texture2D::TexParams lutParams = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
        lutTex->setTexParameters(lutParams);
        
        auto p = GLProgram::createWithFilenames("gray.vsh","gray.fsh");
        node->setGLProgram(p);
        node->getGLProgramState()->setUniformTexture("lut", lutTex);
    }
}