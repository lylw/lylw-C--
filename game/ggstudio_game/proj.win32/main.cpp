#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "MouseMoveEventExtend.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    EGLView* eglView = EGLView::getInstance();
    eglView->setViewName("ºÃÓÎÏ·");
    eglView->setFrameSize(960, 640);

    return Application::getInstance()->run();
}

