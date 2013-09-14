#include <cocos2d.h>

USING_NS_CC;

static CCRenderTexture* createStroke(cocos2d::CCLabelTTF *label, float size, cocos2d::ccColor3B color)
{
    float x=label->getTexture()->getContentSize().width+size*2;
    float y=label->getTexture()->getContentSize().height+size*2;
    CCRenderTexture *rt=CCRenderTexture::create(x, y);
    CCPoint originalPos=label->getPosition();
    ccColor3B originalColor=label->getColor();
    label->setColor(color);
    ccBlendFunc originalBlend=label->getBlendFunc();

    ccBlendFunc func = { GL_SRC_ALPHA, GL_ONE};
    label->setBlendFunc(func);
    CCPoint center=ccp(x/2+size, y/2+size);
    rt->begin();
    for (int i=0; i<360; i+=15) {
        float _x=center.x+sin(CC_DEGREES_TO_RADIANS(i))*size;
        float _y=center.y+cos(CC_DEGREES_TO_RADIANS(i))*size;

        label->setPosition(ccp(_x, _y));
        label->visit();

    }
    rt->end();
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    float rtX=originalPos.x-size;
    float rtY=originalPos.y-size;

    rt->setPosition(ccp(rtX, rtY));

    return rt;
}