#ifndef __WINGUI_IMAGE_LIST_H__
#define __WINGUI_IMAGE_LIST_H__

extern HINSTANCE _hAppInstance;

namespace wingui {

class ImageList
{
public:
	ImageList () :
		_hImageList(NULL)
	{ }

	ImageList (int BmpCx, int BmpCy, int *bmpResIds, size_t count) :
		_hImageList(NULL)
	{
		Create(BmpCx, BmpCy, bmpResIds, count);
	}
	
	inline HIMAGELIST GetHandle() const { return _hImageList; }
	
	BOOL Create (int BmpCx, int BmpCy, int *bmpResIds, size_t count)
	{	
		if ((_hImageList = ImageList_Create(BmpCx, BmpCy, ILC_COLOR32 /*| LR_CREATEDIBSECTION */, (int)count, 0)) == NULL) {
			return FALSE; 
		}

		ImageList_SetBkColor(_hImageList, RGB(0xFF,0xFF,0xFF));

		for (int i = 0; i < (int)count; i++) {
			HBITMAP hbmp = LoadBitmap (_hAppInstance, MAKEINTRESOURCE(bmpResIds[i])); 
			ImageList_Add (_hImageList, hbmp, (HBITMAP)NULL); 
			DeleteObject (hbmp); 
		}

		if (ImageList_GetImageCount(_hImageList) < (int)count) {
			return FALSE;
		}

		return TRUE;
	}

	void Destroy () {
		if (_hImageList != NULL) {
			ImageList_Destroy (_hImageList);
		}
	}
	
protected:
	HIMAGELIST _hImageList;
};

}; // wingui namespace

#endif // !__WINGUI_IMAGE_LIST_H__
