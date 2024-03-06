#include "pch.h"
#include "SoundMgr.h"

SoundMgr::SoundMgr()
{
}

SoundMgr::~SoundMgr()
{
	Release();
}

void SoundMgr::Init(int totalChannels)
{
	Release();
	for (int i = 0; i < totalChannels; ++i)
	{
		waiting.push_back(new sf::Sound());
	}
}

void SoundMgr::Release()
{
	for (auto sound : waiting)
	{
		delete sound;
	}
	waiting.clear();
}

void SoundMgr::Update(float dt)
{
	for (auto go = playing.begin(); go != playing.end();)
	{
		sf::Sound* sound = *go;
		if (sound->getStatus() == sf::SoundSource::Stopped)
		{
			go = playing.erase(go);
			waiting.push_back(sound);
		}
		else
		{
			++go;
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		PlayBgm("sound/cuning.mp3");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		PlayBgm("sound/sans.mp3");
	}


	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		bgmVolume = 5.f;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
	{
		bgmVolume = 100.f;
	}

	if (isFading)
	{
		bool isEndFront = false;
		bool isEndBack = false;

		int backBgmIndex = (frontBgmIndex == 1) ? 0 : 1;
		
		float backVolume = bgm[backBgmIndex].getVolume();
		backVolume = Utils::Lerp(backVolume, 0.f, fadeSpeed * dt);
		bgm[backBgmIndex].setVolume(backVolume);
		if (std::abs(backVolume) < fadeLimit)
		{
			bgm[backBgmIndex].setVolume(0.f);
			bgm[backBgmIndex].stop();
			bool isEndBack = true;

		}

		float frontVolume = bgm[frontBgmIndex].getVolume();
		frontVolume = Utils::Lerp(frontVolume, bgmVolume, fadeSpeed * dt);
		bgm[frontBgmIndex].setVolume(frontVolume);

		if (std::abs(bgmVolume - frontVolume) < fadeLimit)
		{
			bgm[frontBgmIndex].setVolume(bgmVolume);
			bool isEndFront = true;
		}
		if (isEndFront && isEndBack)
		{
			isFading = false;
		}
	}
	

}

void SoundMgr::PlayBgm(std::string id)
{
	isFading = true;
	frontBgmIndex = (frontBgmIndex + 1) % 2; // 0 1 0 1 0 1
	int backBgmIndex = (frontBgmIndex == 1) ? 0 : 1;

	bgm[frontBgmIndex].setBuffer(RES_MGR_SOUND_BUFFER.Get(id));
	bgm[frontBgmIndex].setVolume(0.f);
	bgm[frontBgmIndex].setLoop(true);
	bgm[frontBgmIndex].play();
}

void SoundMgr::StopBgm()
{
	isFading = false;
	bgm[0].stop();
	bgm[1].stop();
}

void SoundMgr::PlaySfx(std::string id, bool loop)
{
	PlaySfx(RES_MGR_SOUND_BUFFER.Get(id), loop);
}

void SoundMgr::PlaySfx(sf::SoundBuffer& buffer, bool loop)
{
	sf::Sound* sound = nullptr;
	if (waiting.empty())
	{	
		sound = playing.front();
		playing.pop_front();
		sound->stop();

		sound->getStatus() == sf::SoundSource::Stopped;
	}
	else
	{
		sound = waiting.front();
		waiting.pop_front();
	}

	sound->setBuffer(buffer);
	sound->setLoop(loop);
	sound->setVolume(sfxVolume);
	sound->play();
	playing.push_back(sound);
}

void SoundMgr::StopAllSfx()
{
	for (auto sound : playing)
	{
		sound->stop();
		waiting.push_back(sound);
	}
	playing.clear();
}
