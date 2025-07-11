#include "stdafx.h"

#include "ShipControllerAI.h"
#include "Ship.h"
#include "game.h"
#include "PathNavigation\PathNavigator.h"

CShipControllerAI::CShipControllerAI(CShip* aShip)
	: CShipControllerBase(aShip)
{
	mCurrentCornerToReach = BOTTOM_LEFT;
	mPathId = -1;
}

ENextMove CShipControllerAI::ThinkNextMove()
{
	ENextMove retMove = eNM_DoNothing;

	//std::list<CCelestialObject *> objList = gGame.GetListOfObjectsOfType( CObjectIDManager::GetInstance().GetObjectTypeID("Bullet") );
	//if(objList.size() > 0)
	{
		CVector nextPt(-1,-1);
		if(mPathId != -1)
		{
			PathNavigation::Path *pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
			if(pPath && pPath->IsValid())
				nextPt = pPath->GetMoveToPt();
		}

		// basic movement AI: move from corner to corner.
		if(nextPt.x == -1)
		{
			int x, y, width, height;
			Game::GetInstance().GetPlayableSpace(x,y,width, height);

			switch(mCurrentCornerToReach)
			{
			case TOP_LEFT:
				mCurrentCornerToReach = TOP_RIGHT;
				nextPt = CVector(width - 10, 10);
				break;

			case TOP_RIGHT:
				mCurrentCornerToReach = BOTTOM_RIGHT;
				nextPt = CVector(width - 10, height - 10);
				break;

			case BOTTOM_RIGHT:
				mCurrentCornerToReach = BOTTOM_LEFT;
				nextPt = CVector(10, height - 10);
				break;

			case BOTTOM_LEFT:
				mCurrentCornerToReach = TOP_LEFT;
				nextPt = CVector(10, 10);
				break;
			}

			PathNavigation::Path *pPath = PathNavigation::GetInstance()->CreteNewPath( mShip->GetPos(), nextPt );
			if(pPath && pPath->IsValid())
			{
				mPathId = pPath->GetPathId();
				nextPt = pPath->GetMoveToPt();
			}
			else
			{
				mPathId = -1;
				return retMove;
			}
		}

		CVector vecShipToTarget (nextPt - mShip->GetPos());
		if( vecShipToTarget.GetLength() < 50.0f)
		{
			if(mPathId != -1)
			{
				PathNavigation::Path *pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
				if(pPath && pPath->IsValid())
				{
					pPath->ReachedPoint();
				}
				else //if(!pPath->IsValid())
				{
					PathNavigation::GetInstance()->DestroyPath(mPathId);
					nextPt.x = nextPt.y = mPathId = -1;
					return retMove;
				}
			}
			else
			{
				nextPt.x = nextPt.y = mPathId = -1;
				return retMove;
			}
		}

		vecShipToTarget.Normalise();
		CVector vecShipFacingDir(mShip->GetFacingDir());
		CVector vecShipVelDir(mShip->mVelocity);

		// turn. crossPr(vecShipFacingDir, vecShipToTarget)
		// dot Pr. Find angle by which to turn.
		float fCrossPrVal = (vecShipFacingDir.x * vecShipToTarget.y) - (vecShipToTarget.x * vecShipFacingDir.y);
		if(fCrossPrVal > 0.0f) 
			retMove = (ENextMove) (retMove | eNM_SteerRight);
		else if(fCrossPrVal < 0.0f) 
			retMove = (ENextMove) (retMove | eNM_SteerLeft);

		float fDotPrVal = vecShipFacingDir.DotProduct(vecShipToTarget);
		if(fDotPrVal > 0.95f)
			retMove = (ENextMove) (retMove | eNM_Accelerate);
		else
			retMove = (ENextMove) (retMove | eNM_Deaccelerate);
	}
	return retMove;
}

bool CShipControllerAI::OnEvent(EventBase*)
{
	return false;
}

void CShipControllerAI::DrawControllerInfo()
{
	if(mPathId != -1)
	{
		PathNavigation::Path *pPath = PathNavigation::GetInstance()->GetPathById(mPathId);
		if(pPath && pPath->IsValid() )
		{
			Game::GetInstance().DrawLine( mShip->GetPos(), pPath->GetMoveToPt() );
		}
	}
}