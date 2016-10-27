#include <Windows.h>
#include "main.hpp"
#pragma comment(lib, "Winmm.lib") 

class hitmarker
{
	class player_hurt_listener
		: public IGameEventListener2
	{
	public:
		void start()
		{
			if (!g_pGameEventManager->AddListener(this, "player_hurt", false)) {
				throw misc::exception("Failed to register the event");
			}
		}
		void stop()
		{
			g_pGameEventManager->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			hitmarker::singleton()->on_fire_event(event);
		}
		int GetEventDebugID(void) override
		{
			return EVENT_DEBUG_ID_INIT /*0x2A*/;
		}
	};
public:
	static hitmarker* singleton()
	{
		static hitmarker* instance = new hitmarker;
		return instance;
	}

	void initialize()
	{
		_listener.start();
	}

	void on_fire_event(IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "player_hurt")) {
			int attacker = event->GetInt("attacker");
			if (g_pEngine->GetPlayerForUserID(attacker) == g_pEngine->GetLocalPlayer()) {
				//g_pSurface->PlaySound doesnt work for some reason
				PlaySoundA(_soundFileName, NULL, SND_ASYNC);
				_flHurtTime = g_pGlobalVars->curtime;
			}
		}
	}

	void on_paint()
	{
		auto curtime = g_pGlobalVars->curtime;
		auto lineSize = 8;
		if (_flHurtTime + 0.25f >= curtime) {
			int screenSizeX, screenCenterX;
			int screenSizeY, screenCenterY;
			g_pEngine->GetScreenSize(screenSizeX, screenSizeY);

			screenCenterX = screenSizeX / 2;
			screenCenterY = screenSizeY / 2;

			g_pSurface->DrawSetColor(200, 200, 200, 255);
			g_pSurface->DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
			g_pSurface->DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
			g_pSurface->DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
			g_pSurface->DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));
		}
	}

private:
	player_hurt_listener    _listener;
	const char*             _soundFileName = "csgo\\sound\\hitsound.wav";
	float                   _flHurtTime;
};