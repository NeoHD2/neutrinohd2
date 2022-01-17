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
#define WIDGET_COUNT	83
enum {
	WIDGET_NEUTRINO = 0,
	//MENU
	WIDGET_MAINMENU = 1,	// mainmenu
	WIDGET_MAINSETTINGS,	
	WIDGET_EPGTIMER,	// red
	WIDGET_SERVICE,
	WIDGET_INFORMATION,
	WIDGET_FEATURES,	// blue
	WIDGET_POWERMENU,
	WIDGET_MEDIAPLAYER,
	WIDGET_ODSETTINGS,
	WIDGET_MISCSETTINGS,
	WIDGET_NVOD,		// yellow
	//SETUP
	WIDGET_AUDIOSETUP = 12,
	WIDGET_VIDEOSETUP,
	WIDGET_PARENTALSETUP,
	WIDGET_NETWORKSETUP,
	WIDGET_RECORDINGSETUP,
	WIDGET_MOVIEPLAYERSETUP,
	WIDGET_LCDSETUP,
	WIDGET_AUDIOPLAYERSETUP,
	WIDGET_PICTUREVIEWERSETUP,
	WIDGET_HDDSETUP,
	WIDGET_SCANSETUP,
	WIDGET_CICAMSETUP,
	WIDGET_UPDATESETUP,
	WIDGET_MISCSETUP,
	WIDGET_EPGSETUP,
	WIDGET_CHANNELSSETUP,
	WIDGET_ZAPITSETUP,
	WIDGET_FILEBROWSERSETUP,
	WIDGET_SKINSETUP = 30,
	WIDGET_MENUSETUP,
	WIDGET_INFOBAR_SETUP,
	WIDGET_THEMESETUP,
	WIDGET_LANGUAGESETUP,
	WIDGET_FONTSETUP,
	WIDGET_OSDTIMINGSETUP,
	WIDGET_SCREENSETUP,
	WIDGET_OSDMISCSETUP,
	WIDGET_SKINSTYLESELECTIONSETUP,
	WIDGET_ALPHASETUP = 40,
	WIDGET_PROXYSETUP,
	WIDGET_RCSETUP,
	
	//LIST
	WIDGET_PLUINGSLIST = 43,
	WIDGET_EPGVIEW,
	WIDGET_EVENTLIST,
	WIDGET_EPGPLUS,
	WIDGET_TIMERLIST,
	WIDGET_CHANNELSLIST,
	WIDGET_BOUQUETSLIST,
	WIDGET_BOUQUETEDITOR = 50,
	WIDGET_FILEBROWSER,
	//CORE/PLAYER
	WIDGET_AUDIOPLAYER,
	WIDGET_MOVIEPLAYER,
	WIDGET_PICTUREVIEWER,
	//INFOBAR
	WIDGET_INFOBAR,
	//DIVERS
	WIDGET_AUDIOSELECT,
	WIDGET_AVSELECT,
	WIDGET_SUBSELECT,
	WIDGET_CHANNELSELECT,
	WIDGET_DBOXINFO = 60,
	WIDGET_IMAGEINFO,
	WIDGET_MOTORCONTROL,
	WIDGET_MOVIEINFO,
	WIDGET_NFS,
	WIDGET_SCAN,
	WIDGET_SLEEPTIMER,
	WIDGET_STREAMINFO,
	WIDGET_VFDCONTROLLER,
	WIDGET_COLORCHOOSER,
	WIDGET_KEYCHOOSER = 70,
	WIDGET_MOUNTCHOOSER,
	WIDGET_HINTBOX,
	WIDGET_MESSAGEBOX,
	WIDGET_HELPBOX,
	WIDGET_INFOBOX,
	WIDGET_PROGRESSWINDOW,
	WIDGET_STRINGINPUT,
	WIDGET_HTTPTOOL,
	WIDGET_VOLUME,
	WIDGET_MUTE,
	//
	WIDGET_PLUGIN,
	WIDGET_USERMENU,
	
	//
	WIDGET_MAX
};

//
enum
{
	RETURN_NONE		= 0,
	RETURN_REPAINT 	= 1,
	RETURN_EXIT 		= 2,
	RETURN_EXIT_ALL 	= 4
};

//
class CMenuTarget
{
	protected:
		std::string *valueString;
		std::string valueStringTmp;

	public:
		CMenuTarget(){ valueStringTmp = std::string(); valueString = &valueStringTmp; };
		virtual ~CMenuTarget(){};
		virtual void hide(){valueString->clear();};
		virtual int exec(CMenuTarget* parent, const std::string& actionKey) = 0;
		virtual std::string& getString(void) { return *valueString; };
		
		//
		int id;
		std::string name;
};

//
class CWidget : public CMenuTarget
{
	protected:
		CFrameBuffer *frameBuffer;
		CBox mainFrameBox;

		// 
		std::vector<CWidgetItem*> items;
		//
		std::vector<CComponent*> CCItems;

		//
		neutrino_msg_t      msg;
		neutrino_msg_data_t data;
		int selected;
		bool exit_pressed;
		int retval;
		int pos;
		struct keyAction { std::string action; CMenuTarget *menue; };
		std::map<neutrino_msg_t, keyAction> keyActionMap;
		uint64_t timeout;
		uint32_t sec_timer_id;
		uint64_t sec_timer_interval;
		std::string actionKey;
		
		// screen
		fb_pixel_t * background;
		bool savescreen;
		void saveScreen();
		void restoreScreen();

		// mainframe		
		bool paintframe;
		fb_pixel_t backgroundColor;
		int gradient;
		int radius;
		int corner;
		
		//
		void initFrames();
		
		//
		void paint();

	public:
		CWidget(const int x = 0, const int y = 0, const int dx = DEFAULT_XRES, const int dy = DEFAULT_YRES);
		CWidget(CBox *position);
		virtual ~CWidget();

		//WIDGETITEMS
		virtual void addItem(CWidgetItem *widgetItem, const bool defaultselected = false);
		bool hasItem(){return !items.empty();};
		int getItemsCount(){return items.size();};
		virtual void clearItems(void){return items.clear();};
		virtual void paintItems();
		
		void setSelected(unsigned int _new) {selected = _new; if (selected < 0) selected = 0;};
		
		// CCITEMS
		void addCCItem(CComponent* CCItem);
		bool hasCCItem(){return !CCItems.empty();};
		int getCCItemsCount(){return CCItems.size();};
		void clearCCItems(){CCItems.clear();};
		void paintCCItems();
		
		//
		virtual void hide();
		virtual int exec(CMenuTarget *parent, const std::string &actionKey);

		//
		void setTimeOut(unsigned long long int to = 0){timeout = to;};
		void setSecTimerInterval(uint64_t interval){sec_timer_interval = interval;};
		
		//
		void addKey(neutrino_msg_t key, CMenuTarget *menue = NULL, const std::string &action = "");

		//
		void paintMainFrame(bool p){paintframe = p;};
		void setColor(fb_pixel_t col) {backgroundColor = col;};
		void setGradient(int gra){gradient = gra;};
		void setCorner(int ra, int co){radius = ra; corner = co;};
		//
		void enableSaveScreen();

		// lua compatibility
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
};

typedef std::vector<CWidget*> WIDGETLIST;

#endif // WIDGET_H_

