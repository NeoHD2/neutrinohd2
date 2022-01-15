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
#include <gui/osd_setup.h>
#include <gui/epg_menu.h>
#include <gui/epgplus.h>
#include <gui/timerlist.h>
#include <gui/streaminfo2.h>
#include <gui/service_menu.h>
#include <gui/mediaplayer.h>
#include <gui/misc_setup.h>
#include <gui/dboxinfo.h>
#include <gui/power_menu.h>

#include <system/helpers.h>
#include <system/debug.h>

#if ENABLE_LUA
#include <interfaces/lua/neutrino_lua.h>
#endif


//
CMenuTarget* CNeutrinoApp::convertTarget(const int id)
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::convertTarget: id: %d\n", id);
	
	CMenuTarget* parent = NULL;
	
	switch(id)
	{
		case 0:
			parent = this;
			break;
		
		case 1:
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
			
		case 2:
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
			
		case 3:
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
			
		case 4:
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
			
		case 5:
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
			
		case 6:
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
			
		case 7:
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
			
		case 8:
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
			
		case 9:
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
			
		case 10:
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
			
		case 11:
			{
				if (widget_exists(WIDGET_NVOD))
				{
					parent = getWidget(WIDGET_NVOD);
				}
				else
				{
					dprintf(DEBUG_INFO, "id: %d not found\n", id);
					parent = this;
				}
			}
			break;
			
		case 30:
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
			
		case 34:
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
			
		case 43:
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
			
		case 44:
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
			
		case 45:
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
			
		case 46:
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
			
		case 47:
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
			
		case 67:
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
		
		default:
			break;
	}
	
	return parent;
}

neutrino_locale_t CNeutrinoApp::convertLocale(const int loc)
{
	neutrino_locale_t locale = NONEXISTANT_LOCALE;
	
	return locale;
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
	uint32_t rgba = 0;
	
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
	
	std::string filename = CONFIGDIR "/skin/";
	filename += g_settings.preferred_skin;
	filename += "/skin.xml";

	parser = parseXmlFile(filename.c_str());
	
	if (parser == NULL)
		return;
		
	_xmlNodePtr root = xmlDocGetRootElement(parser);
	_xmlNodePtr search = root->xmlChildrenNode;
	_xmlNodePtr subsearch;
	_xmlNodePtr node;
	_xmlNodePtr subnode;
	
	if (search) 
	{
		dprintf(DEBUG_NORMAL, "CNeutrinoApp:parseSkin: %s\n", filename.c_str());

		while ((search = xmlGetNextOccurence(search, "widget")) != NULL) 
		{
			CWidget* wdg = NULL;
			
			//
			char* name = xmlGetAttribute(search, (char*)"name");
			unsigned int id = xmlGetSignedNumericAttribute(search, "id", 0);
			
			unsigned int x = xmlGetSignedNumericAttribute(search, "posx", 0);
			unsigned int y = xmlGetSignedNumericAttribute(search, "posy", 0);
			unsigned int dx = xmlGetSignedNumericAttribute(search, "width", 0);
			unsigned int dy = xmlGetSignedNumericAttribute(search, "height", 0);
			
			unsigned int corner = xmlGetSignedNumericAttribute(search, "corner", 0);
			unsigned int radius = xmlGetSignedNumericAttribute(search, "radius", 0);
			
			unsigned int paintframe = xmlGetSignedNumericAttribute(search, "paintframe", 0);
			unsigned int savescreen = xmlGetSignedNumericAttribute(search, "savescreen", 0);
			unsigned int timeout = xmlGetSignedNumericAttribute(search, "timeout", 0);
			
			//
			wdg = new CWidget(x, y, dx, dy);
			
			wdg->id = id;
			if (name) wdg->name = name;
			wdg->paintMainFrame(paintframe);
			//wdg->setColor(color);
			wdg->setCorner(corner, radius);
			if (savescreen) wdg->enableSaveScreen();
			wdg->setTimeOut(timeout);
			
			widgets.push_back(wdg);
			
			// widgetitem
			node = search->xmlChildrenNode;
			
			while ((node = xmlGetNextOccurence(node, "widgetitem")) != NULL) 
			{
				char* name = xmlGetAttribute(node, (char*)"name");
				unsigned int id = xmlGetSignedNumericAttribute(node, "id", 0);
				
				unsigned int x = xmlGetSignedNumericAttribute(node, "posx", 0);
				unsigned int y = xmlGetSignedNumericAttribute(node, "posy", 0);
				unsigned int dx = xmlGetSignedNumericAttribute(node, "width", 0);
				unsigned int dy = xmlGetSignedNumericAttribute(node, "height", 0);
				
				char* color = xmlGetAttribute(node, (char*)"color");
				unsigned int gradient = xmlGetSignedNumericAttribute(node, "gradient", 0);
				unsigned int corner = xmlGetSignedNumericAttribute(node, "corner", 0);
				unsigned int radius = xmlGetSignedNumericAttribute(node, "radius", 0);
				
				// parse color
				uint32_t finalColor = 0;
				
				if (color) finalColor = convertColor(color);
				
				// CCitems/CMenuItems
				subnode = node->xmlChildrenNode;
				
				if (id == WIDGETITEM_HEAD)
				{
					neutrino_locale_t locale = (neutrino_locale_t)xmlGetSignedNumericAttribute(node, "locale", 0);
					const char* title = xmlGetAttribute(node, "localename");
					unsigned int halign = xmlGetSignedNumericAttribute(node, "halign", 0);
					const char* icon = xmlGetAttribute(node, "icon");
					unsigned int line = xmlGetSignedNumericAttribute(node, "line", 0);
					unsigned int paintdate = xmlGetSignedNumericAttribute(node, "paintdate", 0);
					const char* format = xmlGetAttribute(node, "format");
				
					CHeaders* head = NULL;
					head = new CHeaders(x, y, dx, dy);
					
					if (title) head->setTitle(title); //FIXME: corrupted
					head->setHAlign(halign);
					if (icon) head->setIcon(icon); //FIXME: corrupted
					if(color) head->setColor(finalColor);
					head->setGradient(gradient);
					head->setCorner(corner);
					head->setRadius(radius);
					head->setHeadLine(line);
					if (paintdate) head->enablePaintDate();
					if (format) head->setFormat(format); //FIXME: corrupted
					
					while ((subnode = xmlGetNextOccurence(subnode, "button")) != NULL) 
					{
						char* button = xmlGetAttribute(subnode, (char*)"name");
						char* localename = xmlGetAttribute(subnode, (char*)"localename");
						neutrino_locale_t locale = (neutrino_locale_t)xmlGetSignedNumericAttribute(subnode, "locale", 0);
						
						button_label_struct btn;
						btn.button = NULL;
						if (button) btn.button = button;
						btn.localename = NULL;
						if (localename) btn.localename = localename;
						btn.locale = locale;
						
						head->setButtons(&btn); //FIXME: corrupted
				
						subnode = subnode->xmlNextNode;
					}
					
					wdg->addItem(head);
				}
				else if (id == WIDGETITEM_FOOT)
				{
					unsigned int line = xmlGetSignedNumericAttribute(node, "line", 0);
						
					CFooters* foot = NULL;
					foot = new CFooters(x, y, dx, dy);
					
					if (color) foot->setColor(finalColor);
					foot->setGradient(gradient);
					foot->setCorner(corner);
					foot->setRadius(radius);
					foot->setFootLine(line);
					
					while ((subnode = xmlGetNextOccurence(subnode, "button")) != NULL) 
					{
						char* button = xmlGetAttribute(subnode, (char*)"name");
						neutrino_locale_t locale = (neutrino_locale_t)xmlGetSignedNumericAttribute(subnode, "locale", 0);
						char* localename = xmlGetAttribute(subnode, "localename");
						
						button_label_struct btn;
						btn.button = NULL;
						if (button) btn.button = button;
						btn.localename = NULL;
						if (localename) btn.localename = localename;
						btn.locale = locale;
						
						foot->setButtons(&btn); //FIXME: corrupted
				
						subnode = subnode->xmlNextNode;
					}
					
					wdg->addItem(foot);
				}
				else if (id == WIDGETITEM_LISTBOX)
				{
					unsigned int type = xmlGetSignedNumericAttribute(node, "type", 0);
					unsigned int scrollbar = xmlGetSignedNumericAttribute(node, "scrollbar", 0);
					unsigned int paintframe = xmlGetSignedNumericAttribute(node, "paintframe", 0);
					
					ClistBox* listBox = new ClistBox(x, y, dx, dy);
					listBox->setWidgetType(type);
					listBox->paintScrollBar(scrollbar);
					listBox->paintMainFrame(paintframe);
					
					while ((subnode = xmlGetNextOccurence(subnode, "item")) != NULL) 
					{
						char* name = xmlGetAttribute(subnode, (char*)"name");
						unsigned int itemid = xmlGetSignedNumericAttribute(subnode, "id", 0);
						
						char* localename = xmlGetAttribute(subnode, (char*)"localename");
						char* option = xmlGetAttribute(subnode, (char*)"option");
						char* actionkey = xmlGetAttribute(subnode, (char*)"actionkey");
						unsigned int target = xmlGetSignedNumericAttribute(subnode, "target", 0);
						char* itemIcon = xmlGetAttribute(subnode, (char*)"itemicon");
						char* hint = xmlGetAttribute(subnode, (char*)"hint");
						char* iconName = xmlGetAttribute(subnode, (char*)"iconname");
						char* directkey = xmlGetAttribute(subnode, (char*)"directkey");
						unsigned int lines = xmlGetSignedNumericAttribute(subnode, "lines", 0);
						
						CMenuItem* menuItem = NULL;
						CMenuTarget* parent = NULL;
						std::string actionKey = "";
						std::string itemName = "";
						
						if (localename) itemName = localename;
						
						if (itemid == ITEM_TYPE_FORWARDER)
							menuItem = new CMenuForwarder(itemName.c_str());
						else if (itemid == ITEM_TYPE_LISTBOXITEM)
							menuItem = new ClistBoxItem(itemName.c_str());
						
						if (actionkey) actionKey = actionkey;	
						parent = convertTarget(target);
							
						menuItem->setActionKey(parent, actionKey.c_str());
						
						if (iconName) menuItem->setIconName(iconName);	
						if (hint) menuItem->setHint(hint);
						if (itemIcon) menuItem->setItemIcon(itemIcon);
						if (lines) menuItem->set2lines();
						if (option) menuItem->setOption(option);
						
						listBox->addItem(menuItem);
				
						subnode = subnode->xmlNextNode;
					}
					
					wdg->addItem(listBox);
				}
				else if (id == WIDGETITEM_WINDOW)
				{
					unsigned int refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
					
					CWindow* window = new CWindow(x, y, dx, dy);
					
					if (color != NULL) window->setColor(finalColor);
					if (refresh) window->enableRepaint();
					
					while ((subnode = xmlGetNextOccurence(subnode, "ccitem")) != NULL) 
					{
						char* name = xmlGetAttribute(subnode, (char*)"name");
						unsigned int id = xmlGetSignedNumericAttribute(subnode, "id", 0);
				
						unsigned int x = xmlGetSignedNumericAttribute(subnode, "posx", 0);
						unsigned int y = xmlGetSignedNumericAttribute(subnode, "posy", 0);
						unsigned int dx = xmlGetSignedNumericAttribute(subnode, "width", 0);
						unsigned int dy = xmlGetSignedNumericAttribute(subnode, "height", 0);
						
						unsigned int refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
						
						if (id == CC_LABEL)
						{
							char* text = xmlGetAttribute(subnode, (char*)"text");
							unsigned int halign = xmlGetSignedNumericAttribute(subnode, "halign", 0);
							neutrino_locale_t locale = (neutrino_locale_t)xmlGetSignedNumericAttribute(node, "locale", 0);
							printf("CCITEM:LABEL NODE: locale:%d\n", locale);
						
							CCLabel* label = new CCLabel(x, y, dx, dy);
							
							if (text) label->setText(text);
							label->setHAlign(halign);
							
							window->addCCItem(label);	
						}
						else if (id == CC_IMAGE)
						{
							char* image = xmlGetAttribute(subnode, (char*)"image");
						
							CCImage* pic = new CCImage(x, y, dx, dy);
							
							std::string filename = CONFIGDIR "/skin/MetrixHD/";
							filename += image;
							
							if (image) pic->setImage(filename.c_str());
							
							window->addCCItem(pic);	
						}
						else if (id == CC_TIME)
						{
							char* format = xmlGetAttribute(subnode, (char*)"format");
						
							CCTime* time = new CCTime(x, y, dx, dy);
							
							if (format) time->setFormat(format); //FIXME: corrupted
							if (refresh) time->enableRepaint();
							
							window->addCCItem(time);	
						}
				
						subnode = subnode->xmlNextNode;
					}
					
					wdg->addItem(window);
				}
				else if (id == WIDGETITEM_FRAMEBOX)
				{
					CFrameBox* frameBox = new CFrameBox(x, y, dx, dy);
					
					wdg->addItem(frameBox);
				}
			
				node = node->xmlNextNode;
			}
			
			// key
			subsearch = search->xmlChildrenNode;
			
			while ((subsearch = xmlGetNextOccurence(subsearch, "key")) != NULL) 
			{
				neutrino_msg_t name = (neutrino_msg_t)xmlGetSignedNumericAttribute(subsearch, "name", 16);
				char* actionkey = xmlGetAttribute(subsearch, (char*)"actionkey");
				unsigned int target = xmlGetSignedNumericAttribute(subsearch, "target", 0);
				
				CMenuTarget* parent = NULL;
				
				parent = convertTarget(target);
				
				wdg->addKey((neutrino_msg_t)name, parent, actionkey);
			
				subsearch = subsearch->xmlNextNode;
			}
						
			//
			search = search->xmlNextNode;		
		}
	}
	
	xmlFreeDoc(parser);
	parser = NULL;
	
	//DEBUG
	for (unsigned int i = 0; i < (unsigned int )widgets.size(); i++)
	{
		printf("SKIN:widgets: %d (%s)\n", widgets[i]->id, widgets[i]->name.c_str());
	}
}

//
void CNeutrinoApp::loadSkin(std::string skinName)
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::loadSkin: %s\n", skinName.c_str());
	
	//TEST
	parseSkin();
	
#if ENABLE_LUA	
	// unload
	unloadSkin();
	
	//
	std::string skinPath = CONFIGDIR "/skin/";
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
						dprintf(DEBUG_NORMAL, "CNeutrinoApp::loadskin: add: %s\n", filename.c_str());
						
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
		std::string iconsDir = CONFIGDIR "/skin/";
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
		std::string buttonsDir = CONFIGDIR "/skin/";
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
		std::string hintsDir = CONFIGDIR "/skin/";
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
		std::string skinConfigFile = CONFIGDIR "/skin/";
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
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::startSkin: %s\n", filename);
	
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
		dprintf(DEBUG_NORMAL, "CPlugins::startSkin: could not find %s\n", filename);
		
		std::string hint = filename;
		hint += " ";
		hint += g_Locale->getText(LOCALE_PLUGINS_NOT_INSTALLED);
		
		HintBox(LOCALE_MESSAGEBOX_INFO, hint.c_str());
	}
#endif	
}

bool CNeutrinoApp::skin_exists(const char* const filename)
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::skin_exists: %s\n", filename);
	
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
	g_settings.menu_shadow = true;
	
	// infobar
	g_settings.infobar_gradient = NOGRADIENT;
	g_settings.infobar_gradient_direction = GRADIENT_HORIZONTAL;
	g_settings.infobar_radius = NO_RADIUS;
	g_settings.infobar_corner = CORNER_NONE;
	g_settings.infobar_buttonbar = true;
	g_settings.infobar_buttonline = false;
	g_settings.infobar_shadow = true;
	
	// head
	g_settings.Head_radius = RADIUS_MID;
	g_settings.Head_corner = CORNER_TOP;
	g_settings.Head_gradient = LIGHT2DARK;
	
	// foot
	g_settings.Foot_radius = RADIUS_MID;
	g_settings.Foot_corner = CORNER_BOTTOM;
	g_settings.Foot_gradient = DARK2LIGHT;
	
	// itemInfo
	g_settings.Foot_Info_shadow = configfile.getBool("Foot_Info_shadow", true);
	g_settings.Foot_Info_gradient = configfile.getInt32("Foot_Info_gradient", NOGRADIENT);
	
	delete themes;
	themes = NULL;
}

//
void CNeutrinoApp::readSkinConfig(const char* const filename)
{
	dprintf(DEBUG_NORMAL, "CNeutrinpApp::readSkinConfig %s\n", filename);
	
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
		g_settings.infobar_shadow = skinConfig->getBool("infobar_shadow", true);
		g_settings.infobar_buttonbar = skinConfig->getBool("infobar_buttonbar", true);
		g_settings.infobar_buttonline = skinConfig->getBool("infobar_buttonline", false);
		
		//
		g_settings.menu_shadow = skinConfig->getBool("menu_shadow", true);
		
		// itemInfo
		g_settings.Foot_Info_shadow = skinConfig->getBool("Foot_Info_shadow", true);
		g_settings.Foot_Info_gradient = configfile.getInt32("Foot_Info_gradient", NOGRADIENT);
		
		strcpy( g_settings.font_file, skinConfig->getString( "font_file", DATADIR "/neutrino/fonts/arial.ttf" ).c_str() );

		colorSetupNotifier = new CColorSetupNotifier;
		colorSetupNotifier->changeNotify(NONEXISTANT_LOCALE, NULL);
		
		delete colorSetupNotifier;
	}
	else
		printf("CNeutrinoApp::readSkinConfig: %s not found\n", filename);
}

void CNeutrinoApp::saveSkinConfig(const char * const filename)
{
	dprintf(DEBUG_NORMAL, "CNeutrinoApp::saveFile: %s\n", filename);
	
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


