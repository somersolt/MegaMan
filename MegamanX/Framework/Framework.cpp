#include "pch.h"
#include "Framework.h"

void Framework::Init(int width, int height, const std::string& name)
{
    srand(std::time(NULL));

    windowSize.x = width;
    windowSize.y = height;

    window.create(sf::VideoMode(windowSize.x, windowSize.y), name);

    DT_MGR.Init();
    InputMgr::Init();
    SOUND_MGR.Init();
    SCENE_MGR.Init();
}

void Framework::Do()
{
    while (window.isOpen())
    {
        deltaTime = realDeltaTime = clock.restart();
        deltaTime *= timeScale;

        time += deltaTime;
        realTime += realDeltaTime;

        fixedDeltaTime += deltaTime;

        InputMgr::Clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            InputMgr::UpdateEvent(event);
        }
        InputMgr::Update(GetDT());

        SCENE_MGR.Update(GetDT());
        SCENE_MGR.LateUpdate(GetDT());
        SOUND_MGR.Update(GetDT());

        float fdt = fixedDeltaTime.asSeconds();
        if (fdt > fixedUpdateTime)
        {
            SCENE_MGR.FixedUpdate(fdt);
            fixedDeltaTime = sf::Time::Zero;
        }
        
        window.clear();
        SCENE_MGR.Draw(window);
        window.display();
    }
}

void Framework::Release()
{
    SCENE_MGR.Release();
    SOUND_MGR.Release();
    DT_MGR.Release();

    RES_MGR_TEXTURE.UnloadAll();
    RES_MGR_FONT.UnloadAll();
    RES_MGR_SOUND_BUFFER.UnloadAll();
}
