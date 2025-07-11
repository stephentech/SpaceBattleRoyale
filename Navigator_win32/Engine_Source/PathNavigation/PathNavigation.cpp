#include "stdafx.h"
#include "PathNavigator.h"
#include "../game.h"

#include "../object.h"
#include "../Sun.h"
#include "../SceneGraph/ObjectManager.h"


PathNavigation* PathNavigation::GetInstance()
{
	static PathNavigation sInstance;
	return &sInstance;
}

PathNavigation::PathNavigation()
	: mArrPlayArea(NULL)
	, mDivX(0)
	, mDivY(0)
{
	Game::GetInstance().GetGameEventListener().AddEventListener(this);
}

PathNavigation::~PathNavigation()
{
	DestroyAllPaths();

	if(mArrPlayArea)
	{
		for(int iIndexX = 0; iIndexX < mDivY; iIndexX ++)
			delete[] mArrPlayArea[iIndexX];
		delete[] mArrPlayArea;
	}

	Game::GetInstance().GetGameEventListener().RemoveEventListener(this);
}

void PathNavigation::Initialize()
{
	mDivX = 20;
	mDivY = 15;

	float fBlockWidth	= GetWidthOfBlock();
	float fBlockHeight	= GetHeightOfBlock(); 

	// allocate memory.
	mArrPlayArea = new Block*[mDivX];
	for(int iIndexX = 0; iIndexX<mDivX; iIndexX ++)
		mArrPlayArea[iIndexX] = new Block[mDivY];

	for(int iIndexX=0; iIndexX<mDivX; iIndexX++)
		for(int iIndexY=0; iIndexY<mDivY; iIndexY++)
		{
			Block* pBlock = &mArrPlayArea[iIndexX][iIndexY];

			pBlock->eBlockState = Block::eTraversable;
			pBlock->startX	= (iIndexX * fBlockWidth);
			pBlock->startY	= (iIndexY * fBlockHeight);
			pBlock->width	= fBlockWidth;
			pBlock->height	= fBlockHeight;
		}
}

int PathNavigation::GetWidthOfBlock()
{
	int x, y, width, height;
	Game::GetInstance().GetPlayableSpace(x,y,width, height);

	return (width / mDivX);
}

int PathNavigation::GetHeightOfBlock()
{
	int x, y, width, height;
	Game::GetInstance().GetPlayableSpace(x,y,width, height);

	return (height / mDivY);
}

// event handler
bool PathNavigation::OnEvent(EventBase* aEvent)
{
	if(SunSpawnEvent *sunSpawnEvt = dynamic_cast<SunSpawnEvent *>(aEvent) )
	{
		float fBlockWidth	= GetWidthOfBlock();
		float fBlockHeight	= GetHeightOfBlock(); 

		int iIndexX = (sunSpawnEvt->vPos.x) / fBlockWidth;
		int iIndexY = (sunSpawnEvt->vPos.y) / fBlockHeight;

		int iRadiusX = (int)(sunSpawnEvt->fRadius / fBlockWidth)  + 1;
		int iRadiusY = (int)(sunSpawnEvt->fRadius / fBlockHeight) + 1;
		for(int i = iIndexX - iRadiusX; i <= iIndexX + iRadiusX; i ++)
			for(int j = iIndexY - iRadiusY; j <= iIndexY + iRadiusY; j ++)
			{
				if(IsBlockIndexValid(i, j) )
					mArrPlayArea[i][j].eBlockState = Block::eOpaque;
			}
	}
	return false;
}

bool PathNavigation::IsPositionSafe(CVector aPos)
{
	float fBlockWidth	= GetWidthOfBlock();
	float fBlockHeight	= GetHeightOfBlock(); 

	int iIndexX = (aPos.x) / fBlockWidth;
	int iIndexY = (aPos.y) / fBlockHeight;

	if( IsBlockIndexValid(iIndexX, iIndexY) )
		return mArrPlayArea[iIndexX][iIndexY].eBlockState == PathNavigation::Block::eTraversable;

	return false;
}

bool PathNavigation::IsBlockIndexValid(int aX, int aY)
{
	return	(	0 <= aX && aX < mDivX 
			&&	0 <= aY && aY < mDivY);
}

void PathNavigation::DrawNavigationMeshInfo(HDC hdc)
{
	Game::GetInstance().ChangeDrawColorTo(0x00bbbbbb);

	// draw the vertical lines
	for(int iIndexX=0; iIndexX<mDivX; iIndexX++)
	{
		Block *pBlockStart	= &mArrPlayArea[iIndexX][0];
		Block *pBlockend	= &mArrPlayArea[iIndexX][mDivY - 1];

		MoveToEx(hdc,	pBlockStart->startX, pBlockStart->startY, 0);
		LineTo(hdc,		pBlockend->startX, pBlockend->startY + pBlockend->height);
	}

	// draw the horizontal lines
	for(int iIndexY=0; iIndexY<mDivY; iIndexY++)
	{
		Block *pBlockStart	= &mArrPlayArea[0][iIndexY];
		Block *pBlockend	= &mArrPlayArea[mDivX-1][iIndexY];

		MoveToEx(hdc,	pBlockStart->startX, pBlockStart->startY, 0);
		LineTo(hdc,		pBlockend->startX + pBlockend->width, pBlockend->startY);
	}

	// mark the opaque blocks
	for(int iIndexX=0; iIndexX<mDivX; iIndexX++)
	{
		for(int iIndexY=0; iIndexY<mDivY; iIndexY++)
		{
			Block* pBlock = &mArrPlayArea[iIndexX][iIndexY];
			if(pBlock->eBlockState == Block::eOpaque)
			{
				MoveToEx(hdc,	pBlock->startX, pBlock->startY, 0);
				LineTo(hdc,		pBlock->startX + pBlock->width, pBlock->startY + pBlock->height);
			}
		}
	}

	// redraw all the paths selected.
	if(!mPaths.empty())
	{
		std::list<Path *>::iterator it = mPaths.begin();
		for(int iPathIndex=0; it != mPaths.end(); it ++, iPathIndex ++)
		{
			int blueColor = 0xbb * iPathIndex;
			Game::GetInstance().ChangeDrawColorTo(0x00bb2200 | blueColor);

			PathNavigation::Path *pPath = *it; //mPaths[iPathIndex];
			for(int iIndex=0; iIndex<pPath->mArrPathsPts.size(); iIndex ++)
			{
				PathNavigation::Block *pBlock = pPath->mArrPathsPts[iIndex];
				CVector vStart( pBlock->startX + 1, pBlock->startY + 1);
				Game::GetInstance().DrawRectangle( vStart, CVector(pBlock->width - 2, pBlock->height - 2) );
			}

			if(pPath->mPathsPts.size() > 1)
			{
				// draw the internal arrow indicating change in direction. // mPathsPts
				std::list<CVector>::iterator it = pPath->mPathsPts.begin();
				CVector vPrevPt = *it;
				it ++;

				for(; it != pPath->mPathsPts.end(); it ++)
				{
					CVector vPt = *it;// pPath->mPathsPts[iIndex];
					Game::GetInstance().DrawLine( vPrevPt, vPt);
					Game::GetInstance().DrawCircle( vPt, 3);

					vPrevPt = vPt;
				}
			}
		}
	}
}

PathNavigation::Path* PathNavigation::GetPathById(uint32 aId)
{
	if(!mPaths.empty())
	{
		std::list<Path *>::iterator it = mPaths.begin();
		for(; it != mPaths.end(); it ++)
			if((*it)->GetPathId() == aId)
				return *it;
	}

	return NULL;
}

void PathNavigation::GetBlockIndexValueFromPoint(int &aRetX, int &aRetY, CVector aPt)
{
	float fBlockWidth	= GetWidthOfBlock();
	float fBlockHeight	= GetHeightOfBlock(); 

	int iIndexFromX = (aPt.x) / fBlockWidth;
	int iIndexFromY = (aPt.y) / fBlockHeight;
	//if( IsBlockIndexValid( iIndexFromX, iIndexFromY ) )
	//{
	//	// if the point is not in valid range, set them as invalid indexed values.
	//	aRetX = aRetY = -1; 
	//	return;
	//}

	aRetX = iIndexFromX;
	aRetY = iIndexFromY;
}


PathNavigation::Path* PathNavigation::CreteNewPath(CVector aFrom, CVector aTo)
{
	int iIndexFromX, iIndexFromY;
	GetBlockIndexValueFromPoint( iIndexFromX, iIndexFromY, aFrom);

	int iIndexToX, iIndexToY;
	GetBlockIndexValueFromPoint( iIndexToX, iIndexToY, aTo);

	// verify to and from are valid.
	if(!IsBlockIndexValid( iIndexFromX, iIndexFromY ) || !IsBlockIndexValid( iIndexToX, iIndexToY ))
		return NULL;

	// check if the from point is opaque. 
	if(mArrPlayArea[iIndexFromX][iIndexFromY].eBlockState == Block::eOpaque)
	{
		// in this case look the nearest traversable block.
		return CreteNewPathToSafeZone(aFrom);
	}

	CVector vFinalPos(iIndexToX, iIndexToY);

	// flag matrix. to mark the blocks we have covered/traversed.
	int** blockParsedMat = new int*[mDivX];
	for(int i=0; i<mDivX; i++)
	{
		blockParsedMat[i] = new int[mDivY];
		for(int j=0; j< mDivY; j++)
			blockParsedMat[i][j] = 0;
	}

	std::vector<CVector> stack;
	stack.push_back( CVector(iIndexFromX, iIndexFromY) );

	PathNavigation::Path* pRetPath = NULL;
	while( !stack.empty() )
	{
		CVector vCurPos = (*(stack.end() - 1) );

		// check if the current pos is travversable or opaque.
		if(mArrPlayArea[(int)vCurPos.x][(int)vCurPos.y].eBlockState == Block::eOpaque)
		{
			stack.pop_back();
			continue;
		}

		// mark this block as parsed.
		blockParsedMat[(int)vCurPos.x][(int)vCurPos.y] = 1;

		CVector vDiff = vFinalPos - vCurPos;
		if(vDiff.GetLength() == 0)
		{
			// we have a path.
			// the stack is the path. populate the Path.
			pRetPath = new PathNavigation::Path;

			// add to the list of paths.
			mPaths.push_back(pRetPath);

			pRetPath->mStartPt = aFrom;
			pRetPath->mEndPt = aTo;

			// populate the path with stack value. debug purposes.
			for(int iIndex = 0; iIndex < stack.size(); iIndex++)
				pRetPath->mArrPathsPts.push_back( &mArrPlayArea[(int)stack[iIndex].x][(int)stack[iIndex].y] );


			if(stack.size() > 1)
			{
				// pRetPath->mPathsPts.push_back( aFrom );
			
				Block* pInitialBlock = &mArrPlayArea[(int)stack[0].x][(int)stack[0].y];
				CVector vInitialBlockMidPos( pInitialBlock->GetCenter() );
				
				CVector vDir(-0.707f,-0.707f);
				//vDir.Normalise();

				CVector vPrevBlockMidPos(vInitialBlockMidPos);

				// populate the path points except for the first and the last value, as that will be the start and final point itself.
				// also, add points which have a direction change in the path.
				for(int iIndex = 1; iIndex < stack.size(); iIndex++)
				{
					Block* pBlock = &mArrPlayArea[(int)stack[iIndex].x][(int)stack[iIndex].y];
					CVector vBlockMidPos( CVector(pBlock->startX, pBlock->startY) + CVector(pBlock->width, pBlock->height) / 2.0f );

					CVector vNewDir = vBlockMidPos - vPrevBlockMidPos;
					vNewDir.Normalise();
					if(vNewDir.DotProduct(vDir) != 1.0f)
					{
						pRetPath->mPathsPts.push_back( vPrevBlockMidPos );
						vDir = vNewDir;
					}

					vPrevBlockMidPos = vBlockMidPos;
				}

				pRetPath->mPathsPts.push_back( aTo );
			}

			stack.clear();
			break;
		}

		CVector vFixedVectors[eMax] = { CVector(0, -1),
										CVector(0, 1),
										CVector(-1, 0),
										CVector(1, 0) };


		vDiff.Normalise();
		float fDotPr[eMax];
		for(int i=0; i < eMax; i++)
			fDotPr[i] = vDiff.DotProduct(vFixedVectors[i]);

		float fDotPrCmp = 0.707f;
		bool bFoundNextDirection = false;
		for(int iTries = 0; iTries < eMax && !bFoundNextDirection; iTries ++) // try with increasing the dot pr range.
		{
			for(int i=0; i < eMax && !bFoundNextDirection; i++)
			{
				CVector vNextPossiblePos = vCurPos + vFixedVectors[i];
				if(	fDotPr[i] > fDotPrCmp 
					&& ((int)vNextPossiblePos.x >= 0 && (int)vNextPossiblePos.x < mDivX && (int)vNextPossiblePos.y >= 0 && (int)vNextPossiblePos.y < mDivY)
					&& mArrPlayArea[(int)vNextPossiblePos.x][(int)vNextPossiblePos.y].eBlockState == Block::eTraversable
					&& blockParsedMat[(int)vNextPossiblePos.x][(int)vNextPossiblePos.y] == 0)
				{
					bFoundNextDirection = true;
					stack.push_back(vNextPossiblePos);
				}
			}

			if(iTries == 0)
				fDotPrCmp = 0.0f; // incase we dont find the next direction in the first run.
			else if(iTries == 1)
				fDotPrCmp = -0.707f; // incase we dont find the next direction in the second run.
			else if(iTries == 2)
				fDotPrCmp = -2.0f; // final attempt. look for any adjecent blocks which can be traversed and not already parsed..
		}

		if(!bFoundNextDirection)
			stack.pop_back();

	}

	// clear the matrix.
	for(int i=0; i<mDivX; i++)
		delete[] blockParsedMat[i];
	delete[] blockParsedMat;
	
	return pRetPath;
}

PathNavigation::Path* PathNavigation::CreteNewPathToSafeZone(CVector aFrom)
{
	PathNavigation::Path* pRetPath = NULL;

	int iIndexFromX, iIndexFromY;
	GetBlockIndexValueFromPoint(iIndexFromX, iIndexFromY, aFrom);

	if(mArrPlayArea[iIndexFromX][iIndexFromY].eBlockState == Block::eOpaque)
	{
		CVector vIndexedFrom(iIndexFromX, iIndexFromY);

		CVector vFixedVectors[eMax] = { CVector(0, -1),
										CVector(0, 1),
										CVector(1, 0),
										CVector(-1, 0) };

		int iLowestCount = -1;
		int iDirectionVector = eMax;
		for(int i=0; i<eMax; i++)
		{
			CVector currIndexPos = vIndexedFrom;
			int iOpaqueCells = 0;
			bool bReachedEndOfWorld = false;
			while(mArrPlayArea[(int)currIndexPos.x][(int)currIndexPos.y].eBlockState == Block::eOpaque)
			{
				iOpaqueCells ++;
				currIndexPos = currIndexPos + vFixedVectors[i];

				if(!IsBlockIndexValid((int)currIndexPos.x, (int)currIndexPos.y))
				{
					// reached end of the world.
					bReachedEndOfWorld = true;
					break;
				}
			}

			if(!bReachedEndOfWorld && (iLowestCount == -1 || iOpaqueCells < iLowestCount ))
			{
				iLowestCount = iOpaqueCells;
				iDirectionVector = i;
			}
		}

		if(iLowestCount != -1)
		{
			pRetPath = new PathNavigation::Path;

			// add to the list of paths.
			mPaths.push_back(pRetPath);

			// populate the path with stack value. debug purposes.
			CVector currIndexPos = vIndexedFrom + vFixedVectors[iDirectionVector];
			while(mArrPlayArea[(int)currIndexPos.x][(int)currIndexPos.y].eBlockState == Block::eOpaque)
			{
				pRetPath->mPathsPts.push_back( mArrPlayArea[(int)currIndexPos.x][(int)currIndexPos.y].GetCenter() );
				currIndexPos = currIndexPos + vFixedVectors[iDirectionVector];
			}

			pRetPath->mPathsPts.push_back( mArrPlayArea[(int)currIndexPos.x][(int)currIndexPos.y].GetCenter() );

			pRetPath->mStartPt = aFrom;
			pRetPath->mEndPt = mArrPlayArea[(int)currIndexPos.x][(int)currIndexPos.y].GetCenter();
		}
	}

	return pRetPath;
}

void PathNavigation::DestroyPath(uint32 aId)
{
	PathNavigation::Path* pPath = GetPathById(aId);
	if(pPath)
	{
		mPaths.remove(pPath);

		pPath->mArrPathsPts.clear();
		delete pPath;
	}
}

void PathNavigation::DestroyAllPaths()
{
	std::list<Path *>::reverse_iterator it = mPaths.rbegin();
	for(; it != mPaths.rend(); it ++)
	{
		PathNavigation::Path *pPath = *it;
		delete pPath;
	}

	mPaths.clear();
}

////////////////////////////////////////////////////////////////////////////////////////
// PathNavigation::Block

CVector	PathNavigation::Block::GetCenter()
{
	return CVector(startX + width / 2, startY + height / 2);
}

////////////////////////////////////////////////////////////////////////////////////////
// PathNavigation::Path

uint32 PathNavigation::Path::sPathIdCounter = 0;

PathNavigation::Path::Path()
{
	mPathId = sPathIdCounter ++;
}

PathNavigation::Path::~Path()
{
	//mPathsPts.clear();

	if(mArrPathsPts.size() > 0)
	{
		for(int i=0; i<mArrPathsPts.size(); i++)
			mArrPathsPts[i] = NULL;
		//mArrPathsPts.clear();
	}
}

uint32 PathNavigation::Path::GetPathId()
{
	return mPathId;
}

CVector PathNavigation::Path::GetDirection(CVector aCurrPos)
{
	if(mPathsPts.size() > 1)
	{
		std::list<CVector>::iterator it = mPathsPts.begin();
		std::list<CVector>::iterator it2 = mPathsPts.begin();
		it2 ++;
		return (*it2 - *it);
	}
	
	return *mPathsPts.begin() - aCurrPos;
}

CVector PathNavigation::Path::GetMoveToPt()
{
	return *mPathsPts.begin();
}

void PathNavigation::Path::ReachedPoint()
{
	mPathsPts.remove( *mPathsPts.begin() );
}

bool PathNavigation::Path::IsValid()
{
	if(mPathsPts.size())
		return true;

	return false;
}