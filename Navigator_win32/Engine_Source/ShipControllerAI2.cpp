#include "stdafx.h"

#include "ShipControllerAI2.h"
#include "Ship.h"
#include "game.h"
#include "SceneGraph\ObjectManager.h"
#include "PathNavigation\PathNavigator.h"

#define THRESHOLD_DIST_TO_FIRE 500

CShipControllerAI2::CShipControllerAI2(CShip* aShip)
	:	CShipControllerBase(aShip),
		mShipIDToAttack(0),
		mAIState(eAI2State_Find),
		mPathId (-1)
{	
}

CShipControllerAI2::~CShipControllerAI2()
{
	if(mPathId != -1)
	{
		PathNavigation::Path* pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
		if(pPath)
			PathNavigation::GetInstance()->DestroyPath(pPath->GetPathId());

	}
}

ENextMove CShipControllerAI2::ThinkNextMove()
{
	ENextMove retMove = eNM_DoNothing;

	// if in danger, escape:
	if( PathNavigation::GetInstance()->IsPositionSafe( mShip->GetPos() ) == false )
	{
		mAIState = eAI2State_DangerZone;
	}


	switch(mAIState)
	{
	case eAI2State_Find:
		{
			if(mShipIDToAttack == 0)
				mShipIDToAttack = mShip->GetClosestShipInSight();

			if(mShipIDToAttack != 0)
			{
				InvalidatePath();
				mAIState = eAI2State_Attack;
			}
			else
			{
				// move to a random location.
				if(mPathId == -1)
				{
					CVector newPt = Game::GetInstance().GetSafeLocationSpawnShip();
					CreateNewPath(newPt);
				}

				retMove = ENextMove( retMove | MoveOnPath() );
			}
		}
		break;

	case eAI2State_FindAttacker:
		{
			if(mShipIDToAttack == 0)
				mShipIDToAttack = mShip->GetClosestShipInSight();

			if(mShipIDToAttack != 0)
			{
				mAIState = eAI2State_Attack;
			}
			else
			{
				CVector vecToShip = mLastKnownAttackPt - mShip->GetPos();
				vecToShip.Normalise();

				CVector vecMyFacingDir = mShip->GetFacingDir();
				float fCrossPr = vecToShip.x * vecMyFacingDir.y - vecMyFacingDir.x * vecToShip.y;
				if(fCrossPr > 0.0)
					retMove = (ENextMove)(retMove | eNM_SteerLeft);
				else if(fCrossPr < 0.0)
					retMove = (ENextMove)(retMove | eNM_SteerRight);

				// incase we've turned around and we do not have a target.
				float fDotPr = vecToShip.DotProduct(vecMyFacingDir);
				if(fDotPr > 0.9f)
					mAIState = eAI2State_Find;

				retMove = (ENextMove)(retMove | eNM_Deaccelerate);
			}
		}
		break;

	case eAI2State_Attack:
		{
			CVector vMoveTo(-1,-1);

			// check whether the ship we're attacking is still in visible range.
			if(mShipIDToAttack == 0 || mShip->IsShipVisible(mShipIDToAttack) == false)
			{
				mShipIDToAttack = 0;
				mAIState = eAI2State_Find;
				return retMove;
			}

			if(mShipIDToAttack != 0)
			{
				// now that we have a target, lets get it.

				CShip *pShipToAttack = (CShip *)Game::GetInstance().GetObjectManager()->GetObject(mShipIDToAttack);
				if(!pShipToAttack)
				{
					mShipIDToAttack = 0;
					return retMove;
				}

				CVector vecToShip = pShipToAttack->GetPos() - mShip->GetPos();
				float fDist = vecToShip.GetLength();
				vecToShip.Normalise();

				CVector vecMyFacingDir = mShip->GetFacingDir();
				float fDotPr = vecMyFacingDir.DotProduct(vecToShip);
				if( fDotPr > 0.8f ) 
				{
					retMove = (ENextMove)(retMove | eNM_Accelerate);

					if(fDist < THRESHOLD_DIST_TO_FIRE && fDotPr > 0.8)
						retMove = (ENextMove)(retMove | eNM_FireBullet);
				}
				else
					retMove = (ENextMove)(retMove | eNM_Deaccelerate);

				float fCrossPr = vecToShip.x * vecMyFacingDir.y - vecMyFacingDir.x * vecToShip.y;
				if(fCrossPr > 0.0)
					retMove = (ENextMove)(retMove | eNM_SteerLeft);
				else if(fCrossPr < 0.0)
					retMove = (ENextMove)(retMove | eNM_SteerRight);
			}
		}
		break;

	case eAI2State_Defend:
		break;

	case eAI2State_DangerZone:
		{
			CVector vNextPt;

			if(mPathId == -1)
			{
				CVector newPt = Game::GetInstance().GetSafeLocationSpawnShip();
				CreateNewPath(newPt);
			}

			retMove = ENextMove( retMove | MoveOnPath() );

			PathNavigation::Path *pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
			if(pPath &&  pPath->IsValid() )
			{
				if(PathNavigation::GetInstance()->IsPositionSafe( mShip->GetPos() ))
					mAIState = eAI2State_Find;
			}
		}
		break;
	}

	return retMove;
}

void CShipControllerAI2::CreateNewPath( CVector aToPos )
{
	InvalidatePath();

	PathNavigation::Path *pPath = PathNavigation::GetInstance()->CreteNewPath( mShip->GetPos(), aToPos );
	if(pPath && pPath->IsValid())
	{
		mPathId = pPath->GetPathId();
	}
	else
		delete pPath;
}

ENextMove CShipControllerAI2::MoveOnPath()
{
	ENextMove retMove = eNM_DoNothing;

	CVector vMoveTo(-1, -1); 
	PathNavigation::Path *pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
	if(pPath && pPath->IsValid())
		vMoveTo = pPath->GetMoveToPt();
	else
	{
		InvalidatePath();
		return retMove;
	}
	
	CVector vFaceDir(mShip->GetFacingDir());
	CVector vVec = vMoveTo - mShip->GetPos();

	// accelerate / deaccelerate
	float fDist = vVec.GetLength();
	vVec.Normalise();
	float fDotPr = vFaceDir.DotProduct( vVec );
	
	if(fDotPr > 0.6f) // > 0.95f )
		retMove = ENextMove(retMove | eNM_Accelerate);
	else 
		retMove = ENextMove( retMove | eNM_Deaccelerate);


	// turn left/right
	// in 2D the crossPr will be into the plane of screen or out.
	float fCrossPr = vVec.x * vFaceDir.y - vVec.y * vFaceDir.x;
	if(fCrossPr > 0.05f)
	{
		retMove = ENextMove( retMove | eNM_SteerLeft);
	}
	else if(fCrossPr < -0.05f 
			|| fDotPr < 0.0f) // failsafe condition: incase the facing is directly opposite to moveTo pos.
	{
		retMove = ENextMove( retMove | eNM_SteerRight);
	}

	// if we're close o the path, inform the PathNavigation system.
	if(fDist < 10.0f)
	{
		PathNavigation::Path* pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
		if(pPath)
			pPath->ReachedPoint();
	}

	return retMove;
}

void CShipControllerAI2::InvalidatePath()
{
	if(mPathId != -1)
		PathNavigation::GetInstance()->DestroyPath(mPathId);
	mPathId = -1;
}


bool CShipControllerAI2::OnEvent(EventBase* aEvent)
{
	if(ShipExplodeEvent *shipExplodeEvt = dynamic_cast<ShipExplodeEvent *>(aEvent) )
	{
		if(mShipIDToAttack && mShipIDToAttack == shipExplodeEvt->mID)
			mShipIDToAttack = 0;
	}
	return false;
}

void CShipControllerAI2::DrawControllerInfo()
{
	if(mPathId != -1)
	{
		PathNavigation::Path *pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
		if(pPath && pPath->IsValid() )
			Game::GetInstance().DrawLine(mShip->GetPos(), pPath->GetMoveToPt());
	}
}

void CShipControllerAI2::DetectedAttack(CVector aDir)
{
	if(mAIState == eAI2State_Find)
	{
		InvalidatePath();

		// turn towards the attacker
		mAIState = eAI2State_FindAttacker;
		mLastKnownAttackPt = mShip->GetPos() + aDir * 10;
	}
}