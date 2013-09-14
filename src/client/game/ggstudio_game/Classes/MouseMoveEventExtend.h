#ifndef __MOUSE_MOVE_EXTEND_H__
#define __MOUSE_MOVE_EXTEND_H__

#include <Common.h>
#include <cocos2d.h>
#include <vector>
#include <functional>

typedef std::function<void (cocos2d::CCObject* sender, const cocos2d::CCPoint& point)> MouseMoveCallback;

class MouseMoveEventExtend
{
public:
    static MouseMoveEventExtend& instance()
    {
        static MouseMoveEventExtend inst;
        return inst;
    }

private:
    MouseMoveEventExtend()
    {
        cocos2d::EGLView::getInstance()->setWndProc(WndProcHook);
    }

public:
    void addListener(cocos2d::CCObject* object, const MouseMoveCallback& cb)
    {
        if (!object)
            return;

        objectList_.push_back(object);
    }

    void removeListener(cocos2d::CCObject* object)
    {
        auto iter = objectList_.begin();
        for (; iter != objectList_.end(); ++iter)
        {
            if (*iter == object)
            {
                objectList_.erase(iter);
                return;
            }
        }
    }

private:
    static LRESULT WndProcHook(UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed)
    {
        switch (message)
        {
            case WM_MOUSEMOVE:
            {
                //MessageBoxA(NULL, "000", "XX", 0);
                uint16 xpos = LOWORD(lParam); 
                uint16 ypos = HIWORD(lParam);

#ifdef _DEBUG
                char sz[256] = {0};
                sprintf(sz, "x = %d, y = %d\n", xpos, ypos);
                //EGLView::getInstance()->setViewName("asdasd");
                OutputDebugStringA(sz);
#endif //_DEBUG

                
            }
            break;
        }

        return 0;
    }

    void dispatch()
    {
    
    }

private:
    std::vector<cocos2d::CCObject*> objectList_;
};

#endif