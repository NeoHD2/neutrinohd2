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
#include <gui/widget/messagebox.h>

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


#define OPTIONS_OFF0_ON1_OPTION_COUNT 2
const keyval OPTIONS_OFF0_ON1_OPTIONS[OPTIONS_OFF0_ON1_OPTION_COUNT] =
{
        { 0, _("off") },
        { 1, _("on") }
};

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

	showMenu();
	
	return ret;
}

// showmenu
void COSDSettings::showMenu(void)
{
	dprintf(DEBUG_NORMAL, "COSDSettings::showMenu:\n");
	
	int shortcutOSD = 1;
	
	CMenuWidget * osdSettings = new CMenuWidget(_("OSD settings"), NEUTRINO_ICON_COLORS );

	osdSettings->setWidgetMode(MODE_MENU);
	osdSettings->setWidgetType(WIDGET_TYPE_CLASSIC);
	osdSettings->enableShrinkMenu();
	osdSettings->setMenuPosition(MENU_POSITION_LEFT);
	osdSettings->enablePaintDate();
	
	// skin
	osdSettings->addItem( new CMenuForwarder(_("Skin select"), true, NULL, new CSkinManager(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_THEMES, _("Here you can setup the GUI Skin\n")));

	// Themes
	osdSettings->addItem( new CMenuForwarder(_("Themes"), true, NULL, new CThemes(), NULL, RC_red, NEUTRINO_ICON_BUTTON_RED, NEUTRINO_ICON_MENUITEM_THEMES, _("Here you can setup the GUI themes.\n")));

	// menu colors
	osdSettings->addItem( new CMenuForwarder(_("Menu"), true, NULL, new COSDMenuColorSettings(), NULL, RC_green, NEUTRINO_ICON_BUTTON_GREEN, NEUTRINO_ICON_MENUITEM_MENUCOLORS));

	// infobar
	osdSettings->addItem( new CMenuForwarder(_("Infobar"), true, NULL, new COSDInfoBarColorSettings(), NULL, RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW, NEUTRINO_ICON_MENUITEM_INFOBARCOLORS/*, LOCALE_HELPTEXT_INFOBARCOLORS*/));

	// language
	osdSettings->addItem(new CMenuForwarder(_("Language settings"), true, NULL, new CLanguageSettings(), NULL, RC_blue, NEUTRINO_ICON_BUTTON_BLUE, NEUTRINO_ICON_MENUITEM_LANGUAGE/*, LOCALE_HELPTEXT_LANGUAGE*/));
	
	// font
	osdSettings->addItem(new CMenuForwarder(_("Font"), true, NULL, new CFontSettings(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_FONT/*, LOCALE_HELPTEXT_FONT*/));
	
	// osd timing
	osdSettings->addItem(new CMenuForwarder(_("OSD timing"), true, NULL, new COSDTimingSettings(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_OSDTIMING/*, LOCALE_HELPTEXT_OSDTIMING*/));

	// sceensetup
	osdSettings->addItem(new CMenuForwarder(_("Screen settings"), true, NULL, new CScreenSetup(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_SCREENSETUP));
	
	// alpha setup
	//FIXME:
	//CAlphaSetup * chAlphaSetup = new CAlphaSetup(_("Alpha settings"), &g_settings.gtx_alpha);
	//osdSettings->addItem( new CMenuForwarder(_("Alpha Setup"), true, NULL, chAlphaSetup, NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_ALPHASETUP));
	
	// diverses
	osdSettings->addItem(new CMenuForwarder(_("OSD misc-settings"), true, NULL, new COSDDiverses(), NULL, CRCInput::convertDigitToKey(shortcutOSD++), NULL, NEUTRINO_ICON_MENUITEM_OSDSETTINGS));
	
	osdSettings->exec(NULL, "");
	osdSettings->hide();
	delete osdSettings;
	osdSettings = NULL;
}

// osd menucolor settings
#define COLOR_GRADIENT_TYPE_OPTION_COUNT	5
const keyval COLOR_GRADIENT_TYPE_OPTIONS[COLOR_GRADIENT_TYPE_OPTION_COUNT] =
{
	{ NOGRADIENT, _("no gradient") },
	{ DARK2LIGHT, _("Dark to Light") },
	{ LIGHT2DARK, _("Light to Dark") },
	{ DARK2LIGHT2DARK, _("Dark to Light to Dark") },
	{ LIGHT2DARK2LIGHT, _("Light to Dark to Light") }
};

#define GRADIENT_DIRECTION_TYPE_OPTION_COUNT	2
const keyval GRADIENT_DIRECTION_TYPE_OPTIONS[GRADIENT_DIRECTION_TYPE_OPTION_COUNT] =
{
	{ GRADIENT_HORIZONTAL, _("Horizontal") },
	{ GRADIENT_VERTICAL, _("Vertical") }
};

#define CORNER_TYPE_OPTION_COUNT	10
const keyval CORNER_TYPE_OPTIONS[CORNER_TYPE_OPTION_COUNT] =
{
	{ CORNER_NONE, _("CORNER NONE") },
	{ CORNER_TOP_LEFT, _("TOP LEFT") },
	{ CORNER_TOP_RIGHT, _("TOP RIGHT") },
	{ CORNER_TOP, _("TOP") },
	{ CORNER_BOTTOM_RIGHT, _("BOTTOM RIGHT") },
	{ CORNER_RIGHT, _("RIGHT") },
	{ CORNER_BOTTOM_LEFT, _("BOTTOM LEFT") },
	{ CORNER_LEFT, _("LEFT") },
	{ CORNER_BOTTOM, _("BOTTOM") },
	{ CORNER_ALL, _("CORNER ALL") },
};

#define RADIUS_TYPE_OPTION_COUNT	4
const keyval RADIUS_TYPE_OPTIONS[RADIUS_TYPE_OPTION_COUNT] =
{
	{ NO_RADIUS, _("angular") },
	{ RADIUS_SMALL, _("small") },
	{ RADIUS_MID, _("middle") },
	{ RADIUS_LARGE, _("large") },
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
		if (g_settings.use_default_skin)
			CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		else
			CNeutrinoApp::getInstance()->exec(NULL, "saveskinsettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDMenuColorSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDMenuColorSettings::showMenu:\n");
	
	CMenuWidget OSDmenuColorsSettings(_("Menu"), NEUTRINO_ICON_SETTINGS );

	OSDmenuColorsSettings.setWidgetMode(MODE_SETUP);
	OSDmenuColorsSettings.enableShrinkMenu();
	
	// intros
	OSDmenuColorsSettings.addItem(new CMenuForwarder(_("back"), true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));

	// save settings
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE) );
	OSDmenuColorsSettings.addItem(new CMenuForwarder(_("Save settings now"), true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));

	// head
	CColorChooser* chHeadcolor = new CColorChooser(_("Background"), &g_settings.menu_Head_red, &g_settings.menu_Head_green, &g_settings.menu_Head_blue, &g_settings.menu_Head_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chHeadTextcolor = new CColorChooser(_("Textcolor"), &g_settings.menu_Head_Text_red, &g_settings.menu_Head_Text_green, &g_settings.menu_Head_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// window content
	CColorChooser* chContentcolor = new CColorChooser(_("Background"), &g_settings.menu_Content_red, &g_settings.menu_Content_green, &g_settings.menu_Content_blue,&g_settings.menu_Content_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chContentTextcolor = new CColorChooser(_("Textcolor"), &g_settings.menu_Content_Text_red, &g_settings.menu_Content_Text_green, &g_settings.menu_Content_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// window content inactive
	CColorChooser* chContentInactivecolor = new CColorChooser(_("Background"), &g_settings.menu_Content_inactive_red, &g_settings.menu_Content_inactive_green, &g_settings.menu_Content_inactive_blue,&g_settings.menu_Content_inactive_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chContentInactiveTextcolor = new CColorChooser(_("Textcolor"), &g_settings.menu_Content_inactive_Text_red, &g_settings.menu_Content_inactive_Text_green, &g_settings.menu_Content_inactive_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// window content selected
	CColorChooser* chContentSelectedcolor = new CColorChooser(_("Background"), &g_settings.menu_Content_Selected_red, &g_settings.menu_Content_Selected_green, &g_settings.menu_Content_Selected_blue,&g_settings.menu_Content_Selected_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser* chContentSelectedTextcolor = new CColorChooser(_("Textcolor"), &g_settings.menu_Content_Selected_Text_red, &g_settings.menu_Content_Selected_Text_green, &g_settings.menu_Content_Selected_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// foot
	CColorChooser* chFootcolor = new CColorChooser(_("Background"), &g_settings.menu_Foot_red, &g_settings.menu_Foot_green, &g_settings.menu_Foot_blue, &g_settings.menu_Foot_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser * chFootTextcolor = new CColorChooser(_("Textcolor"), &g_settings.menu_Foot_Text_red, &g_settings.menu_Foot_Text_green, &g_settings.menu_Foot_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// foot info
	CColorChooser* chFootInfoColor = new CColorChooser(_("Background"), &g_settings.menu_FootInfo_red, &g_settings.menu_FootInfo_green, &g_settings.menu_FootInfo_blue, &g_settings.menu_FootInfo_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);

	CColorChooser * chFootInfoTextColor = new CColorChooser(_("Textcolor"), &g_settings.menu_FootInfo_Text_red, &g_settings.menu_FootInfo_Text_green, &g_settings.menu_FootInfo_Text_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	// head
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, _("Head")));
	
	// head colr
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chHeadcolor ));
	
	// head text
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chHeadTextcolor ));

	// head gradient
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Gradient"), &g_settings.Head_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// head corner
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Corner"), &g_settings.Head_corner, CORNER_TYPE_OPTIONS, CORNER_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// head radius
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Radius"), &g_settings.Head_radius, RADIUS_TYPE_OPTIONS, RADIUS_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));

	// window content
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, _("Window-Content")));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chContentcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chContentTextcolor ));

	// window content inactiv
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, _("Window-Content inactive")));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chContentInactivecolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chContentInactiveTextcolor));

	// window content selected
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, _("Window-Content selected")));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chContentSelectedcolor ));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chContentSelectedTextcolor ));
	
	// foot
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, _("Foot")));
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chFootcolor ));

	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chFootTextcolor ));

	// foot gradient
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Gradient"), &g_settings.Foot_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// foot corner
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Corner"), &g_settings.Foot_corner, CORNER_TYPE_OPTIONS, CORNER_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// foot radius
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Radius"), &g_settings.Foot_radius, RADIUS_TYPE_OPTIONS, RADIUS_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));

	// footInfo
	OSDmenuColorsSettings.addItem( new CMenuSeparator(LINE | STRING, _("Item Info")));
	
	// fontInfocolor
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chFootInfoColor ));

	// fontinfo text color
	OSDmenuColorsSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chFootInfoTextColor ));

	// footInfo gradient
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Gradient"), &g_settings.Foot_Info_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// footinfo shadow
	OSDmenuColorsSettings.addItem(new CMenuOptionChooser(_("Shadow"), &g_settings.Foot_Info_shadow, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
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
		if (g_settings.use_default_skin)
			CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		else
			CNeutrinoApp::getInstance()->exec(NULL, "saveskinsettings");
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void COSDInfoBarColorSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDInfoBarColorSettings::showMenu:\n");
	
	CMenuWidget OSDinfobarColorSettings(_("Infobar"), NEUTRINO_ICON_SETTINGS);

	OSDinfobarColorSettings.setWidgetMode(MODE_SETUP);
	OSDinfobarColorSettings.enableShrinkMenu();
	
	// intros
	OSDinfobarColorSettings.addItem(new CMenuForwarder(_("back"), true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));

	OSDinfobarColorSettings.addItem(new CMenuSeparator(LINE));
	OSDinfobarColorSettings.addItem(new CMenuForwarder(_("Save settings now"), true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));

	// bg
	CColorChooser * chInfobarcolor = new CColorChooser(_("Background"), &g_settings.infobar_red, &g_settings.infobar_green, &g_settings.infobar_blue,&g_settings.infobar_alpha, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// text
	CColorChooser * chInfobarTextcolor = new CColorChooser(_("Textcolor"), &g_settings.infobar_Text_red, &g_settings.infobar_Text_green, &g_settings.infobar_Text_blue,NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);
	
	// colored events
	CColorChooser * chColored_Events = new CColorChooser(_("Events Textcolor"), &g_settings.infobar_colored_events_red, &g_settings.infobar_colored_events_green, &g_settings.infobar_colored_events_blue, NULL, CNeutrinoApp::getInstance()->colorSetupNotifier);

	OSDinfobarColorSettings.addItem( new CMenuSeparator(LINE));

	// bg color
	OSDinfobarColorSettings.addItem( new CMenuForwarder(_("Background"), true, NULL, chInfobarcolor ));

	// text color
	OSDinfobarColorSettings.addItem( new CMenuForwarder(_("Textcolor"), true, NULL, chInfobarTextcolor ));

	// events text color
	OSDinfobarColorSettings.addItem( new CMenuForwarder(_("Events Textcolor"), true, NULL, chColored_Events ));
	
	// infobar shadow
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Shadow"), &g_settings.infobar_shadow, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// infobar buttonbar
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Buttons Bar"), &g_settings.infobar_buttonbar, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// infobar buttonline
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Buttons Line"), &g_settings.infobar_buttonline, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));

	// gradient
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Gradient"), &g_settings.infobar_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// gradient direction
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Gradient direction"), &g_settings.infobar_gradient_direction, GRADIENT_DIRECTION_TYPE_OPTIONS, GRADIENT_DIRECTION_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// corner
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Corner"), &g_settings.infobar_corner, CORNER_TYPE_OPTIONS, CORNER_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
	// radius
	OSDinfobarColorSettings.addItem(new CMenuOptionChooser(_("Radius"), &g_settings.infobar_radius, RADIUS_TYPE_OPTIONS, RADIUS_TYPE_OPTION_COUNT, true, NULL, RC_nokey, "", true ));
	
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
		
		/*
		if (MessageBox(_("Information"), _("Neutrino restart"), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			CNeutrinoApp::getInstance()->exec(NULL, "restart");
		}
		*/
		
		return ret;
	}
	else if (actionKey == "en")
	{
		strcpy(g_settings.language, "en");
		g_Locale->loadLocale(g_settings.language);
		
		/*
		if (MessageBox(_("Information"), _("Neutrino restart"), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			CNeutrinoApp::getInstance()->exec(NULL, "restart");
		}
		*/
		
		return ret;
	}
	else if (actionKey == "de")
	{
		strcpy(g_settings.language, "de");
		g_Locale->loadLocale(g_settings.language);
		
		/*
		if (MessageBox(_("Information"), _("Neutrino restart"), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			CNeutrinoApp::getInstance()->exec(NULL, "restart");
		}
		*/
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

void CLanguageSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CLanguageSettings::showMenu:\n");
	
	CMenuItem* item = NULL;
	CMenuWidget languageSettings(_("Language Setup"), NEUTRINO_ICON_LANGUAGE );

	languageSettings.setWidgetMode(MODE_SETUP);
	//languageSettings.enableShrinkMenu();
	
	// intros
	languageSettings.addItem(new CMenuForwarder(_("back"), true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	languageSettings.addItem(new CMenuSeparator(LINE));

	languageSettings.addItem(new CMenuForwarder(_("Save Settings"), true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	languageSettings.addItem(new CMenuSeparator(LINE));

	struct dirent **namelist;
	int n;

	char *path[] = {(char *) DATADIR "/neutrino/locale", (char *) CONFIGDIR "/locale"};

	for(int p = 0; p < 2; p++) 
	{
		n = scandir(path[p], &namelist, 0, alphasort);
		
		if(n > 0)
		{
			while(n--)
			{
				if(namelist[n]->d_type == DT_DIR && !strstr(namelist[n]->d_name, ".") && !strstr(namelist[n]->d_name, ".."))
				{
					
					item = new CMenuForwarder(locale2lang(namelist[n]->d_name).c_str(), true, NULL, this, namelist[n]->d_name);
					item->setIconName(namelist[n]->d_name);
					
					languageSettings.addItem(item);	
				}
				free(namelist[n]);
			}
			free(namelist);
		}
	}
	
	languageSettings.exec(NULL, "");
}

bool CLanguageSettings::changeNotify(const std::string& OptionName, void */*data*/)
{
	if (OptionName == _("Select language"))
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
		
		if (g_settings.use_default_skin)
			CNeutrinoApp::getInstance()->exec(NULL, "savesettings");
		else
			CNeutrinoApp::getInstance()->exec(NULL, "saveskinsettings");
		
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
	
	CMenuWidget fontSettings(_("Font"), NEUTRINO_ICON_COLORS, 700);
	
	fontSettings.setWidgetMode(MODE_SETUP);
	fontSettings.enableShrinkMenu();
	
	// intros
	fontSettings.addItem(new CMenuForwarder(_("back"), true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	fontSettings.addItem(new CMenuSeparator(LINE));

	fontSettings.addItem(new CMenuForwarder(_("Save settings now"), true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	fontSettings.addItem(new CMenuSeparator(LINE));
	
	// font name
	fontSettings.addItem(new CMenuForwarder(_("Font name"), true, g_settings.font_file, this, "select_font"));
	
	// font scaling
	fontSettings.addItem(new CMenuSeparator(LINE|STRING, _("Font")));
	fontSettings.addItem(new CMenuOptionNumberChooser(_("Horizontal (in %)"), &g_settings.screen_xres, true, 50, 200, NULL) );
	fontSettings.addItem(new CMenuOptionNumberChooser(_("Vertikal (in %)"), &g_settings.screen_yres, true, 50, 200, NULL) );
		
	
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
	
	CMenuWidget osdTimingSettings(_("OSD Timing"), NEUTRINO_ICON_SETTINGS);

	osdTimingSettings.setWidgetMode(MODE_SETUP);
	osdTimingSettings.enableShrinkMenu();
	
	// intros
	osdTimingSettings.addItem(new CMenuForwarder(_("back"), true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	osdTimingSettings.addItem(new CMenuSeparator(LINE));

	osdTimingSettings.addItem(new CMenuForwarder(_("Save settings now"), true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	osdTimingSettings.addItem(new CMenuSeparator(LINE));

	for (int i = 0; i < TIMING_SETTING_COUNT; i++)
	{
		CStringInput * colorSettings_timing_item = new CStringInput(timing_setting_name[i], g_settings.timing_string[i], 3, _("Time in sec. After this time the"), _("infobar will be faded out."), "0123456789 ", &timingsettingsnotifier);

		osdTimingSettings.addItem(new CMenuForwarder(timing_setting_name[i], true, g_settings.timing_string[i], colorSettings_timing_item));
	}

	osdTimingSettings.addItem(new CMenuSeparator(LINE));
	osdTimingSettings.addItem(new CMenuForwarder(_("Default"), true, NULL, this, "osd.def", RC_green, NEUTRINO_ICON_BUTTON_GREEN));
	
	osdTimingSettings.exec(NULL, "");
	osdTimingSettings.hide();
}

// timing settings notifier
bool CTimingSettingsNotifier::changeNotify(const std::string& OptionName, void *)
{
	dprintf(DEBUG_NORMAL, "CTimingSettingsNotifier::changeNotify:\n");
		
	for (int i = 0; i < TIMING_SETTING_COUNT; i++)
	{
		if (OptionName == timing_setting_name[i])
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

			dprintf(DEBUG_NORMAL, "COSDMiscSettings::exec: new icons dir %s\n", g_settings.icons_dir.c_str());

			CFrameBuffer::getInstance()->setIconBasePath(g_settings.icons_dir);
			//CNeutrinoApp::getInstance()->saveSetup(NEUTRINO_SETTINGS_FILE);
		}
		
		getString() = g_settings.icons_dir;
		
		return ret;
	}
	else if(actionKey == "select_buttons_dir")
	{
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.buttons_dir.c_str())) 
		{
			g_settings.buttons_dir = b.getSelectedFile()->Name + "/";

			dprintf(DEBUG_NORMAL, "COSDMiscSettings::exec: new buttons dir %s\n", g_settings.buttons_dir.c_str());

			CFrameBuffer::getInstance()->setButtonBasePath(g_settings.buttons_dir);
			//CNeutrinoApp::getInstance()->saveSetup(NEUTRINO_SETTINGS_FILE);
		}
		
		getString() = g_settings.buttons_dir;
		
		return ret;
	}
	else if(actionKey == "select_hints_dir")
	{
		CFileBrowser b;
		b.Dir_Mode = true;
		
		if (b.exec(g_settings.hints_dir.c_str())) 
		{
			g_settings.hints_dir = b.getSelectedFile()->Name + "/";

			dprintf(DEBUG_NORMAL, "COSDMiscSettings::exec: new hints dir %s\n", g_settings.hints_dir.c_str());

			CFrameBuffer::getInstance()->setHintBasePath(g_settings.hints_dir);
			//CNeutrinoApp::getInstance()->saveSetup(NEUTRINO_SETTINGS_FILE);
		}
		
		getString() = g_settings.hints_dir;
		
		return ret;
	}
	
	showMenu();
	
	return ret;
}

// progressbar color
#define PROGRESSBAR_COLOR_OPTION_COUNT 2
const keyval PROGRESSBAR_COLOR_OPTIONS[PROGRESSBAR_COLOR_OPTION_COUNT] =
{
	{ 0, _("monochrom") },
	{ 1, _("colored") }
};

// volumebar position
#define VOLUMEBAR_DISP_POS_OPTIONS_COUNT 6
const keyval  VOLUMEBAR_DISP_POS_OPTIONS[VOLUMEBAR_DISP_POS_OPTIONS_COUNT]=
{
	{ 0 , _("top right") },
	{ 1 , _("top left") },
	{ 2 , _("bottom left") },
	{ 3 , _("bottom right") },
	{ 4 , _("default center") },
	{ 5 , _("higher center") }
};

#define INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT 4
const keyval  INFOBAR_SUBCHAN_DISP_POS_OPTIONS[INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT]=
{
	{ 0 , _("top right") },
	{ 1 , _("top left") },
	{ 2 , _("bottom left") },
	{ 3 , _("bottom right") }
};

void COSDDiverses::showMenu()
{
	dprintf(DEBUG_NORMAL, "COSDTimingSettings::showMenu:\n");
	
	CMenuWidget osdDiverseSettings(_("OSD Miscsettings"), NEUTRINO_ICON_SETTINGS, 800, 600);

	osdDiverseSettings.setWidgetMode(MODE_SETUP);
	osdDiverseSettings.enableShrinkMenu();
	
	// intros
	osdDiverseSettings.addItem(new CMenuForwarder(_("back"), true, NULL, NULL, NULL, RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	osdDiverseSettings.addItem(new CMenuSeparator(LINE));

	osdDiverseSettings.addItem(new CMenuForwarder(_("Save settings now"), true, NULL, this, "savesettings", RC_red, NEUTRINO_ICON_BUTTON_RED));
	osdDiverseSettings.addItem(new CMenuSeparator(LINE));

	// progressbar color
	osdDiverseSettings.addItem(new CMenuOptionChooser(_("Progressbar Color"), &g_settings.progressbar_color, PROGRESSBAR_COLOR_OPTIONS, PROGRESSBAR_COLOR_OPTION_COUNT, true));
	
	// progressbar_gradient
	osdDiverseSettings.addItem(new CMenuOptionChooser("ProgressBar Gradient", &g_settings.progressbar_gradient, COLOR_GRADIENT_TYPE_OPTIONS, COLOR_GRADIENT_TYPE_OPTION_COUNT, true));
	
	// icons dir
	osdDiverseSettings.addItem(new CMenuForwarder("Icons Dir", true, g_settings.icons_dir.c_str(), this, "select_icons_dir"));
	
	// buttons dir
	osdDiverseSettings.addItem(new CMenuForwarder("Buttons Dir", true, g_settings.buttons_dir.c_str(), this, "select_buttons_dir"));
	
	// hints dir
	osdDiverseSettings.addItem(new CMenuForwarder("Hints Dir", true, g_settings.hints_dir.c_str(), this, "select_hints_dir"));
	
	// logos dir
	osdDiverseSettings.addItem( new CMenuForwarder(_("logos Dir"), true, g_settings.logos_dir.c_str(), this, "logos_dir" ) );
	
	// epgplus logos
	osdDiverseSettings.addItem(new CMenuOptionChooser(_("Channel Logo(EPGPlus)"), &g_settings.epgplus_show_logo, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// infobar channelname
	osdDiverseSettings.addItem(new CMenuOptionChooser(_("Channel name"), &g_settings.show_channelname, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true ));
	
	// sig/snr
	osdDiverseSettings.addItem(new CMenuOptionChooser(_("Satellite display on infobar"), &g_settings.infobar_sat_display, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true));
	
	// subchan pos
	osdDiverseSettings.addItem(new CMenuOptionChooser(_("Subchannel display"), &g_settings.infobar_subchan_disp_pos, INFOBAR_SUBCHAN_DISP_POS_OPTIONS, INFOBAR_SUBCHAN_DISP_POS_OPTIONS_COUNT, true, NULL, RC_nokey, "", true));
	
	// volumebar position
	osdDiverseSettings.addItem(new CMenuOptionChooser(_("Volumebar"), &g_settings.volume_pos, VOLUMEBAR_DISP_POS_OPTIONS, VOLUMEBAR_DISP_POS_OPTIONS_COUNT, true, NULL, RC_nokey, "", true ));

	// volume bar steps
	CStringInput * audio_step = new CStringInput(_("Volume Step Size"), g_settings.audio_step, 2, NULL, NULL, "0123456789 " );
	CMenuForwarder *as = new CMenuForwarder(_("Volume Step Size"), true, g_settings.audio_step, audio_step );
	osdDiverseSettings.addItem(as);
	
	// menu_shadow
	osdDiverseSettings.addItem(new CMenuOptionChooser("Menu Shadow", &g_settings.menu_shadow, OPTIONS_OFF0_ON1_OPTIONS, OPTIONS_OFF0_ON1_OPTION_COUNT, true));

	osdDiverseSettings.exec(NULL, "");
	osdDiverseSettings.hide();
}

// skin
void CSkinManager::showMenu()
{
	dprintf(DEBUG_NORMAL, "CSkinManager::showMenu:\n");
	
	CMenuItem* item = NULL;
	CMenuWidget* skinMenu = new CMenuWidget(_("Skin Select"), NEUTRINO_ICON_COLORS, 800, 600);
	skinMenu->setWidgetMode(MODE_LISTBOX);
	skinMenu->setWidgetType(WIDGET_TYPE_CLASSIC);
	skinMenu->enablePaintDate();
	
	// default
	item = new ClistBoxItem(_("neutrino (default)"), true, NULL, this, "neutrino_default", RC_nokey, NULL, DATADIR "/neutrino/icons/prev.jpg");
	item->setHint(_("Here you can select a skin from the following list."));
	
	skinMenu->addItem(item);
	
	std::string skinPath = CONFIGDIR "/skins";
	
	struct dirent **namelist;
	int i = 0;

	i = scandir(skinPath.c_str(), &namelist, 0, 0);

	if (i > 0)
	{
		while(i--)
		{
			if(namelist[i]->d_type == DT_DIR && !strstr(namelist[i]->d_name, ".") && !strstr(namelist[i]->d_name, ".."))
			{	
				item = new ClistBoxItem(namelist[i]->d_name);
				
				item->setActionKey(this, namelist[i]->d_name);
				
				std::string hint = skinPath;
				hint += "/";
				hint += namelist[i]->d_name;
				hint += "/prev.png";
				item->setItemIcon(hint.c_str());
				item->setHint(_("Here you can select a skin from the following list.")); // FIXME: localize
				item->set2lines();
				
				skinMenu->addItem(item);	
			}
			free(namelist[i]);
		}
		free(namelist);
	}
	
	skinMenu->exec(NULL, "");
	
	delete skinMenu;
	skinMenu = NULL;
}

int CSkinManager::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CSkinManager::exec: actionKey:%s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if (parent)
		parent->hide();
		
		
	if (actionKey == "neutrino_default")
	{
		if (MessageBox(_("Skin Select"), _("Neutrino restart"), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			g_settings.use_default_skin = true;
			CNeutrinoApp::getInstance()->unloadSkin();
			CNeutrinoApp::getInstance()->exec(NULL, "restart");
		}
	}
	else if (!actionKey.empty())
	{
		if (MessageBox(_("Skin Select"), _("Neutrino restart"), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			g_settings.use_default_skin = false;
			CNeutrinoApp::getInstance()->unloadSkin();
			g_settings.preferred_skin = actionKey;
			CNeutrinoApp::getInstance()->exec(NULL, "restart");
		}
	}
		
	showMenu();
	
	return ret;
}

// skinSettings
void CSkinSettings::showMenu()
{
	dprintf(DEBUG_NORMAL, "CSkinSettings::showMenu:\n");
	
	CMenuItem* item = NULL;
	CMenuWidget* skinSettings = new CMenuWidget(_("Skin Style selection"), NEUTRINO_ICON_COLORS, 800, 600);// FIXME: localize
	skinSettings->setWidgetMode(MODE_MENU);
	//skinSettings->setWidgetType(WIDGET_TYPE_CLASSIC);
	skinSettings->enablePaintDate();
	
	// load config files
	std::string skinPath = CONFIGDIR "/skins/";
	skinPath += g_settings.preferred_skin.c_str();
	
	struct dirent **namelist;
	int i = 0;

	i = scandir(skinPath.c_str(), &namelist, 0, 0);

	if (i > 0)
	{
		while(i--)
		{
			if( (strcmp(namelist[i]->d_name, ".") != 0) && (strcmp(namelist[i]->d_name, "..") != 0) )
			{
				std::string filename = namelist[i]->d_name;
				
				std::string extension = getFileExt(filename);
				
				// file to skip
				std::string skipFile = g_settings.preferred_skin;
				skipFile += ".config";
				
				if ( strcasecmp("config", extension.c_str()) == 0)
				{
					if (!filename.empty() && filename != skipFile.c_str())
					{
						item = new ClistBoxItem(removeExtension(filename).c_str());
				
						item->setActionKey(this, namelist[i]->d_name);
						item->setHint(_("choose Skin Style."));
						
						skinSettings->addItem(item);
					}
				}
			}
			free(namelist[i]);
		}
		free(namelist);
	}
	
	
	skinSettings->exec(NULL, "");
	
	delete skinSettings;
	skinSettings = NULL;
}

int CSkinSettings::exec(CMenuTarget* parent, const std::string& actionKey)
{
	dprintf(DEBUG_NORMAL, "CSkinSettings::exec: actionKey:%s\n", actionKey.c_str());
	
	int ret = RETURN_REPAINT;
	
	if (parent)
		parent->hide();
		
	if (!actionKey.empty())
	{
		// FIXME: localize
		if (MessageBox(_("Skin Style Selection"), _("Skin Style selection"), mbrNo, mbYes | mbNo, NULL, 600, 30, true) == mbrYes) 
		{
			//
			std::string skinConfigFile = CONFIGDIR "/skins/";
			skinConfigFile += g_settings.preferred_skin.c_str();
			skinConfigFile += "/";
			skinConfigFile += actionKey.c_str();
			
			CNeutrinoApp::getInstance()->readSkinConfig(skinConfigFile.c_str());
			
			//
			std::string skinConfig = CONFIGDIR "/skins/";
			skinConfig += g_settings.preferred_skin.c_str();
			skinConfig += "/";
			skinConfig += g_settings.preferred_skin.c_str();
			skinConfig += ".config";
			
			CNeutrinoApp::getInstance()->saveSkinConfig(skinConfig.c_str());
		}
	}
		
	showMenu();
	
	return ret;
}

