/*
 * $Id: widget_helpers.h 2016/01/12 mohousch Exp $
 *
 * (C) 2003 by thegoodguy <thegoodguy@berlios.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __gui_widget_helpers_h__
#define __gui_widget_helpers_h__

#include <driver/fontrenderer.h>
#include <driver/framebuffer.h>

#include <system/localize.h>
#include <system/settings.h>

#include <driver/rcinput.h>


class CMenuTarget;
class CWidgetItem;
class CWidget;

// dimension helper
class CBox
{
	public:
		// Variables
		int iX;
		int iY;
		int iWidth;
		int iHeight;
		int iRadius;

		//
		inline CBox(){};
		inline CBox( const int _iX, const int _iY, const int _iWidth, const int _iHeight){iX =_iX; iY=_iY; iWidth =_iWidth; iHeight =_iHeight;};
		~CBox(){};
};

// CComponent
enum {
	CC_WINDOW,
	CC_ICON,
	CC_IMAGE,
	CC_LABEL,
	CC_TEXT,
	CC_SCROLLBAR,
	CC_PROGRESSBAR,
	CC_BUTTON,
	CC_HLINE,
	CC_VLINE,
	CC_FRAMELINE,
	CC_DETAILSLINE,
	CC_PIG,
	CC_GRID,
	CC_SLIDER,
	CC_TIME
};

class CComponent
{
	public:
		//
		CFrameBuffer *frameBuffer;
		CBox itemBox;
		
		int cc_type;
		
		CWidgetItem* parent;
		
		//
		CComponent(){frameBuffer = CFrameBuffer::getInstance(); parent = NULL;};
		virtual ~CComponent(){};
		
		//
		virtual void paint(void){};
		virtual void hide(void){};
		
		//
		virtual int getCCType(){return cc_type;};
		
		//
		virtual void setParent(CWidgetItem* w){parent = w;};
		virtual void setPosition(const int _x, const int _y, const int _width, const int _height)
		{
			itemBox.iX = _x;
			itemBox.iY = _y;
			itemBox.iWidth = _width;
			itemBox.iHeight = _height;
		};
		
		//
		virtual inline CBox getWindowsPos(void){return (itemBox);};
};

class CIcon : public CComponent
{
	public:
		int iWidth;
		int iHeight;
		std::string iconName;

		inline CIcon(){cc_type = CC_ICON;};
		
		void setIcon(const char* icon)
		{
			iconName = std::string(icon); 
			frameBuffer->getIconSize(iconName.c_str(), &iWidth, &iHeight);
		};
		
		inline CIcon(const char* icon)
		{
			iconName = std::string(icon); 
			CFrameBuffer::getInstance()->getIconSize(iconName.c_str(), &iWidth, &iHeight);
			
			cc_type = CC_ICON;
		};

		//
		void paint(const int x, const int y)
		{
			CFrameBuffer::getInstance()->paintIcon(iconName.c_str(), x, y);
		};
};

class CImage : public CComponent
{
	public:
		int iWidth;
		int iHeight;
		int iNbp;
		std::string imageName;

		inline CImage(){cc_type = CC_IMAGE;};

		void setImage(const char* image)
		{
			imageName = std::string(image); 
			frameBuffer->getSize(imageName, &iWidth, &iHeight, &iNbp);
		};

		inline CImage(const char* image)
		{
			imageName = std::string(image); 
			frameBuffer->getSize(imageName, &iWidth, &iHeight, &iNbp);
			
			cc_type = CC_IMAGE;
		};
		
		//
		void paint(const int x, const int y, const int dx, const int dy)
		{
			frameBuffer->displayImage(imageName.c_str(), x, y, dx, dy);
		};
};

// buttons
typedef struct button_label
{
	const char * button;
	neutrino_locale_t locale;
	const char * localename;
} button_label_struct;

typedef std::vector<button_label_struct> button_label_list_t;

// CButtons
class CButtons : public CComponent
{
	private:
		button_label_list_t buttons;
	public:
		CButtons(){buttons.clear(); cc_type = CC_BUTTON;};

		// foot buttons
		void paintFootButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label* const content);

		// head buttons right
		void paintHeadButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label * const content);
};

//CScrollBar
class CScrollBar : public CComponent
{
	private:
	public:
		CScrollBar(){cc_type = CC_SCROLLBAR;};
		virtual ~CScrollBar(){};

		void paint(const int x, const int y, const int dy, const int NrOfPages, const int CurrentPage);
		void paint(CBox* position, const int NrOfPages, const int CurrentPage);
};

// CProgressbar
class CProgressBar : public CComponent
{
	private:
		short width;
		short height;
		unsigned char percent;
		short red, green, yellow;
		bool inverse;

	public:
		CProgressBar(int w, int h, int r = 40, int g = 100, int b = 70, bool inv = true);
		void paint(unsigned int x, unsigned int y, unsigned char pcr);
		void reset();
		int getPercent() { return percent; };
};

// detailsLine
class CItems2DetailsLine : public CComponent
{
	public:
		CItems2DetailsLine(){cc_type = CC_DETAILSLINE;};
		virtual ~CItems2DetailsLine(){};
		
		void paint(int x, int y, int width, int height, int info_height, int iheight, int iy);
		void clear(int x, int y, int width, int height, int info_height);
};

// CHline
class CHline : public CComponent
{
	public:
		//
		fb_pixel_t color;
		
		CHline();
		virtual ~CHline(){};
		
		//
		void setColor(fb_pixel_t col){color = col;};
		
		void paint(const int x, const int y, const int dx)
		{
			frameBuffer->paintHLineRel(x, dx, y, color);
		};
};

// CVline
class CVline : public CComponent
{
	public:
		//
		fb_pixel_t color;
		
		//
		CVline();
		virtual ~CVline(){};
		
		//
		void setColor(fb_pixel_t col){color = col;};
		
		void paint(const int x, const int y, const int dy)
		{
			frameBuffer->paintVLineRel(x, y, dy, color);
		};
};

// CFrameline
class CFrameLine : public CComponent
{
	public:
		//
		fb_pixel_t color;
		
		//
		CFrameLine();
		virtual ~CFrameLine(){};
		
		//
		void setColor(fb_pixel_t col){color = col;};
		
		void paint(const int x, const int y, const int dx, const int dy)
		{
			frameBuffer->paintFrameBox(x, y, dx, dy, color);
		};
};

// CLabel
class CLabel : public CComponent
{
	public:
		uint8_t color;
		CFont* font;
		std::string label;
		bool paintBG;
		bool utf8;
		
		CLabel();
		virtual ~CLabel(){};
		
		//
		void setColor(uint8_t col){color = col;};
		void setFont(CFont *f){font = f;};
		void setText(const char* text){label = text;};
		void enablePaintBG(){paintBG = true;};
		
		void paint(const int x, const int y, const int dx)
		{
			font->RenderString(x, y + font->getHeight(), dx, label.c_str(), color, 0, utf8, paintBG);
		};
		
		int getHeight(){return itemBox.iHeight;};
};

// CWidgetItem
enum {
	WIDGET_ITEM_HEAD = 0,
	WIDGET_ITEM_FOOT,
	WIDGET_ITEM_LISTBOX,
	WIDGET_ITEM_FRAMEBOX,
	WIDGET_ITEM_LISTFRAME
};

class CWidgetItem
{
	public:
		CBox itemBox;

		int itemType;
		bool inFocus;
		bool paintDate;
		std::string actionKey;
		
		CWidget* parent;

		CWidgetItem(){inFocus = true; actionKey = ""; parent = NULL;};
		virtual ~CWidgetItem(){};

		virtual bool isSelectable(void){return false;}
		virtual bool hasItem(){return false;};

		virtual void paint(){};
		virtual void hide(){};

		virtual void scrollLineDown(const int lines = 1){};
		virtual void scrollLineUp(const int lines = 1){};
		virtual void scrollPageDown(const int pages = 1){};
		virtual void scrollPageUp(const int pages = 1){};
		virtual void swipLeft(){};
		virtual void swipRight(){};

		virtual void setOutFocus(bool focus = true){inFocus = !focus;};
		virtual void setSelected(unsigned int _new) {};

		virtual inline CBox getWindowsPos(void){return (itemBox);};

		virtual int getWidgetType(){return (4);};
		virtual void enablePaintDate(void){paintDate = true;};

		virtual int oKKeyPressed(CMenuTarget *parent){return 0;};

		virtual void onHomeKeyPressed(){};
		virtual void onUpKeyPressed(){};
		virtual void onDownKeyPressed(){};
		virtual void onRightKeyPressed(){};
		virtual void onLeftKeyPressed(){};
		virtual void onPageUpKeyPressed(){};
		virtual void onPageDownKeyPressed(){};

		//
		virtual std::string getActionKey(void){ return actionKey;};
		
		//
		virtual void setParent(CWidget* w_parent){parent = w_parent;};
};

// CHeaders
class CHeaders : public CWidgetItem
{
	private:
		fb_pixel_t bgcolor;
		int radius;
		int corner;
		int gradient;
		bool paintDate;
		bool logo;
		int hbutton_count;
		button_label_list_t hbutton_labels;
		const char *htitle;
		const char *hicon;
	
	public:
		CHeaders(const int x, const int y, const int dx, const int dy, const char * const title = NULL, const char * const icon = NULL);
		CHeaders(CBox position, const char * const title = NULL, const char * const icon = NULL);
		virtual ~CHeaders(){};

		void setTitle(const char * const title){htitle = title;};
		void setIcon(const char * const icon){hicon = icon;};

		// head
		void setColor(fb_pixel_t col){bgcolor = col;};
		void setCorner(int ra = NO_RADIUS, int co = CORNER_NONE){radius = ra; corner = co;};
		void setGradient(int grad){gradient = grad;};
		void setButtons(const struct button_label* _hbutton_labels, const int _hbutton_count = 1);
		void enablePaintDate(void){paintDate = true;};
		void enableLogo(void){logo = true;};

		void paint();
		void hide();
};

// CFooters
class CFooters : public CWidgetItem
{
	private:
		unsigned int fcount;
		button_label_list_t fbuttons;

		fb_pixel_t fbgcolor;
		int fradius;
		int fcorner;
		int fgradient;
	
	public:
		CFooters(int x, int y, int dx, int dy);
		CFooters(CBox position);
		virtual ~CFooters(){};

		void setColor(fb_pixel_t col){fbgcolor = col;};
		void setCorner(int ra = NO_RADIUS, int co = CORNER_NONE){fradius = ra; fcorner = co;};
		void setGradient(int grad){fgradient = grad;};
		void setButtons(const struct button_label *button_label, const int button_count = 1);

		void paint();
		void hide();
};

#endif /* __gui_widget_helpers_h__ */
