
#include "stdafx.h"
#include "ShipControllerPlayer.h"

CShipControlleerPlayer::CShipControlleerPlayer(CShip *aShip)
	: CShipControllerBase(aShip)
{
	mShipControllerType = eSCT_Player;
}

ENextMove CShipControlleerPlayer::ThinkNextMove()
{
	ENextMove retVal = (ENextMove)0;
	if (GetKeyState(VK_LEFT) & 0x800)
	{
		retVal = (ENextMove)(retVal | eNM_SteerLeft);
	}
	if (GetKeyState(VK_RIGHT) & 0x800)
	{
		retVal = (ENextMove)(retVal | eNM_SteerRight);
	}
	
	if (GetKeyState(VK_UP) & 0x800)
	{
		retVal = (ENextMove)(retVal | eNM_Accelerate);
	}
	if (GetKeyState(VK_DOWN) & 0x800)
	{
		retVal = (ENextMove)(retVal | eNM_Deaccelerate);
	}
 
	if (GetKeyState(VK_SPACE) & 0x800)
	{
		retVal = (ENextMove)(retVal | eNM_FireBullet);
	}

	if (GetKeyState(VK_CONTROL) & 0x800)
	{
		retVal = (ENextMove)(retVal | eNM_FireMissile);
	}

	return retVal;
}

bool CShipControlleerPlayer::OnEvent(EventBase*)
{
	return false;
}