/*
	Neutrino-GUI  -   DBoxII-Project

	$id: main_setup.cpp 2015.12.22 21:31:30 mohousch $
	
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

#include <global.h>
#include <neutrino.h>

#include <stdio.h>

#include <gui/main_setup.h>

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

#include <system/debug.h>
#include <system/setting_helpers.h>
#include <system/helpers.h>


// main settings
int CMainSettingsMenu::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CMainSettingsMenu::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	showMenu();
	
	return ret;
}

// showmenu
void CMainSettingsMenu::showMenu(void)
{
	dprintf(DEBUG_NORMAL, "CMainSettingsMenu::showMenu:\n");
	
	int shortcutMainSettings = 1;

	CMenuWidget * mainSettings = new CMenuWidget(_("Main Settings"), NEUTRINO_ICON_SETTINGS);

	mainSettings->setWidgetMode(MODE_MENU);
	mainSettings->setWidgetType(WIDGET_TYPE_CLASSIC);
	mainSettings->enableShrinkMenu();
	mainSettings->setMenuPosition(MENU_POSITION_LEFT);
	mainSettings->enablePaintFootInfo();
	mainSettings->enablePaintDate();

	// video settings
	mainSettings->addItem(new CMenuForwarder(_("Video settings"), true, NULL, new CVideoSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_VIDEOSETTINGS));

	// audio settings
	mainSettings->addItem(new CMenuForwarder(_("Audio settings"), true, NULL, new CAudioSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_AUDIOSETTINGS));

	// parentallock
	mainSettings->addItem(new CMenuForwarder(_("Parentallock settings"), true, NULL, new CParentalLockSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL,  NEUTRINO_ICON_MENUITEM_PARENTALLOCKSETTINGS));

	// network settings
	mainSettings->addItem(new CMenuForwarder(_("Network settings"), true, NULL,  CNetworkSettings::getInstance(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_NETWORKSETTINGS));

	// recording settings
	mainSettings->addItem(new CMenuForwarder(_("Recording settings"), true, NULL, new CRecordingSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL,  NEUTRINO_ICON_MENUITEM_RECORDINGSETTINGS));

	// movieplayer settings
	mainSettings->addItem(new CMenuForwarder(_("Movie Player settings"), true, NULL, new CMoviePlayerSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL,  NEUTRINO_ICON_MENUITEM_MOVIEPLAYERSETTINGS));

	//OSD settings
	mainSettings->addItem(new CMenuForwarder(_("OSD settings"), true, NULL, new COSDSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_OSDSETTINGS));

	// vfd/lcd settings
	//if(CVFD::getInstance()->has_lcd)
		mainSettings->addItem(new CMenuForwarder(_("LCD settings"), true, NULL, new CLCDSettings(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL,  NEUTRINO_ICON_MENUITEM_LCDSETTINGS));	

	// remote control settings
	mainSettings->addItem(new CMenuForwarder(_("Remote Control settings"), true, NULL, new CRemoteControlSettings(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_REMOTECONTROLSETTINGS));

	// audioplayer settings
	mainSettings->addItem(new CMenuForwarder(_("Audio Player settings"), true, NULL, new CAudioPlayerSettings(), NULL, RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_AUDIOPLAYERSETTINGS));
	
	// pictureviewer settings
	mainSettings->addItem(new CMenuForwarder("Picture Viewer settings", true, NULL, new CPictureViewerSettings(), NULL, RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_PICTUREVIEWERSETTINGS));

	// misc settings
	mainSettings->addItem(new CMenuForwarder(_("Misc settings"), true, NULL, new CMiscSettingsMenu(), NULL, RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_MISCSETTINGS));

	//HDD settings
	mainSettings->addItem(new CMenuForwarder(_("HDD settings"), true, NULL, new CHDDMenuHandler(), NULL, CRCInput::convertDigitToKey(shortcutMainSettings++), NULL, NEUTRINO_ICON_MENUITEM_HDDSETTINGS));
	
	mainSettings->exec(NULL, "");
	mainSettings->hide();
	delete mainSettings;
	mainSettings = NULL;
}

