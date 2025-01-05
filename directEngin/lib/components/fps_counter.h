#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

struct FPSCounter
{
    float fpsMax{ 0.0f };
    float fpsMin{ 0.0f };
    float fpsAverage{ 0.0f };
    float fps{ 0.0f };
    size_t currentFrameCount{ 0u };
    float lastUpdate{0.0f};
};

struct FPSView
{
};

#endif // FPS_COUNTER_H