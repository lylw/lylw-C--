#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "CodePage.hpp"

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
    eglView->setViewName(a2u("�й�����Ϸ").c_str());
    eglView->setFrameSize(1360, 768);

    return Application::getInstance()->run();
}

