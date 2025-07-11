

#ifndef __PATHNAVIGATION_H__
#define __PATHNAVIGATION_H__

#include "../Event/EventBase.h"

// theory: The entire play area is to be divided into small blocks. Each block will store its level of height.
class PathNavigation : public EventListener
{
	
public:

	enum {
			eUp = 0,
			eDown,
			eRight,
			eLeft,

			eMax
		};

	class Block
	{
	public:
		enum EBlockAvailablity
		{
			eOpaque = 0,
			eTraversable
		} eBlockState;

		float startX, startY;
		float width, height;

		CVector		GetCenter();
	};

	class Path
	{
	public:
		Path();
		~Path();

		uint32		GetPathId();
		CVector		GetMoveToPt();
		CVector		GetDirection(CVector aCurrPos);
		void		ReachedPoint();

		bool		IsValid();


	//private:
		static uint32 sPathIdCounter;
		uint32 mPathId;
		std::list<CVector> mPathsPts;
		CVector mStartPt, mEndPt;

		// for debug info.
		std::vector<Block*> mArrPathsPts;
	};

	static PathNavigation* GetInstance();
	void Initialize();

	~PathNavigation();

	// event handler
	virtual bool OnEvent(EventBase*);

	bool IsPositionSafe(CVector aPos);
	bool IsBlockIndexValid(int aX, int aY);

	Path* GetPathById(uint32);
	void GetBlockIndexValueFromPoint(int &aRetX, int &aRetY, CVector aPt);

	// create and destroy paths.
	Path* CreteNewPath(CVector aFrom, CVector aTo);
	Path* CreteNewPathToSafeZone(CVector aFrom);
	void DestroyPath(uint32);
	void DestroyAllPaths();

	void DrawNavigationMeshInfo(HDC hdc);

protected:
	int GetWidthOfBlock();
	int GetHeightOfBlock();

private:
	PathNavigation();

	// access this as PlayArea[x][y]
	Block** mArrPlayArea;
	int mDivX;
	int mDivY;

	std::list<Path *> mPaths;
};

#endif // __PATHNAVIGATION_H__