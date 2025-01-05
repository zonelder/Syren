#include "fps_counter_system.h"
#include "components/fps_counter.h"
#include "components/text_ui.h"
#include <format>

void FPSCounterSystem::onUpdate(SceneManager& scene, float time)
{
	auto& view = scene.view<FPSCounter>();
	for (auto [entt, fpsCounter] : view)
	{
		fpsCounter.currentFrameCount++;
		if (time - fpsCounter.lastUpdate >= 1.0f)
		{
			fpsCounter.fpsAverage = fpsCounter.currentFrameCount;
			fpsCounter.fpsMax =  fpsCounter.currentFrameCount > fpsCounter.fpsMax ? fpsCounter.currentFrameCount : fpsCounter.fpsMax;
			fpsCounter.fpsMin =  fpsCounter.currentFrameCount < fpsCounter.fpsMin ? fpsCounter.currentFrameCount : fpsCounter.fpsMin;
			fpsCounter.lastUpdate = time;
			fpsCounter.currentFrameCount = 0u;
		}
	}
}

void FPSCounterSystem::onFrame(SceneManager& scene)
{
	auto view = scene.view<FPSCounter, TextUI>();
	for (auto [entt, fpsCounter, textUI] : view)
	{
		if (fpsCounter.currentFrameCount == 0u)
		{
			textUI.content = std::format(L"fpsAvg: {} | fpsMax: {} | fpsMin: {} |", fpsCounter.fpsAverage, fpsCounter.fpsMax, fpsCounter.fpsMin);
		}
	}
}
