/*
	Neutrino-GUI  -   DBoxII-Project

	$id: service_setup.cpp 2015.12.22 17:19:30 mohousch $
	
	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h> 

#include <global.h>
#include <neutrino.h>

#include <gui/service_menu.h>

#include <configfile.h>

#include <system/debug.h>
#include <system/settings.h>
#include <system/flashtool.h>

#include <gui/widget/hintbox.h>
#include <gui/widget/messagebox.h>

#include <gui/scan_setup.h>
#include <gui/widget/icons.h>
#include <gui/update.h>
#include <gui/cam_menu.h>
#include <gui/imageinfo.h>

#include <gui/bedit/bouqueteditor_bouquets.h>


#if !defined (PLATFORM_COOLSTREAM)
#if defined (ENABLE_CI)
extern CCAMMenuHandler * g_CamHandler;		// defined neutrino.cpp
#endif
#endif

extern int FrontendCount;			// defined in zapit.cpp

//
int CServiceMenu::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CServiceMenu::exec: actionKey:%s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "reloadchannels")
	{
		if (MessageBox(_("Information"), _("Reloading channel lists, please be patient."), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			HintBox(_("Information"), _("Reloading channel lists, please be patient."));
			g_Zapit->reinitChannels();
		}
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

// showmenu
void CServiceMenu::showMenu(void)
{
	dprintf(DEBUG_NORMAL, "CServiceMenu::showMenu\n");
	
	int shortcutService = 1;
	
	CMenuWidget * service = new CMenuWidget(_("Service"), NEUTRINO_ICON_UPDATE);

	service->setWidgetMode(MODE_MENU);
	service->setWidgetType(WIDGET_TYPE_CLASSIC);
	service->enableShrinkMenu();
	service->setMenuPosition(MENU_POSITION_CENTER);
	service->enablePaintFootInfo();
	service->enablePaintDate();
	
	// tuner/scan setup
	service->addItem(new CMenuForwarder(_("Scan transponder"), true, NULL, new CTunerSetup(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_SCANSETTINGS, _("Scan transponder")));

	// reload Channels
	service->addItem(new CMenuForwarder(_("Reload channel lists"), true, NULL, this, "reloadchannels", RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_RELOADCHANNELS, _("Reload channel lists")));

	// Bouquets Editor
	service->addItem(new CMenuForwarder(_("Bouquet Editor"), true, NULL, new CBEBouquetWidget(), NULL, RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_BOUQUETSEDITOR, _("Bouquet Editor")));
	
	// CI Cam 	
#if defined (ENABLE_CI)
	service->addItem(new CMenuForwarder(_("CI Cam"), true, NULL, g_CamHandler, NULL, CRCInput::convertDigitToKey(shortcutService++), NULL, NEUTRINO_ICON_MENUITEM_CICAM, _("CI Cam Settings")));
#endif
	
	// image info
	service->addItem(new CMenuForwarder(_("Image info"),  true, NULL, new CImageInfo(), NULL, RC_info, NEUTRINO_ICON_BUTTON_HELP, NEUTRINO_ICON_MENUITEM_IMAGEINFO, _("Image info")), false);
	
	// software update
	service->addItem(new CMenuForwarder(_("Software Update"), true, NULL, new CUpdateSettings(), NULL, RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_SOFTUPDATE, _("Software Update")));

	service->integratePlugins(CPlugins::I_TYPE_SERVICE, shortcutService++);
	
	service->exec(NULL, "");
	service->hide();
	delete service;
	service = NULL;
}


