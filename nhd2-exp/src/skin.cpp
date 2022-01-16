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
	dprintf(DEBUG_INFO, "CNeutrinoApp::convertTarget: id: %d\n", id);
	
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
	dprintf(DEBUG_INFO, "CNeutrinoApp::convertLocale: id: %d\n", loc);
	
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
	_xmlNodePtr search = root->xmlChildrenNode;
	_xmlNodePtr subsearch = NULL;
	_xmlNodePtr node = NULL;
	_xmlNodePtr head_node = NULL;
	_xmlNodePtr foot_node = NULL;
	_xmlNodePtr listbox_node = NULL;
	_xmlNodePtr window_node = NULL;
	
	if (search) 
	{
		dprintf(DEBUG_NORMAL, "CNeutrinoApp:parseSkin: %s\n", filename.c_str());

		while ((search = xmlGetNextOccurence(search, "widget")) != NULL) 
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
			wdg->setColor(wColor);
			wdg->setGradient(gradient);
			wdg->setCorner(corner, radius);
			if (savescreen) wdg->enableSaveScreen();
			wdg->setTimeOut(timeout);
			
			widgets.push_back(wdg);
			
			// widgetitem
			node = search->xmlChildrenNode;
			
			while ((node = xmlGetNextOccurence(node, "widgetitem")) != NULL) 
			{
				//char* widgetitemname = NULL;
				unsigned int widgetitemid = 0;
				
				unsigned int posx = 0;
				unsigned int posy = 0;
				unsigned int width = 0;
				unsigned int height = 0;
				
				unsigned int i_paintframe = 1;
				char* i_color = NULL;
				unsigned int i_gradient = 0;
				unsigned int i_corner = 0;
				unsigned int i_radius = 0;
				
				//widgetitemname = xmlGetAttribute(node, (char*)"name");
				widgetitemid = xmlGetSignedNumericAttribute(node, "id", 0);
				
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
				
				if (widgetitemid == WIDGETITEM_HEAD)
				{
					char* title = NULL;
					unsigned int halign = 0;
					const char* icon = NULL;
					unsigned int h_line = 0;
					unsigned int paintdate = 0;
					char* format = NULL;
					
					title = xmlGetAttribute(node, (char*)"localename");
					halign = xmlGetSignedNumericAttribute(node, "halign", 0);
					icon = xmlGetAttribute(node, (char*)"icon");
					h_line = xmlGetSignedNumericAttribute(node, "line", 0);
					paintdate = xmlGetSignedNumericAttribute(node, "paintdate", 0);
					format = xmlGetAttribute(node, (char*)"format");
				
					CHeaders* head = NULL;
					head = new CHeaders(posx, posy, width, height, title, icon);
					
					//if (title != NULL) head->setTitle(title);
					head->setHAlign(halign);
					//if (icon != NULL) head->setIcon(icon);
					if(i_color != NULL) head->setColor(finalColor);
					head->setGradient(i_gradient);
					head->setCorner(i_corner);
					head->setRadius(i_radius);
					head->setHeadLine(h_line);
					if (paintdate) head->enablePaintDate();
					if (format != NULL) head->setFormat(format);
					
					//
					head_node = node->xmlChildrenNode;
					
					while ((head_node = xmlGetNextOccurence(head_node, "button")) != NULL) 
					{
						char* button = NULL;
						char* localename = NULL;
						
						button = xmlGetAttribute(head_node, (char*)"name");
						localename = xmlGetAttribute(head_node, (char*)"localename");
						
						/*
						button_label_struct btn;
						btn.button = NULL;
						if (button != NULL) btn.button = button;
						btn.localename = NULL;
						if (localename != NULL) btn.localename = localename;
						btn.locale = NONEXISTANT_LOCALE;
						
						head->setButtons(&btn); //FIXME: corrupted
						*/
				
						head_node = head_node->xmlNextNode;
					}
					
					wdg->addItem(head);
				}
				else if (widgetitemid == WIDGETITEM_FOOT)
				{
					unsigned int f_line = 0;
					f_line = xmlGetSignedNumericAttribute(node, "line", 0);
						
					CFooters* foot = NULL;
					foot = new CFooters(posx, posy, width, height);
					
					if (i_color != NULL) foot->setColor(finalColor);
					foot->setGradient(i_gradient);
					foot->setCorner(i_corner);
					foot->setRadius(i_radius);
					foot->setFootLine(f_line);
					
					foot_node = node->xmlChildrenNode;
					
					while ((foot_node = xmlGetNextOccurence(foot_node, "button")) != NULL) 
					{
						char* button = NULL;
						char* localename = NULL;
						
						button = xmlGetAttribute(foot_node, (char*)"name");
						localename = xmlGetAttribute(foot_node, (char*)"localename");
						
						/*
						button_label_struct btn;
						btn.button = NULL;
						if (button != NULL) btn.button = button;
						btn.localename = NULL;
						if (localename != NULL) btn.localename = localename;
						btn.locale = NONEXISTANT_LOCALE;
						
						foot->setButtons(&btn); //FIXME: corrupted
						*/
				
						foot_node = foot_node->xmlNextNode;
					}
					
					wdg->addItem(foot);
				}
				else if (widgetitemid == WIDGETITEM_LISTBOX)
				{
					unsigned int type = 0;
					unsigned int scrollbar = 1;
					unsigned int shadow = 0;
					
					type = xmlGetSignedNumericAttribute(node, "type", 0);
					scrollbar = xmlGetSignedNumericAttribute(node, "scrollbar", 0);
					shadow = xmlGetSignedNumericAttribute(node, "shadow", 0);
					
					ClistBox* listBox = NULL;
					
					listBox = new ClistBox(posx, posy, width, height);
					listBox->setWidgetType(type);
					listBox->paintScrollBar(scrollbar);
					listBox->paintMainFrame(i_paintframe);
					listBox->paintItemShadow(shadow);
					
					CMenuItem* menuItem = NULL;
					
					listbox_node = node->xmlChildrenNode;
					
					while ((listbox_node = xmlGetNextOccurence(listbox_node, "item")) != NULL) 
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
						//char* directkey = NULL;
						unsigned int lines = 0;
						
						
						//itemname = xmlGetAttribute(listbox_node, (char*)"name");
						itemid = xmlGetSignedNumericAttribute(listbox_node, "id", 0);
						
						item_localename = xmlGetAttribute(listbox_node, (char*)"localename");
						option = xmlGetAttribute(listbox_node, (char*)"option");
						item_actionkey = xmlGetAttribute(listbox_node, (char*)"actionkey");
						item_target = xmlGetSignedNumericAttribute(listbox_node, "target", 0);
						itemIcon = xmlGetAttribute(listbox_node, (char*)"itemicon");
						hint = xmlGetAttribute(listbox_node, (char*)"hint");
						iconName = xmlGetAttribute(listbox_node, (char*)"iconname");
						//directkey = xmlGetAttribute(listbox_node, (char*)"directkey");
						lines = xmlGetSignedNumericAttribute(listbox_node, "lines", 0);
						
						CMenuTarget* parent = NULL;
						std::string actionKey = "";
						std::string itemName = "";
						
						if (item_localename) itemName = item_localename;
						
						if (itemid == ITEM_TYPE_FORWARDER)
							menuItem = new CMenuForwarder(itemName.c_str());
						else if (itemid == ITEM_TYPE_LISTBOXITEM)
							menuItem = new ClistBoxItem(itemName.c_str());
						
						if (item_actionkey) actionKey = item_actionkey;	
						parent = convertTarget(item_target);
							
						menuItem->setActionKey(parent, actionKey.c_str());
						
						if (iconName) menuItem->setIconName(iconName);	
						if (hint) menuItem->setHint(hint);
						if (itemIcon) menuItem->setItemIcon(itemIcon);
						if (lines) menuItem->set2lines();
						if (option) menuItem->setOption(option);
						
						listBox->addItem(menuItem);
				
						listbox_node = listbox_node->xmlNextNode;
					}
					
					wdg->addItem(listBox);
				}
				else if (widgetitemid == WIDGETITEM_WINDOW)
				{
					unsigned int refresh = 0;
					refresh = xmlGetSignedNumericAttribute(node, "refresh", 0);
					
					CWindow* window = NULL;
					
					window = new CWindow(posx, posy, width, height);
					
					window->paintMainFrame(i_paintframe);
					if (i_color) window->setColor(finalColor);
					if (refresh) window->enableRepaint();
					
					window_node = node->xmlChildrenNode;
					
					while ((window_node = xmlGetNextOccurence(window_node, "ccitem")) != NULL) 
					{
						//char* cc_name = NULL;
						unsigned int cc_id = 0;
				
						unsigned int cc_x = 0;
						unsigned int cc_y = 0;
						unsigned int cc_dx = 0;
						unsigned int cc_dy = 0;
						
						unsigned int cc_refresh = 0;
						
						
						//cc_name = xmlGetAttribute(window_node, (char*)"name");
						cc_id = xmlGetSignedNumericAttribute(window_node, "id", 0);
				
						cc_x = xmlGetSignedNumericAttribute(window_node, "posx", 0);
						cc_y = xmlGetSignedNumericAttribute(window_node, "posy", 0);
						cc_dx = xmlGetSignedNumericAttribute(window_node, "width", 0);
						cc_dy = xmlGetSignedNumericAttribute(window_node, "height", 0);
						
						cc_refresh = xmlGetSignedNumericAttribute(window_node, "refresh", 0);
						
						if (cc_id == CC_LABEL)
						{
							char* text = NULL;
							unsigned int l_halign = 0;
							
							text = xmlGetAttribute(window_node, (char*)"text");
							l_halign = xmlGetSignedNumericAttribute(window_node, "halign", 0);
						
							CCLabel* label = NULL;
							
							label = new CCLabel(cc_x, cc_y, cc_dx, cc_dy);
							
							if (text != NULL) label->setText(text);
							label->setHAlign(l_halign);
							
							window->addCCItem(label);	
						}
						else if (cc_id == CC_IMAGE)
						{
							char* image = NULL;
							image = xmlGetAttribute(window_node, (char*)"image");
						
							CCImage* pic = NULL;
							
							pic = new CCImage(cc_x, cc_y, cc_dx, cc_dy);
							
							std::string filename = CONFIGDIR "/skins/";
							filename += g_settings.preferred_skin;
							filename += "/";
							filename += image;
							
							if (image != NULL) pic->setImage(filename.c_str());
							
							window->addCCItem(pic);	
						}
						else if (cc_id == CC_TIME)
						{
							char* cc_format = NULL;
							cc_format = xmlGetAttribute(window_node, (char*)"format");
						
							CCTime* time = NULL;
							
							time = new CCTime(cc_x, cc_y, cc_dx, cc_dy);
							
							if (cc_format != NULL) time->setFormat(cc_format); //FIXME: corrupted
							if (cc_refresh) time->enableRepaint();
							
							window->addCCItem(time);	
						}
				
						window_node = window_node->xmlNextNode;
					}
					
					wdg->addItem(window);
				}
				else if (widgetitemid == WIDGETITEM_FRAMEBOX)
				{
					CFrameBox* frameBox = NULL;
					
					frameBox = new CFrameBox(posx, posy, width, height);
					
					wdg->addItem(frameBox);
				}
			
				node = node->xmlNextNode;
			}
			
			// key
			subsearch = search->xmlChildrenNode;
			
			while ((subsearch = xmlGetNextOccurence(subsearch, "key")) != NULL) 
			{
				neutrino_msg_t key_name = (neutrino_msg_t)xmlGetSignedNumericAttribute(subsearch, "name", 16);
				char* key_actionkey = xmlGetAttribute(subsearch, (char*)"actionkey");
				unsigned int key_target = xmlGetSignedNumericAttribute(subsearch, "target", 0);
				
				CMenuTarget* key_parent = NULL;
				
				key_parent = convertTarget(key_target);
				
				wdg->addKey((neutrino_msg_t)key_name, key_parent, key_actionkey);
			
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


