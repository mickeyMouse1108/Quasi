#include "GameTime.h"

#include "imgui.h"

namespace Game
{
    GameTime::GameTime() : deltaTime(0), frameRate(0) {}
    
    GameTime::~GameTime() {}

    float GameTime::TotalDuration(TimeUnit unit) const
    {
        switch (unit)
        {
            case Second:      return currentMs / 1000.0f;
            case MilliSecond: return currentMs;
            case NanoSecond:  return currentMs * 1000.0f;
            case Frame:       return (float)currentFrame;
        }
        return -1;
    }

    float GameTime::DeltaTime(TimeUnit unit) const
    {
        switch (unit)
        {
            case Second:      return deltaTime / 1000.0f;
            case MilliSecond: return deltaTime;
            case NanoSecond:  return deltaTime * 1000.0f;
            case Frame:       return 1;
        }
        return -1;
    }

    void GameTime::Update()
    {
        ++currentFrame;
        frameRate = ImGui::GetIO().Framerate;
        deltaTime = 1000.0f / frameRate;

        currentMs += deltaTime;
    }
    
    GameTime GameTime = {};
}
