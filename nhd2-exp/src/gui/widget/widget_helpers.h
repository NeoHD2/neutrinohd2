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


extern CLocaleManager* g_Locale; // for DL

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
	CC_ICON,
	CC_IMAGE,
	CC_LABEL,
	CC_TEXT,
	CC_BUTTON,
	CC_HLINE,
	CC_VLINE,
	CC_FRAMELINE,
	CC_PIG,
	CC_GRID,
	//CC_SLIDER,
	//CC_TIME
	CC_SCROLLBAR,
	CC_PROGRESSBAR,
	CC_DETAILSLINE
};

enum {
	CC_ALIGN_LEFT,
	CC_ALIGN_CENTER,
	CC_ALIGN_RIGHT
};

class CComponent
{
	public:
		//
		CFrameBuffer *frameBuffer;
		int cc_type;
		CBox cCBox;
		int halign;
		int valign;
		
		//
		CComponent(){frameBuffer = CFrameBuffer::getInstance();};
		virtual ~CComponent(){};
		
		virtual bool isSelectable(void){return false;}
		
		//
		virtual void paint(void){};
		virtual void hide(void){};
		
		//
		virtual int getCCType(){return cc_type;};
		
		//
		virtual void setPosition(const int _x, const int _y, const int _width, const int _height)
		{
			cCBox.iX = _x;
			cCBox.iY = _y;
			cCBox.iWidth = _width;
			cCBox.iHeight = _height;
		};
		
		virtual void setPosition(const CBox * position){cCBox = *position;};
		
		//
		virtual inline CBox getWindowsPos(void){return (cCBox);};
};

class CIcon : public CComponent
{
	public:
		//		
		int iWidth;
		int iHeight;
		std::string iconName;

		CIcon(){frameBuffer = CFrameBuffer::getInstance(); iWidth = 0; iHeight = 0; cc_type = CC_ICON;};
		
		void setIcon(const char* icon)
		{
			iconName = std::string(icon); 
			frameBuffer->getIconSize(iconName.c_str(), &iWidth, &iHeight);
		};
		
		CIcon(const char* icon)
		{
			frameBuffer = CFrameBuffer::getInstance();
			iconName = std::string(icon); 
			frameBuffer->getIconSize(iconName.c_str(), &iWidth, &iHeight);
			
			cc_type = CC_ICON;
		};

		//
		void paint()
		{
			frameBuffer->paintIcon(iconName.c_str(), cCBox.iX + (cCBox.iWidth - iWidth)/2, cCBox.iY + (cCBox.iHeight - iHeight)/2);
		};
};

class CImage : public CComponent
{
	public:
		//
		int iWidth;
		int iHeight;
		int iNbp;
		std::string imageName;

		CImage(){frameBuffer = CFrameBuffer::getInstance(); cc_type = CC_IMAGE;};

		void setImage(const char* image)
		{
			imageName = std::string(image); 
			frameBuffer->getSize(imageName, &iWidth, &iHeight, &iNbp);
		};

		CImage(const char* image)
		{
			frameBuffer = CFrameBuffer::getInstance();
			imageName = std::string(image); 
			frameBuffer->getSize(imageName, &iWidth, &iHeight, &iNbp);
			
			cc_type = CC_IMAGE;
		};
		
		//
		void paint()
		{
			frameBuffer->displayImage(imageName.c_str(), cCBox.iX /*+ (cCBox.iWidth - iWidth)/2*/, cCBox.iY /*+ (cCBox.iHeight - iHeight)/2*/, cCBox.iWidth, cCBox.iHeight);
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

enum {
	BUTTON,
	FRAME
};

// CButtons
class CButtons : public CComponent
{
	private:
		button_label_list_t buttons;
		unsigned int count;
		int mode;
	public:
		//
		CButtons(){frameBuffer = CFrameBuffer::getInstance(); buttons.clear(); count = 0; mode = BUTTON; cc_type = CC_BUTTON;};
		
		//
		void setButtons(const struct button_label *button_label, const int button_count = 1);
		void setMode(int m){mode = m;};
		void paint();

		//
		void paintFootButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label* const content);
		void paintHeadButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label * const content);
};

//CScrollBar
class CScrollBar : public CComponent
{
	public:
		//
		CScrollBar(){frameBuffer = CFrameBuffer::getInstance(); cc_type = CC_SCROLLBAR;};
		virtual ~CScrollBar(){};

		// currentPage start with 0
		void paint(const int x, const int y, const int dy, const int NrOfPages, const int CurrentPage);
		void paint(CBox* position, const int NrOfPages, const int CurrentPage);
};

// CProgressBar
class CProgressBar : public CComponent
{
	private:
		unsigned char percent;
		short red, green, yellow;
		bool inverse;

	public:
		//
		CProgressBar(int r = 40, int g = 100, int b = 70, bool inv = true);
		
		//
		void setPosition(const int x, const int y, const int w, const int h);
		void paintPCR(unsigned char pcr);
		void reset();
		int getPercent() { return percent; };
		
		//
		void paint(){};
};

// detailsLine
enum {
	DL_INFO,
	DL_HINT
};

class CItems2DetailsLine : public CComponent
{
	public:
		std::string info1, option_info1;
		std::string info2, option_info2;
		std::string hint;
		std::string icon;
		int mode;

		//
		bool paintLines;
		
		//
		CItems2DetailsLine();
		virtual ~CItems2DetailsLine(){};
		
		//
		void disablePaintLines(){paintLines = false;};
		
		//
		void paint(int x, int y, int width, int height, int info_height, int iheight, int iy);
		void clear(int x, int y, int width, int height, int info_height);
		
		//
		virtual void setMode(int m){mode = m;};
		virtual void setInfo1(const char* const text){info1 = text;};
		virtual void setInfo2(const char* const text){info2 = text;};
		virtual void setOptionInfo1(const char* const text){option_info1 = text;};
		virtual void setOptionInfo2(const char* const text){option_info2 = text;};
		virtual void setHint(const char* const Text){hint =  Text;};
		virtual void setHint(const neutrino_locale_t locale){hint = g_Locale->getText(locale); };
		virtual void setIcon(const char* const ic){icon = ic;};
};

// CHline
class CHline : public CComponent
{
	public:
		//
		fb_pixel_t color;
		
		//
		CHline();
		virtual ~CHline(){};
		
		//
		void setColor(fb_pixel_t col){color = col;};
		
		//
		void paint()
		{
			frameBuffer->paintHLineRel(cCBox.iX, cCBox.iWidth, cCBox.iY, color);
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
		
		//
		void paint()
		{
			frameBuffer->paintVLineRel(cCBox.iX, cCBox.iY, cCBox.iHeight, color);
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
		
		//
		void paint()
		{
			frameBuffer->paintFrameBox(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight, color);
		};
};

// CLabel
class CLabel : public CComponent
{
	public:
		//
		int width;
		int height;
		
		uint8_t color;
		CFont* font;
		std::string label;
		bool paintBG;
		bool utf8;
		
		//
		CLabel();
		virtual ~CLabel(){};
		
		//
		void setColor(uint8_t col){color = col;};
		void setFont(CFont *f){font = f;};
		void setText(const char* text){label = text;};
		void enablePaintBG(){paintBG = true;};
		void setHAlign(int h){halign = h;};
		
		//
		void paint();
		
		//
		int getHeight(){return height;};
		int getWidth(){return font->getRenderWidth(label);};
};

//CText
class CText : public CComponent
{
	public:
		//
		CFont* font;
		int mode;
		std::string Text;
		
		//
		CText();
		~CText(){};
		
		//
		void setFont(CFont* f){font = f;};
		void setMode(int m){mode = m;};
		void setText(const char* const text){Text = text;};
		
		//
		void paint();
};

//
class CGrid : public CComponent
{
	private:
		//
		fb_pixel_t rgb;
		int inter_frame;

	public:
		//
		CGrid(const int x = 0, const int y = 0, const int dx = MENU_WIDTH, const int dy = MENU_HEIGHT);
		CGrid(CBox* position);
		virtual ~CGrid(){};

		//
		void init();
		void setPosition(const int x, const int y, const int dx, const int dy);
		void setPosition(CBox* position);
		void setColor(fb_pixel_t col){rgb = col;};
		void setInterFrame(int iframe = 15){inter_frame = iframe;};

		void paint();
		void hide();
};

// pig
class CPig : public CComponent
{
	public:
		//
		CPig(const int x = 0, const int y = 0, const int dx = MENU_WIDTH, const int dy = MENU_HEIGHT);
		CPig(CBox* position);
		virtual ~CPig(){};

		//
		void init();
		void setPosition(const int x, const int y, const int dx, const int dy);
		void setPosition(CBox* position);

		void paint();
		void hide();
};

// CWidgetItem
enum {
	WIDGET_ITEM_WINDOW,
	WIDGET_ITEM_HEAD,
	WIDGET_ITEM_FOOT,
	WIDGET_ITEM_LISTBOX,
	WIDGET_ITEM_FRAMEBOX,
	WIDGET_ITEM_LISTFRAME,
	WIDGET_ITEM_TEXTBOX
};

class CWidgetItem
{
	public:
		//
		CFrameBuffer* frameBuffer;
		CBox itemBox;

		int itemType;
		bool inFocus;
		bool paintDate;
		std::string actionKey; // lua
		
		//
		bool painted;
		CWidget* parent;

		CWidgetItem(){inFocus = true; actionKey = ""; parent = NULL;};
		virtual ~CWidgetItem(){};

		virtual bool isSelectable(void){return false;}
		virtual bool hasItem(){return false;};

		virtual void paint(){painted = true;};
		virtual void hide(){painted = false;};

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
		virtual void homeKeyPressed(){};
		
		//
		virtual std::string getActionKey(void){ return actionKey;}; // lua
		
		//
		virtual void setParent(CWidget* w_parent){parent = w_parent;};
		
		inline bool isPainted(void){return painted;};
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
		int hbutton_count;
		button_label_list_t hbutton_labels;
		const char *htitle;
		const char *hicon;
		int tMode;
	
	public:
		CHeaders(const int x, const int y, const int dx, const int dy, const char * const title = NULL, const char * const icon = NULL);
		CHeaders(CBox position, const char * const title = NULL, const char * const icon = NULL);
		virtual ~CHeaders(){};

		void setTitle(const char * const title){htitle = title;};
		void setTitleIcon(const char * const icon){hicon = icon;};
		void setTitleMode(const int m){tMode = m;};
		void setColor(fb_pixel_t col){bgcolor = col;};
		void setCorner(int ra = NO_RADIUS, int co = CORNER_NONE){radius = ra; corner = co;};
		void setGradient(int grad){gradient = grad;};
		void setButtons(const struct button_label* _hbutton_labels, const int _hbutton_count = 1);
		void enablePaintDate(void){paintDate = true;};

		//
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
