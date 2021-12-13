/*
	Neutrino-GUI  -   DBoxII-Project

	$id: osd_setup.cpp 21.09.21 mohousch $
	
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
 
#include <sys/stat.h>
#include <dirent.h>

#include <gui/widget/hintbox.h>

#include <gui/widget/colorchooser.h>
#include <gui/widget/stringinput.h>

#include <gui/filebrowser.h>
#include <gui/osd_setup.h>
#include <gui/themes.h>
#include <gui/screensetup.h>
#include <gui/alphasetup.h>

#include <system/debug.h>
#include <system/setting_helpers.h>
#include <system/helpers.h>


// osd settings
COSDSettings::COSDSettings()
{
}

COSDSettings::~COSDSettings()
{
}

int COSDSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}

	showMenu();
	
	return ret;
}

// showmenu
void COSDSettings::showMenu(void)
{
	dprintf(DEBUG_NORMAL, "COSDSettings::showMenu:\n");
	
	int shortcutOSD = 1;
	
	CMenuWidget * osdSettings = new CMenuWidget(LOCALE_MAINSETTINGS_OSD, NEUTRINO_ICON_COLORS );

	osdSettings->setWidgetMode(MODE_MENU);
	osdSettings->setWidgetType(WIDGET_TYPE_CLASSIC);
	osdSettings->enableWidgetChange();
	osdSettings->enableShrinkMenu();
	osdSettings->setMenuPosition(MENU_POSITION_LEFT);
	osdSettings->enablePaintFootInfo();
	osdSettings->enablePaintDate();
	
	// skin
	osdSettings->addItem( new CMenuForwarder("Skin Auswahl", g_settings.use_skin, NULL, new CSkinManager(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_THEMES, LOCALE_HELPTEXT_THEMES));

	// Themes
	CThemes * osdSettings_Themes = new CThemes();
	
	osdSettings->addItem( new CMenuForwarder(LOCALE_COLORMENU_THEMESELECT, true, NULL, osdSettings_Themes, NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_THEMES, LOCALE_HELPTEXT_THEMES));

	// menu colors
	osdSettings->addItem( new CMenuForwarder(LOCALE_COLORMENU_MENUCOLORS, true, NULL, new COSDMenuColorSettings(), NULL, RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_MENUCOLORS, LOCALE_HELPTEXT_MENUCOLORS));

	// infobar
	osdSettings->addItem( new CMenuForwarder(LOCALE_COLORSTATUSBAR_HEAD, true, NULL, new COSDInfoBarColorSettings(), NULL, RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_INFOBARCOLORS, LOCALE_HELPTEXT_INFOBARCOLORS));

	// language
	osdSettings->addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_LANGUAGE, true, NULL, new CLanguageSettings(), NULL, RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_LANGUAGE, LOCALE_HELPTEXT_LANGUAGE));
	
	// font
	osdSettings->addItem(new CMenuForwarder(LOCALE_EPGPLUS_SELECT_FONT_NAME, true, NULL, new CFontSettings(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_FONT, LOCALE_HELPTEXT_FONT));
	
	// osd timing
	osdSettings->addItem(new CMenuForwarder(LOCALE_TIMING_HEAD, true, NULL, new COSDTimingSettings(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_OSDTIMING, LOCALE_HELPTEXT_OSDTIMING));

	// sceensetup
	osdSettings->addItem(new CMenuForwarder(LOCALE_VIDEOMENU_SCREENSETUP, true, NULL, new CScreenSetup(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_SCREENSETUP, LOCALE_HELPTEXT_SCREENSETUP));
	
	// alpha setup
	//FIXME:
	//CAlphaSetup * chAlphaSetup = new CAlphaSetup(LOCALE_COLORMENU_GTX_ALPHA, &g_settings.gtx_alpha);
	//osdSettings->addItem( new CMenuForwarder(LOCALE_COLORMENU_GTX_ALPHA, true, NULL, chAlphaSetup, NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_ALPHASETUP, LOCALE_HELPTEXT_ALPHA_SETUP));
	
	// diverses
	osdSettings->addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_MISC, true, NULL, new COSDDiverses(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_OSDSETTINGS, LOCALE_HELPTEXT_OSDSETTINGS));
	
	osdSettings->exec(NULL, "");
	osdSettings->hide();
	delete osdSettings;
	osdSettings = NULL;
}

// osd menucolor settings
#define COLOR_GRADIENT_TYPE_OPTION_COUNT 5
const keyval COLOR_GRADIENT_TYPE_OPTIONS[COLOR_GRADIENT_TYPE_OPTION_COUNT] =
{
	{ NOGRADIENT, NONEXISTANT_LOCALE, "no gradient" },
	{ DARK2LIGHT, NONEXISTANT_LOCALE, "Dark to Light" },
	{ LIGHT2DARK, NONEXISTANT_LOCALE, "Light to Dark" },
	{ DARK2LIGHT2DARK, NONEXISTANT_LOCALE, "Dark to Light to Dark" },
	{ LIGHT2DARK2LIGHT, NONEXISTANT_LOCALE, "Light to Dark to Light" }
};

COSDMenuColorSettings::COSDMenuColorSettings()
{
}

COSDMenuColorSettings::~COSDMenuColorSettings()
{
}

int COSDMenuColorSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDMenuColorSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDMenuColorSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDMenuColorSettings::showMenu:\n");
	
	CMenuWidget OSDmenuColorsSettings(LOCALE_COLORMENUSETUP_HEAD, NEUTRINO_ICON_SETTINGS );

	OSDmenuColorsSettings.setWidgetMode(MODE_SETUP);
	OSDmenuColorsSettings.enableShrinkMenu();
	
	// intros
	OSDmenuColorsSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));

	// save settings
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE) );
	OSDmenuColorsSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));

	// head
	CColorChooser* chHeadcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Head_red, &g_settings.menu_Head_green, &g_settings.menu_Head_blue, &g_settings.menu_Head_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chHeadTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Head_Text_red, &g_settings.menu_Head_Text_green, &g_settings.menu_Head_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// window content
	CColorChooser* chContentcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Content_red, &g_settings.menu_Content_green, &g_settings.menu_Content_blue,&g_settings.menu_Content_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chContentTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Content_Text_red, &g_settings.menu_Content_Text_green, &g_settings.menu_Content_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// window content inactive
	CColorChooser* chContentInactivecolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Content_inactive_red, &g_settings.menu_Content_inactive_green, &g_settings.menu_Content_inactive_blue,&g_settings.menu_Content_inactive_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chContentInactiveTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Content_inactive_Text_red, &g_settings.menu_Content_inactive_Text_green, &g_settings.menu_Content_inactive_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// window content selected
	CColorChooser* chContentSelectedcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Content_Selected_red, &g_settings.menu_Content_Selected_green, &g_settings.menu_Content_Selected_blue,&g_settings.menu_Content_Selected_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chContentSelectedTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Content_Selected_Text_red, &g_settings.menu_Content_Selected_Text_green, &g_settings.menu_Content_Selected_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// foot
	CColorChooser* chFootcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_Foot_red, &g_settings.menu_Foot_green, &g_settings.menu_Foot_blue, &g_settings.menu_Foot_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser * chFootTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_Foot_Text_red, &g_settings.menu_Foot_Text_green, &g_settings.menu_Foot_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// foot info
	CColorChooser* chFootInfoColor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.menu_FootInfo_red, &g_settings.menu_FootInfo_green, &g_settings.menu_FootInfo_blue, &g_settings.menu_FootInfo_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser * chFootInfoTextColor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.menu_FootInfo_Text_red, &g_settings.menu_FootInfo_Text_green, &g_settings.menu_FootInfo_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// head
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORMENUSETUP_MENUHEAD)));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chHeadcolor ));

	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chHeadTextcolor ));

	// head gradient
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(LOCALE_COLORMENU_GRADIENT, &g_settings.Head_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));

	// window content
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORMENUSETUP_MENUCONTENT)));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chContentcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chContentTextcolor ));

	// window content inactiv
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORMENUSETUP_MENUCONTENT_INACTIVE)));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chContentInactivecolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chContentInactiveTextcolor));

	// window content selected
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORMENUSETUP_MENUCONTENT_SELECTED)));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chContentSelectedcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chContentSelectedTextcolor ));
	
	// foot
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORMENU_HELPBAR)));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chFootcolor ));

	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chFootTextcolor ));

	// foot gradient
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(LOCALE_COLORMENU_GRADIENT, &g_settings.Foot_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));

	// foot info
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORMENU_FOOT_TITLE)));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chFootInfoColor ));

	OSDmenuColorsSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chFootInfoTextColor ));

	// foot info gradient
	//OSDmenuColorsSettings.addItem(new CMenuOptionChooser(LOCALE_COLORMENU_GRADIENT, &g_settings.Foot_Info_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	OSDmenuColorsSettings.exec(NULL, "");
	OSDmenuColorsSettings.hide();
}

// osd infobarcolor settings
COSDInfoBarColorSettings::COSDInfoBarColorSettings()
{
}

COSDInfoBarColorSettings::~COSDInfoBarColorSettings()
{
}

int COSDInfoBarColorSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDInfoBarColorSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDInfoBarColorSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDInfoBarColorSettings::showMenu:\n");
	
	CMenuWidget OSDinfobarColorSettings(LOCALE_COLORMENU_STATUSBAR, NEUTRINO_ICON_SETTINGS);

	OSDinfobarColorSettings.setWidgetMode(MODE_SETUP);
	OSDinfobarColorSettings.enableShrinkMenu();
	
	// intros
	OSDinfobarColorSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));

	OSDinfobarColorSettings.addItem(new CMenuSeparator(LINE));
	OSDinfobarColorSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));

	// bg
	CColorChooser * chInfobarcolor = new CColorChooser(LOCALE_COLORMENU_BACKGROUND, &g_settings.infobar_red, &g_settings.infobar_green, &g_settings.infobar_blue,&g_settings.infobar_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// text
	CColorChooser * chInfobarTextcolor = new CColorChooser(LOCALE_COLORMENU_TEXTCOLOR, &g_settings.infobar_Text_red, &g_settings.infobar_Text_green, &g_settings.infobar_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// colored events
	CColorChooser * chColored_Events = new CColorChooser(LOCALE_MISCSETTINGS_INFOBAR_COLORED_EVENTS, &g_settings.infobar_colored_events_red, &g_settings.infobar_colored_events_green, &g_settings.infobar_colored_events_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	OSDinfobarColorSettings.addItem( new CMenuSeparator(LINE | STRING, g_Locale->getText(LOCALE_COLORSTATUSBAR_TEXT)));

	// bg color
	OSDinfobarColorSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_BACKGROUND, true, NULL, chInfobarcolor ));

	// text color
	OSDinfobarColorSettings.addItem( new CMenuForwarder(LOCALE_COLORMENU_TEXTCOLOR, true, NULL, chInfobarTextcolor ));

	// events text color
	OSDinfobarColorSettings.addItem( new CMenuForwarder(LOCALE_MISCSETTINGS_INFOBAR_COLORED_EVENTS, true, NULL, chColored_Events ));

	// gradient
	//OSDinfobarColorSettings.addItem(new CMenuOptionChooser(LOCALE_COLORMENU_GRADIENT, &g_settings.infobar_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	OSDinfobarColorSettings.exec(NULL, "");
	OSDinfobarColorSettings.hide();
}

// osd language settings
CLanguageSettings::CLanguageSettings()
{
}

CLanguageSettings::~CLanguageSettings()
{
}

int CLanguageSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CLanguageSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();

	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CLanguageSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CLanguageSettings::showMenu:\n");
	
	CMenuWidget languageSettings(LOCALE_LANGUAGESETUP_HEAD, NEUTRINO_ICON_LANGUAGE );

	languageSettings.setWidgetMode(MODE_SETUP);
	languageSettings.enableShrinkMenu();
	
	// intros
	languageSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	languageSettings.addItem(new CMenuSeparator(LINE));

	languageSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	languageSettings.addItem(new CMenuSeparator(LINE));

	struct dirent **namelist;
	int n;

	char *path[] = {(char *) DATADIR "/neutrino/locale", (char *) CONFIGDIR "/locale"};

	for(int p = 0; p < 2; p++) 
	{
		n = scandir(path[p], &namelist, 0, alphasort);
		
		if(n > 0)
		{
			for(int count = 0; count < n; count++) 
			{
				char * locale = strdup(namelist[count]->d_name);
				char * pos = strstr(locale, ".locale");

				if(pos != NULL) 
				{
					*pos = '\0';
				
					CMenuOptionLanguageChooser* oj = new CMenuOptionLanguageChooser((char*)locale, this, locale);
					oj->addOption(locale);
					languageSettings.addItem( oj );
				} 
				else
					free(locale);
				free(namelist[count]);
			}
			free(namelist);
		}
	}
	
	languageSettings.exec(NULL, "");
	languageSettings.hide();
}

bool CLanguageSettings::changeNotify(const neutrino_locale_t OptionName, void */*data*/)
{
	if (ARE_LOCALES_EQUAL(OptionName, LOCALE_LANGUAGESETUP_SELECT)) 
	{
		dprintf(DEBUG_NORMAL, "CLanguageSettings::changeNotify: %s\n", g_settings.language);
		
		g_Locale->loadLocale(g_settings.language);

		return true;
	}
	
	return false;
}


// CFontSettings
CFontSettings::CFontSettings()
{
}

CFontSettings::~CFontSettings()
{
}

int CFontSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CFontSettings::exec: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if (parent)
		parent->hide();
		
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->SetupFonts(g_settings.font_file);
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "select_font")
	{
		CFileBrowser fileBrowser;
		CFileFilter fileFilter;
		fileFilter.addFilter("ttf");
		fileBrowser.Filter = &fileFilter;
		
		if (fileBrowser.exec(DATADIR "/neutrino/fonts") == true)
		{
			strcpy(g_settings.font_file, fileBrowser.getSelectedFile()->Name.c_str());
			dprintf(DEBUG_NORMAL, "COSDSettings::exec: new font file %s\n", fileBrowser.getSelectedFile()->Name.c_str());
			
			CNeutrinoApp::getInstance()->SetupFonts(g_settings.font_file);
			CNeutrinoApp::getInstance()->saveSetup(NEUTRINO_SETTINGS_FILE);
		}
		
		getString() = g_settings.font_file;
		
		return ret;
	}
		
	showMenu();
	
	return ret;
}

void CFontSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CFontSettings::showMenu:\n");
	
	CMenuWidget fontSettings(LOCALE_EPGPLUS_SELECT_FONT_NAME, NEUTRINO_ICON_COLORS, 700);
	
	fontSettings.setWidgetMode(MODE_SETUP);
	fontSettings.enableShrinkMenu();
	
	// intros
	fontSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	fontSettings.addItem(new CMenuSeparator(LINE));

	fontSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	fontSettings.addItem(new CMenuSeparator(LINE));
	
	// font name
	fontSettings.addItem(new CMenuForwarder(LOCALE_EPGPLUS_SELECT_FONT_NAME, true, g_settings.font_file, this, "select_font"));
	
	// font scaling
	fontSettings.addItem(new CMenuSeparator(LINE|STRING, g_Locale->getText(LOCALE_FONTMENU_HEAD)));
	fontSettings.addItem(new CMenuOptionNumberChooser(LOCALE_FONTMENU_SCALING_X, &g_settings.screen_xres, true, 50, 200, NULL) );
	fontSettings.addItem(new CMenuOptionNumberChooser(LOCALE_FONTMENU_SCALING_Y, &g_settings.screen_yres, true, 50, 200, NULL) );
		
	
	fontSettings.exec(NULL, "");
}

// osd timing settings
static CTimingSettingsNotifier timingsettingsnotifier;

COSDTimingSettings::COSDTimingSettings()
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings\n");
}

COSDTimingSettings::~COSDTimingSettings()
{
	dprintf(DEBUG_NORMAL, "~COSDTimingSettings\n");
}

int COSDTimingSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
	
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "osd.def") 
	{
		for (int i = 0; i < TIMING_SETTING_COUNT; i++)
			g_settings.timing[i] = default_timing[i];

		CNeutrinoApp::getInstance()->SetupTiming();
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDTimingSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings::showMenu:\n");
	
	CMenuWidget osdTimingSettings(LOCALE_COLORMENU_TIMING, NEUTRINO_ICON_SETTINGS);

	osdTimingSettings.setWidgetMode(MODE_SETUP);
	osdTimingSettings.enableShrinkMenu();
	
	// intros
	osdTimingSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	osdTimingSettings.addItem(new CMenuSeparator(LINE));

	osdTimingSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	osdTimingSettings.addItem(new CMenuSeparator(LINE));

	for (int i = 0; i < TIMING_SETTING_COUNT; i++)
	{
		CStringInput * colorSettings_timing_item = new CStringInput(timing_setting_name[i], g_settings.timing_string[i], 3, LOCALE_TIMING_HINT_1, LOCALE_TIMING_HINT_2, "0123456789 ", &timingsettingsnotifier);

		osdTimingSettings.addItem(new CMenuForwarder(timing_setting_name[i], true, g_settings.timing_string[i], colorSettings_timing_item));
	}

	osdTimingSettings.addItem(new CMenuSeparator(LINE));
	osdTimingSettings.addItem(new CMenuForwarder(LOCALE_OPTIONS_DEFAULT, true, NULL, this, "osd.def", RC_green, NEUTRINO_ICON_BUTTON_GREEN));
	
	osdTimingSettings.exec(NULL, "");
	osdTimingSettings.hide();
}

// timing settings notifier
bool CTimingSettingsNotifier::changeNotify(const neutrino_locale_t OptionName, void *)
{
	dprintf(DEBUG_NORMAL, "CTimingSettingsNotifier::changeNotify:\n");
		
	for (int i = 0; i < TIMING_SETTING_COUNT; i++)
	{
		if (ARE_LOCALES_EQUAL(OptionName, timing_setting_name[i]))
		{
			g_settings.timing[i] = atoi(g_settings.timing_string[i]);
			return true;
		}
	}

	return false;
}

// diverses
COSDDiverses::COSDDiverses()
{
	dprintf(DEBUG_NORMAL, "COSDDiverses:\n");
}

COSDDiverses::~COSDDiverses()
{
	dprintf(DEBUG_NORMAL, "~COSDDiverses:\n");
}

int COSDDiverses::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "COSDDiverses::exec: actionKey: %s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if(parent)
		parent->hide();
		
	if(actionKey == "savesettings")
	{
		CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		
		return ret;
	}
	else if(actionKey == "logos_dir") 
	{
		if(parent)
			parent->hide();
		
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.logos_dir.c_str())) 
		{
			g_settings.logos_dir = b.getSelectedFile()->Name;

			dprintf(DEBUG_NORMAL, "CMiscSettings::exec: new logos dir %s\n", b.getSelectedFile()->Name.c_str());
		}

		getString() = g_settings.logos_dir;

		return ret;
	}
	else if(actionKey == "select_icons_dir")
	{
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.icons_dir.c_str())) 
		{
			g_settings.icons_dir = b.getSelectedFile()->Name + "/";

			dprintf(DEBUG_NORMAL, "CMiscSettings::exec: new icons dir %s\n", g_settings.icons_dir.c_str());

			CFrameBuffer::getInstance()->setIconBasePath(g_settings.icons_dir);
			//CNeutrinoApp::getInstance()->saveSetup(NEUTRINO_SETTINGS_FILE);
		}
		
		getString() = g_settings.icons_dir;
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

// widget type
/*
#define WIDGET_TYPE_OPTION_COUNT	4
const keyval WIDGET_TYPE_OPTIONS[WIDGET_TYPE_OPTION_COUNT] =
{
	{ WIDGET_TYPE_STANDARD, NONEXISTANT_LOCALE, "Standard"},
	{ WIDGET_TYPE_CLASSIC, NONEXISTANT_LOCALE, "Classic"},
	{ WIDGET_TYPE_EXTENDED, NONEXISTANT_LOCALE, "Extended"},
	{ WIDGET_TYPE_FRAME, NONEXISTANT_LOCALE, "Frame"}
};
*/

#define OPTIONS_OFF0_ON1_OPTION_COUNT 2
const keyval OPTIONS_OFF0_ON1_OPTIONS[OPTIONS_OFF0_ON1_OPTION_COUNT] =
{
        { 0, LOCALE_OPTIONS_OFF, NULL },
        { 1, LOCALE_OPTIONS_ON, NULL }
};

// progressbar color
#define PROGRESSBAR_COLOR_OPTION_COUNT 2
const keyval PROGRESSBAR_COLOR_OPTIONS[PROGRESSBAR_COLOR_OPTION_COUNT] =
{
	{ 0, NONEXISTANT_LOCALE, "colored" },
	{ 1, NONEXISTANT_LOCALE, "Mono Chrom" }
};

// volumebar position
#define VOLUMEBAR_DISP_POS_OPTIONS_COUNT 6
const keyval  VOLUMEBAR_DISP_POS_OPTIONS[VOLUMEBAR_DISP_POS_OPTIONS_COUNT]=
{
	{ 0 , LOCALE_SETTINGS_POS_TOP_RIGHT, NULL },
	{ 1 , LOCALE_SETTINGS_POS_TOP_LEFT, NULL },
	{ 2 , LOCALE_SETTINGS_POS_BOTTOM_LEFT, NULL },
	{ 3 , LOCALE_SETTINGS_POS_BOTTOM_RIGHT, NULL },
	{ 4 , LOCALE_SETTINGS_POS_DEFAULT_CENTER, NULL },
	{ 5 , LOCALE_SETTINGS_POS_HIGHER_CENTER, NULL }
};

#define MENU_CORNERSETTINGS_TYPE_OPTION_COUNT 2
const keyval MENU_CORNERSETTINGS_TYPE_OPTIONS[MENU_CORNERSETTINGS_TYPE_OPTION_COUNT] =
{
	{ NO_RADIUS, LOCALE_EXTRA_ROUNDED_CORNERS_OFF, NULL },
	{ ROUNDED, LOCALE_EXTRA_ROUNDED_CORNERS_ON, NULL }	
};

/*
#define MENU_POSITION_OPTION_COUNT 3
const keyval MENU_POSITION_OPTIONS[MENU_POSITION_OPTION_COUNT] =
{
	{ SNeutrinoSettings::MENU_POSITION_LEFT, LOCALE_EXTRA_MENU_POSITION_LEFT, NULL },
	{ SNeutrinoSettings::MENU_POSITION_CENTER, LOCALE_EXTRA_MENU_POSITION_CENTER, NULL },
	{ SNeutrinoSettings::MENU_POSITION_RIGHT, LOCALE_EXTRA_MENU_POSITION_RIGHT, NULL }
};
*/

#define INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT 4
const keyval  INFOBAR_SUBCHAN_DISP_POS_OPTIONS[INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT]=
{
	{ 0 , LOCALE_SETTINGS_POS_TOP_RIGHT, NULL },
	{ 1 , LOCALE_SETTINGS_POS_TOP_LEFT, NULL },
	{ 2 , LOCALE_SETTINGS_POS_BOTTOM_LEFT, NULL },
	{ 3 , LOCALE_SETTINGS_POS_BOTTOM_RIGHT, NULL }
};

void COSDDiverses::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings::showMenu:\n");
	
	CMenuWidget osdDiverseSettings(LOCALE_MAINSETTINGS_MISC, NEUTRINO_ICON_SETTINGS);

	osdDiverseSettings.setWidgetMode(MODE_SETUP);
	osdDiverseSettings.enableShrinkMenu();
	
	// intros
	osdDiverseSettings.addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	osdDiverseSettings.addItem(new CMenuSeparator(LINE));

	osdDiverseSettings.addItem(new CMenuForwarder(LOCALE_MAINSETTINGS_SAVESETTINGSNOW, true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	osdDiverseSettings.addItem(new CMenuSeparator(LINE));
	
	// widget type
	//osdDiverseSettings.addItem(new CMenuOptionChooser("Menu Design", &g_settings.menu_design, WIDGET_TYPE_OPTIONS, WIDGET_TYPE_OPTION_COUNT, true));

	// menu position
	//osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_EXTRA_MENU_POSITION, &g_settings.menu_position, MENU_POSITION_OPTIONS, MENU_POSITION_OPTION_COUNT, true));
	
	// use skin?
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_INFOBAR_SAT_DISPLAY, &g_settings.use_skin, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true));

	// corners
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_EXTRA_ROUNDED_CORNERS, &g_settings.rounded_corners, MENU_CORNERSETTINGS_TYPE_OPTIONS, MENU_CORNERSETTINGS_TYPE_OPTION_COUNT, true));

	// progressbar color
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_PROGRESSBAR_COLOR, &g_settings.progressbar_color, PROGRESSBAR_COLOR_OPTIONS, PROGRESSBAR_COLOR_OPTION_COUNT, true));
	
	// progressbar_gradient
	osdDiverseSettings.addItem(new CMenuOptionChooser("ProgressBar Gradient", &g_settings.progressbar_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true));
	
	// icons dir
	osdDiverseSettings.addItem(new CMenuForwarder("Icons Dir", true, g_settings.icons_dir.c_str(), this, "select_icons_dir"));
	
	// logos dir
	osdDiverseSettings.addItem( new CMenuForwarder(LOCALE_MISCSETTINGS_LOGOSDIR, true, g_settings.logos_dir.c_str(), this, "logos_dir" ) );
	
	// epgplus logos
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_EPGPLUS_SHOW_LOGOS, &g_settings.epgplus_show_logo, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// infobar show channelname
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_INFOBAR_SHOW_CHANNELNAME, &g_settings.show_channelname, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// sig/snr
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_MISCSETTINGS_INFOBAR_SAT_DISPLAY, &g_settings.infobar_sat_display, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true));
	
	// subchan pos
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_INFOVIEWER_SUBCHAN_DISP_POS, &g_settings.infobar_subchan_disp_pos, INFOBAR_SUBCHAN_DISP_POS_OPTIONS, INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT, true, NULL, RC_nokey, "", true));
	
	// volumebar position
	osdDiverseSettings.addItem(new CMenuOptionChooser(LOCALE_EXTRA_VOLUME_POS, &g_settings.volume_pos, VOLUMEBAR_DISP_POS_OPTIONS, VOLUMEBAR_DISP_POS_OPTIONS_COUNT, true, NULL, RC_nokey, "", true ));

	// volume bar steps
	CStringInput * audio_step = new CStringInput(LOCALE_AUDIOMENU_VOLUMEBAR_AUDIOSTEPS,g_settings.audio_step, 2, NONEXISTANT_LOCALE, NONEXISTANT_LOCALE, "0123456789 " );
	CMenuForwarder *as = new CMenuForwarder(LOCALE_AUDIOMENU_VOLUMEBAR_AUDIOSTEPS, true, g_settings.audio_step, audio_step );
	osdDiverseSettings.addItem(as);

	osdDiverseSettings.exec(NULL, "");
	osdDiverseSettings.hide();
}

// skin
void CSkinManager::showMenu()
{
	dprintf(DEBUG_NORMAL, "CSkinManager::showMenu:\n");
	
/*
	CMenuWidget* skinMenu = new CMenuWidget("Skin Auswahl", NEUTRINO_ICON_COLORS);
	
	skinMenu->exec(NULL, "");
	
	delete skinMenu;
	skinMenu = NULL;
*/
	CFileBrowser skinBrowser;
	
	skinBrowser.Dir_Mode = true;
	skinBrowser.Dirs_Selectable= true;
	
	std::string skinDir = CONFIGDIR "/skin";
	
	if (skinBrowser.exec(skinDir.c_str()))
		//strncpy(g_settings.network_nfs_picturedir, b.getSelectedFile()->Name.c_str(), sizeof(g_settings.network_nfs_picturedir)-1);
		g_settings.preferred_skin = skinBrowser.getSelectedFile()->getFileName().c_str();
		
	CNeutrinoApp::getInstance()->loadSkin(g_settings.preferred_skin);
}

int CSkinManager::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CSkinManager::exec: actionKey:%s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if (parent)
		parent->hide();
		
	showMenu();
	
	return RETURN_EXIT;
}




