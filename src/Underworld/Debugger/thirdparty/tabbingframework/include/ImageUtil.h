
#ifndef __ImageUtil_h__
#define __ImageUtil_h__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
	#error ImageUtil.h requires atlapp.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error ImageUtil.h requires atlwin.h to be included first
#endif

#ifndef __ATLGDI_H__
	#error ImageUtil.h requires atlgdi.h to be included first
#endif

namespace ImageUtil
{
	enum eCheckbox
	{
		eCheckboxUnchecked,
		eCheckboxChecked,
		eCheckboxIndeterminate,
		eCheckboxLast
	};

	inline HBITMAP CreateCheckboxImage(HDC dcScreen,
		eCheckbox checkState,
		int cx, int cy,
		COLORREF transparentColor)
	{
#ifdef __ATLTHEME_H__
		UINT stateDTB = CBS_CHECKEDNORMAL;
		UINT stateDFC = (DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT);
		switch(checkState)
		{
		case eCheckboxUnchecked:
			stateDTB = CBS_UNCHECKEDNORMAL;
			stateDFC = (DFCS_BUTTONCHECK | DFCS_FLAT);
			break;
		case eCheckboxChecked:
			stateDTB = CBS_CHECKEDNORMAL;
			stateDFC = (DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT);
			break;
		case eCheckboxIndeterminate:
			stateDTB = CBS_MIXEDNORMAL;
			stateDFC = (DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_INACTIVE | DFCS_FLAT);
			break;
		}
#else
		UINT stateDFC = (DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT);
		switch(checkState)
		{
		case eCheckboxUnchecked:
			stateDFC = (DFCS_BUTTONCHECK | DFCS_FLAT);
			break;
		case eCheckboxChecked:
			stateDFC = (DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_FLAT);
			break;
		case eCheckboxIndeterminate:
			stateDFC = (DFCS_BUTTONCHECK | DFCS_CHECKED | DFCS_INACTIVE | DFCS_FLAT);
			break;
		}
#endif

		HBITMAP bitmap = NULL;
		CDC dc;
		dc.CreateCompatibleDC(dcScreen);
		if(!dc.IsNull())
		{
			CRect rcImage(0,0,cx,cy);
			bitmap = ::CreateCompatibleBitmap(dcScreen, rcImage.Width(), rcImage.Height());
			CBitmapHandle hOldBitmap = dc.SelectBitmap(bitmap);

			dc.FillSolidRect(&rcImage, transparentColor);
			rcImage.DeflateRect(1,1);
#ifdef __ATLTHEME_H__
			CTheme theme;
			if(theme.OpenThemeData(m_list, L"Button"))
			{
				theme.DrawThemeBackground(dc, BP_CHECKBOX, stateDTB, &rcImage);
			}
			else
			{
				dc.DrawFrameControl(&rcImage, DFC_BUTTON, stateDFC);
			}
#else
			dc.DrawFrameControl(&rcImage, DFC_BUTTON, stateDFC);
#endif
			dc.SelectBitmap(hOldBitmap);
		}

		return bitmap;
	}
};

#endif //__ImageUtil_h__
