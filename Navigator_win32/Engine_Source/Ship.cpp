
#include "stdafx.h"
#include "Ship.h"

#include "game.h"
#include "CommonConversion.h"
#include "WeaponExplosives.h"

// physics.
#include "Physics\Physics.h"
#include "Physics\Circle.h"

#include "ShipControllerPlayer.h"
#include "ShipControllerAI.h"
#include "ShipControllerAI2.h"

#include "SceneGraph\ObjectManager.h"
//--------------------------------------------------------------------------------------------------------------
// Ship

CShip::CShip(CVector aPos)
: CCelestialObject()
, mController(NULL)
, mColor(0x00000000)
, mControllerType(eSCT_None)
{
	// populate the ships default values.
	Spawn(aPos);
	mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("ship");

	mPhysicsObject = (engPhysics::PhysicsObject *) new engPhysics::Circle(this, aPos, mShieldRadius); 
	if(mPhysicsObject)
	{
		if( engPhysics::Physics::GetInstance()->AddPhysicsObject(mPhysicsObject) == false)
			delete mPhysicsObject;
	}
}

CShip::~CShip()
{
	if(mController)
	{
		delete mController;
		mController = NULL;
	}
}


void CShip::Update(float aDT)
{
	if(IsDestroyed())
		return;

	// reset the following var every frame.
	mCollidedWithAnotherShipDisplacementDone = false;

	float timeDelta = aDT; //gGame.mTimer.GetTimeDelta();

	ENextMove eNextMove = eNM_DoNothing;
	if(mController)
		eNextMove = mController->ThinkNextMove();

	if(eNextMove & eNM_SteerLeft)
	{
		if(mController)
		{
			TurnShip(-STEERING_ANGLE * aDT, timeDelta);
		}
	}
	

	if(eNextMove & eNM_SteerRight)
	{
		if(mController)
		{
			TurnShip(STEERING_ANGLE * aDT, timeDelta);
		}
	}
	
			

	if(eNextMove & eNM_Accelerate)
	{
		mVelocity = mVelocity + mFacingDir * mAcceleration * timeDelta;
	}
	
	if(eNextMove & eNM_Deaccelerate)
	{
		CVector vel(mVelocity);

		if(vel.GetLength() > 0.0f)
		{
			vel.Normalise();
			mVelocity = mVelocity - vel * mDeacceleration * timeDelta; 
		}
	}

	if(eNextMove & eNM_FireBullet)
	{
		if(mCoolDownTimeBullet < 0.0f)
		{
			Game::GetInstance().GetObjectManager()->AddObject( new CBullet(mID, GetPos(), mFacingDir) );
			mCoolDownTimeBullet = 60.0f / mBulletFireRate;
		}
	}
	
	if(eNextMove & eNM_FireMissile)
	{
		if(mCountMissileLeft && mCoolDownTimeMissile < 0.0f)
		{
			Game::GetInstance().GetObjectManager()->AddObject( new CMissile(mID, GetPos(), mFacingDir) );
			mCoolDownTimeMissile = 60.0f / mMissileFireRate;

			mCountMissileLeft --;
		}
	}
	
	//if(eNextMove & eNM_DoNothing)
	//{
	//}

	if(mCoolDownTimeBullet >= 0.0)
		mCoolDownTimeBullet -= timeDelta;
	
	if(mCoolDownTimeMissile >= 0.0)
		mCoolDownTimeMissile -= timeDelta;

	if(mVelocity.GetLength() > mMaxSpeed)
	{
		mVelocity.Normalise();
		mVelocity = mVelocity * mMaxSpeed;
	}

	//fprintf(stderr, "\n Velocity : %f %f  :::", mVelocity.x, mVelocity.y);

	SetPos(GetPos() + mVelocity * timeDelta);

	
#if 1
	// bounce the ship back into level.
	int iX, iY, iWidth, iHeight;
	Game::GetInstance().GetPlayableSpace(iX, iY, iWidth, iHeight);

		CVector vPos(GetPos());
		if(vPos.x < 0)
		{	
			mVelocity.x *= -1.0f;
			vPos.x = 0 + 2;
		}
		else if(vPos.x > iWidth)
		{	
			mVelocity.x *= -1.0f;
			vPos.x = iWidth - 2;
		}

		if(vPos.y < 0)
		{
			mVelocity.y *= -1.0f;
			vPos.y = 0 + 2;
		}
		else if(vPos.y > iHeight)
		{
			mVelocity.y *= -1.0f;
			vPos.y = iHeight - 2;
		}
#endif

}

void CShip::TurnShip(float aSteering, float aDT)
{
	clamp(aSteering, -mMaxAngleTurnInOneSec, mMaxAngleTurnInOneSec);
	aSteering *= mAgility;
	float fTheta = DegreeToRadian(aSteering) * aDT * 50;
	float fCosTheta = cos(fTheta);
	float fSinTheta = sin(fTheta);

	mFacingDir = CVector(mFacingDir.x * fCosTheta - mFacingDir.y * fSinTheta, mFacingDir.x * fSinTheta + mFacingDir.y * fCosTheta);
	mVelocity = mFacingDir * mVelocity.GetLength();
	//mVelocity = CVector(mVelocity.x * fCosTheta - mVelocity.y * fSinTheta, mVelocity.x * fSinTheta + mVelocity.y * fCosTheta);
}
//--------------------------------------------------------------------------------------------------------------
// Draw

void CShip::Draw()
{
	if(IsDestroyed())
		return;

	CVector vPos(GetPos());
	Game &refGame = Game::GetInstance();

	// draw ship's Field of View
	CVector vVec = mFacingDir;
	vVec.RotateBy(DegreeToRadian(mFOVAngle/2.0f) );
	CVector vVec2(vVec);
	vVec2.RotateBy(DegreeToRadian(-mFOVAngle));

	refGame.ChangeDrawColorTo(0x00dedede);
	refGame.DrawLine(vPos, vPos + vVec * mFOVDist );
	refGame.DrawLine(vPos + vVec * mFOVDist, vPos + vVec2 * mFOVDist );
	refGame.DrawLine(vPos, vPos + vVec2 * mFOVDist );

	// any info the controller wants to be drawn.
	mController->DrawControllerInfo();

	// draw the shield
	if(mShieldHealth > 0.0f)
	{
		refGame.ChangeDrawColorTo(0x00ffaaaa);
		refGame.DrawCircle( GetPos(), mShieldRadius);
	}

	// draw the ship.
	refGame.ChangeDrawColorTo(0x00000000);
	refGame.DrawCircle( GetPos(), mShipPhysicsRadius);
	refGame.DrawLine(vPos, vPos + mFacingDir * 10 );

	DrawHealth();
}

void CShip::DrawHealth()
{
	Game &refGame = Game::GetInstance();
	int iIndicatorMeter = 0;
	int iColour = 0;
	if(mShieldHealth > 0)
	{
		// draw blue line
		iColour = 0x00ff0000;
		iIndicatorMeter = mShieldHealth;
	}
	else
	{
		// draw red line
		iColour = 0x000000ff;
		iIndicatorMeter = mHealth;
	}

	int iBarMaxLength = 30;
	int iBarLength = iBarMaxLength * (iIndicatorMeter / 100.0f);

	CVector vPos(GetPos() + CVector( 10, -15)); // offset the health bar a bit

	refGame.ChangeDrawColorTo(iColour);
	refGame.DrawBar( vPos, CVector(iBarLength, 5));
}

int CShip::GetColor()
{
	return mColor;
}

bool CShip::CanBeDeleted()
{
	if(CCelestialObject::CanBeDeleted() || mHealth <= 0.0f)
	{
		Explode();
		//return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------------
bool CShip::IsDestroyed()
{
	return mHealth <= 0.0f;
}

void CShip::Spawn(CVector aPos)
{
	SetPos(aPos);
	mVelocity = CVector(0,0);
	
	mFacingDir = CVector( RandomRange(-10, 10) * 0.1f, RandomRange(-10, 10) * 0.1f);
	mFacingDir.Normalise();

	// failsafe. if no facing direction, the ship will not move.
	if(mFacingDir.GetLength() == 0.0f)
		mFacingDir = CVector(1,0);

	mShieldRadius = 10.0f;
	mCollidedWithAnotherShipDisplacementDone = false;
	mShipPhysicsRadius = 3.0f;
	mCoolDownTimeBullet = 0.0f;
	mCoolDownTimeMissile = 0.0f;
	mCountMissileLeft = 100;
	mAcceleration = 10;
	mDeacceleration = 40;
	mMaxSpeed = 40;
	mMinHealthSpeed = 10;
	mHealth = 100;
	mShieldHealth = 100;
	mBulletFireRate = 90;
	mMissileFireRate = 10;
	mMaxAngleTurnInOneSec = 5;
	mAgility = 0.75f;
	mFOVDist = 200;
	mFOVAngle = 45;

	mExplosionEventGenerated = false;

	if( mPhysicsObject )
		((engPhysics::Circle *)mPhysicsObject)->SetRadius( mShieldRadius );
}

// Explode
void CShip::Explode()
{
	// force destroy this ship.
	mHealth = 0.0f;
	if(!mExplosionEventGenerated)
	{
		mExplosionEventGenerated = true;
		Game::GetInstance().GetGameEventListener().OnEvent( new ShipExplodeEvent(mID, mControllerType) );
	}
}

// set the controller type. fall back to basic AI.
void CShip::SetControllerType(EShipControllerType aType)
{
	mControllerType = aType;
	switch(mControllerType)
	{
	case eSCT_Player:
		mColor = 0x00ff0000;
		mController = new CShipControlleerPlayer(this);
		break;

	case eSCT_AI:
		mColor = 0x000002211;
		mController = new CShipControllerAI(this);
		break;

	case eSCT_AI2:
		mColor = 0x00000000;
		mController = new CShipControllerAI2(this);
		break;

	case eSCT_PlayerNetwork:
		break;
	};

}

// create an Object of Controller and pass in to the ship.
void CShip::SetControllerType(EShipControllerType aType, CShipControllerBase* aController)
{

}

void CShip::CollidedWith(CCelestialObject* aObj) 
{
	bool bShieldDestroyed = false;
	bool bInformControllerAttack = false;

	if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Sun") )
	{
		if(mShieldHealth > 0.0f)
		{
			// drop shield.
			mShieldHealth = -1.0f;
			bShieldDestroyed = true;
		}
		else 
			mHealth = -1.0f;
	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Bullet") )
	{
		CBullet* pBullet = (CBullet *)aObj;
		if(pBullet->mFiredFromShipID != mID)
		{
			// first damage the shield.
			// if shield is destroyed, carry over the blast effect to the Hull as well.
			float fImpactDamage = aObj->mVelocity.GetLength() / CBullet::MAX_VELOCITY;
			float fRemainingDamage = 0.0f;
			if(mShieldHealth > 0.0f)
			{
				float fShieldDamage = pBullet->GetShieldDamage();
				mShieldHealth -= fShieldDamage * fImpactDamage;
				if(mShieldHealth < 0.0f)
				{
					bShieldDestroyed = true;
					fRemainingDamage = -mShieldHealth;
				}
			}
			else 
				fRemainingDamage = fImpactDamage;
			
			if(fRemainingDamage > 0.0f)
			{
				float fShieldDamage = pBullet->GetShipHullDamage();
				mHealth -= fImpactDamage * fShieldDamage;
			}
		}

		// inform the ShipController that it was attacked.
		bInformControllerAttack = true;

	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Missile") )
	{
		CMissile *pMissile = (CMissile *)aObj;
		if(pMissile->mFiredFromShipID != mID)
		{
			if( mShieldHealth > 0.0f)
			{
				float fShieldDamage = pMissile->GetShieldDamage();
				mShieldHealth -= fShieldDamage;
				if(mShieldHealth <= 0.0f)
					bShieldDestroyed = true;
			}
			else
				mHealth = -1.0f;
		}

		// inform the ShipController that it was attacked.
		bInformControllerAttack = true;
	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("ship") )
	{
		if( mShieldHealth > 0.0f)
		{
			if(!mCollidedWithAnotherShipDisplacementDone)
			{
				CShip* pShip = (CShip *)aObj;
				pShip->mCollidedWithAnotherShipDisplacementDone = true;
				mCollidedWithAnotherShipDisplacementDone = true;

				CVector vecMeToShip = pShip->GetPos() - GetPos();
				CVector vecMyVel = mVelocity;
				CVector vecForDisplacement = vecMeToShip;
				vecForDisplacement.Normalise();

				vecMeToShip.Normalise();

				CVector vecShipToMe = vecMeToShip * (-1);
				CVector vecShipVel = pShip->mVelocity;

				vecMyVel.Normalise();
				vecShipVel.Normalise();
			
				float fDotPr = vecMyVel.DotProduct(vecMeToShip);
				float fDotPr2 = vecShipVel.DotProduct(vecShipToMe);
				// add the other objects velocity to mine by fDotPr Times.
			
				CVector vecMyVelCopy = mVelocity;
				mVelocity = mVelocity + (pShip->mVelocity * fDotPr2);
				pShip->mVelocity = pShip->mVelocity + (vecMyVelCopy * fDotPr);

				SetPos(GetPos() - vecForDisplacement );
				pShip->SetPos( pShip->GetPos() + vecForDisplacement );
			}
		}
		else
			mHealth = -1.0f;
	}

	mMaxSpeed = mMaxSpeed * (mHealth / 100.0f);
	if(mMaxSpeed < mMinHealthSpeed)
		mMaxSpeed = mMinHealthSpeed;

	if(bShieldDestroyed)
	{
		((engPhysics::Circle *)mPhysicsObject)->SetRadius( mShipPhysicsRadius );
	}

	// inform the ShipController that it was attacked.
	if(bInformControllerAttack && mController)
	{
		CVector attackDirection( aObj->mVelocity );//GetPos() - aObj->GetPos() );
		attackDirection.Normalise();
		mController->DetectedAttack( attackDirection );
	}

	//if( mHealth <= 0.0f )
	//	Explode();
		
}

ulong CShip::GetClosestShipInSight()
{
	// enable return 0 if you wish the ships not attack each other.
	//return 0;

	std::vector<ulong> ShipsInSight = GetShipsInSight();
	if(ShipsInSight.size() == 0)
		return 0;

	float fMinDist = -1;
	ulong retShipID = 0;
	for( std::vector<ulong>::iterator itShip = ShipsInSight.begin(); itShip != ShipsInSight.end(); itShip ++)
	{
		CShip *pShip = (CShip *)Game::GetInstance().GetObjectManager()->GetObject(*itShip);

		// if no such ship or ship is destroyed, no need inspecting it.
		if(!pShip || pShip->IsDestroyed() )
			continue;

		CVector vVec( pShip->GetPos() - GetPos() );
		float fDist = vVec.GetLength();
		if(fMinDist == -1 || fDist < fMinDist)
		{
			fMinDist = fDist;
			retShipID = *itShip;
		}
	}

	return retShipID;
}

std::vector<ulong> CShip::GetShipsInSight()
{
	std::vector<ulong> retVal;
	int iShipType = CObjectIDManager::GetInstance().GetObjectTypeID("ship");
	int iCount = Game::GetInstance().GetObjectManager()->GetCount( iShipType );
	if(iCount)
	{
		float fMinDist = mFOVDist;

		// get the list of objects.
		CCelestialObject** Objects = new CCelestialObject*[iCount];
		Game::GetInstance().GetObjectManager()->GetObjects(iShipType, Objects );

		for(int iIndex = 0; iIndex < iCount; iIndex ++)
		{
			CShip *pShip = (CShip *)Objects[iIndex];
			if( !pShip || pShip == this || pShip->IsDestroyed())
				continue;

			CVector vVec( Objects[iIndex]->GetPos() - GetPos() );
			float fDist = vVec.GetLength();
			vVec.Normalise();

			float fDot = GetFacingDir().DotProduct(vVec);
			if(fDot > 0.75f && fDist < fMinDist )
			{
				fMinDist = fDist;
				retVal.push_back( Objects[iIndex]->GetObjectID() );
			}
		}

		delete[] Objects;
	}

	return retVal;
}

bool CShip::IsShipVisible(ulong aShipID)
{
	std::vector<ulong> VisibleShips = GetShipsInSight();
	if(VisibleShips.size() == 0)
		return false;

	for(std::vector<ulong>::iterator itShipID = VisibleShips.begin(); itShipID != VisibleShips.end(); itShipID ++)
	{
		if(*itShipID == aShipID)
			return true;
	}
	
	return false;
}
