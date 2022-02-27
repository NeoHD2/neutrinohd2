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
	dprintf(DEBUG_INFO, "CNeutrinoApp::convertColor: color: %s\n", color);
	
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

uint32_t CNeutrinoApp::convertMenuColor(int col)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::convertColor:\n");
	
	uint32_t rgba = COL_MENUCONTENT_PLUS_0;
}

//
void CNeutrinoApp::parseClistBox(_xmlNodePtr node, CWidget* widget)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseClistBox:\n");
	
	ClistBox* listBox = NULL;
	CMenuItem* menuItem = NULL;
	
	//
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
	
	unsigned int type = 0;
	unsigned int mode = 2;
	unsigned int scrollbar = 1;
	
	unsigned int shrink = 0;
	unsigned int position = 0;
				
	// head
	unsigned int painthead = 0;
	unsigned int paintdate = 0;
	const char* title = NULL;
	const char* icon = NULL;
	const char* format = NULL;
	unsigned int halign = 0;
	unsigned int head_line = 0;
				
	// foot
	unsigned int paintfoot = 0;
	unsigned int foot_line = 0;
				
	// iteminfo
	unsigned int paintiteminfo = 0;
	unsigned int iteminfomode = 0;		
	unsigned int iteminfoframe = 0;
	unsigned int iteminfo_posx = 0;
	unsigned int iteminfo_posy = 0;
	unsigned int iteminfo_width = 0;
	unsigned int iteminfo_height = 0;
	const char* iteminfo_color = NULL;
	
	//
	_xmlNodePtr listboxitem_node = NULL;
	_xmlNodePtr listboxintegration_node = NULL;
	_xmlNodePtr buttonlabel_node = NULL;
				
	while ((node = xmlGetNextOccurence(node, "LISTBOX")) != NULL) 
	{	
		//widgetitemname = xmlGetAttribute(node, (char*)"name");
		//widgetitemid = xmlGetSignedNumericAttribute(node, "id", 0);
				
		posx = xmlGetSignedNumericAttribute(node, "posx", 0);
		posy = xmlGetSignedNumericAttribute(node, "posy", 0);
		width = xmlGetSignedNumericAttribute(node, "width", 0);
		height = xmlGetSignedNumericAttribute(node, "height", 0);
				
		i_paintframe = xmlGetSignedNumericAttribute(node, "paintframe", 0);
				
		i_color = xmlGetAttribute(node, (char*)"color");
		i_gradient = xmlGetSignedNumericAttribute(node, "gradient", 0);
		i_corner = xmlGetSignedNumericAttribute(node, "corner", 0);
		i_radius = xmlGetSignedNumericAttribute(node, "radius", 0);
				
		// parse color
		uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
		if (i_color) finalColor = convertColor(i_color);
					
		type = xmlGetSignedNumericAttribute(node, "type", 0);
		mode = xmlGetSignedNumericAttribute(node, "mode", 0);
		scrollbar = xmlGetSignedNumericAttribute(node, "scrollbar", 0);
		
		shrink = xmlGetSignedNumericAttribute(node, "shrink", 0);
		position = xmlGetSignedNumericAttribute(node, "position", 0);
				
		// head
		painthead = xmlGetSignedNumericAttribute(node, "painthead", 0);
		paintdate = xmlGetSignedNumericAttribute(node, "paintdate", 0);
		title = xmlGetAttribute(node, "title");
		icon = xmlGetAttribute(node, "icon");
		format = xmlGetAttribute(node, (char*)"format");
		halign = xmlGetSignedNumericAttribute(node, "halign", 0);
		head_line = xmlGetSignedNumericAttribute(node, "head_line", 0);
				
		// foot
		paintfoot = xmlGetSignedNumericAttribute(node, "paintfoot", 0);
		foot_line = xmlGetSignedNumericAttribute(node, "foot_line", 0);
				
		// iteminfo
		paintiteminfo = xmlGetSignedNumericAttribute(node, "paintiteminfo", 0);
		iteminfomode = xmlGetSignedNumericAttribute(node, "iteminfomode", 0);
		iteminfoframe = xmlGetSignedNumericAttribute(node, "iteminfoframe", 0);
		iteminfo_posx = xmlGetSignedNumericAttribute(node, "iteminfoposx", 0);
		iteminfo_posy = xmlGetSignedNumericAttribute(node, "iteminfoposy", 0);
		iteminfo_width = xmlGetSignedNumericAttribute(node, "iteminfowidth", 0);
		iteminfo_height = xmlGetSignedNumericAttribute(node, "iteminfoheight", 0);

		iteminfo_color = xmlGetAttribute(node, (char*)"iteminfocolor");
		uint32_t hintColor = COL_MENUCONTENT_PLUS_0;
				
		if (iteminfo_color) hintColor = convertColor(iteminfo_color);
					
		listBox = new ClistBox(posx, posy, width, height);
		listBox->setWidgetType(type);
		listBox->setWidgetMode(mode);
		listBox->paintScrollBar(scrollbar);
		listBox->paintMainFrame(i_paintframe);
		if (i_color != NULL) listBox->setColor(finalColor);
		if (mode == 0) listBox->setMenuPosition(position);
		if (shrink) listBox->enableShrinkMenu();
				
		//
		if (painthead)
		{
			listBox->enablePaintHead();
			listBox->setTitle(title, icon);
			if (paintdate) listBox->enablePaintDate();
			if (format) listBox->setFormat(format);
			listBox->setTitleHAlign(halign);
			listBox->setHeadLine(head_line);
		}
				
		//
		if (paintfoot)
		{
			listBox->enablePaintFoot();
			listBox->setFootLine(foot_line);
		}
				
		// iteminfo
		if (paintiteminfo)
		{
			listBox->enablePaintItemInfo(70);
			listBox->setItemInfoMode(iteminfomode);
					
			listBox->setItemInfoPos(iteminfo_posx, iteminfo_posy, iteminfo_width, iteminfo_height);
			listBox->paintItemInfoFrame(iteminfoframe);
			if (iteminfo_color) listBox->setItemInfoColor(hintColor);
		}
				
		// ITEM	
		listboxitem_node = node->xmlChildrenNode;
		
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
		unsigned int shadow = 0;
					
		while ((listboxitem_node = xmlGetNextOccurence(listboxitem_node, "ITEM")) != NULL) 
		{	
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
			shadow = xmlGetSignedNumericAttribute(listboxitem_node, "shadow", 0);
						
			CMenuTarget* parent = NULL;
			std::string actionKey = "";
			std::string itemName = "";
						
			if (item_localename) itemName = _(item_localename);
						
			if (itemid == MENUITEM_FORWARDER)
				menuItem = new CMenuForwarder(itemName.c_str());
			else if (itemid == MENUITEM_LISTBOXITEM)
				menuItem = new ClistBoxItem(itemName.c_str());
						
			if (item_actionkey) actionKey = item_actionkey;	
				parent = convertTarget(item_target);
							
			menuItem->setActionKey(parent, actionKey.c_str());
					
			if (directkey) menuItem->setDirectKey(directkey);	
			if (iconName) menuItem->setIconName(iconName);	
			if (hint) menuItem->setHint(hint);
			if (lines) menuItem->set2lines();
			if (option) menuItem->setOption(option);
			if (shadow) menuItem->enableItemShadow();
					
			if (itemIcon)
			{
				std::string filename = CONFIGDIR "/skins/";
				filename += g_settings.preferred_skin;
				filename += "/";
				filename += itemIcon;
						
				if (file_exists(filename.c_str()))
					menuItem->setHintIcon(filename.c_str());
				else
				{
					menuItem->setHintIcon(itemIcon);
				}
			}
						
			listBox->addItem(menuItem);
				
			listboxitem_node = listboxitem_node->xmlNextNode;
		}
				
		// INTEGRATION
		listboxintegration_node = node->xmlChildrenNode;
		
		CPlugins::i_type_t integration = CPlugins::I_TYPE_DISABLED;
		unsigned int mode = MODE_MENU;
		unsigned int shortcut = RC_nokey;
		unsigned int type = WIDGET_TYPE_STANDARD;
		unsigned int i_lines = false;
		unsigned int i_shadow = false;
					
		while ((listboxintegration_node = xmlGetNextOccurence(listboxintegration_node, "INTEGRATION")) != NULL) 
		{	
			integration = (CPlugins::i_type_t)xmlGetSignedNumericAttribute(listboxintegration_node, "id", 0);
			mode = xmlGetSignedNumericAttribute(listboxintegration_node, "mode", 0);
			shortcut = xmlGetSignedNumericAttribute(listboxintegration_node, "shortcut", 0);
			type = xmlGetSignedNumericAttribute(listboxintegration_node, "type", 0);
			i_lines = xmlGetSignedNumericAttribute(listboxintegration_node, "lines", 0);
			i_shadow = xmlGetSignedNumericAttribute(listboxintegration_node, "shadow", 0);
						
			listBox->integratePlugins(integration, shortcut? shortcut : RC_nokey, true, mode, type, i_lines, i_shadow);
				
			listboxintegration_node = listboxintegration_node->xmlNextNode;
		}
				
		// BUTTON_LABEL / FOOT
		buttonlabel_node = node->xmlChildrenNode;
		
		char* button = NULL;
		char* localename = NULL;
					
		while ((buttonlabel_node = xmlGetNextOccurence(buttonlabel_node, "BUTTON_LABEL")) != NULL) 
		{	
			button = xmlGetAttribute(buttonlabel_node, (char*)"name");
			localename = xmlGetAttribute(buttonlabel_node, (char*)"localename");
						
			button_label_struct btn;
			btn.button = " ";
			if (button) btn.button = button;
			btn.localename = " ";
			if (localename) btn.localename = localename;
						
			listBox->setFootButtons(&btn);
				
			buttonlabel_node = buttonlabel_node->xmlNextNode;
		}
				
		// BUTTON_LABEL / HEAD
		
		if (widget) widget->addItem(listBox);
			
		node = node->xmlNextNode;
	}
}

void CNeutrinoApp::parseCWindow(_xmlNodePtr node, CWidget* widget)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCWindow\n");
	
	CWindow* window = NULL;
	
	//char* widgetitemname = NULL;
	//unsigned int widgetitemid = 0;
				
	unsigned int posx = 0;
	unsigned int posy = 0;
	unsigned int width = 0;
	unsigned int height = 0;
				
	unsigned int i_paintframe = 1;
	char* i_color = NULL;
	
	unsigned int i_corner = 0;
	unsigned int i_radius = 0;
	unsigned int i_shadow = 0;
	
	unsigned int i_gradient = 0;
	unsigned int i_direction = 0;
	unsigned int i_intensity = 0;
	unsigned int i_type = 0;
	
	unsigned int refresh = 0;
	
//#TODO: add CCItems
	
	while ((node = xmlGetNextOccurence(node, "WINDOW")) != NULL) 
	{
		//widgetitemname = xmlGetAttribute(node, (char*)"name");
		//widgetitemid = xmlGetSignedNumericAttribute(node, "id", 0);
				
		posx = xmlGetSignedNumericAttribute(node, "posx", 0);
		posy = xmlGetSignedNumericAttribute(node, "posy", 0);
		width = xmlGetSignedNumericAttribute(node, "width", 0);
		height = xmlGetSignedNumericAttribute(node, "height", 0);
				
		i_paintframe = xmlGetSignedNumericAttribute(node, "paintframe", 0);		
		i_color = xmlGetAttribute(node, (char*)"color");

		i_corner = xmlGetSignedNumericAttribute(node, "corner", 0);
		i_radius = xmlGetSignedNumericAttribute(node, "radius", 0);
		i_shadow = xmlGetSignedNumericAttribute(node, "shadow", 0);
		
		i_gradient = xmlGetSignedNumericAttribute(node, "gradient", 0);
		i_direction = xmlGetSignedNumericAttribute(node, "direction", 0);
		i_intensity = xmlGetSignedNumericAttribute(node, "intensity", 0);
		i_type = xmlGetSignedNumericAttribute(node, "type", 0);
				
		// parse color
		uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
		if (i_color) finalColor = convertColor(i_color);
				
		refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
					
		window = new CWindow(posx, posy, width, height);
					
		window->paintMainFrame(i_paintframe);
		if (i_color) window->setColor(finalColor);
		if (refresh) window->enableRepaint();
		window->setCorner(i_radius, i_corner);
		window->setShadowMode(i_shadow);
		window->setGradient(i_gradient, i_direction, i_intensity, i_type);
		
		////
		// LABEL
		parseCCLabel(node->xmlChildrenNode, NULL, window);
			
		// IMAGE
		parseCCImage(node->xmlChildrenNode, NULL, window);
			
		// TIME
		parseCCTime(node->xmlChildrenNode, NULL, window);
			
		// BUTTONS
		parseCCButtons(node->xmlChildrenNode, NULL, window);
		////
					
		if (widget) widget->addItem(window);
			
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseCHead(_xmlNodePtr node, CWidget* widget)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCHead:\n");
	
	CHeaders* head = NULL;
	
	//char* widgetitemname = NULL;
	//unsigned int widgetitemid = 0;
				
	unsigned int posx = 0;
	unsigned int posy = 0;
	unsigned int width = 0;
	unsigned int height = 0;
				
	//unsigned int i_paintframe = 1;
	char* i_color = NULL;
	unsigned int i_gradient = 0;
	unsigned int i_corner = 0;
	unsigned int i_radius = 0;
	
	char* title = NULL;
	unsigned int halign = 0;
	const char* icon = NULL;
	unsigned int h_line = 0;
	unsigned int paintdate = 0;
	char* format = NULL;
	
	_xmlNodePtr buttonlabel_node = NULL;
	
	while ((node = xmlGetNextOccurence(node, "HEAD")) != NULL) 
	{		
		//widgetitemname = xmlGetAttribute(node, (char*)"name");
		//widgetitemid = xmlGetSignedNumericAttribute(node, "id", 0);
				
		posx = xmlGetSignedNumericAttribute(node, "posx", 0);
		posy = xmlGetSignedNumericAttribute(node, "posy", 0);
		width = xmlGetSignedNumericAttribute(node, "width", 0);
		height = xmlGetSignedNumericAttribute(node, "height", 0);
				
		//i_paintframe = xmlGetSignedNumericAttribute(node, "paintframe", 0);
				
		i_color = xmlGetAttribute(node, (char*)"color");
		i_gradient = xmlGetSignedNumericAttribute(node, "gradient", 0);
		i_corner = xmlGetSignedNumericAttribute(node, "corner", 0);
		i_radius = xmlGetSignedNumericAttribute(node, "radius", 0);
				
		// parse color
		uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
		if (i_color) finalColor = convertColor(i_color);
					
		title = xmlGetAttribute(node, (char*)"localename");
		halign = xmlGetSignedNumericAttribute(node, "halign", 0);
		icon = xmlGetAttribute(node, (char*)"icon");
		h_line = xmlGetSignedNumericAttribute(node, "line", 0);
		paintdate = xmlGetSignedNumericAttribute(node, "paintdate", 0);
		format = xmlGetAttribute(node, (char*)"format");

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
					
		// BUTTON_LABEL
		buttonlabel_node = node->xmlChildrenNode;
		
		char* button = NULL;
		char* localename = NULL;
					
		while ((buttonlabel_node = xmlGetNextOccurence(buttonlabel_node, "BUTTON_LABEL")) != NULL) 
		{	
			button = xmlGetAttribute(buttonlabel_node, (char*)"name");
			localename = xmlGetAttribute(buttonlabel_node, (char*)"localename");
						
			button_label_struct btn;
			btn.button = " ";
			if (button) btn.button = button;
			btn.localename = " ";
			if (localename) btn.localename = localename;
						
			head->setButtons(&btn);
				
			buttonlabel_node = buttonlabel_node->xmlNextNode;
		}
					
		if (widget) widget->addItem(head);
			
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseCFoot(_xmlNodePtr node, CWidget* widget)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCFoot:\n");
	
	CFooters* foot = NULL;
	
	unsigned int posx = 0;
	unsigned int posy = 0;
	unsigned int width = 0;
	unsigned int height = 0;
				
	//unsigned int i_paintframe = 1;
	char* i_color = NULL;
	unsigned int i_gradient = 0;
	unsigned int i_corner = 0;
	unsigned int i_radius = 0;
	
	unsigned int f_line = 0;
	
	_xmlNodePtr buttonlabel_node = NULL;
	
	while ((node = xmlGetNextOccurence(node, "FOOT")) != NULL) 
	{
		//widgetitemname = xmlGetAttribute(node, (char*)"name");
		//widgetitemid = xmlGetSignedNumericAttribute(node, "id", 0);
				
		posx = xmlGetSignedNumericAttribute(node, "posx", 0);
		posy = xmlGetSignedNumericAttribute(node, "posy", 0);
		width = xmlGetSignedNumericAttribute(node, "width", 0);
		height = xmlGetSignedNumericAttribute(node, "height", 0);
				
		//i_paintframe = xmlGetSignedNumericAttribute(node, "paintframe", 0);
				
		i_color = xmlGetAttribute(node, (char*)"color");
		i_gradient = xmlGetSignedNumericAttribute(node, "gradient", 0);
		i_corner = xmlGetSignedNumericAttribute(node, "corner", 0);
		i_radius = xmlGetSignedNumericAttribute(node, "radius", 0);
				
		// parse color
		uint32_t finalColor = COL_MENUCONTENT_PLUS_0;
				
		if (i_color) finalColor = convertColor(i_color);
				
		f_line = xmlGetSignedNumericAttribute(node, "line", 0);
						
		foot = new CFooters(posx, posy, width, height);
					
		if (i_color != NULL) foot->setColor(finalColor);
		foot->setGradient(i_gradient);
		foot->setCorner(i_corner);
		foot->setRadius(i_radius);
		foot->setFootLine(f_line);
					
		// BUTTON_LABEL
		buttonlabel_node = node->xmlChildrenNode;
		
		char* button = NULL;
		char* localename = NULL;
					
		while ((buttonlabel_node = xmlGetNextOccurence(buttonlabel_node, "BUTTON_LABEL")) != NULL) 
		{	
			button = xmlGetAttribute(buttonlabel_node, (char*)"name");
			localename = xmlGetAttribute(buttonlabel_node, (char*)"localename");
						
			button_label_struct btn;
			btn.button = " ";
			if (button) btn.button = button;
			btn.localename = " ";
			if (localename) btn.localename = localename;
						
			foot->setButtons(&btn);
				
			buttonlabel_node = buttonlabel_node->xmlNextNode;
		}
					
		if (widget) widget->addItem(foot);
			
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseCCLabel(_xmlNodePtr node, CWidget* widget, CWindow* window)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCCLabel:\n");
	
	CCLabel* label = NULL;
	
	//
	//char* cc_name = NULL;
	//unsigned int cc_id = 0;
				
	unsigned int cc_x = 0;
	unsigned int cc_y = 0;
	unsigned int cc_dx = 0;
	unsigned int cc_dy = 0;
						
	unsigned int cc_refresh = 0;
	unsigned int l_halign = 0;
	
	while ((node = xmlGetNextOccurence(node, "LABEL")) != NULL) 
	{				
		//cc_name = xmlGetAttribute(node, (char*)"name");
		//cc_id = xmlGetSignedNumericAttribute(node, "id", 0);
				
		cc_x = xmlGetSignedNumericAttribute(node, "posx", 0);
		cc_y = xmlGetSignedNumericAttribute(node, "posy", 0);
		cc_dx = xmlGetSignedNumericAttribute(node, "width", 0);
		cc_dy = xmlGetSignedNumericAttribute(node, "height", 0);
						
		cc_refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
						
		std::string text = "";
							
		text = xmlGetAttribute(node, (char*)"text");
		l_halign = xmlGetSignedNumericAttribute(node, "halign", 0);
							
		label = new CCLabel(cc_x, cc_y, cc_dx, cc_dy);
							
		if (!text.empty()) label->setText(_(text.c_str()));
		label->setHAlign(l_halign);
							
		if (widget) widget->addCCItem(label);
		if (window) window->addCCItem(label);
			
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseCCImage(_xmlNodePtr node, CWidget* widget, CWindow* window)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCCImage:\n");
	
	CCImage* pic = NULL;
	
	//char* cc_name = NULL;
	//unsigned int cc_id = 0;
				
	unsigned int cc_x = 0;
	unsigned int cc_y = 0;
	unsigned int cc_dx = 0;
	unsigned int cc_dy = 0;
						
	unsigned int cc_refresh = 0;
	
	while ((node = xmlGetNextOccurence(node, "IMAGE")) != NULL) 
	{		
		//cc_name = xmlGetAttribute(node, (char*)"name");
		//cc_id = xmlGetSignedNumericAttribute(node, "id", 0);
				
		cc_x = xmlGetSignedNumericAttribute(node, "posx", 0);
		cc_y = xmlGetSignedNumericAttribute(node, "posy", 0);
		cc_dx = xmlGetSignedNumericAttribute(node, "width", 0);
		cc_dy = xmlGetSignedNumericAttribute(node, "height", 0);
						
		cc_refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
				
		char* image = NULL;
		image = xmlGetAttribute(node, (char*)"image");
							
		pic = new CCImage(cc_x, cc_y, cc_dx, cc_dy);
							
		if (image != NULL)
		{
			std::string filename = CONFIGDIR "/skins/";
			filename += g_settings.preferred_skin;
			filename += "/";
			filename += image;
					
			if (file_exists(filename.c_str()))
				pic->setImage(filename.c_str());
			else
				pic->setImage(image);
		}
							
		if (widget) widget->addCCItem(pic);
		if (window) window->addCCItem(pic);	
			
		node = node->xmlNextNode;
	}	
}

//
void CNeutrinoApp::parseCCTime(_xmlNodePtr node, CWidget* widget, CWindow* window)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCCTime:\n");
	
	CCTime* time = NULL;
	
	//char* cc_name = NULL;
	//unsigned int cc_id = 0;
				
	unsigned int cc_x = 0;
	unsigned int cc_y = 0;
	unsigned int cc_dx = 0;
	unsigned int cc_dy = 0;
						
	unsigned int cc_refresh = 0;
	
	char* cc_format = NULL;
	
	while ((node = xmlGetNextOccurence(node, "TIME")) != NULL) 
	{		
		//cc_name = xmlGetAttribute(node, (char*)"name");
		//cc_id = xmlGetSignedNumericAttribute(node, "id", 0);
				
		cc_x = xmlGetSignedNumericAttribute(node, "posx", 0);
		cc_y = xmlGetSignedNumericAttribute(node, "posy", 0);
		cc_dx = xmlGetSignedNumericAttribute(node, "width", 0);
		cc_dy = xmlGetSignedNumericAttribute(node, "height", 0);
						
		cc_refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
					
		cc_format = xmlGetAttribute(node, (char*)"format");
							
		time = new CCTime(cc_x, cc_y, cc_dx, cc_dy);
							
		if (cc_format != NULL) time->setFormat(_(cc_format));
		if (cc_refresh) time->enableRepaint();
							
		if (widget) widget->addCCItem(time);
		if (window) window->addCCItem(time);
			
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseCCButtons(_xmlNodePtr node, CWidget* widget, CWindow* window)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseCCButtons:\n");
	
	CCButtons* cButton = NULL;
	
	unsigned int b_x = 0;
	unsigned int b_y = 0;
	unsigned int b_dx = 0;
	unsigned int b_dy = 0;
	
	_xmlNodePtr buttonlabel_node = NULL;
	
	while ((node = xmlGetNextOccurence(node, "BUTTON")) != NULL) 
	{
		b_x = xmlGetSignedNumericAttribute(node, "posx", 0);
		b_y = xmlGetSignedNumericAttribute(node, "posy", 0);
		b_dx = xmlGetSignedNumericAttribute(node, "width", 0);
		b_dy = xmlGetSignedNumericAttribute(node, "height", 0);
				
		cButton = new CCButtons(b_x, b_y, b_dx, b_dy);
				
		// BUTTON_LABEL
		buttonlabel_node = node->xmlChildrenNode;
		
		char* button = NULL;
		char* localename = NULL;
				
		while ((buttonlabel_node = xmlGetNextOccurence(buttonlabel_node, "BUTTON_LABEL")) != NULL) 
		{		
			button = xmlGetAttribute(buttonlabel_node, (char*)"name");
			localename = xmlGetAttribute(buttonlabel_node, (char*)"localename");
							
			button_label_struct btn;
			btn.button = " ";
			if (button) btn.button = button;
			btn.localename = " ";
			if (localename) btn.localename = localename;
							
			cButton->setButtons(&btn);
					
			buttonlabel_node = buttonlabel_node->xmlNextNode;
		}
					
		if (widget) widget->addCCItem(cButton);
		if (window) window->addCCItem(cButton);
				
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseKey(_xmlNodePtr node, CWidget* widget)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseKey:\n");
	
	neutrino_msg_t key_name = RC_nokey;
	char* key_actionkey = NULL;
	unsigned int key_target = 0;
	
	while ((node = xmlGetNextOccurence(node, "KEY")) != NULL) 
	{
		key_name = (neutrino_msg_t)xmlGetSignedNumericAttribute(node, "name", 16);
		key_actionkey = xmlGetAttribute(node, (char*)"actionkey");
		key_target = xmlGetSignedNumericAttribute(node, "target", 0);
				
		CMenuTarget* key_parent = NULL;
				
		key_parent = convertTarget(key_target);
				
		if (widget) widget->addKey((neutrino_msg_t)key_name, key_parent, key_actionkey);
			
		node = node->xmlNextNode;
	}
}

//
void CNeutrinoApp::parseSkin(const char* const filename, bool xml_data)
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::parseSkin\n");
	
	//
	_xmlDocPtr parser = NULL;

	if (xml_data)
		parser = parseXml(filename);
	else
		parser = parseXmlFile(filename);
	
	if (parser == NULL)
		return;
		
	_xmlNodePtr root = xmlDocGetRootElement(parser);
	_xmlNodePtr search = root->xmlChildrenNode; //WIDGET
	
	if (search) 
	{
		dprintf(DEBUG_NORMAL, "CNeutrinoApp:parseSkin: %s\n", filename);
		
		CWidget* wdg = NULL;
		
		while ((search = xmlGetNextOccurence(search, "WIDGET")) != NULL) 
		{
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
			if (color != NULL) wdg->setColor(wColor);
			wdg->setGradient(gradient);
			wdg->setCorner(corner, radius);
			if (savescreen) wdg->enableSaveScreen();
			wdg->setTimeOut(timeout);
			
			// skip duplicate
			for (unsigned long i = 0; i < (unsigned long)widgets.size(); i++)
			{
				if(widgets[i]->id == wdg->id)
					widgets.erase(widgets.begin() + i); 
			}
			
			widgets.push_back(wdg);
			
			// WINDOW
			parseCWindow(search->xmlChildrenNode, wdg);
			
			// HEAD
			parseCHead(search->xmlChildrenNode, wdg);
			
			// FOOT
			parseCFoot(search->xmlChildrenNode, wdg);
			
			// LISTBOX
			parseClistBox(search->xmlChildrenNode, wdg);
			
			// LABEL
			parseCCLabel(search->xmlChildrenNode, wdg);
			
			// IMAGE
			parseCCImage(search->xmlChildrenNode, wdg);
			
			// TIME
			parseCCTime(search->xmlChildrenNode, wdg);
			
			// BUTTONS
			parseCCButtons(search->xmlChildrenNode, wdg);
			
			// KEY
			parseKey(search->xmlChildrenNode, wdg);
						
			//
			search = search->xmlNextNode;		
		}
	}
	
	xmlFreeDoc(parser);
	parser = NULL;
	
	//
	dprintf(DEBUG_INFO, "CNeutrinoApp::parseSkin: widgets count:%d\n\n", (int)widgets.size());
}

//
void CNeutrinoApp::loadSkin(std::string skinName)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::loadSkin: %s\n", skinName.c_str());
	
	std::string skinPath = CONFIGDIR "/skins/";
	skinPath += skinName.c_str();
	
	// read skin config
	std::string skinConfigFile = skinPath.c_str();
	skinConfigFile += "/";
	skinConfigFile += skinName.c_str();
	skinConfigFile += ".config";
	
	readSkinConfig(skinConfigFile.c_str());
	
	//
	std::string skinFileName = skinPath.c_str();
	skinFileName += "/skin.xml";
	
	parseSkin(skinFileName.c_str());
	
	//
	std::string fontFileName;
	
	struct dirent **namelist;
	int i = 0;
	
	if (CNeutrinoApp::getInstance()->skin_exists(skinName.c_str()))
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
	else //fallback to default (neutrino intern)
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
}

bool CNeutrinoApp::execSkin(const int id)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::execSkin: %d\n", id);
	
	bool ret = false;
	
	if (getWidget(id))
	{
		ret = true;
		getWidget(id)->exec(NULL, "");
	}
	
	return ret;	
}

bool CNeutrinoApp::paintSkin(const int id)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::paintSkin: %d\n", id);
	
	bool ret = false;
	
	if (getWidget(id))
	{
		ret = true;
		getWidget(id)->paint();
	}
	
	return ret;	
}

bool CNeutrinoApp::hideSkin(const int id)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::hideSkin: %d\n", id);
	
	bool ret = false;
	
	if (getWidget(id))
	{
		ret = true;
		getWidget(id)->hide();
	}
	
	return ret;	
}

bool CNeutrinoApp::removeSkin(const int id)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::hideSkin: %d\n", id);
	
	bool ret = false;
	
	if (getWidget(id))
	{
		ret = true;
		widgets.erase(widgets.begin() + id); 
	}
	
	return ret;	
}

bool CNeutrinoApp::skin_exists(const char* const filename)
{
	dprintf(DEBUG_INFO, "CNeutrinoApp::skin_exists: %s\n", filename);
	
	bool ret = false;
	
	std::string skin = CONFIGDIR "/skins/";
	skin += filename;
	skin += "/skin.xml";
	
	if (::file_exists(skin.c_str()))
		ret = true;
	
	return ret;
}

void CNeutrinoApp::unloadSkin()
{
	// 
	for (unsigned int i = 0; i < (unsigned int) widgets.size(); i++)
	{
		if (widgets[i])
		{
			delete widgets[i];
			widgets[i] = NULL;
		}
	}
	
	//
	widgets.clear();
	
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
	g_settings.Hint_shadow = configfile.getBool("Hint_shadow", true);
	g_settings.Hint_gradient = configfile.getInt32("Hint_gradient", NOGRADIENT);
	
	// progressbar color
	g_settings.progressbar_color = configfile.getInt32("progressbar_color", 1);
	g_settings.progressbar_gradient = configfile.getInt32("progressbar_gradient", DARK2LIGHT2DARK);
	
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

		g_settings.menu_Hint_alpha = skinConfig->getInt32( "menu_Hint_alpha", 0);
		g_settings.menu_Hint_red = skinConfig->getInt32( "menu_Hint_red", 15);
		g_settings.menu_Hint_green = skinConfig->getInt32( "menu_Hint_green", 15);
		g_settings.menu_Hint_blue = skinConfig->getInt32( "menu_Hint_blue", 15);
		
		g_settings.menu_Hint_Text_alpha = skinConfig->getInt32( "menu_Hint_Text_alpha", 0);
		g_settings.menu_Hint_Text_red = skinConfig->getInt32( "menu_Hint_Text_red", 85);
		g_settings.menu_Hint_Text_green = skinConfig->getInt32( "menu_Hint_Text_green", 85);
		g_settings.menu_Hint_Text_blue = skinConfig->getInt32( "menu_Hint_Text_blue", 85);
		
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
		g_settings.Hint_shadow = skinConfig->getBool("Hint_shadow", true);
		g_settings.Hint_gradient = configfile.getInt32("Hint_gradient", NOGRADIENT);
		
		// progressbar color
		g_settings.progressbar_color = skinConfig->getInt32("progressbar_color", 1);
		g_settings.progressbar_gradient = skinConfig->getInt32("progressbar_gradient", DARK2LIGHT2DARK);
		
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

	skinConfig->setInt32( "menu_Hint_alpha", g_settings.menu_Hint_alpha );
	skinConfig->setInt32( "menu_Hint_red", g_settings.menu_Hint_red );
	skinConfig->setInt32( "menu_Hint_green", g_settings.menu_Hint_green );
	skinConfig->setInt32( "menu_Hint_blue", g_settings.menu_Hint_blue );
	skinConfig->setInt32( "menu_Hint_Text_alpha", g_settings.menu_Hint_Text_alpha );
	skinConfig->setInt32( "menu_Hint_Text_red", g_settings.menu_Hint_Text_red );
	skinConfig->setInt32( "menu_Hint_Text_green", g_settings.menu_Hint_Text_green );
	skinConfig->setInt32( "menu_Hint_Text_blue", g_settings.menu_Hint_Text_blue );
	
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
	skinConfig->setBool("Hint_shadow", g_settings.Hint_shadow);
	skinConfig->setInt32("Hint_gradient", g_settings.Hint_gradient);
	
	// progressbar color
	skinConfig->setInt32("progressbar_color", g_settings.progressbar_color);
	skinConfig->setInt32("progressbar_gradient", g_settings.progressbar_gradient);
		
	skinConfig->setString("font_file", g_settings.font_file);

	if (!skinConfig->saveConfig(filename))
		printf("CNeutrinoApp::saveSkinConfig %s write error\n", filename);
}


