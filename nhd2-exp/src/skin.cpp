/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: skin.cpp.cpp 15.01.2022 mohousch Exp $

	Copyright (C) 2001 Steffen Hehn 'McClean' and some other guys
	Homepage: http://dbox.cyberphoria.org/

	Kommentar:

	Diese GUI wurde von Grund auf neu programmiert und sollte nun vom
	Aufbau und auch den Ausbaumoeglichkeiten gut aussehen. Neutrino basiert
	auf der Client-Server Idee, diese GUI ist also von der direkten DBox-
	Steuerung getrennt. Diese wird dann von Daemons uebernommen.


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
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>
#include <dirent.h>

#include <global.h>
#include <neutrino.h>

#include <gui/widget/widget.h>
#include <gui/widget/listbox.h>
#include <gui/widget/framebox.h>
#include <gui/widget/window.h>
#include <gui/widget/hintbox.h>

#include <gui/plugins.h>
#include <gui/themes.h>
//
#include <gui/main_setup.h>
#include <gui/epg_menu.h>
#include <gui/epgplus.h>
#include <gui/timerlist.h>
#include <gui/streaminfo2.h>
#include <gui/service_menu.h>
#include <gui/mediaplayer.h>
#include <gui/dboxinfo.h>
#include <gui/power_menu.h>

#include <gui/audio_setup.h>
#include <gui/video_setup.h>
#include <gui/parentallock_setup.h>
#include <gui/network_setup.h>
#include <gui/movieplayer_setup.h>
#include <gui/osd_setup.h>
#include <gui/audioplayer_setup.h>
#include <gui/pictureviewer_setup.h>
#include <gui/lcd_setup.h>
#include <gui/rc_setup.h>
#include <gui/recording_setup.h>
#include <gui/misc_setup.h>
#include <gui/hdd_menu.h>
#include <gui/screensetup.h>
#include <gui/alphasetup.h>
#include <gui/zapit_setup.h>
#include <gui/bedit/bouqueteditor_bouquets.h>
#include <gui/cam_menu.h>
#include <gui/update.h>
#include <gui/imageinfo.h>
#include <gui/sleeptimer.h>

#include <system/helpers.h>
#include <system/debug.h>

#if ENABLE_LUA
#include <interfaces/lua/neutrino_lua.h>
#endif


//
CMenuTarget* CNeutrinoApp::convertTarget(const int id)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::convertTarget: id: %d\n", id);
	
	CMenuTarget* parent = NULL;
	
	switch(id)
	{
		case 0:
			parent = this;
			break;
		
		case WIDGET_MAINMENU:
			{
				if (widget_exists(WIDGET_MAINMENU))
				{
					parent = getWidget(WIDGET_MAINMENU);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = this;
				}
			}
			break;
			
		case WIDGET_MAINSETTINGS:
			{
				if (widget_exists(WIDGET_MAINSETTINGS))
				{
					parent = getWidget(WIDGET_MAINSETTINGS);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CMainSettingsMenu();
				}
			}
			break;
			
		case WIDGET_EPGTIMER:
			{
				if (widget_exists(WIDGET_EPGTIMER))
				{
					parent = getWidget(WIDGET_EPGTIMER);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CEPGMenuHandler();
				}
			}
			break;
			
		case WIDGET_SERVICE:
			{
				if (widget_exists(WIDGET_SERVICE))
				{
					parent = getWidget(WIDGET_SERVICE);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CServiceMenu();
				}
			}
			break;
			
		case WIDGET_INFORMATION:
			{
				if (widget_exists(WIDGET_INFORMATION))
				{
					parent = getWidget(WIDGET_INFORMATION);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CInfoMenu();
				}
			}
			break;
			
		case WIDGET_FEATURES:
			{
				if (widget_exists(WIDGET_FEATURES))
				{
					parent = getWidget(WIDGET_FEATURES);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = this;
				}
			}
			break;
			
		case WIDGET_POWERMENU:
			{
				if (widget_exists(WIDGET_POWERMENU))
				{
					parent = getWidget(WIDGET_POWERMENU);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CPowerMenu();
				}
			}
			break;
			
		case WIDGET_MEDIAPLAYER:
			{
				if (widget_exists(WIDGET_MEDIAPLAYER))
				{
					parent = getWidget(WIDGET_MEDIAPLAYER);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CMediaPlayerMenu();
				}
			}
			break;
			
		case WIDGET_VIDEOSETUP:
			{
				if (widget_exists(WIDGET_VIDEOSETUP))
				{
					parent = getWidget(WIDGET_VIDEOSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CVideoSettings();
				}
			}
			break;
			
		case WIDGET_AUDIOSETUP:
			{
				if (widget_exists(WIDGET_AUDIOSETUP))
				{
					parent = getWidget(WIDGET_AUDIOSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CAudioSettings();
				}
			}
			break;
			
		case WIDGET_PARENTALSETUP:
			{
				if (widget_exists(WIDGET_PARENTALSETUP))
				{
					parent = getWidget(WIDGET_PARENTALSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CParentalLockSettings();
				}
			}
			break;
			
		case WIDGET_NETWORKSETUP:
			{
				if (widget_exists(WIDGET_NETWORKSETUP))
				{
					parent = getWidget(WIDGET_NETWORKSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = CNetworkSettings::getInstance();
				}
			}
			break;
			
		case WIDGET_RECORDINGSETUP:
			{
				if (widget_exists(WIDGET_RECORDINGSETUP))
				{
					parent = getWidget(WIDGET_RECORDINGSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CRecordingSettings();
				}
			}
			break;
			
		case WIDGET_MOVIEPLAYERSETUP:
			{
				if (widget_exists(WIDGET_MOVIEPLAYERSETUP))
				{
					parent = getWidget(WIDGET_MOVIEPLAYERSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CMoviePlayerSettings();
				}
			}
			break;
			
		case WIDGET_ODSETTINGS:
			{
				if (widget_exists(WIDGET_ODSETTINGS))
				{
					parent = getWidget(WIDGET_ODSETTINGS);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new COSDSettings();
				}
			}
			break;
			
		case WIDGET_LCDSETUP:
			{
				if (widget_exists(WIDGET_LCDSETUP))
				{
					parent = getWidget(WIDGET_LCDSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CLCDSettings();
				}
			}
			break;
			
		case WIDGET_REMOTECONTROLSETUP:
			{
				if (widget_exists(WIDGET_REMOTECONTROLSETUP))
				{
					parent = getWidget(WIDGET_REMOTECONTROLSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CRemoteControlSettings();
				}
			}
			break;
		
		case WIDGET_AUDIOPLAYERSETUP:
			{
				if (widget_exists(WIDGET_AUDIOPLAYERSETUP))
				{
					parent = getWidget(WIDGET_AUDIOPLAYERSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CAudioPlayerSettings();
				}
			}
			break;
			
		case WIDGET_PICTUREVIEWERSETUP:
			{
				if (widget_exists(WIDGET_PICTUREVIEWERSETUP))
				{
					parent = getWidget(WIDGET_PICTUREVIEWERSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CPictureViewerSettings();
				}
			}
			break;
			
		case WIDGET_MISCSETTINGS:
			{
				if (widget_exists(WIDGET_MISCSETTINGS))
				{
					parent = getWidget(WIDGET_MISCSETTINGS);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CMiscSettingsMenu();
				}
			}
			break;
			
		case WIDGET_HDDSETUP:
			{
				if (widget_exists(WIDGET_HDDSETUP))
				{
					parent = getWidget(WIDGET_HDDSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CHDDMenuHandler();
				}
			}
			break;
			
		case WIDGET_SKINSETUP:
			{
				if (widget_exists(WIDGET_SKINSETUP))
				{
					parent = getWidget(WIDGET_SKINSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CSkinManager();
				}
			}
			break;
			
		case WIDGET_MENUSETUP:
			{
				if (widget_exists(WIDGET_MENUSETUP))
				{
					parent = getWidget(WIDGET_MENUSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new COSDMenuColorSettings();
				}
			}
			break;
			
		case WIDGET_INFOBARSETUP:
			{
				if (widget_exists(WIDGET_INFOBARSETUP))
				{
					parent = getWidget(WIDGET_INFOBARSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new COSDInfoBarColorSettings();
				}
			}
			break;
			
		case WIDGET_THEMESETUP:
			{
				if (widget_exists(WIDGET_THEMESETUP))
				{
					parent = getWidget(WIDGET_THEMESETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CThemes();
				}
			}
			break;
			
		case WIDGET_LANGUAGESETUP:
			{
				if (widget_exists(WIDGET_LANGUAGESETUP))
				{
					parent = getWidget(WIDGET_LANGUAGESETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CLanguageSettings();
				}
			}
			break;
			
		case WIDGET_FONTSETUP:
			{
				if (widget_exists(WIDGET_FONTSETUP))
				{
					parent = getWidget(WIDGET_FONTSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CFontSettings();
				}
			}
			break;
			
		case WIDGET_OSDTIMINGSETUP:
			{
				if (widget_exists(WIDGET_OSDTIMINGSETUP))
				{
					parent = getWidget(WIDGET_OSDTIMINGSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new COSDTimingSettings();
				}
			}
			break;
			
		case WIDGET_SCREENSETUP:
			{
				if (widget_exists(WIDGET_SCREENSETUP))
				{
					parent = getWidget(WIDGET_SCREENSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CScreenSetup();
				}
			}
			break;
			
		case WIDGET_OSDMISCSETUP:
			{
				if (widget_exists(WIDGET_OSDMISCSETUP))
				{
					parent = getWidget(WIDGET_OSDMISCSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new COSDDiverses();
				}
			}
			break;
			
		case WIDGET_ALPHASETUP:
			{
				if (widget_exists(WIDGET_ALPHASETUP))
				{
					parent = getWidget(WIDGET_ALPHASETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CAlphaSetup(_("Alpha"), &g_settings.gtx_alpha);;
				}
			}
			break;
			
		case WIDGET_SKINSTYLESELECTIONSETUP:
			{
				if (widget_exists(WIDGET_SKINSTYLESELECTIONSETUP))
				{
					parent = getWidget(WIDGET_SKINSTYLESELECTIONSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CSkinSettings();
				}
			}
			break;
			
		case WIDGET_MISCSETUP:
			{
				if (widget_exists(WIDGET_MISCSETUP))
				{
					parent = getWidget(WIDGET_MISCSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CGeneralSettings();
				}
			}
			break;
			
		case WIDGET_EPGSETUP:
			{
				if (widget_exists(WIDGET_EPGSETUP))
				{
					parent = getWidget(WIDGET_EPGSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CEPGSettings();
				}
			}
			break;
			
		case WIDGET_CHANNELSSETUP:
			{
				if (widget_exists(WIDGET_CHANNELSSETUP))
				{
					parent = getWidget(WIDGET_CHANNELSSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CChannelListSettings();
				}
			}
			break;
			
		case WIDGET_ZAPITSETUP:
			{
				if (widget_exists(WIDGET_ZAPITSETUP))
				{
					parent = getWidget(WIDGET_ZAPITSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CZapitSetup();
				}
			}
			break;
			
		case WIDGET_FILEBROWSERSETUP:
			{
				if (widget_exists(WIDGET_FILEBROWSERSETUP))
				{
					parent = getWidget(WIDGET_FILEBROWSERSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CFileBrowserSettings();
				}
			}
			break;
			
		case WIDGET_SCANSETUP:
			{
				if (widget_exists(WIDGET_SCANSETUP))
				{
					parent = getWidget(WIDGET_SCANSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CTunerSetup();
				}
			}
			break;
			
		case WIDGET_CICAMSETUP:
			{
				if (widget_exists(WIDGET_CICAMSETUP))
				{
					parent = getWidget(WIDGET_CICAMSETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CCAMMenuHandler();
				}
			}
			break;
			
		case WIDGET_UPDATESETUP:
			{
				if (widget_exists(WIDGET_UPDATESETUP))
				{
					parent = getWidget(WIDGET_UPDATESETUP);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CUpdateSettings();
				}
			}
			break;
			
		case WIDGET_BOUQUETEDITOR:
			{
				if (widget_exists(WIDGET_BOUQUETEDITOR))
				{
					parent = getWidget(WIDGET_BOUQUETEDITOR);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CBEBouquetWidget();
				}
			}
			break;
			
		case WIDGET_IMAGEINFO:
			{
				if (widget_exists(WIDGET_IMAGEINFO))
				{
					parent = getWidget(WIDGET_IMAGEINFO);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CImageInfo();
				}
			}
			break;
			
		case WIDGET_DBOXINFO:
			{
				if (widget_exists(WIDGET_DBOXINFO))
				{
					parent = getWidget(WIDGET_DBOXINFO);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CDBoxInfoWidget();
				}
			}
			break;
			
		case WIDGET_STREAMINFO:
			{
				if (widget_exists(WIDGET_STREAMINFO))
				{
					parent = getWidget(WIDGET_STREAMINFO);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CStreamInfo2Handler();
				}
			}
			break;
			
		case WIDGET_SLEEPTIMER:
			{
				if (widget_exists(WIDGET_SLEEPTIMER))
				{
					parent = getWidget(WIDGET_SLEEPTIMER);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CSleepTimerWidget();
				}
			}
			break;
			
		case WIDGET_EVENTLIST:
			{
				if (widget_exists(WIDGET_EVENTLIST))
				{
					parent = getWidget(WIDGET_EVENTLIST);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CEventListHandler();
				}
			}
			break;
			
		case WIDGET_EPGVIEW:
			{
				if (widget_exists(WIDGET_EPGVIEW))
				{
					parent = getWidget(WIDGET_EPGVIEW);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CEPGDataHandler();
				}
			}
			break;
			
		case WIDGET_EPGPLUS:
			{
				if (widget_exists(WIDGET_EPGPLUS))
				{
					parent = getWidget(WIDGET_EPGPLUS);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CEPGplusHandler();
				}
			}
			break;
			
		case WIDGET_PLUINGSLIST:
			{
				if (widget_exists(WIDGET_PLUINGSLIST))
				{
					parent = getWidget(WIDGET_PLUINGSLIST);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CPluginList();
				}
			}
			break;
			
		case WIDGET_TIMERLIST:
			{
				if (widget_exists(WIDGET_TIMERLIST))
				{
					parent = getWidget(WIDGET_TIMERLIST);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = new CTimerList();
				}
			}
			break;
		
		default:
			break;
	}
	
	return parent;
}

//
CWidget* CNeutrinoApp::getWidget(int id)
{
	CWidget* ret = NULL;
	
	for (unsigned int i = 0; i < (unsigned int )widgets.size(); i++)
	{
		if ( (widgets[i] != NULL) && (widgets[i]->id == id) )
		{
			ret = widgets[i];
			break;
		}
	}
	
	return ret;
}

bool CNeutrinoApp::widget_exists(int id)
{
	bool ret = false;
	
	if (CNeutrinoApp::getInstance()->getWidget(id))
		ret = true;
		
	return ret;
}

uint32_t CNeutrinoApp::convertColor(const char* const color)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::convertColor: id: %d\n", id);
	
	uint32_t rgba = COL_MENUCONTENT_PLUS_0;
	
	unsigned int r = 0;
	unsigned int g = 0;
	unsigned int b = 0;
	unsigned int a = 0;
				
	if (color != NULL)
	{
		if (color[0] == '#')
		{
			unsigned long col = 0;
						
			if (sscanf(color + 1, "%lx", &col) == 1)
			{
				r = (col>>24)&0xFF; 
				g = (col>>16)&0xFF;
				b = (col >> 8)&0xFF;
				a = (col)&0xFF;
			}
		}
	}
				
	rgba = convertSetupColor2Color(r, g, b, a); 
	
	return rgba;
}

//
void CNeutrinoApp::parseSkin()
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::parseSkin\n");
	
	//
	_xmlDocPtr parser = NULL;
	
	std::string filename = CONFIGDIR "/skins/";
	filename += g_settings.preferred_skin;
	filename += "/skin.xml";

	parser = parseXmlFile(filename.c_str());
	
	if (parser == NULL)
		return;
		
	_xmlNodePtr root = xmlDocGetRootElement(parser);
	_xmlNodePtr search = root->xmlChildrenNode; //WIDGET
	_xmlNodePtr listbox_node = NULL; // LISTBOX
	_xmlNodePtr listboxitem_node = NULL; //CMENUITEM FIXME:split
	_xmlNodePtr listboxintegration_node = NULL;
	_xmlNodePtr key_node = NULL; // KEY
	_xmlNodePtr label_node = NULL;
	_xmlNodePtr image_node = NULL;
	_xmlNodePtr time_node = NULL;
	_xmlNodePtr head_node = NULL;
	_xmlNodePtr headbutton_node = NULL;
	_xmlNodePtr foot_node = NULL;
	_xmlNodePtr footbutton_node = NULL;
	_xmlNodePtr window_node = NULL;
	//_xmlNodePtr windowitem_node = NULL;
	
	if (search) 
	{
		dprintf(DEBUG_NORMAL, "CNeutrinoApp:parseSkin: %s\n", filename.c_str());

		while ((search = xmlGetNextOccurence(search, "WIDGET")) != NULL) 
		{
			CWidget* wdg = NULL;
			
			char* name = NULL;
			unsigned int id = 0;
			
			unsigned int x = 0;
			unsigned int y = 0;
			unsigned int dx = 0;
			unsigned int dy = 0;
			
			char* color = NULL;
			unsigned int gradient = 0;
			unsigned int corner = 0;
			unsigned int radius = 0;
			
			unsigned int paintframe = 0;
			unsigned int savescreen = 0;
			unsigned int timeout = 0;
			
			//
			name = xmlGetAttribute(search, (char*)"name");
			id = xmlGetSignedNumericAttribute(search, "id", 0);
			
			x = xmlGetSignedNumericAttribute(search, "posx", 0);
			y = xmlGetSignedNumericAttribute(search, "posy", 0);
			dx = xmlGetSignedNumericAttribute(search, "width", 0);
			dy = xmlGetSignedNumericAttribute(search, "height", 0);
			
			color = xmlGetAttribute(search, (char*)"color");
			gradient = xmlGetSignedNumericAttribute(search, "gradient", 0);
			corner = xmlGetSignedNumericAttribute(search, "corner", 0);
			radius = xmlGetSignedNumericAttribute(search, "radius", 0);
			
			paintframe = xmlGetSignedNumericAttribute(search, "paintframe", 0);
			savescreen = xmlGetSignedNumericAttribute(search, "savescreen", 0);
			timeout = xmlGetSignedNumericAttribute(search, "timeout", 0);
			
			// parse color
			uint32_t wColor = COL_MENUCONTENT_PLUS_0;
				
			if (color != NULL) wColor = convertColor(color);
			
			//
			wdg = new CWidget(x, y, dx, dy);
			
			wdg->id = id;
			if (name != NULL) wdg->name = name;
			wdg->paintMainFrame(paintframe);
			if (color != NULL) wdg->setColor(wColor); // FIXME: corrupted
			wdg->setGradient(gradient);
			wdg->setCorner(corner, radius);
			if (savescreen) wdg->enableSaveScreen();
			wdg->setTimeOut(timeout);
			
			widgets.push_back(wdg);
			
			// WINDOW
			window_node = search->xmlChildrenNode;
			
			while ((window_node = xmlGetNextOccurence(window_node, "WINDOW")) != NULL) 
			{
				//char* widgetitemname = NULL;
				//unsigned int widgetitemid = 0;
				
				unsigned int posx = 0;
				unsigned int posy = 0;
				unsigned int width = 0;
				unsigned int height = 0;
				
				unsigned int i_paintframe = 1;
				char* i_color = NULL;
				unsigned int i_gradient = 0;
				unsigned int i_corner = 0;
				unsigned int i_radius = 0;
				
				//widgetitemname = xmlGetAttribute(window_node, (char*)"name");
				//widgetitemid = xmlGetSignedNumericAttribute(window_node, "id", 0);
				
				posx = xmlGetSignedNumericAttribute(window_node, "posx", 0);
				posy = xmlGetSignedNumericAttribute(window_node, "posy", 0);
				width = xmlGetSignedNumericAttribute(window_node, "width", 0);
				height = xmlGetSignedNumericAttribute(window_node, "height", 0);
				
				i_paintframe = xmlGetSignedNumericAttribute(window_node, "paintframe", 0);
				
				i_color = xmlGetAttribute(window_node, (char*)"color");
				i_gradient = xmlGetSignedNumericAttribute(window_node, "gradient", 0);
				i_corner = xmlGetSignedNumericAttribute(window_node, "corner", 0);
				i_radius = xmlGetSignedNumericAttribute(window_node, "radius", 0);
				
				// parse color
				uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
				if (i_color) finalColor = convertColor(i_color);
				
				unsigned int refresh = 0;
				refresh = xmlGetSignedNumericAttribute(window_node, "refresh", 0);
					
				CWindow* window = NULL;
					
				window = new CWindow(posx, posy, width, height);
					
				window->paintMainFrame(i_paintframe);
				if (i_color) window->setColor(finalColor);
				if (refresh) window->enableRepaint();
					
				wdg->addItem(window);
			
				window_node = window_node->xmlNextNode;
			}
			
			// HEAD
			head_node = search->xmlChildrenNode;
			
			while ((head_node = xmlGetNextOccurence(head_node, "HEAD")) != NULL) 
			{
				//char* widgetitemname = NULL;
				//unsigned int widgetitemid = 0;
				
				unsigned int posx = 0;
				unsigned int posy = 0;
				unsigned int width = 0;
				unsigned int height = 0;
				
				unsigned int i_paintframe = 1;
				char* i_color = NULL;
				unsigned int i_gradient = 0;
				unsigned int i_corner = 0;
				unsigned int i_radius = 0;
				
				//widgetitemname = xmlGetAttribute(head_node, (char*)"name");
				//widgetitemid = xmlGetSignedNumericAttribute(head_node, "id", 0);
				
				posx = xmlGetSignedNumericAttribute(head_node, "posx", 0);
				posy = xmlGetSignedNumericAttribute(head_node, "posy", 0);
				width = xmlGetSignedNumericAttribute(head_node, "width", 0);
				height = xmlGetSignedNumericAttribute(head_node, "height", 0);
				
				i_paintframe = xmlGetSignedNumericAttribute(head_node, "paintframe", 0);
				
				i_color = xmlGetAttribute(head_node, (char*)"color");
				i_gradient = xmlGetSignedNumericAttribute(head_node, "gradient", 0);
				i_corner = xmlGetSignedNumericAttribute(head_node, "corner", 0);
				i_radius = xmlGetSignedNumericAttribute(head_node, "radius", 0);
				
				// parse color
				uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
				if (i_color) finalColor = convertColor(i_color);
				
				char* title = NULL;
				unsigned int halign = 0;
				const char* icon = NULL;
				unsigned int h_line = 0;
				unsigned int paintdate = 0;
				char* format = NULL;
					
				title = xmlGetAttribute(head_node, (char*)"localename");
				halign = xmlGetSignedNumericAttribute(head_node, "halign", 0);
				icon = xmlGetAttribute(head_node, (char*)"icon");
				h_line = xmlGetSignedNumericAttribute(head_node, "line", 0);
				paintdate = xmlGetSignedNumericAttribute(head_node, "paintdate", 0);
				format = xmlGetAttribute(head_node, (char*)"format");
				
				CHeaders* head = NULL;
				head = new CHeaders(posx, posy, width, height);
					
				if (title != NULL) head->setTitle(_(title));
				head->setHAlign(halign);
				if (icon != NULL) head->setIcon(icon);
				if(i_color != NULL) head->setColor(finalColor);
				head->setGradient(i_gradient);
				head->setCorner(i_corner);
				head->setRadius(i_radius);
				head->setHeadLine(h_line);
				if (paintdate) head->enablePaintDate();
				if (format != NULL) head->setFormat(_(format));
					
				//
				headbutton_node = head_node->xmlChildrenNode;
					
				while ((headbutton_node = xmlGetNextOccurence(headbutton_node, "BUTTON")) != NULL) 
				{
					char* button = NULL;
					char* localename = NULL;
						
					button = xmlGetAttribute(headbutton_node, (char*)"name");
					localename = xmlGetAttribute(headbutton_node, (char*)"localename");
						
					button_label_struct btn;
					btn.button = "";
					if (button) btn.button = button;
					btn.localename = "";
					if (localename) btn.localename = localename;
						
					head->setButtons(&btn);
				
					headbutton_node = headbutton_node->xmlNextNode;
				}
					
				wdg->addItem(head);
			
				head_node = head_node->xmlNextNode;
			}
			
			// FOOT
			foot_node = search->xmlChildrenNode;
			
			while ((foot_node = xmlGetNextOccurence(foot_node, "FOOT")) != NULL) 
			{
				//char* widgetitemname = NULL;
				//unsigned int widgetitemid = 0;
				
				unsigned int posx = 0;
				unsigned int posy = 0;
				unsigned int width = 0;
				unsigned int height = 0;
				
				unsigned int i_paintframe = 1;
				char* i_color = NULL;
				unsigned int i_gradient = 0;
				unsigned int i_corner = 0;
				unsigned int i_radius = 0;
				
				//widgetitemname = xmlGetAttribute(foot_node, (char*)"name");
				//widgetitemid = xmlGetSignedNumericAttribute(foot_node, "id", 0);
				
				posx = xmlGetSignedNumericAttribute(foot_node, "posx", 0);
				posy = xmlGetSignedNumericAttribute(foot_node, "posy", 0);
				width = xmlGetSignedNumericAttribute(foot_node, "width", 0);
				height = xmlGetSignedNumericAttribute(foot_node, "height", 0);
				
				i_paintframe = xmlGetSignedNumericAttribute(foot_node, "paintframe", 0);
				
				i_color = xmlGetAttribute(foot_node, (char*)"color");
				i_gradient = xmlGetSignedNumericAttribute(foot_node, "gradient", 0);
				i_corner = xmlGetSignedNumericAttribute(foot_node, "corner", 0);
				i_radius = xmlGetSignedNumericAttribute(foot_node, "radius", 0);
				
				// parse color
				uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
				if (i_color) finalColor = convertColor(i_color);
				
				unsigned int f_line = 0;
				f_line = xmlGetSignedNumericAttribute(foot_node, "line", 0);
						
				CFooters* foot = NULL;
				foot = new CFooters(posx, posy, width, height);
					
				if (i_color != NULL) foot->setColor(finalColor);
				foot->setGradient(i_gradient);
				foot->setCorner(i_corner);
				foot->setRadius(i_radius);
				foot->setFootLine(f_line);
					
				footbutton_node = foot_node->xmlChildrenNode;
					
				while ((footbutton_node = xmlGetNextOccurence(footbutton_node, "BUTTON")) != NULL) 
				{
					char* button = NULL;
					char* localename = NULL;
						
					button = xmlGetAttribute(footbutton_node, (char*)"name");
					localename = xmlGetAttribute(footbutton_node, (char*)"localename");
						
					button_label_struct btn;
					btn.button = "";
					if (button) btn.button = button;
					btn.localename = "";
					if (localename) btn.localename = localename;
						
					foot->setButtons(&btn);
				
					footbutton_node = footbutton_node->xmlNextNode;
				}
					
				wdg->addItem(foot);
			
				foot_node = foot_node->xmlNextNode;
			}
			
			// LISTBOX
			listbox_node = search->xmlChildrenNode;
			
			while ((listbox_node = xmlGetNextOccurence(listbox_node, "LISTBOX")) != NULL) 
			{
				//char* widgetitemname = NULL;
				//unsigned int widgetitemid = 0;
				
				unsigned int posx = 0;
				unsigned int posy = 0;
				unsigned int width = 0;
				unsigned int height = 0;
				
				unsigned int i_paintframe = 1;
				char* i_color = NULL;
				unsigned int i_gradient = 0;
				unsigned int i_corner = 0;
				unsigned int i_radius = 0;
				
				//widgetitemname = xmlGetAttribute(listbox_node, (char*)"name");
				//widgetitemid = xmlGetSignedNumericAttribute(listbox_node, "id", 0);
				
				posx = xmlGetSignedNumericAttribute(listbox_node, "posx", 0);
				posy = xmlGetSignedNumericAttribute(listbox_node, "posy", 0);
				width = xmlGetSignedNumericAttribute(listbox_node, "width", 0);
				height = xmlGetSignedNumericAttribute(listbox_node, "height", 0);
				
				i_paintframe = xmlGetSignedNumericAttribute(listbox_node, "paintframe", 0);
				
				i_color = xmlGetAttribute(listbox_node, (char*)"color");
				i_gradient = xmlGetSignedNumericAttribute(listbox_node, "gradient", 0);
				i_corner = xmlGetSignedNumericAttribute(listbox_node, "corner", 0);
				i_radius = xmlGetSignedNumericAttribute(listbox_node, "radius", 0);
				
				// parse color
				uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
				if (i_color) finalColor = convertColor(i_color);
				
				unsigned int type = 0;
				unsigned int scrollbar = 1;
				unsigned int shadow = 0;
					
				type = xmlGetSignedNumericAttribute(listbox_node, "type", 0);
				scrollbar = xmlGetSignedNumericAttribute(listbox_node, "scrollbar", 0);
				shadow = xmlGetSignedNumericAttribute(listbox_node, "shadow", 0);
					
				ClistBox* listBox = NULL;
					
				listBox = new ClistBox(posx, posy, width, height);
				listBox->setWidgetType(type);
				listBox->paintScrollBar(scrollbar);
				listBox->paintMainFrame(i_paintframe);
				if (i_color != NULL) listBox->setColor(finalColor);
				listBox->paintItemShadow(shadow);
					
				CMenuItem* menuItem = NULL;
					
				listboxitem_node = listbox_node->xmlChildrenNode;
					
				while ((listboxitem_node = xmlGetNextOccurence(listboxitem_node, "ITEM")) != NULL) 
				{
					//char* itemname = NULL;
					unsigned int itemid = 5;
						
					char* item_localename = NULL;
					char* option = NULL;
					char* item_actionkey = NULL;
					int item_target = -1;
					char* itemIcon = NULL;
					char* hint = NULL;
					char* iconName = NULL;
					neutrino_msg_t directkey = 0;
					unsigned int lines = 0;
						
						
					//itemname = xmlGetAttribute(listbox_node, (char*)"name");
					itemid = xmlGetSignedNumericAttribute(listboxitem_node, "id", 0);
						
					item_localename = xmlGetAttribute(listboxitem_node, (char*)"localename");
					option = xmlGetAttribute(listboxitem_node, (char*)"option");
					item_actionkey = xmlGetAttribute(listboxitem_node, (char*)"actionkey");
					item_target = xmlGetSignedNumericAttribute(listboxitem_node, "target", 0);
					itemIcon = xmlGetAttribute(listboxitem_node, (char*)"itemicon");
					hint = xmlGetAttribute(listboxitem_node, (char*)"hint");
					iconName = xmlGetAttribute(listboxitem_node, (char*)"iconname");
					directkey = (neutrino_msg_t)xmlGetSignedNumericAttribute(listboxitem_node, "directkey", 0);
					lines = xmlGetSignedNumericAttribute(listboxitem_node, "lines", 0);
						
					CMenuTarget* parent = NULL;
					std::string actionKey = "";
					std::string itemName = "";
						
					if (item_localename) itemName = _(item_localename);
						
					if (itemid == ITEM_TYPE_FORWARDER)
						menuItem = new CMenuForwarder(itemName.c_str());
					else if (itemid == ITEM_TYPE_LISTBOXITEM)
						menuItem = new ClistBoxItem(itemName.c_str());
						
					if (item_actionkey) actionKey = item_actionkey;	
					parent = convertTarget(item_target);
							
					menuItem->setActionKey(parent, actionKey.c_str());
					
					if (directkey) menuItem->setDirectKey(directkey);	
					if (iconName) menuItem->setIconName(iconName);	
					if (hint) menuItem->setHint(hint);
					if (itemIcon) menuItem->setItemIcon(itemIcon);
					if (lines) menuItem->set2lines();
					if (option) menuItem->setOption(option);
						
					listBox->addItem(menuItem);
				
					listboxitem_node = listboxitem_node->xmlNextNode;
				}
				
				//
				listboxintegration_node = listbox_node->xmlChildrenNode;
					
				while ((listboxintegration_node = xmlGetNextOccurence(listboxintegration_node, "INTEGRATION")) != NULL) 
				{
					CPlugins::i_type_t integration = CPlugins::I_TYPE_DISABLED;
					unsigned int mode = MODE_MENU;
					unsigned int shortcut = RC_nokey;
					unsigned int type = WIDGET_TYPE_STANDARD;
					unsigned int lines = false;
					unsigned int shadow = false;
						
					integration = (CPlugins::i_type_t)xmlGetSignedNumericAttribute(listboxintegration_node, "id", 0);
					mode = xmlGetSignedNumericAttribute(listboxintegration_node, "mode", 0);
					shortcut = xmlGetSignedNumericAttribute(listboxintegration_node, "shortcut", 0);
					type = xmlGetSignedNumericAttribute(listboxintegration_node, "type", 0);
					lines = xmlGetSignedNumericAttribute(listboxintegration_node, "lines", 0);
					shadow = xmlGetSignedNumericAttribute(listboxintegration_node, "shadow", 0);
						
					listBox->integratePlugins(integration, shortcut, true, mode, type, lines, shadow);
				
					listboxintegration_node = listboxintegration_node->xmlNextNode;
				}
					
				wdg->addItem(listBox);
			
				listbox_node = listbox_node->xmlNextNode;
			}
			
			// LABEL
			label_node = search->xmlChildrenNode;
			
			while ((label_node = xmlGetNextOccurence(label_node, "LABEL")) != NULL) 
			{
				//char* cc_name = NULL;
				//unsigned int cc_id = 0;
				
				unsigned int cc_x = 0;
				unsigned int cc_y = 0;
				unsigned int cc_dx = 0;
				unsigned int cc_dy = 0;
						
				unsigned int cc_refresh = 0;
						
						
				//cc_name = xmlGetAttribute(label_node, (char*)"name");
				//cc_id = xmlGetSignedNumericAttribute(label_node, "id", 0);
				
				cc_x = xmlGetSignedNumericAttribute(label_node, "posx", 0);
				cc_y = xmlGetSignedNumericAttribute(label_node, "posy", 0);
				cc_dx = xmlGetSignedNumericAttribute(label_node, "width", 0);
				cc_dy = xmlGetSignedNumericAttribute(label_node, "height", 0);
						
				cc_refresh = xmlGetSignedNumericAttribute(label_node, "refresh", 0);
						
				char* text = NULL;
				unsigned int l_halign = 0;
							
				text = xmlGetAttribute(label_node, (char*)"text");
				l_halign = xmlGetSignedNumericAttribute(label_node, "halign", 0);
						
				CCLabel* label = NULL;
							
				label = new CCLabel(cc_x, cc_y, cc_dx, cc_dy);
							
				if (text != NULL) label->setText(_(text));
				label->setHAlign(l_halign);
							
				wdg->addCCItem(label);	
			
				label_node = label_node->xmlNextNode;
			}
			
			// IMAGE
			image_node = search->xmlChildrenNode;
			
			while ((image_node = xmlGetNextOccurence(image_node, "IMAGE")) != NULL) 
			{
				//char* cc_name = NULL;
				//unsigned int cc_id = 0;
				
				unsigned int cc_x = 0;
				unsigned int cc_y = 0;
				unsigned int cc_dx = 0;
				unsigned int cc_dy = 0;
						
				unsigned int cc_refresh = 0;
						
						
				//cc_name = xmlGetAttribute(image_node, (char*)"name");
				//cc_id = xmlGetSignedNumericAttribute(image_node, "id", 0);
				
				cc_x = xmlGetSignedNumericAttribute(image_node, "posx", 0);
				cc_y = xmlGetSignedNumericAttribute(image_node, "posy", 0);
				cc_dx = xmlGetSignedNumericAttribute(image_node, "width", 0);
				cc_dy = xmlGetSignedNumericAttribute(image_node, "height", 0);
						
				cc_refresh = xmlGetSignedNumericAttribute(image_node, "refresh", 0);
				
				char* image = NULL;
				image = xmlGetAttribute(image_node, (char*)"image");
						
				CCImage* pic = NULL;
							
				pic = new CCImage(cc_x, cc_y, cc_dx, cc_dy);
							
				std::string filename = CONFIGDIR "/skins/";
				filename += g_settings.preferred_skin;
				filename += "/";
				filename += image;
							
				if (image != NULL) pic->setImage(filename.c_str());
							
				wdg->addCCItem(pic);	
			
				image_node = image_node->xmlNextNode;
			}
			
			// TIME
			time_node = search->xmlChildrenNode;
			
			while ((time_node = xmlGetNextOccurence(time_node, "TIME")) != NULL) 
			{
				//char* cc_name = NULL;
				//unsigned int cc_id = 0;
				
				unsigned int cc_x = 0;
				unsigned int cc_y = 0;
				unsigned int cc_dx = 0;
				unsigned int cc_dy = 0;
						
				unsigned int cc_refresh = 0;
						
						
				//cc_name = xmlGetAttribute(time_node, (char*)"name");
				//cc_id = xmlGetSignedNumericAttribute(time_node, "id", 0);
				
				cc_x = xmlGetSignedNumericAttribute(time_node, "posx", 0);
				cc_y = xmlGetSignedNumericAttribute(time_node, "posy", 0);
				cc_dx = xmlGetSignedNumericAttribute(time_node, "width", 0);
				cc_dy = xmlGetSignedNumericAttribute(time_node, "height", 0);
						
				cc_refresh = xmlGetSignedNumericAttribute(time_node, "refresh", 0);
					
				char* cc_format = NULL;
				cc_format = xmlGetAttribute(time_node, (char*)"format");
						
				CCTime* time = NULL;
							
				time = new CCTime(cc_x, cc_y, cc_dx, cc_dy);
							
				if (cc_format != NULL) time->setFormat(_(cc_format)); //FIXME: corrupted
				if (cc_refresh) time->enableRepaint();
							
				wdg->addCCItem(time);	
			
				time_node = time_node->xmlNextNode;
			}
			
			// key
			key_node = search->xmlChildrenNode;
			
			while ((key_node = xmlGetNextOccurence(key_node, "KEY")) != NULL) 
			{
				neutrino_msg_t key_name = (neutrino_msg_t)xmlGetSignedNumericAttribute(key_node, "name", 16);
				char* key_actionkey = xmlGetAttribute(key_node, (char*)"actionkey");
				unsigned int key_target = xmlGetSignedNumericAttribute(key_node, "target", 0);
				
				CMenuTarget* key_parent = NULL;
				
				key_parent = convertTarget(key_target);
				
				wdg->addKey((neutrino_msg_t)key_name, key_parent, key_actionkey);
			
				key_node = key_node->xmlNextNode;
			}
						
			//
			search = search->xmlNextNode;		
		}
	}
	
	xmlFreeDoc(parser);
	parser = NULL;
}

//
void CNeutrinoApp::loadSkin(std::string skinName)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::loadSkin: %s\n", skinName.c_str());
	
	//
	parseSkin();
	
#if ENABLE_LUA	
	// unload
	unloadSkin();
	
	//
	std::string skinPath = CONFIGDIR "/skins/";
	skinPath += skinName.c_str();
	
	std::string fontFileName;
	
	// load skin
	struct dirent **namelist;
	int i = 0;

	i = scandir(skinPath.c_str(), &namelist, 0, 0);

	if (i > 0)
	{
		while(i--)
		{
			if( (strcmp(namelist[i]->d_name, ".") != 0) && (strcmp(namelist[i]->d_name, "..") != 0) )
			{
				std::string filename = skinPath.c_str();
				filename += "/";
				filename += namelist[i]->d_name;
				
				std::string extension = getFileExt(filename);
				
				if ( strcasecmp("lua", extension.c_str()) == 0)
				{
					CPlugins::plugin new_skin;
		
					if (!filename.empty())
					{
						dprintf(DEBUG_INFO, "CNeutrinoApp::loadskin: add: %s\n", filename.c_str());
						
						new_skin.pluginfile = filename;
						new_skin.type = CPlugins::P_TYPE_LUA;
						new_skin.hide = true;
						
						new_skin.filename = getBaseName(filename);
						new_skin.filename = removeExtension(new_skin.filename);
					
						skin_list.push_back(new_skin);
					}
				}
					
				if ( strcasecmp("ttf", extension.c_str()) == 0)
					fontFileName = filename;
				
				filename.clear();			
			}
			free(namelist[i]);
		}
		free(namelist);
	}
	
	if (CNeutrinoApp::getInstance()->skin_exists("mainmenu"))
	{
		// setup font
		std::string fontPath = skinPath.c_str();
		fontPath += "/fonts";
		
		i = scandir(fontPath.c_str(), &namelist, 0, 0);

		if (i > 0)
		{
			while(i--)
			{
				if( (strcmp(namelist[i]->d_name, ".") != 0) && (strcmp(namelist[i]->d_name, "..") != 0) )
				{
					std::string filename = fontPath.c_str();
					filename += "/";
					filename += namelist[i]->d_name;
					
					std::string extension = getFileExt(filename);
						
					if ( strcasecmp("ttf", extension.c_str()) == 0)
						fontFileName = filename;
					
					filename.clear();			
				}
				free(namelist[i]);
			}
			free(namelist);
		}
		 
		strcpy( g_settings.font_file, fontFileName.c_str() );
		
		CNeutrinoApp::getInstance()->SetupFonts(g_settings.font_file);
		
		// setIconPath
		std::string iconsDir = CONFIGDIR "/skins/";
		iconsDir += skinName.c_str();
		iconsDir += "/icons/";
		
		// check if not empty
		i = scandir(iconsDir.c_str(), &namelist, 0, 0);
		if(i < 0)
		{
			g_settings.icons_dir = DATADIR "/neutrino/icons/"; //fallback to default if empty
		}
		else
		{
			g_settings.icons_dir = iconsDir;
			free(namelist);
		}
		
		frameBuffer->setIconBasePath(g_settings.icons_dir);
		
		// setButtonPath
		std::string buttonsDir = CONFIGDIR "/skins/";
		buttonsDir += skinName.c_str();
		buttonsDir += "/buttons/";
		
		// check if not empty
		i = scandir(buttonsDir.c_str(), &namelist, 0, 0);
		if(i < 0)
		{
			g_settings.buttons_dir = DATADIR "/neutrino/buttons/"; //fallback to default if empty
		}
		else
		{
			g_settings.buttons_dir = buttonsDir;
			free(namelist);
		}
		
		frameBuffer->setButtonBasePath(g_settings.buttons_dir);
		
		// setHintPath
		std::string hintsDir = CONFIGDIR "/skins/";
		hintsDir += skinName.c_str();
		hintsDir += "/hints/";
		
		// check if not empty
		i = scandir(hintsDir.c_str(), &namelist, 0, 0);
		if(i < 0)
		{
			g_settings.hints_dir = DATADIR "/neutrino/hints/"; //fallback to default if empty
		}
		else
		{
			g_settings.hints_dir = hintsDir;
			free(namelist);
		}
		
		frameBuffer->setHintBasePath(g_settings.hints_dir);
		
		// setup colors / corners / position
		std::string skinConfigFile = CONFIGDIR "/skins/";
		skinConfigFile += skinName.c_str();
		skinConfigFile += "/";
		skinConfigFile += skinName.c_str();
		skinConfigFile += ".config";
		
		readSkinConfig(skinConfigFile.c_str());
	}
	else // if mainmenu.lua not found fallback to default (neutrino intern)
	{
		strcpy( g_settings.font_file, DATADIR "/neutrino/fonts/arial.ttf");
		
		CNeutrinoApp::getInstance()->SetupFonts(DATADIR "/neutrino/fonts/arial.ttf");
		
		g_settings.icons_dir = DATADIR "/neutrino/icons/";
		g_settings.buttons_dir = DATADIR "/neutrino/buttons/";
		g_settings.hints_dir = DATADIR "/neutrino/hints/";
		
		frameBuffer->setIconBasePath(DATADIR "/neutrino/icons/");
		frameBuffer->setButtonBasePath(DATADIR "/neutrino/buttons/");
		frameBuffer->setHintBasePath(DATADIR "/neutrino/hints/");
	}
#endif	
}

void CNeutrinoApp::startSkin(const char * const filename)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::startSkin: %s\n", filename);
	
#if ENABLE_LUA	
	int skinnr = -1;
	
	for (int i = 0; i <  (int) skin_list.size(); i++)
	{
		if ( strcasecmp(filename, skin_list[i].filename.c_str()) == 0)
		{
			skinnr = i;
		}
	}
	
	if (skinnr > -1)
	{
		// LUA
		neutrinoLua* luaInvoker = new neutrinoLua();

		luaInvoker->execFile(skin_list[skinnr].pluginfile.c_str());

		delete luaInvoker;
		luaInvoker = NULL;
	}
	else
	{
		dprintf(DEBUG_INFO, "CPlugins::startSkin: could not find %s\n", filename);
		
		std::string hint = filename;
		hint += " ";
		hint += _("is not installed please install again.");
		
		HintBox(_("Information"), _(hint.c_str()));
	}
#endif	
}

bool CNeutrinoApp::skin_exists(const char* const filename)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::skin_exists: %s\n", filename);
	
	for (int i = 0; i <  (int) skin_list.size(); i++)
	{
		if ( strcasecmp(filename, skin_list[i].filename.c_str()) == 0)
			return true;
	}
	
	return false;
}

void CNeutrinoApp::unloadSkin()
{
	// clearb cache
	skin_list.clear();
	
	// set font to arial
	strcpy( g_settings.font_file, DATADIR "/neutrino/fonts/arial.ttf");
		
	CNeutrinoApp::getInstance()->SetupFonts(DATADIR "/neutrino/fonts/arial.ttf");
		
	g_settings.icons_dir = DATADIR "/neutrino/icons/";
	g_settings.buttons_dir = DATADIR "/neutrino/buttons/";
	g_settings.hints_dir = DATADIR "/neutrino/hints/";
		
	frameBuffer->setIconBasePath(DATADIR "/neutrino/icons/");
	frameBuffer->setButtonBasePath(DATADIR "/neutrino/buttons/");
	frameBuffer->setHintBasePath(DATADIR "/neutrino/hints/");
	
	// set colors to default
	CThemes* themes = new CThemes();
	//themes->exec(NULL, "theme_default");
	themes->setupDefaultColors();
	
	// menu
	g_settings.menu_shadow = false;
	
	// infobar
	g_settings.infobar_gradient = NOGRADIENT;
	g_settings.infobar_gradient_direction = GRADIENT_HORIZONTAL;
	g_settings.infobar_radius = NO_RADIUS;
	g_settings.infobar_corner = CORNER_NONE;
	g_settings.infobar_buttonbar = true;
	g_settings.infobar_buttonline = false;
	g_settings.infobar_shadow = false;
	
	// head
	g_settings.Head_radius = RADIUS_MID;
	g_settings.Head_corner = CORNER_TOP;
	g_settings.Head_gradient = LIGHT2DARK;
	
	// foot
	g_settings.Foot_radius = RADIUS_MID;
	g_settings.Foot_corner = CORNER_BOTTOM;
	g_settings.Foot_gradient = DARK2LIGHT;
	
	// itemInfo
	g_settings.Foot_Info_shadow = configfile.getBool("Foot_Info_shadow", false);
	g_settings.Foot_Info_gradient = configfile.getInt32("Foot_Info_gradient", NOGRADIENT);
	
	delete themes;
	themes = NULL;
}

//
void CNeutrinoApp::readSkinConfig(const char* const filename)
{
	dprintf(DEBUG_INFO, "CNeutrinpApp::readSkinConfig %s\n", filename);
	
	CConfigFile* skinConfig = new CConfigFile(',');
	
	if(skinConfig->loadConfig(filename))
	{
		g_settings.menu_Head_alpha = skinConfig->getInt32( "menu_Head_alpha", 15);
		g_settings.menu_Head_red = skinConfig->getInt32( "menu_Head_red", 15);
		g_settings.menu_Head_green = skinConfig->getInt32( "menu_Head_green", 15);
		g_settings.menu_Head_blue = skinConfig->getInt32( "menu_Head_blue", 15);

		g_settings.menu_Head_Text_alpha = skinConfig->getInt32( "menu_Head_Text_alpha", 0);
		g_settings.menu_Head_Text_red = skinConfig->getInt32( "menu_Head_Text_red", 100 );
		g_settings.menu_Head_Text_green = skinConfig->getInt32( "menu_Head_Text_green", 100 );
		g_settings.menu_Head_Text_blue = skinConfig->getInt32( "menu_Head_Text_blue", 100 );
	
		g_settings.menu_Content_alpha = skinConfig->getInt32( "menu_Content_alpha", 20);
		g_settings.menu_Content_red = skinConfig->getInt32( "menu_Content_red", 25);
		g_settings.menu_Content_green = skinConfig->getInt32( "menu_Content_green", 25);
		g_settings.menu_Content_blue = skinConfig->getInt32( "menu_Content_blue", 25);
		
		g_settings.menu_Content_Text_alpha = skinConfig->getInt32( "menu_Content_Text_alpha", 0);
		g_settings.menu_Content_Text_red = skinConfig->getInt32( "menu_Content_Text_red", 85 );
		g_settings.menu_Content_Text_green = skinConfig->getInt32( "menu_Content_Text_green", 85 );
		g_settings.menu_Content_Text_blue = skinConfig->getInt32( "menu_Content_Text_blue", 85 );
	
		g_settings.menu_Content_Selected_alpha = skinConfig->getInt32( "menu_Content_Selected_alpha", 20);
		g_settings.menu_Content_Selected_red = skinConfig->getInt32( "menu_Content_Selected_red", 75);
		g_settings.menu_Content_Selected_green = skinConfig->getInt32( "menu_Content_Selected_green", 75);
		g_settings.menu_Content_Selected_blue = skinConfig->getInt32( "menu_Content_Selected_blue", 75);
		
		g_settings.menu_Content_Selected_Text_alpha = skinConfig->getInt32( "menu_Content_Selected_Text_alpha", 0);
		g_settings.menu_Content_Selected_Text_red = skinConfig->getInt32( "menu_Content_Selected_Text_red", 25 );
		g_settings.menu_Content_Selected_Text_green = skinConfig->getInt32( "menu_Content_Selected_Text_green", 25 );
		g_settings.menu_Content_Selected_Text_blue = skinConfig->getInt32( "menu_Content_Selected_Text_blue", 25 );
	
		g_settings.menu_Content_inactive_alpha = skinConfig->getInt32( "menu_Content_inactive_alpha", 20);
		g_settings.menu_Content_inactive_red = skinConfig->getInt32( "menu_Content_inactive_red", 25);
		g_settings.menu_Content_inactive_green = skinConfig->getInt32( "menu_Content_inactive_green", 25);
		g_settings.menu_Content_inactive_blue = skinConfig->getInt32( "menu_Content_inactive_blue", 25);
		
		g_settings.menu_Content_inactive_Text_alpha = skinConfig->getInt32( "menu_Content_inactive_Text_alpha", 0);
		g_settings.menu_Content_inactive_Text_red = skinConfig->getInt32( "menu_Content_inactive_Text_red", 55);
		g_settings.menu_Content_inactive_Text_green = skinConfig->getInt32( "menu_Content_inactive_Text_green", 55);
		g_settings.menu_Content_inactive_Text_blue = skinConfig->getInt32( "menu_Content_inactive_Text_blue", 55);

		g_settings.infobar_alpha = skinConfig->getInt32( "infobar_alpha", 20);
		g_settings.infobar_red = skinConfig->getInt32( "infobar_red", 25);
		g_settings.infobar_green = skinConfig->getInt32( "infobar_green", 25);
		g_settings.infobar_blue = skinConfig->getInt32( "infobar_blue", 25);
		
		g_settings.infobar_Text_alpha = skinConfig->getInt32( "infobar_Text_alpha", 0);
		g_settings.infobar_Text_red = skinConfig->getInt32( "infobar_Text_red", 100);
		g_settings.infobar_Text_green = skinConfig->getInt32( "infobar_Text_green", 100);
		g_settings.infobar_Text_blue = skinConfig->getInt32( "infobar_Text_blue", 100);
		
		g_settings.infobar_colored_events_alpha = skinConfig->getInt32( "infobar_colored_events_alpha", 0);
		g_settings.infobar_colored_events_red = skinConfig->getInt32( "infobar_colored_events_red", 95);
		g_settings.infobar_colored_events_green = skinConfig->getInt32( "infobar_colored_events_green", 70);
		g_settings.infobar_colored_events_blue = skinConfig->getInt32( "infobar_colored_events_blue", 0);
	
		g_settings.menu_Foot_alpha = skinConfig->getInt32( "menu_Foot_alpha", 15);
		g_settings.menu_Foot_red = skinConfig->getInt32( "menu_Foot_red", 15);
		g_settings.menu_Foot_green = skinConfig->getInt32( "menu_Foot_green", 15);
		g_settings.menu_Foot_blue = skinConfig->getInt32( "menu_Foot_blue", 15);
		
		g_settings.menu_Foot_Text_alpha = skinConfig->getInt32( "menu_Foot_Text_alpha", 0);
		g_settings.menu_Foot_Text_red = skinConfig->getInt32( "menu_Foot_Text_red", 100);
		g_settings.menu_Foot_Text_green = skinConfig->getInt32( "menu_Foot_Text_green", 100);
		g_settings.menu_Foot_Text_blue = skinConfig->getInt32( "menu_Foot_Text_blue", 100);

		g_settings.menu_FootInfo_alpha = skinConfig->getInt32( "menu_FootInfo_alpha", 0);
		g_settings.menu_FootInfo_red = skinConfig->getInt32( "menu_FootInfo_red", 15);
		g_settings.menu_FootInfo_green = skinConfig->getInt32( "menu_FootInfo_green", 15);
		g_settings.menu_FootInfo_blue = skinConfig->getInt32( "menu_FootInfo_blue", 15);
		
		g_settings.menu_FootInfo_Text_alpha = skinConfig->getInt32( "menu_FootInfo_Text_alpha", 0);
		g_settings.menu_FootInfo_Text_red = skinConfig->getInt32( "menu_FootInfo_Text_red", 85);
		g_settings.menu_FootInfo_Text_green = skinConfig->getInt32( "menu_FootInfo_Text_green", 85);
		g_settings.menu_FootInfo_Text_blue = skinConfig->getInt32( "menu_FootInfo_Text_blue", 85);
		
		// head
		g_settings.Head_gradient = skinConfig->getInt32("Head_gradient", DARK2LIGHT2DARK);
		g_settings.Head_corner = skinConfig->getInt32("Head_corner", CORNER_TOP);
		g_settings.Head_radius = skinConfig->getInt32("Head_radius", RADIUS_MID);
		g_settings.Head_line = skinConfig->getBool("Head_line", false);
		
		// foot
		g_settings.Foot_gradient = skinConfig->getInt32("Foot_gradient", DARK2LIGHT2DARK);
		g_settings.Foot_corner = skinConfig->getInt32("Foot_corner", CORNER_BOTTOM);
		g_settings.Foot_radius = skinConfig->getInt32("Foot_radius", RADIUS_MID);
		g_settings.Foot_line = skinConfig->getBool("Foot_line", false);
		
		// infobar
		g_settings.infobar_gradient = skinConfig->getInt32("infobar_gradient", NOGRADIENT);
		g_settings.infobar_gradient_direction = skinConfig->getInt32("infobar_gradient_direction", GRADIENT_HORIZONTAL);
		g_settings.infobar_corner = skinConfig->getInt32("infobar_corner", CORNER_ALL);
		g_settings.infobar_radius = skinConfig->getInt32("infobar_radius", RADIUS_MID);
		g_settings.infobar_shadow = skinConfig->getBool("infobar_shadow", false);
		g_settings.infobar_buttonbar = skinConfig->getBool("infobar_buttonbar", true);
		g_settings.infobar_buttonline = skinConfig->getBool("infobar_buttonline", false);
		
		//
		g_settings.menu_shadow = skinConfig->getBool("menu_shadow", false);
		
		// itemInfo
		g_settings.Foot_Info_shadow = skinConfig->getBool("Foot_Info_shadow", false);
		g_settings.Foot_Info_gradient = configfile.getInt32("Foot_Info_gradient", NOGRADIENT);
		
		strcpy( g_settings.font_file, skinConfig->getString( "font_file", DATADIR "/neutrino/fonts/arial.ttf" ).c_str() );

		colorSetupNotifier = new CColorSetupNotifier;
		colorSetupNotifier->changeNotify("", NULL);
		
		delete colorSetupNotifier;
	}
	else
		printf("CNeutrinoApp::readSkinConfig: %s not found\n", filename);
}

void CNeutrinoApp::saveSkinConfig(const char * const filename)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::saveFile: %s\n", filename);
	
	CConfigFile* skinConfig = new CConfigFile(',');;
	
	skinConfig->setInt32( "menu_Head_alpha", g_settings.menu_Head_alpha );
	skinConfig->setInt32( "menu_Head_red", g_settings.menu_Head_red );
	skinConfig->setInt32( "menu_Head_green", g_settings.menu_Head_green );
	skinConfig->setInt32( "menu_Head_blue", g_settings.menu_Head_blue );
	skinConfig->setInt32( "menu_Head_Text_alpha", g_settings.menu_Head_Text_alpha );
	skinConfig->setInt32( "menu_Head_Text_red", g_settings.menu_Head_Text_red );
	skinConfig->setInt32( "menu_Head_Text_green", g_settings.menu_Head_Text_green );
	skinConfig->setInt32( "menu_Head_Text_blue", g_settings.menu_Head_Text_blue );

	skinConfig->setInt32( "menu_Content_alpha", g_settings.menu_Content_alpha );
	skinConfig->setInt32( "menu_Content_red", g_settings.menu_Content_red );
	skinConfig->setInt32( "menu_Content_green", g_settings.menu_Content_green );
	skinConfig->setInt32( "menu_Content_blue", g_settings.menu_Content_blue );
	skinConfig->setInt32( "menu_Content_Text_alpha", g_settings.menu_Content_Text_alpha );
	skinConfig->setInt32( "menu_Content_Text_red", g_settings.menu_Content_Text_red );
	skinConfig->setInt32( "menu_Content_Text_green", g_settings.menu_Content_Text_green );
	skinConfig->setInt32( "menu_Content_Text_blue", g_settings.menu_Content_Text_blue );

	skinConfig->setInt32( "menu_Content_Selected_alpha", g_settings.menu_Content_Selected_alpha );
	skinConfig->setInt32( "menu_Content_Selected_red", g_settings.menu_Content_Selected_red );
	skinConfig->setInt32( "menu_Content_Selected_green", g_settings.menu_Content_Selected_green );
	skinConfig->setInt32( "menu_Content_Selected_blue", g_settings.menu_Content_Selected_blue );
	skinConfig->setInt32( "menu_Content_Selected_Text_alpha", g_settings.menu_Content_Selected_Text_alpha );
	skinConfig->setInt32( "menu_Content_Selected_Text_red", g_settings.menu_Content_Selected_Text_red );
	skinConfig->setInt32( "menu_Content_Selected_Text_green", g_settings.menu_Content_Selected_Text_green );
	skinConfig->setInt32( "menu_Content_Selected_Text_blue", g_settings.menu_Content_Selected_Text_blue );

	skinConfig->setInt32( "menu_Content_inactive_alpha", g_settings.menu_Content_inactive_alpha );
	skinConfig->setInt32( "menu_Content_inactive_red", g_settings.menu_Content_inactive_red );
	skinConfig->setInt32( "menu_Content_inactive_green", g_settings.menu_Content_inactive_green );
	skinConfig->setInt32( "menu_Content_inactive_blue", g_settings.menu_Content_inactive_blue );
	skinConfig->setInt32( "menu_Content_inactive_Text_alpha", g_settings.menu_Content_inactive_Text_alpha );
	skinConfig->setInt32( "menu_Content_inactive_Text_red", g_settings.menu_Content_inactive_Text_red );
	skinConfig->setInt32( "menu_Content_inactive_Text_green", g_settings.menu_Content_inactive_Text_green );
	skinConfig->setInt32( "menu_Content_inactive_Text_blue", g_settings.menu_Content_inactive_Text_blue );

	skinConfig->setInt32( "infobar_alpha", g_settings.infobar_alpha );
	skinConfig->setInt32( "infobar_red", g_settings.infobar_red );
	skinConfig->setInt32( "infobar_green", g_settings.infobar_green );
	skinConfig->setInt32( "infobar_blue", g_settings.infobar_blue );
	skinConfig->setInt32( "infobar_Text_alpha", g_settings.infobar_Text_alpha );
	skinConfig->setInt32( "infobar_Text_red", g_settings.infobar_Text_red );
	skinConfig->setInt32( "infobar_Text_green", g_settings.infobar_Text_green );
	skinConfig->setInt32( "infobar_Text_blue", g_settings.infobar_Text_blue );
	
	skinConfig->setInt32( "infobar_colored_events_alpha", g_settings.infobar_colored_events_alpha );
	skinConfig->setInt32( "infobar_colored_events_red", g_settings.infobar_colored_events_red );
	skinConfig->setInt32( "infobar_colored_events_green", g_settings.infobar_colored_events_green );
	skinConfig->setInt32( "infobar_colored_events_blue", g_settings.infobar_colored_events_blue );
	
	skinConfig->setInt32( "menu_Foot_alpha", g_settings.menu_Foot_alpha );
	skinConfig->setInt32( "menu_Foot_red", g_settings.menu_Foot_red );
	skinConfig->setInt32( "menu_Foot_green", g_settings.menu_Foot_green );
	skinConfig->setInt32( "menu_Foot_blue", g_settings.menu_Foot_blue );
	skinConfig->setInt32( "menu_Foot_Text_alpha", g_settings.menu_Foot_Text_alpha );
	skinConfig->setInt32( "menu_Foot_Text_red", g_settings.menu_Foot_Text_red );
	skinConfig->setInt32( "menu_Foot_Text_green", g_settings.menu_Foot_Text_green );
	skinConfig->setInt32( "menu_Foot_Text_blue", g_settings.menu_Foot_Text_blue );

	skinConfig->setInt32( "menu_FootInfo_alpha", g_settings.menu_FootInfo_alpha );
	skinConfig->setInt32( "menu_FootInfo_red", g_settings.menu_FootInfo_red );
	skinConfig->setInt32( "menu_FootInfo_green", g_settings.menu_FootInfo_green );
	skinConfig->setInt32( "menu_FootInfo_blue", g_settings.menu_FootInfo_blue );
	skinConfig->setInt32( "menu_FootInfo_Text_alpha", g_settings.menu_FootInfo_Text_alpha );
	skinConfig->setInt32( "menu_FootInfo_Text_red", g_settings.menu_FootInfo_Text_red );
	skinConfig->setInt32( "menu_FootInfo_Text_green", g_settings.menu_FootInfo_Text_green );
	skinConfig->setInt32( "menu_FootInfo_Text_blue", g_settings.menu_FootInfo_Text_blue );
	
	//
	skinConfig->setInt32("Head_gradient", g_settings.Head_gradient);
	skinConfig->setInt32("Head_corner", g_settings.Head_corner);
	skinConfig->setInt32("Head_radius", g_settings.Head_radius);
	skinConfig->setBool("Head_line", g_settings.Head_line);
	
	//
	skinConfig->setInt32("Foot_gradient", g_settings.Foot_gradient);
	skinConfig->setInt32("Foot_corner", g_settings.Foot_corner);
	skinConfig->setInt32("Foot_radius", g_settings.Foot_radius);
	skinConfig->setBool("Foot_line", g_settings.Foot_line);
	
	//
	skinConfig->setInt32("infobar_gradient", g_settings.infobar_gradient);
	skinConfig->setInt32("infobar_gradient_direction", g_settings.infobar_gradient_direction);
	skinConfig->setInt32("infobar_corner", g_settings.infobar_corner);
	skinConfig->setInt32("infobar_radius", g_settings.infobar_radius);
	skinConfig->setBool("infobar_buttonbar", g_settings.infobar_buttonbar);
	skinConfig->setBool("infobar_buttonline", g_settings.infobar_buttonline);
	skinConfig->setBool("infobar_shadow", g_settings.infobar_shadow);
	
	//
	skinConfig->setBool("menu_shadow", g_settings.menu_shadow);
	
	// itemInfo
	skinConfig->setBool("Foot_Info_shadow", g_settings.Foot_Info_shadow);
	skinConfig->setInt32("Foot_Info_gradient", g_settings.Foot_Info_gradient);
		
	skinConfig->setString("font_file", g_settings.font_file);

	if (!skinConfig->saveConfig(filename))
		printf("CNeutrinoApp::saveSkinConfig %s write error\n", filename);
}


