/****************************************************************************
Copyright (c) 2011      Laschweinski
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "base/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_QT5

#include "CCApplication.h"
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"
#include "CCGLView.h"
#include <QtCore/QThread>
#include "UIMessageAdapter.h"
#include "lua_qt_bridge_auto.hpp"

/* Test */
#define TEST_UI_MESSGAE
#ifdef TEST_UI_MESSGAE
#include "2d/CCLabelTTF.h"
#include "2d/CCScene.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

#endif /* TEST_UI_MESSGAE */

NS_CC_BEGIN


// sharedApplication pointer
Application * Application::sm_pSharedApplication = 0;

static long getCurrentMillSecond() {
    long lLastTime;
    struct timeval stCurrentTime;

    gettimeofday(&stCurrentTime,NULL);
    lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
    return lLastTime;
}

Application::Application(int argc, char** argv)
: QApplication(argc, argv),
  _animationInterval(1.0f/60.0f*1000.0f)
{
    CCLOG("Application::Application(int argc, char** argv)");
    
    CC_UNUSED_PARAM(argc);
    CC_UNUSED_PARAM(argv);

    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

Application::Application()
 : Application(0, nullptr)
{
    CCLOG("Application::Application()");
}

Application::~Application()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

int Application::run()
{
    // Initialize instance and cocos2d.
    if (! applicationDidFinishLaunching())
    {
        return 0;
    }

//    CCLOG("Run: ThreadId: %p", QThread::currentThread());

    long lastTime = 0L;
    long curTime = 0L;

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    // Retain glview to avoid glview being released in the while loop
    glview->retain();

    while (!glview->windowShouldClose())
    {
        lastTime = getCurrentMillSecond();

        director->mainLoop();
        glview->pollEvents();
        this->pollUIEvents();
//        this->processEvents();

        curTime = getCurrentMillSecond();
        if (curTime - lastTime < _animationInterval)
        {
            usleep((_animationInterval - curTime + lastTime)*1000);
        }
    }
    /* Only work on Desktop
    *  Director::mainLoop is really one frame logic
    *  when we want to close the window, we should call Director::end();
    *  then call Director::mainLoop to do release of internal resources
    */
    if (glview->isOpenGLReady())
    {
        director->end();
        director->mainLoop();
        director = nullptr;
    }
    glview->release();

    this->closeAllWindows();
    this->quit();
    return -1;
}

void Application::setAnimationInterval(double interval)
{
    //TODO do something else
    _animationInterval = interval*1000.0f;
}

void Application::setResourceRootPath(const std::string& rootResDir)
{
    _resourceRootPath = rootResDir;
    if (_resourceRootPath[_resourceRootPath.length() - 1] != '/')
    {
        _resourceRootPath += '/';
    }
    FileUtils* pFileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths = pFileUtils->getSearchPaths();
    searchPaths.insert(searchPaths.begin(), _resourceRootPath);
    pFileUtils->setSearchPaths(searchPaths);
}

const std::string& Application::getResourceRootPath(void)
{
    return _resourceRootPath;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_LINUX;
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::getInstance()
{
    CC_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

// @deprecated Use getInstance() instead
Application* Application::sharedApplication()
{
    return Application::getInstance();
}

const char * Application::getCurrentLanguageCode()
{
    static char code[3]={0};
    char *pLanguageName = getenv("LANG");
    if (!pLanguageName)
        return "en";
    strtok(pLanguageName, "_");
    if (!pLanguageName)
        return "en";
    strncpy(code,pLanguageName,2);
    code[2]='\0';
    return code;
}

LanguageType Application::getCurrentLanguage()
{
	char *pLanguageName = getenv("LANG");
	LanguageType ret = LanguageType::ENGLISH;
	if (!pLanguageName)
	{
		return LanguageType::ENGLISH;
	}
	strtok(pLanguageName, "_");
	if (!pLanguageName)
	{
		return LanguageType::ENGLISH;
	}
	
	if (0 == strcmp("zh", pLanguageName))
	{
		ret = LanguageType::CHINESE;
	}
	else if (0 == strcmp("en", pLanguageName))
	{
		ret = LanguageType::ENGLISH;
	}
	else if (0 == strcmp("fr", pLanguageName))
	{
		ret = LanguageType::FRENCH;
	}
	else if (0 == strcmp("it", pLanguageName))
	{
		ret = LanguageType::ITALIAN;
	}
	else if (0 == strcmp("de", pLanguageName))
	{
		ret = LanguageType::GERMAN;
	}
	else if (0 == strcmp("es", pLanguageName))
	{
		ret = LanguageType::SPANISH;
	}
	else if (0 == strcmp("nl", pLanguageName))
	{
		ret = LanguageType::DUTCH;
	}
	else if (0 == strcmp("ru", pLanguageName))
	{
		ret = LanguageType::RUSSIAN;
	}
	else if (0 == strcmp("ko", pLanguageName))
	{
		ret = LanguageType::KOREAN;
	}
	else if (0 == strcmp("ja", pLanguageName))
	{
		ret = LanguageType::JAPANESE;
	}
	else if (0 == strcmp("hu", pLanguageName))
	{
		ret = LanguageType::HUNGARIAN;
	}
    else if (0 == strcmp("pt", pLanguageName))
    {
        ret = LanguageType::PORTUGUESE;
    }
    else if (0 == strcmp("ar", pLanguageName))
    {
        ret = LanguageType::ARABIC;
    }
    else if (0 == strcmp("nb", pLanguageName))
    {
        ret = LanguageType::NORWEGIAN;
    }
    else if (0 == strcmp("pl", pLanguageName))
    {
        ret = LanguageType::POLISH;
    }
    
    return ret;
}

void Application::pollUIEvents()
{
    bool hasMessage = false;
    auto message = UIMessageAdapter::getInstance()->pop(hasMessage);
    auto scene = Director::getInstance()->getRunningScene();

    if(!hasMessage) return;

#ifdef TEST_UI_MESSGAE
    CCLOG("UIEvents: %s %p", message.c_str(), scene);

    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);

    LuaStack* stack = engine->getLuaStack();
    stack->executeString("QTEventHandler = require \"MapEditor.QTEventHandler\"");
    std::string code = "";
    code.append("QTEventHandler:getInstance():onEvent(");
    code.append(message);
    code.append(")");
    CCLOG("UIEvents: %s", code.c_str());

    stack->executeString(code.c_str());

    // auto visibleSize = Director::getInstance()->getVisibleSize();
    // auto origin = Director::getInstance()->getVisibleOrigin();

    // /////////////////////////////
    // // 3. add your codes below...

    // // add a label shows "Hello World"
    // // create and initialize a label

    // auto label = LabelTTF::create(message.c_str(), "Arial", 24.0f);

    // // position the label on the center of the screen
    // label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                         origin.y + visibleSize.height/2));

    // // add the label as a child to this layer
    // scene->addChild(label, 1);
#endif /* TEST_UI_MESSGAE */
}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_QT5

