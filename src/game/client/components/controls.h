/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_COMPONENTS_CONTROLS_H
#define GAME_CLIENT_COMPONENTS_CONTROLS_H
#include <SDL_joystick.h>
#include <base/system.h>
#include <base/vmath.h>
#include <game/client/component.h>

class CControls : public CComponent
{
    bool IsFrozen();
    bool IsGrounded();
    int GetNearestID() {
        int LocalClientID = m_pClient->m_LocalIDs[g_Config.m_ClDummy];
        float LastDistance = -1;
        int NearestID = -1;
        vec2 LocalPos = m_pClient->m_aClients[LocalClientID].m_Predicted.m_Pos;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            vec2 PlayerPos = m_pClient->m_aClients[i].m_Predicted.m_Pos;
            float Distance = distance(LocalPos, PlayerPos) < LastDistance;

            if (i == LocalClientID) {
                continue;
            }

            if (NearestID == -1 || Distance < LastDistance) {
                LastDistance = Distance;
                NearestID = i;
            }
        }
        return NearestID;
    };
    int LocalID() { return m_pClient->m_LocalIDs[g_Config.m_ClDummy]; };
    vec2 GetPos(int ClientID) { return m_pClient->m_aClients[ClientID].m_Predicted.m_Pos; };
    int HookedID(int ClientID) { return m_pClient->m_aClients[ClientID].m_Predicted.m_HookedPlayer; }

    int m_FreezeStart;
    float m_LastWalk;

public:
	vec2 m_MousePos[NUM_DUMMIES];
	vec2 m_TargetPos[NUM_DUMMIES];
	float m_OldMouseX;
	float m_OldMouseY;
	SDL_Joystick *m_Joystick;
	bool m_JoystickFirePressed;
	bool m_JoystickRunPressed;
	int64 m_JoystickTapTime;

	SDL_Joystick *m_Gamepad;
	bool m_UsingGamepad;

	int m_AmmoCount[NUM_WEAPONS];

	CNetObj_PlayerInput m_InputData[NUM_DUMMIES];
	CNetObj_PlayerInput m_LastData[NUM_DUMMIES];
	int m_InputDirectionLeft[NUM_DUMMIES];
	int m_InputDirectionRight[NUM_DUMMIES];
	int m_ShowHookColl[NUM_DUMMIES];
	int m_ResetDummy[NUM_DUMMIES];
	int m_LastDummy;
	int m_OtherFire;

	CControls();

	virtual void OnReset();
	virtual void OnRelease();
	virtual void OnRender();
	virtual void OnMessage(int MsgType, void *pRawMsg);
	virtual bool OnMouseMove(float x, float y);
	virtual void OnConsoleInit();
	virtual void OnPlayerDeath();

	int SnapInput(int *pData);
	void ClampMousePos();
	void ResetInput(int Dummy);
};
#endif
