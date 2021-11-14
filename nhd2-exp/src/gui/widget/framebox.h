/*
	$Id: framebox.h 09.02.2019 mohousch Exp $


	License: GPL

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#if !defined(_FRAMEBOX_H_)
#define _FRAMEBOX_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <vector>

#include <driver/framebuffer.h>
#include <driver/fontrenderer.h>

#include <gui/widget/window.h>
#include <gui/widget/widget.h>


class CFrameBox;

enum {
	FRAME_BOX = 0, // caption, option and left icon | optionInfo and 2lines
	FRAME_PICTURE,
	FRAME_ICON,
	FRAME_TEXT,
	FRAME_LABEL,
	FRAME_PLUGIN,
	FRAME_HLINE,
	FRAME_VLINE,
	FRAME_HEAD,
	FRAME_FOOT,
	FRAME_PIG
};

class CFrame
{
	public:
		CWindow window;
		
		std::string iconName;
		std::string caption;
		std::string option;

		CFont *captionFont;
		CFont *optionFont;

		CMenuTarget* jumpTarget;
		std::string actionKey;
		neutrino_msg_t directKey;
		neutrino_msg_t msg;
		
		//
		bool active;
		bool marked;

		int mode;	//frame mode
		bool shadow;
		bool paintFrame;
		bool pluginOrigName;
		
		//
		fb_pixel_t fcolor;
		int radius;
		int corner;
		int gradient;

		// head
		fb_pixel_t headColor;
		int headRadius;
		int headCorner;
		int headGradient;
		int hbutton_count;
		button_label_list_t hbutton_labels;
		bool paintDate;
		bool logo;

		// foot
		fb_pixel_t footColor;
		int footRadius;
		int footCorner;
		int footGradient;
		int fbutton_count;
		button_label_list_t fbutton_labels;
		
		//
		CFrameBox* parent;
		
		//
		CFrame();
		virtual ~CFrame(){};

		int paint(bool selected = false, bool AfterPulldown = false);
		
		virtual void setMode(int m = FRAME_BOX);
		int getMode(void){return mode;};
		
		virtual void setTitle(const char *text){if (text != NULL) caption = text;};
		virtual void setIconName(const char *icon){ if (icon != NULL) iconName = icon;};
		virtual void setOption(const char *text){if (text != NULL) option = text;};
		virtual void setPlugin(const char * const pluginName);
		virtual void showPluginName(){pluginOrigName = true;};

		//
		virtual void setActionKey(CMenuTarget *Target, const char *const ActionKey){jumpTarget = Target; actionKey = ActionKey;};
		virtual void setDirectKey(neutrino_msg_t key){directKey = key;};
		
		//
		virtual void setCaptionFont(CFont * font){captionFont = font;};
		virtual void setOptionFont(CFont *font){optionFont = font;}; 

		int exec(CMenuTarget *parent);
		
		//
		virtual void setActive(const bool Active);
		virtual void setMarked(const bool Marked);

		virtual bool isSelectable(void)
		{
			return active;				
		}

		virtual void disableShadow(void){shadow = false;};
		virtual void disablePaintFrame(void){paintFrame = false;};
		virtual void setPosition(int x, int y, int dx, int dy){window.setPosition(x, y, dx, dy);};
		virtual void setPosition(CBox *position){window.setPosition(position);};
		
		//
		virtual void setColor(fb_pixel_t col) {fcolor = col;};
		virtual void setCorner(int ra, int co){radius = ra; corner = co;};
		virtual void setGradient(int grad){gradient = grad;};

		// headFrame
		void enablePaintDate(void){paintDate = true;};
		void enableLogo(){logo = true;};
		void setHeadColor(fb_pixel_t col) {headColor = col;};
		void setHeadCorner(int ra, int co){headRadius = ra; headCorner = co;};
		void setHeadGradient(int grad){headGradient = grad;};
		void setHeaderButtons(const struct button_label *_hbutton_label, const int _hbutton_count = 1);

		// footFrame
		void setFootColor(fb_pixel_t col) {footColor = col;};
		void setFootCorner(int ra, int co){footRadius = ra; footCorner = co;};
		void setFootGradient(int grad){footGradient = grad;};
		void setFooterButtons(const struct button_label *_fbutton_label, const int _fbutton_count = 1);
		
		//
		virtual void setParent(CFrameBox* f_parent){parent = f_parent;};
};

//// CFrameBox
class CFrameBox : public CWidgetItem
{
	private:
		CFrameBuffer* frameBuffer;
		CWindow cFrameWindow;
		int selected;
		int pos;

		std::vector<CFrame*> frames;

		virtual void paintFrames();

		std::string actionKey;
		
		//
		bool paintFrame;
		
		fb_pixel_t bgcolor;
		int radius;
		int corner;
		bool shadow;
		bool screen;

	public:
		CFrameBox(const int x = 0, int const y = 0, const int dx = 0, const int dy = 0);
		CFrameBox(CBox* position);
		virtual ~CFrameBox();

		void setPosition(const int x, const int y, const int dx, const int dy)
		{
			itemBox.iX = x;
			itemBox.iY = y;
			itemBox.iWidth = dx;
			itemBox.iHeight = dy;

			initFrames();
		};
		void setPosition(CBox* position){itemBox = *position; initFrames();};

		virtual void addFrame(CFrame *frame, const bool defaultselected = false);
		bool hasItem();
		void clearFrames(void){frames.clear();};
		void setSelected(unsigned int _new) { /*if(_new <= frames.size())*/ selected = _new; };

		virtual void initFrames();
		virtual void paint();
		virtual void hide();

		virtual void swipRight();
		virtual void swipLeft();
		virtual void scrollLineDown(const int lines = 1);
		virtual void scrollLineUp(const int lines = 1);

		int getSelected(){return selected;};
		void disablePaintFrame(void){paintFrame = false;};
		void setColor(fb_pixel_t col){bgcolor = col;};
		void setCorner(int ra, int co){radius = ra; corner = co;};
		void enablePaintShadow(void){shadow = true;};
		void enableSaveScreen(void){screen = true;};

		//
		bool isSelectable(void);

		int oKKeyPressed(CMenuTarget *parent);
		void homeKeyPressed(){selected = -1;};

		std::string getActionKey(void){return actionKey;};
};

#endif
