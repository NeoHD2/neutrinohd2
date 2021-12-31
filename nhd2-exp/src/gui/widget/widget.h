/*
	$Id: widget.h 11.03.2020 mohousch Exp $


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
#if !defined(WIDGET_H_)
#define WIDGET_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gui/widget/widget_helpers.h>


//
enum
{
	RETURN_NONE		= 0,
	RETURN_REPAINT 	= 1,
	RETURN_EXIT 		= 2,
	RETURN_EXIT_ALL 	= 4
};

class CMenuTarget
{
	protected:
		CFrameBuffer *frameBuffer;
		
		std::string *valueString;
		std::string valueStringTmp;

	public:
		CMenuTarget(){ frameBuffer = CFrameBuffer::getInstance(); valueStringTmp = std::string(); valueString = &valueStringTmp; };
		virtual ~CMenuTarget(){};
		virtual void hide(){valueString->clear();};
		virtual int exec(CMenuTarget *parent, const std::string &actionKey) = 0;
		virtual std::string &getString(void) { return *valueString; };
};

class CWidget : public CMenuTarget
{
	protected:
		CFrameBuffer *frameBuffer;
		CBox mainFrameBox;

		std::vector<CWidgetItem*> items;

		//
		neutrino_msg_t      msg;
		neutrino_msg_data_t data;
		int selected;
		bool exit_pressed;
		int retval;
		int pos;
		struct keyAction { std::string action; CMenuTarget *menue; };
		std::map<neutrino_msg_t, keyAction> keyActionMap;
		unsigned long long int timeout;
		uint32_t sec_timer_id;
		int sec_timer_interval;
		std::string actionKey;
		
		// screen
		fb_pixel_t * background;
		bool savescreen;
		void saveScreen();
		void restoreScreen();

		//		
		bool paintFrame;
		fb_pixel_t backgroundColor;
		int radius;
		int corner;

	public:
		CWidget(const int x = 0, const int y = 0, const int dx = DEFAULT_XRES, const int dy = DEFAULT_YRES);
		CWidget(CBox *position);
		virtual ~CWidget();

		virtual void initFrames();
		virtual void addItem(CWidgetItem *widgetItem, const bool defaultselected = false);
		bool hasItem();
		int getItemsCount();
		virtual void clearItems(void);
		virtual void paintItems();
		
		//
		virtual void paint();
		virtual void hide();
		virtual int exec(CMenuTarget *parent, const std::string &actionKey);
		virtual int exec(CWidgetItem* wItem);

		//
		void setTimeOut(unsigned long long int to = 0){timeout = to;};
		void setSecTimerInterval(int interval){sec_timer_interval = interval;};
		void addKey(neutrino_msg_t key, CMenuTarget *menue = NULL, const std::string &action = "");
		void setSelected(unsigned int _new) {selected = _new; if (selected < 0) selected = 0;};

		//
		void enablePaintFrame(){paintFrame = true;};
		void setColor(fb_pixel_t col) {backgroundColor = col;};
		void setCorner(int ra, int co){radius = ra; corner = co;};
		void enableSaveScreen();

		// compatibility
		int getSelected(){return exit_pressed ? -1 : selected;};
		std::string getActionKey(){return actionKey;};
		neutrino_msg_t getKey(){return msg;};
		inline CBox getWindowsPos(void){return(mainFrameBox);};
		bool getExitPressed(){return exit_pressed;};

		// events
		virtual void onOKKeyPressed();
		virtual void onHomeKeyPressed();
		virtual void onUpKeyPressed();
		virtual void onDownKeyPressed();
		virtual void onRightKeyPressed();
		virtual void onLeftKeyPressed();
		virtual void onPageUpKeyPressed();
		virtual void onPageDownKeyPressed();
		virtual void onYellowKeyPressed();
		
		//
		virtual bool onButtonPress(neutrino_msg_t msg, neutrino_msg_data_t data);
};

#endif // WIDGET_H_

