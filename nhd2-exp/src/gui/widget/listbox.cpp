/*
	$Id: listbox.cpp 2018.08.19 mohousch Exp $


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

#include <unistd.h> //acces
#include <cctype>

#include <global.h>
#include <neutrino.h>

#include <gui/widget/listbox.h>
#include <gui/widget/icons.h>
#include <gui/widget/textbox.h>
#include <gui/widget/stringinput.h> // locked menu

#include <driver/color.h>

#include <driver/fontrenderer.h>
#include <driver/rcinput.h>

#include <system/debug.h>


extern CPlugins * g_PluginList;    // defined in neutrino.cpp

// CMenuItem
CMenuItem::CMenuItem()
{
	x = -1;
	directKey = RC_nokey;
	iconName = "";
	can_arrow = false;
	itemIcon = "";
	itemName = "";
	option = "";
	optionInfo = "";
	itemHint = "";
	info1 = "";
	option_info1 = "";
	info2 = "";
	option_info2 = "";

	icon1 = "";
	icon2 = "";

	number = 0;
	runningPercent = -1;

	nameFont = g_Font[SNeutrinoSettings::FONT_TYPE_MENU];
	optionFont = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER];
			
	//
	nLinesItem = false;
	widgetType = WIDGET_TYPE_STANDARD;
	widgetMode = MODE_LISTBOX;
	isPlugin = false;

	active = true;
	marked = false;

	jumpTarget = NULL;
	actionKey = "";
	
	paintFrame = true;
	itemShadow = false;
	itemGradient = NOGRADIENT;
	
	//
	parent = NULL;
	
	//
	background = NULL;
}

void CMenuItem::init(const int X, const int Y, const int DX, const int OFFX)
{
	x    = X;
	y    = Y;
	dx   = DX;
	offx = OFFX;
}

void CMenuItem::setActive(const bool Active)
{
	active = Active;
	
	if (x != -1)
		paint();
}

void CMenuItem::setMarked(const bool Marked)
{
	marked = Marked;
	
	if (x != -1)
		paint();
}

// CMenuOptionChooser
CMenuOptionChooser::CMenuOptionChooser(const neutrino_locale_t OptionName, int *const OptionValue, const struct keyval* const Options, const unsigned Number_Of_Options, const bool Active, CChangeObserver* const Observ, const neutrino_msg_t DirectKey, const std::string& IconName, bool Pulldown)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;
	
	optionNameString = g_Locale->getText(OptionName);
	optionName = OptionName;
	
	options = Options;
	active = Active;
	optionValue = OptionValue;
	number_of_options = Number_Of_Options;
	observ = Observ;
	directKey = DirectKey;
	iconName = IconName;
	can_arrow = true;
	
	pulldown = Pulldown;

	//if (itemHint.empty())
	//	itemHint = optionNameString;

	itemType = ITEM_TYPE_OPTION_CHOOSER;
}

CMenuOptionChooser::CMenuOptionChooser(const char * const OptionName, int* const OptionValue, const struct keyval *const Options, const unsigned Number_Of_Options, const bool Active, CChangeObserver* const Observ, const neutrino_msg_t DirectKey, const std::string & IconName, bool Pulldown)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;

	optionNameString = OptionName;
	optionName = NONEXISTANT_LOCALE;
	
	options = Options;
	active = Active;
	optionValue = OptionValue;
	number_of_options = Number_Of_Options;
	observ = Observ;
	directKey = DirectKey;
	iconName = IconName;
	can_arrow = true;
	pulldown = Pulldown;

	//if (itemHint.empty())
	//	itemHint = optionNameString;

	itemType = ITEM_TYPE_OPTION_CHOOSER;
}

void CMenuOptionChooser::setOptionValue(const int newvalue)
{
	*optionValue = newvalue;
}

int CMenuOptionChooser::getOptionValue(void) const
{
	return *optionValue;
}

int CMenuOptionChooser::exec(CMenuTarget *parent)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionChooser::exec:\n");

	bool wantsRepaint = false;
	int ret = RETURN_REPAINT;
	
	if (parent)
		parent->hide();

	// pulldown
	if( (!parent || msg == RC_ok) && pulldown ) 
	{
		int select = -1;

		CMenuWidget *menu = new CMenuWidget(optionNameString.c_str());

		menu->setWidgetMode(MODE_SETUP);
		menu->enableShrinkMenu();
		menu->enableSaveScreen();
		
		//if(parent)
		//	menu->move(20, 0);

		for(unsigned int count = 0; count < number_of_options; count++) 
		{
			bool selected = false;
			const char *l_option;
			
			if (options[count].key == (*optionValue))
				selected = true;

			if(options[count].valname != 0)
				l_option = options[count].valname;
			else
				l_option = g_Locale->getText(options[count].value);
			
			menu->addItem(new CMenuForwarder(l_option), selected);
		}
		
		menu->exec(NULL, "");
		ret = RETURN_REPAINT;

		select = menu->getSelected();
		
		if(select >= 0) 
			*optionValue = options[select].key;
		
		delete menu;
	} 
	else 
	{
		for(unsigned int count = 0; count < number_of_options; count++) 
		{
			if (options[count].key == (*optionValue)) 
			{
				if( msg == RC_left ) 
				{
					if(count > 0)
						*optionValue = options[(count-1) % number_of_options].key;
					else
						*optionValue = options[number_of_options-1].key;
				} 
				else
					*optionValue = options[(count+1) % number_of_options].key;
				
				wantsRepaint = true;
				break;
			}
		}
	}
	
	if(observ)
		wantsRepaint = observ->changeNotify(optionName, optionValue);

	if ( wantsRepaint )
		ret = RETURN_REPAINT;

	return ret;
}

int CMenuOptionChooser::paint(bool selected, bool AfterPulldown)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionChooser::paint\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = COL_MENUCONTENT_PLUS_0;

	if (selected)
	{
		color = COL_MENUCONTENTSELECTED;
		
		//FIXME:
		if (parent)
		{
			bgcolor = parent->inFocus? COL_MENUCONTENTSELECTED_PLUS_0: COL_MENUCONTENTINACTIVE_PLUS_0;
		}
		else
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
	}
	else if (!active)
	{
		color = COL_MENUCONTENTINACTIVE;
		//bgcolor = COL_MENUCONTENTINACTIVE_PLUS_0;
	}
	
	// paint item
	frameBuffer->paintBoxRel(x, y, dx, height, bgcolor); //FIXME

	neutrino_locale_t option = NONEXISTANT_LOCALE;
	const char * l_option = NULL;

	for(unsigned int count = 0 ; count < number_of_options; count++) 
	{
		if (options[count].key == *optionValue) 
		{
			option = options[count].value;
			if(options[count].valname != 0)
				l_option = options[count].valname;
			else
				l_option = g_Locale->getText(option);
			break;
		}
	}

	if(l_option == NULL) 
	{
		*optionValue = options[0].key;
		option = options[0].value;
		if(options[0].valname != 0)
			l_option = options[0].valname;
		else
			l_option = g_Locale->getText(option);
	}

	// paint icon (left)
	int icon_w = 0;
	int icon_h = 0;

	// icons 
	//frameBuffer->getIconSize(NEUTRINO_ICON_BUTTON_RED, &icon_w, &icon_h);
		
	if (!(iconName.empty()))
	{
		frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
		
		frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + ((height - icon_h)/2) );
	}
	else if (CRCInput::isNumeric(directKey))
	{
		// define icon name depends of numeric value
		char i_name[6]; // X +'\0'
		snprintf(i_name, 6, "%d", CRCInput::getNumericValue(directKey));
		i_name[5] = '\0'; // even if snprintf truncated the string, ensure termination
		iconName = i_name;
		
		if (!iconName.empty())
		{
			frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
			
			frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y+ ((height - icon_h)/2) );
		}
		else
			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(x + BORDER_LEFT, y+ height, height, CRCInput::getKeyName(directKey), color, height);
        }

	int stringwidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_option, true); // UTF-8
	int stringstartposName = x + BORDER_LEFT + icon_w + ICON_OFFSET;
	int stringstartposOption = x + dx - (stringwidth + BORDER_RIGHT); //

	// locale
	const char * l_name = optionNameString.c_str();
	
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposName, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - (stringstartposName - x), l_name, color, 0, true); // UTF-8
	
	// option
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposOption, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_LEFT - (stringstartposOption - x), l_option, color, 0, true); // UTF-8

	// vfd
	if (selected && !AfterPulldown)
	{ 
		char str[256];
		snprintf(str, 255, "%s %s", l_name, l_option);

		CVFD::getInstance()->showMenuText(0, str, -1, true);
	}

	return y + height;
}

//CMenuOptionNumberChooser
CMenuOptionNumberChooser::CMenuOptionNumberChooser(const neutrino_locale_t Name, int * const OptionValue, const bool Active, const int min_value, const int max_value, CChangeObserver * const Observ, const int print_offset, const int special_value, const neutrino_locale_t special_value_name, const char * non_localized_name)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;

	nameString  = g_Locale->getText(Name);
	name = Name;
	
	active = Active;
	optionValue = OptionValue;

	lower_bound = min_value;
	upper_bound = max_value;

	display_offset = print_offset;

	localized_value = special_value;
	localized_value_name = special_value_name;

	optionString = non_localized_name;
	can_arrow = true;
	observ = Observ;

	//if (itemHint.empty())
	//	itemHint = nameString;

	itemType = ITEM_TYPE_OPTION_NUMBER_CHOOSER;
}

CMenuOptionNumberChooser::CMenuOptionNumberChooser(const char * const Name, int * const OptionValue, const bool Active, const int min_value, const int max_value, CChangeObserver * const Observ, const int print_offset, const int special_value, const neutrino_locale_t special_value_name, const char * non_localized_name)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;

	nameString  = Name;
	name = NONEXISTANT_LOCALE;
	
	active = Active;
	optionValue = OptionValue;

	lower_bound = min_value;
	upper_bound = max_value;

	display_offset = print_offset;

	localized_value = special_value;
	localized_value_name = special_value_name;

	optionString = non_localized_name;
	can_arrow = true;
	observ = Observ;

	//if (itemHint.empty())
	//	itemHint = nameString;

	itemType = ITEM_TYPE_OPTION_NUMBER_CHOOSER;
}

int CMenuOptionNumberChooser::exec(CMenuTarget*)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionNumberChooser::exec:\n");

	if( msg == RC_left ) 
	{
		if (((*optionValue) > upper_bound) || ((*optionValue) <= lower_bound))
			*optionValue = upper_bound;
		else
			(*optionValue)--;
	} 
	else 
	{
		if (((*optionValue) >= upper_bound) || ((*optionValue) < lower_bound))
			*optionValue = lower_bound;
		else
			(*optionValue)++;
	}
	
	paint(true);
	
	if(observ)
		observ->changeNotify(optionName, optionValue);

	return RETURN_REPAINT;
}

int CMenuOptionNumberChooser::paint(bool selected, bool /*AfterPulldown*/)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionNumberChooser::paint\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = COL_MENUCONTENT_PLUS_0;

	if (selected)
	{
		color = COL_MENUCONTENTSELECTED;
		
		//FIXME:
		if (parent)
		{
			bgcolor = parent->inFocus? COL_MENUCONTENTSELECTED_PLUS_0: COL_MENUCONTENTINACTIVE_PLUS_0;
		}
		else
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
	}
	else if (!active)
	{
		color = COL_MENUCONTENTINACTIVE;
		//bgcolor = COL_MENUCONTENTINACTIVE_PLUS_0;
	}
	
	// paint item
	frameBuffer->paintBoxRel(x, y, dx, height, bgcolor); //FIXME

	// option
	const char * l_option;
	char option_value[11];

	if ((localized_value_name == NONEXISTANT_LOCALE) || ((*optionValue) != localized_value))
	{
		sprintf(option_value, "%d", ((*optionValue) + display_offset));
		l_option = option_value;
	}
	else
		l_option = g_Locale->getText(localized_value_name);

	int stringwidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_option, true); // UTF-8
	// icons 
	int icon_w = 0;
	int icon_h = 0;

	int stringstartposName = x + BORDER_LEFT + icon_w + ICON_OFFSET;
	int stringstartposOption = x + dx - stringwidth - BORDER_RIGHT; //

	const char * l_name;
	
	if(name == NONEXISTANT_LOCALE)
		l_name = nameString.c_str();
	else
        	l_name = g_Locale->getText(name);
	
	l_name = (optionString != NULL) ? optionString : l_name;

	// locale
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposName, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - (stringstartposName - x), l_name, color, 0, true); // UTF-8
	
	// option value
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposOption, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_LEFT - (stringstartposOption - x), l_option, color, 0, true); // UTF-8
	
	// vfd
	if(selected)
	{ 
		char str[256];
		snprintf(str, 255, "%s %s", l_name, option_value);

		CVFD::getInstance()->showMenuText(0, str, -1, true); 
	}

	return y + height;
}

// CMenuOptionStringChooser
CMenuOptionStringChooser::CMenuOptionStringChooser(const neutrino_locale_t Name, char * OptionValue, bool Active, CChangeObserver* Observ, const neutrino_msg_t DirectKey, const std::string & IconName, bool Pulldown)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;

	nameString = g_Locale->getText(Name);
	name = Name;
	active = Active;
	optionValue = OptionValue;
	observ = Observ;

	directKey = DirectKey;
	iconName = IconName;
	can_arrow = true;
	
	pulldown = Pulldown;

	//if (itemHint.empty())
	//	itemHint = nameString;

	itemType = ITEM_TYPE_OPTION_STRING_CHOOSER;
}

CMenuOptionStringChooser::CMenuOptionStringChooser(const char * const Name, char * OptionValue, bool Active, CChangeObserver* Observ, const neutrino_msg_t DirectKey, const std::string & IconName, bool Pulldown)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;

	nameString = Name;
	name = NONEXISTANT_LOCALE;
	active = Active;
	optionValue = OptionValue;
	observ = Observ;

	directKey = DirectKey;
	iconName = IconName;
	can_arrow = true;
	
	pulldown = Pulldown;

	//if (itemHint.empty())
	//	itemHint = nameString;

	itemType = ITEM_TYPE_OPTION_STRING_CHOOSER;
}

CMenuOptionStringChooser::~CMenuOptionStringChooser()
{
	options.clear();
}

void CMenuOptionStringChooser::addOption(const char * const value)
{
	options.push_back(std::string(value));
}

int CMenuOptionStringChooser::exec(CMenuTarget *parent)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionStringChooser::exec:\n");

	bool wantsRepaint = false;
	int ret = RETURN_REPAINT;

	if (parent)
		parent->hide();

	// pulldown
	if( (!parent || msg == RC_ok) && pulldown ) 
	{
		int select = -1;
		
		CMenuWidget * menu = new CMenuWidget(nameString.c_str());
		
		//if(parent) 
		//	menu->move(20, 0);
		
		menu->setWidgetMode(MODE_SETUP);
		menu->enableSaveScreen();
		
		for(unsigned int count = 0; count < options.size(); count++) 
		{
			bool selected = false;
			if (strcmp(options[count].c_str(), optionValue) == 0)
				selected = true;

			menu->addItem(new CMenuForwarder(options[count].c_str()), selected);
		}
		menu->exec(NULL, "");
		ret = RETURN_REPAINT;

		select = menu->getSelected();
		
		if(select >= 0)
			strcpy(optionValue, options[select].c_str());
		delete menu;
	} 
	else 
	{
		//select next value
		for(unsigned int count = 0; count < options.size(); count++) 
		{
			if (strcmp(options[count].c_str(), optionValue) == 0) 
			{
				if( msg == RC_left ) 
				{
					if(count > 0)
						strcpy(optionValue, options[(count - 1) % options.size()].c_str());
					else
						strcpy(optionValue, options[options.size() - 1].c_str());
				} 
				else
					strcpy(optionValue, options[(count + 1) % options.size()].c_str());
				
				wantsRepaint = true;
				break;
			}
		}
	}

	if(parent)
		paint(true, true);
	
	if(observ) 
		wantsRepaint = observ->changeNotify(name, optionValue);
	
	if (wantsRepaint)
		ret = RETURN_REPAINT;

	return ret;
}

int CMenuOptionStringChooser::paint( bool selected, bool afterPulldown)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionStringChooser::paint\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = COL_MENUCONTENT_PLUS_0;
	
	if (selected) 
	{
		color = COL_MENUCONTENTSELECTED;
		
		//FIXME:
		if (parent)
		{
			bgcolor = parent->inFocus? COL_MENUCONTENTSELECTED_PLUS_0: COL_MENUCONTENTINACTIVE_PLUS_0;
		}
		else
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
	}
	else if (!active) 
	{
		color = COL_MENUCONTENTINACTIVE;
		//bgcolor = COL_MENUCONTENTINACTIVE_PLUS_0;
	}
	
	// paint item
	frameBuffer->paintBoxRel(x, y, dx, height, bgcolor); //FIXME

	// paint icon
	int icon_w = 0;
	int icon_h = 0;

	// icons 
	//frameBuffer->getIconSize(NEUTRINO_ICON_BUTTON_RED, &icon_w, &icon_h);
		
	if (!(iconName.empty()))
	{
		frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
		
		frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + ((height - icon_h)/2) );	
	}
	else if (CRCInput::isNumeric(directKey))
	{
		// define icon name depends of numeric value
		char i_name[6]; // X +'\0'
		snprintf(i_name, 6, "%d", CRCInput::getNumericValue(directKey));
		i_name[5] = '\0'; // even if snprintf truncated the string, ensure termination
		iconName = i_name;
		
		if (!iconName.empty())
		{
			frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
			
			frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + ((height - icon_h)/2) );
		}
		else
			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(x + BORDER_LEFT, y + height, height, CRCInput::getKeyName(directKey), color, height);
        }
        
        // locale text
	const char * l_name;
	
	if(name == NONEXISTANT_LOCALE)
		l_name = nameString.c_str();
	else
        	l_name = g_Locale->getText(name);
	
	int stringstartposName = x + BORDER_LEFT + icon_w + ICON_OFFSET;
	
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposName, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - (stringstartposName - x),  l_name, color, 0, true); // UTF-8
	
	// option value
	int stringwidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(optionValue, true);
	int stringstartposOption = std::max(stringstartposName + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_name, true) + ICON_OFFSET, x + dx - stringwidth - BORDER_RIGHT); //
	
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposOption, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - (stringstartposOption - x),  optionValue, color, 0, true);
	
	if (selected && !afterPulldown)
	{
		char str[256];
		snprintf(str, 255, "%s %s", l_name, optionValue);

		CVFD::getInstance()->showMenuText(0, str, -1, true);
	}

	return y + height;
}

// CMenuOptionLanguageChooser
CMenuOptionLanguageChooser::CMenuOptionLanguageChooser(char *Name, CChangeObserver *Observ, const char * const IconName)
{
	int iconName_w = 0;
	int iconName_h = 0;
	CFrameBuffer::getInstance()->getIconSize(NEUTRINO_ICON_BUTTON_RED, &iconName_w, &iconName_h);
	height = std::max(iconName_h, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 3;
	
	optionValue = Name;
	observ = Observ;

	directKey = RC_nokey;
	iconName = IconName ? IconName : "";

	//if (itemHint.empty())
	//	itemHint = Name;

	itemType = ITEM_TYPE_OPTION_LANGUAGE_CHOOSER;
}

CMenuOptionLanguageChooser::~CMenuOptionLanguageChooser()
{
	options.clear();
}

void CMenuOptionLanguageChooser::addOption(const char * const value)
{
	options.push_back(std::string(value));
}

int CMenuOptionLanguageChooser::exec(CMenuTarget*)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionLanguageChooser::exec:\n");
	
	bool wantsRepaint = false;

	//select value
	for(unsigned int count = 0; count < options.size(); count++)
	{
		if (strcmp(options[count].c_str(), optionValue) == 0)
		{
			strcpy(g_settings.language, options[(count + 1) % options.size()].c_str());
			break;
		}
	}

	paint(true);
	
	if(observ)
		wantsRepaint = observ->changeNotify(LOCALE_LANGUAGESETUP_SELECT, optionValue);

	//FIXME:	
	if ( wantsRepaint )
		return RETURN_REPAINT;
	else
		return RETURN_EXIT;
}

int CMenuOptionLanguageChooser::paint( bool selected, bool /*AfterPulldown*/)
{
	dprintf(DEBUG_DEBUG, "CMenuOptionLanguageChooser::paint\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();
	
	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = COL_MENUCONTENT_PLUS_0;
	
	if(selected)
	{
		color = COL_MENUCONTENTSELECTED;
		
		// FIXME:
		if (parent)
		{
			bgcolor = parent->inFocus? COL_MENUCONTENTSELECTED_PLUS_0: COL_MENUCONTENTINACTIVE_PLUS_0;
		}
		else
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
	}
	
	// paint item
	frameBuffer->paintBoxRel(x, y, dx, height, bgcolor); //FIXME

	// paint icon
	int icon_w = 0;
	int icon_h = 0;

	// icons 
	if (!(iconName.empty()))
	{
		frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
		
		frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + ((height - icon_h)/2) );
	}

	// locale
	int stringstartposOption = x + BORDER_LEFT + icon_w + ICON_OFFSET;
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposOption, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - (stringstartposOption - x), optionValue, color, 0, true); //UTF-8

	// vfd
	if (selected)
	{
		CVFD::getInstance()->showMenuText(1, optionValue);
	}

	return y + height;
}

// CMenuSeparator
CMenuSeparator::CMenuSeparator(const int Type, const char * const Text)
{
	directKey = RC_nokey;
	iconName = "";
	type = Type;
	textString = Text;

	itemType = ITEM_TYPE_SEPARATOR;
}

int CMenuSeparator::getHeight(void) const
{
	return (textString == NULL) ? 10 : g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();	
}

int CMenuSeparator::getWidth(void) const
{
	return 0;
}

const char * CMenuSeparator::getString(void)
{
	return textString;
}

int CMenuSeparator::paint(bool /*selected*/, bool /*AfterPulldown*/)
{
	dprintf(DEBUG_DEBUG, "CMenuSeparator::paint:\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	int height = getHeight();

	if(widgetType != WIDGET_TYPE_FRAME)
	{
		frameBuffer->paintBoxRel(x, y, dx, height, COL_MENUCONTENT_PLUS_0);

		// line
		if ((type & LINE))
		{
			frameBuffer->paintHLineRel(x + BORDER_LEFT, dx - BORDER_LEFT - BORDER_RIGHT, y + (height >> 1), COL_MENUCONTENTDARK_PLUS_0 );
			frameBuffer->paintHLineRel(x + BORDER_LEFT, dx - BORDER_LEFT - BORDER_RIGHT, y + (height >> 1) + 1, COL_MENUCONTENTDARK_PLUS_0 );
		}

		// string
		if ((type & STRING))
		{
			if(textString != NULL)
			{
				int stringstartposX;

				const char * l_text = getString();
				int stringwidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_text, true); // UTF-8

				// if no alignment is specified, align centered
				if (type & ALIGN_LEFT)
					stringstartposX = x + BORDER_LEFT;
				else if (type & ALIGN_RIGHT)
					stringstartposX = x + dx - stringwidth - BORDER_RIGHT;
				else // ALIGN_CENTER
					stringstartposX = x + (dx >> 1) - (stringwidth >> 1);

				frameBuffer->paintBoxRel(stringstartposX - 5, y, stringwidth + 10, height, COL_MENUCONTENT_PLUS_0);

				g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(stringstartposX, y + height, dx - (stringstartposX - x) , l_text, COL_MENUCONTENTINACTIVE, 0, true); // UTF-8
			}
		}
	}

	return y + height;
}

bool CPINProtection::check()
{
	char cPIN[5];
	neutrino_locale_t hint = NONEXISTANT_LOCALE;
	
	do
	{
		cPIN[0] = 0;
		CPINInput * PINInput = new CPINInput(LOCALE_PINPROTECTION_HEAD, cPIN, 4, hint);
		PINInput->exec(getParent(), "");
		delete PINInput;
		hint = LOCALE_PINPROTECTION_WRONGCODE;
	} while ((strncmp(cPIN, validPIN, 4) != 0) && (cPIN[0] != 0));
	
	return ( strncmp(cPIN,validPIN, 4) == 0);
}

bool CZapProtection::check()
{
	int res;
	char cPIN[5];
	neutrino_locale_t hint2 = NONEXISTANT_LOCALE;
	
	do
	{
		cPIN[0] = 0;

		CPLPINInput* PINInput = new CPLPINInput(LOCALE_PARENTALLOCK_HEAD, cPIN, 4, hint2, fsk);

		res = PINInput->exec(getParent(), "");
		delete PINInput;

		hint2 = LOCALE_PINPROTECTION_WRONGCODE;
	} while ( (strncmp(cPIN,validPIN, 4) != 0) &&
		  (cPIN[0] != 0) &&
		  ( res == RETURN_REPAINT ) &&
		  ( fsk >= g_settings.parentallock_lockage ) );
		  
	return ( ( strncmp(cPIN, validPIN, 4) == 0 ) ||
			 ( fsk < g_settings.parentallock_lockage ) );
}

// CMenuForwarder
CMenuForwarder::CMenuForwarder(const neutrino_locale_t Text, const bool Active, const char * const Option, CMenuTarget* Target, const char * const ActionKey, neutrino_msg_t DirectKey, const char * const IconName, const char * const ItemIcon, const neutrino_locale_t Hint )
{
	option = Option? Option : "";

	textString = g_Locale->getText(Text);
	text = Text;
	
	active = Active;
	jumpTarget = Target;
	actionKey = ActionKey ? ActionKey : "";
	directKey = DirectKey;
	
	iconName = IconName ? IconName : "";
	
	itemIcon = ItemIcon ? ItemIcon : "";
	itemHint = g_Locale->getText(Hint);
	itemType = ITEM_TYPE_FORWARDER;
	itemName = g_Locale->getText(Text);

	//if (itemHint.empty())
	//	itemHint = itemName;

	optionValueString = "";
}

CMenuForwarder::CMenuForwarder(const char * const Text, const bool Active, const char * const Option, CMenuTarget* Target, const char * const ActionKey, neutrino_msg_t DirectKey, const char * const IconName, const char * const ItemIcon, const neutrino_locale_t Hint )
{
	textString = Text? Text : "";
	text = NONEXISTANT_LOCALE;

	option = Option? Option : "";
	
	active = Active;
	jumpTarget = Target;
	actionKey = ActionKey ? ActionKey : "";
	directKey = DirectKey;
	
	iconName = IconName ? IconName : "";
	
	itemIcon = ItemIcon ? ItemIcon : "";
	itemHint = g_Locale->getText(Hint);
	itemType = ITEM_TYPE_FORWARDER;
	itemName = Text? Text : "";

	//if (itemHint.empty())
	//	itemHint = itemName;

	optionValueString = "";
}

int CMenuForwarder::getHeight(void) const
{
	int iw = 0;
	int ih = 0;
	int bpp = 0;

	if( (widgetType == WIDGET_TYPE_STANDARD) || (widgetType == WIDGET_TYPE_EXTENDED))
	{
		ih = ITEM_ICON_H_MINI/2;
		
		if (!iconName.empty())
		{
			CFrameBuffer::getInstance()->getIconSize(iconName.c_str(), &iw, &ih);
		}
		
	}
	else if(widgetType == WIDGET_TYPE_CLASSIC)
	{
		ih = ITEM_ICON_H_MINI;
	}
	else if(widgetType == WIDGET_TYPE_FRAME)
	{
		ih = item_height;
	}
	
	return std::max(ih, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 6;
}

int CMenuForwarder::getWidth(void) const
{
	int tw = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(textString);

	if(!option.empty())
                tw += g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(option.c_str(), true);

	return tw;
}

int CMenuForwarder::exec(CMenuTarget *parent)
{
	dprintf(DEBUG_DEBUG, "CMenuForwarder::exec: (%s) actionKey: (%s)\n", getName(), actionKey.c_str());

	int ret = RETURN_EXIT;

	if(jumpTarget)
	{
		ret = jumpTarget->exec(parent, actionKey);

		if(ret && !option.empty()) 
		{
			optionValueString = jumpTarget->getString().c_str();
		}
	}
	else
		ret = RETURN_EXIT;

	return ret;
}

const char * CMenuForwarder::getName(void)
{
	const char * l_name;
	
	if(text == NONEXISTANT_LOCALE)
		l_name = textString.c_str();
	else
        	l_name = g_Locale->getText(text);
	
	return l_name;
}

const char * CMenuForwarder::getOption(void)
{
	if(!optionValueString.empty())
		return optionValueString.c_str();
	else if(!option.empty())
		return option.c_str();
	else
		return NULL;
}

int CMenuForwarder::paint(bool selected, bool /*AfterPulldown*/)
{
	dprintf(DEBUG_DEBUG, "CMenuForwarder::paint\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	int height = getHeight();
	const char * l_text = getName();
	const char * option_text = getOption();	

	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = marked? COL_MENUCONTENTSELECTED_PLUS_1 : COL_MENUCONTENT_PLUS_0;

	if (selected)
	{
		color = COL_MENUCONTENTSELECTED;
		
		// FIXME:
		if (parent)
		{
			bgcolor = parent->inFocus? COL_MENUCONTENTSELECTED_PLUS_0: COL_MENUCONTENT_PLUS_0;
		}
		else
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
	}
	else if (!active)
	{
		color = COL_MENUCONTENTINACTIVE;
		//bgcolor = COL_MENUCONTENTINACTIVE_PLUS_0;
	}

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		//
		frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENT_PLUS_0);

		if(!itemIcon.empty())
			frameBuffer->paintHintIcon(itemIcon, x + 4*ICON_OFFSET, y + 4*ICON_OFFSET, item_width - 8*ICON_OFFSET, item_height - 8*ICON_OFFSET);

		//
		if(selected)
		{
			frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENT_PLUS_6);

			if(!itemIcon.empty())
				frameBuffer->paintHintIcon(itemIcon, x + 2, y + 2, item_width - 4, item_height - 4);
		}

		// vfd
		if (selected)
		{
			CVFD::getInstance()->showMenuText(0, l_text, -1, true);
		}

		return 0;
	}
	else
	{
		int stringstartposX = x;	
	
		// paint item
		if (itemShadow)
		{
			if (selected)
			{
				// shadow
				frameBuffer->paintBoxRel(x, y, dx, height, COL_MENUCONTENT_PLUS_6);	
				// itemBox
				frameBuffer->paintBoxRel(x + 2, y + 2, dx - 4, height - 4, bgcolor, NO_RADIUS, CORNER_NONE, itemGradient); 
			}
			else
				frameBuffer->paintBoxRel(x, y, dx, height, bgcolor);
		}
		else
		{
			if (selected)
				frameBuffer->paintBoxRel(x, y, dx, height, bgcolor, NO_RADIUS, CORNER_NONE, itemGradient); 
			else
				frameBuffer->paintBoxRel(x, y, dx, height, bgcolor);
		}

		// iconName
		int icon_w = 0;
		int icon_h = 0;
		int bpp = 0;
	
		// icon
		if(widgetType == WIDGET_TYPE_STANDARD || widgetType == WIDGET_TYPE_EXTENDED)
		{
			if (!iconName.empty())
			{
				icon_w = ITEM_ICON_H_MINI/2;
				icon_h = ITEM_ICON_H_MINI/2;
				
				frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
				
				frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + (height - icon_h)/2, 0, true, icon_w, icon_h);
			}
			else if (CRCInput::isNumeric(directKey))
			{
				// define icon name depends of numeric value
				char i_name[6]; 							// X +'\0'
				snprintf(i_name, 6, "%d", CRCInput::getNumericValue(directKey));
				i_name[5] = '\0'; 							// even if snprintf truncated the string, ensure termination
				iconName = i_name;
		
				if (!iconName.empty())
				{
					icon_w = ITEM_ICON_H_MINI/2;
					icon_h = ITEM_ICON_H_MINI/2;
					
					frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
			
					frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + (height - icon_h)/2, 0, true, icon_w, icon_h);
				}
				else
					g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(x + BORDER_LEFT, y + height, height, CRCInput::getKeyName(directKey), color, height);
			}
		}
		else if(widgetType == WIDGET_TYPE_CLASSIC)
		{
			if (!itemIcon.empty())
			{
				icon_h = ITEM_ICON_H_MINI;
				icon_w = ITEM_ICON_W_MINI;

				frameBuffer->paintHintIcon(itemIcon.c_str(), x + BORDER_LEFT, y + ((height - icon_h)/2), icon_w, icon_h);
			}
		}
	
		//local-text
		int l_text_width = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_text, true);

		if(nLinesItem)
		{
			// local
			if(l_text != NULL)
			{
				g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(x + BORDER_LEFT + icon_w + ICON_OFFSET, y + 3 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - BORDER_LEFT - icon_w - ICON_OFFSET, l_text, color, 0, true); // UTF-8
			}

			// option
			if(option_text != NULL)
			{
				g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(x + BORDER_LEFT + icon_w + ICON_OFFSET, y + height, dx - BORDER_LEFT - BORDER_RIGHT - ICON_OFFSET - icon_w, option_text, color, 0, true);
			}
		}
		else
		{
			// local
			if(l_text != NULL)
			{
				g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(x + BORDER_LEFT + icon_w + ICON_OFFSET, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight())/2 + /*optionFont*/g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight(), dx - BORDER_RIGHT - BORDER_LEFT - ICON_OFFSET - icon_w, l_text, color, 0, true); // UTF-8
			}

			// option
			if(option_text != NULL)
			{
				int stringwidth = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getRenderWidth(option_text, true);
				int stringstartposOption = std::max(x + BORDER_LEFT + icon_w + ICON_OFFSET + g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_text, true) + ICON_OFFSET, x + dx - (stringwidth + BORDER_RIGHT)); //

				g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(stringstartposOption, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight())/2 + /*optionFont*/g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight(), dx - BORDER_LEFT - BORDER_RIGHT - ICON_OFFSET - l_text_width - icon_w, option_text, color, 0, true);
			}
		}

		// vfd
		if (selected)
		{
			CVFD::getInstance()->showMenuText(0, l_text, -1, true);
		}

		return y + height;
	}
}

// lockedMenuForward
int CLockedMenuForwarder::exec(CMenuTarget * parent)
{
	dprintf(DEBUG_DEBUG, "CLockedMenuForwarder::exec\n");

	Parent = parent;
	
	if( (g_settings.parentallock_prompt != PARENTALLOCK_PROMPT_NEVER) || AlwaysAsk )
	{
		if (!check())
		{
			Parent = NULL;
			return RETURN_REPAINT;
		}
	}

	Parent = NULL;
	
	return CMenuForwarder::exec(parent);
}

//ClistBoxItem
ClistBoxItem::ClistBoxItem(const neutrino_locale_t Text, const bool Active, const char * const Option, CMenuTarget* Target, const char * const ActionKey, const neutrino_msg_t DirectKey, const char * const IconName, const char * const ItemIcon)
{
	text = Text;
	textString = g_Locale->getText(Text);

	option = Option? Option : "";

	active = Active;
	jumpTarget = Target;
	actionKey = ActionKey ? ActionKey : "";

	directKey = DirectKey;

	iconName = IconName ? IconName : "";
	itemIcon = ItemIcon? ItemIcon : "";
	itemName = g_Locale->getText(Text);

	//if (itemHint.empty())
	//	itemHint = itemName;

	itemType = ITEM_TYPE_LISTBOXITEM;
}

ClistBoxItem::ClistBoxItem(const char * const Text, const bool Active, const char * const Option, CMenuTarget* Target, const char * const ActionKey, const neutrino_msg_t DirectKey, const char * const IconName, const char* const ItemIcon)
{
	text = NONEXISTANT_LOCALE;
	textString = Text? Text : "";

	option = Option? Option : "";

	active = Active;
	jumpTarget = Target;
	actionKey = ActionKey ? ActionKey : "";

	directKey = DirectKey;

	iconName = IconName ? IconName : "";
	itemIcon = ItemIcon? ItemIcon : "";
	itemName = Text? Text : "";

	//if (itemHint.empty())
	//	itemHint = itemName;

	itemType = ITEM_TYPE_LISTBOXITEM;
}

int ClistBoxItem::getHeight(void) const
{
	int iw = 0;
	int ih = 0;
	int bpp = 0;

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		ih = item_height;
	}
	else if(widgetType == WIDGET_TYPE_CLASSIC)
	{
		ih = ITEM_ICON_H_MINI;
			
		if (widgetMode == MODE_LISTBOX)
		{
			ih = ITEM_ICON_H_MINI*2;
		}
	}
	else // standard|extended
	{
		ih = ITEM_ICON_H_MINI/2;
		
		if (!iconName.empty())
			CFrameBuffer::getInstance()->getIconSize(iconName.c_str(), &iw, &ih);
			
		if(nLinesItem)
		{
			ih = ITEM_ICON_H_MINI;
		}
	}

	return std::max(ih, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 6;
}

int ClistBoxItem::getWidth(void) const
{
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		return item_width;
	}
	else
	{
		int tw = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(textString); //FIXME:

		return tw;
	}
}

int ClistBoxItem::exec(CMenuTarget* parent)
{
	dprintf(DEBUG_DEBUG, "ClistBoxItem::exec: (%s) actionKey: (%s)\n", getName(), actionKey.c_str());

	int ret = RETURN_EXIT;

	if(jumpTarget)
	{
		ret = jumpTarget->exec(parent, actionKey);

		if(ret) 
		{
			optionValueString = jumpTarget->getString().c_str();
		}
	}
	else
		ret = RETURN_EXIT;

	return ret;
}

const char * ClistBoxItem::getName(void)
{
	const char * l_name;
	
	if(text == NONEXISTANT_LOCALE)
		l_name = textString.c_str();
	else
        	l_name = g_Locale->getText(text);
	
	return l_name;
}

const char * ClistBoxItem::getOption(void)
{
	if(!optionValueString.empty())
		return optionValueString.c_str();
	else if(!option.empty())
		return option.c_str();
	else
		return NULL;
}

int ClistBoxItem::paint(bool selected, bool /*AfterPulldown*/)
{
	dprintf(DEBUG_DEBUG, "ClistBoxItem::paint:\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	int height = getHeight();
	const char * l_text = getName();
	const char * option_text = getOption();	

	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = marked? COL_MENUCONTENTSELECTED_PLUS_1 : COL_MENUCONTENT_PLUS_0;

	if (selected)
	{
		// FIXME:
		if (parent)
		{
			if (parent->inFocus)
			{
				color = COL_MENUCONTENTSELECTED;
				bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
			}
			else
			{
				color = COL_MENUCONTENT;	
				bgcolor = COL_MENUCONTENT_PLUS_0;
			}
		}
		else
		{
			color = COL_MENUCONTENTSELECTED;
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
		}
	}
	else if (!active)
	{
		color = COL_MENUCONTENTINACTIVE;
		//bgcolor = COL_MENUCONTENTINACTIVE_PLUS_0;
	}

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		//
		if(selected)
		{
			if (parent)
			{
				if (parent->inFocus)
				{	
					frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENTSELECTED_PLUS_0);

					if(!itemIcon.empty())
						frameBuffer->paintHintIcon(itemIcon, x + 2, y + 2, item_width - 4, item_height - 4);
				}
				else
				{
					frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENT_PLUS_0 ); //FIXME:

					if(!itemIcon.empty())
						frameBuffer->paintHintIcon(itemIcon, x + 4*ICON_OFFSET, y + 4*ICON_OFFSET, item_width - 8*ICON_OFFSET, item_height - 8*ICON_OFFSET);
				}
			}
			else
			{
				frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENTSELECTED_PLUS_0);

				if(!itemIcon.empty())
					frameBuffer->paintHintIcon(itemIcon, x + 2, y + 2, item_width - 4, item_height - 4);
			}
		}
		else
		{
			// refresh
			frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENT_PLUS_0);

			if(!itemIcon.empty())
				frameBuffer->paintHintIcon(itemIcon, x + 4*ICON_OFFSET, y + 4*ICON_OFFSET, item_width - 8*ICON_OFFSET, item_height - 8*ICON_OFFSET);
		}

		// locale ???

		// vfd
		if (selected)
		{
			CVFD::getInstance()->showMenuText(0, l_text, -1, true);
		}

		return 0;
	}
	else // standard|classic|extended
	{
		if (itemShadow)
		{
			if (selected)
			{
				if (!paintFrame)
				{
					if (background)
					{
						delete [] background;
						background = NULL;
					}
									
					background = new fb_pixel_t[dx*height];
						
					if (background)
					{
						frameBuffer->saveScreen(x, y, dx, height, background);
					}
				}	
				
				// shadow
				frameBuffer->paintBoxRel(x, y, dx, height, COL_MENUCONTENT_PLUS_6);	
				// itemBox
				frameBuffer->paintBoxRel(x + 2, y + 2, dx - 4, height - 4, bgcolor, NO_RADIUS, CORNER_NONE, itemGradient); 
			}
			else
			{
				if (paintFrame)
					frameBuffer->paintBoxRel(x, y, dx, height, bgcolor);
				else
				{
					if (background)
					{
						frameBuffer->restoreScreen(x, y, dx, height, background);
							
						delete [] background;
						background = NULL;
					}
				}
			}
		}
		else
		{
			if (selected)
			{
				if (!paintFrame)
				{
					if (background)
					{
						delete [] background;
						background = NULL;
					}
									
					background = new fb_pixel_t[dx*height];
						
					if (background)
					{
						frameBuffer->saveScreen(x, y, dx, height, background);
					}
				}	

				frameBuffer->paintBoxRel(x, y, dx, height, bgcolor, NO_RADIUS, CORNER_NONE, itemGradient);
			}
			else
			{
				if (paintFrame) //FIXME: TEST
					frameBuffer->paintBoxRel(x, y, dx, height, bgcolor, NO_RADIUS, CORNER_NONE, NOGRADIENT);
				else
				{
					if (background)
					{
						frameBuffer->restoreScreen(x, y, dx, height, background);
							
						delete [] background;
						background = NULL;
					}
				}
			}
		}
			
	
		// icon (left)
		int icon_w = 0;
		int icon_h = 0;
		int bpp = 0;
		int icon_offset = 0;

		if(widgetType == WIDGET_TYPE_CLASSIC)
		{
			icon_h = ITEM_ICON_H_MINI;
			icon_w = ITEM_ICON_W_MINI;
			
			if (widgetMode == MODE_LISTBOX)
			{
				icon_h = ITEM_ICON_H_MINI*2;
				icon_w = ITEM_ICON_W_MINI;
			}
					
			if (!itemIcon.empty())
			{
				frameBuffer->paintHintIcon(itemIcon.c_str(), x + BORDER_LEFT, y + ((height - icon_h)/2), icon_w, icon_h);
			}
		}
		else // standard|extended
		{
			if (!iconName.empty())
			{
				icon_offset = ICON_OFFSET;
				
				//get icon size
				frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
		
				frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + (height - icon_h)/2, 0, true, icon_w, icon_h);
			}
		}

		// icon1 (right)
		int icon1_w = 0;
		int icon1_h = 0;
		int icon1_offset = 0;
	
		if (!icon1.empty())
		{
			icon1_offset = ICON_OFFSET; 
			
			//get icon size
			frameBuffer->getIconSize(icon1.c_str(), &icon1_w, &icon1_h);
		
			frameBuffer->paintIcon(icon1, x + dx - BORDER_LEFT - icon1_w, y + (height - icon1_h)/2 );
		}

		// icon2 (right)
		int icon2_w = 0;
		int icon2_h = 0;
		int icon2_offset = 0;
	
		if (!icon2.empty())
		{
			icon2_offset = ICON_OFFSET;
			
			//get icon size
			frameBuffer->getIconSize(icon2.c_str(), &icon2_w, &icon2_h);
		
			frameBuffer->paintIcon(icon2, x + dx - BORDER_LEFT - (icon1_w? icon1_w + ICON_OFFSET : 0) - icon2_w, y + (height - icon2_h)/2 );
		}

		// optionInfo (right)
		int optionInfo_width = 0;
	
		if(!optionInfo.empty())
		{
			optionInfo_width = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getRenderWidth(optionInfo.c_str());

			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(x + dx - BORDER_RIGHT - (icon1_w? icon1_w + ICON_OFFSET : 0) - (icon2_w? icon2_w + ICON_OFFSET : 0) - optionInfo_width, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight(), optionInfo_width, optionInfo.c_str(), color, 0, true); // UTF-8
		}

		// number
		int numwidth = 0;
		int num_offset = 0;
		
		if(number != 0)
		{
			num_offset = ICON_OFFSET;
			
			char tmp[10];

			sprintf((char*) tmp, "%d", number);

			numwidth = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getRenderWidth("0000");

			int numpos = x + BORDER_LEFT + numwidth - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getRenderWidth(tmp);

			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(numpos, y + (height - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight(), numwidth + 5, tmp, color, 0, true); // UTF-8
		}

		// ProgressBar
		int pBarWidth = 0;
		int pb_offset = 0;
		
		if(runningPercent > -1)
		{
			pb_offset = ICON_OFFSET;
			
			pBarWidth = 35;
			int pBarHeight = height/5;

			CProgressBar timescale;
			timescale.setPosition(x + BORDER_LEFT + numwidth + ICON_OFFSET, y + (height - pBarHeight)/2, pBarWidth, pBarHeight);
		
			timescale.reset();
			timescale.paintPCR(runningPercent);
		}
	
		// locale|option text
		int l_text_width = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_text, true);

		// local
		if(l_text_width >= dx - BORDER_LEFT - BORDER_RIGHT)
			l_text_width = dx - BORDER_LEFT - BORDER_RIGHT;

		if(nLinesItem)
		{
			// local
			if(l_text != NULL)
			{
					nameFont->RenderString(x + BORDER_LEFT + icon_w + numwidth + ICON_OFFSET + pBarWidth + ICON_OFFSET, y + 3 + nameFont->getHeight(), dx - BORDER_RIGHT - BORDER_LEFT - numwidth - pBarWidth - 2*ICON_OFFSET - icon_w - icon1_w - icon2_w - optionInfo_width - ICON_OFFSET, l_text, color, 0, true); // UTF-8
			}

			// option
			if(option_text != NULL)
			{
					optionFont->RenderString(x + BORDER_LEFT + icon_w + numwidth + ICON_OFFSET + pBarWidth + ICON_OFFSET, y + height, dx - BORDER_LEFT - BORDER_RIGHT - numwidth - pBarWidth - 2*ICON_OFFSET - icon_w - icon1_w - icon2_w - optionInfo_width - ICON_OFFSET, option_text, color, 0, true);
			}
		}
		else
		{
			// local
			if(l_text != NULL)
			{
				nameFont->RenderString(x + BORDER_LEFT + icon_w + numwidth + ICON_OFFSET + pBarWidth + ICON_OFFSET, y + 3 + nameFont->getHeight(), dx - BORDER_RIGHT - BORDER_LEFT - numwidth - pBarWidth - 2*ICON_OFFSET - icon_w - icon1_w - icon2_w - optionInfo_width - ICON_OFFSET, l_text, color, 0, true); // UTF-8
			}

			// option
			std::string Option;
			
			if(option_text != NULL)
			{
				int iw, ih;
				//get icon size
				frameBuffer->getIconSize(NEUTRINO_ICON_HD, &iw, &ih);

				Option = " - ";
				Option += option_text;

				optionFont->RenderString(x + BORDER_LEFT + numwidth + pBarWidth + ICON_OFFSET + l_text_width + ICON_OFFSET, y + (height - optionFont->getHeight())/2 + optionFont->getHeight(), dx - BORDER_LEFT - BORDER_RIGHT - numwidth - ICON_OFFSET - pBarWidth - ICON_OFFSET - l_text_width - icon_w - icon1_w - ICON_OFFSET - icon2_w - ICON_OFFSET - 2*iw, Option.c_str(), COL_COLORED_EVENTS_CHANNELLIST, 0, true);
			}
		}
		
		// vfd
		if (selected)
		{
			CVFD::getInstance()->showMenuText(0, l_text, -1, true);
		}
		
		return y + height;
	}
}

//CPluginItem
CPluginItem::CPluginItem(const char * const pluginName, const bool Active, const neutrino_msg_t DirectKey, const char* const IconName)
{
	textString = "";
	option = "";
	
	// 
	if (g_PluginList->plugin_exists(pluginName))
	{
		unsigned int count = g_PluginList->find_plugin(pluginName);

		//iconName
		itemIcon = NEUTRINO_ICON_MENUITEM_PLUGIN;

		std::string icon("");
		icon = g_PluginList->getIcon(count);

		if(!icon.empty())
		{
			itemIcon = PLUGINDIR;
			itemIcon += "/";
			itemIcon += g_PluginList->getFileName(count);
			itemIcon += "/";
			itemIcon += g_PluginList->getIcon(count);
		}

		//
		textString = g_PluginList->getName(count);

		// option
		option = g_PluginList->getDescription(count);

		// jumpTarget
		jumpTarget = CPluginsExec::getInstance();

		// actionKey
		actionKey = g_PluginList->getFileName(count);
	}
	//

	active = Active;
	
	directKey = DirectKey;
	iconName = IconName ? IconName : "";

	itemType = ITEM_TYPE_PLUGINITEM;
}

int CPluginItem::getHeight(void) const
{
	int iw = 0;
	int ih = 0;
	int bpp = 0;

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		ih = item_height;
	}
	else if(widgetType == WIDGET_TYPE_CLASSIC)
	{
		ih = ITEM_ICON_H_MINI;
			
		if (widgetMode == MODE_LISTBOX)
		{
			ih = ITEM_ICON_H_MINI*2;
		}
	}
	else // standard|extended
	{
		ih = ITEM_ICON_H_MINI/2;
		
		if (!iconName.empty())
			CFrameBuffer::getInstance()->getIconSize(iconName.c_str(), &iw, &ih);
			
		if(nLinesItem)
		{
			ih = ITEM_ICON_H_MINI;
		}
	}

	return std::max(ih, g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight()) + 6;
}

int CPluginItem::getWidth(void) const
{
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		return item_width;
	}
	else
	{
		int tw = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(textString); //FIXME:

		return tw;
	}
}

int CPluginItem::exec(CMenuTarget* parent)
{
	dprintf(DEBUG_DEBUG, "CPlugin::exec: (%s) actionKey: (%s)\n", getName(), actionKey.c_str());

	int ret = RETURN_EXIT;

	if(jumpTarget)
	{
		ret = jumpTarget->exec(parent, actionKey);

		if(ret) 
		{
			optionValueString = jumpTarget->getString().c_str();
		}
	}
	else
		ret = RETURN_EXIT;

	return ret;
}

const char * CPluginItem::getName(void)
{
	const char * l_name = textString.c_str();
	
	return l_name;
}

const char * CPluginItem::getOption(void)
{
	if(!optionValueString.empty())
		return optionValueString.c_str();
	else if(!option.empty())
		return option.c_str();
	else
		return NULL;
}

int CPluginItem::paint(bool selected, bool /*AfterPulldown*/)
{
	dprintf(DEBUG_DEBUG, "CPluginItem::paint:\n");

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	int height = getHeight();
	const char * l_text = getName();
	const char * option_text = getOption();	

	uint8_t color = COL_MENUCONTENT;
	fb_pixel_t bgcolor = marked? COL_MENUCONTENTSELECTED_PLUS_1 : COL_MENUCONTENT_PLUS_0;

	if (selected)
	{
		// FIXME:
		if (parent)
		{
			if (parent->inFocus)
			{
				color = COL_MENUCONTENTSELECTED;
				bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
			}
			else
			{
				color = COL_MENUCONTENT;	
				bgcolor = COL_MENUCONTENT_PLUS_0;
			}
		}
		else
		{
			color = COL_MENUCONTENTSELECTED;
			bgcolor = COL_MENUCONTENTSELECTED_PLUS_0;
		}
	}
	else if (!active)
	{
		color = COL_MENUCONTENTINACTIVE;
		//bgcolor = COL_MENUCONTENTINACTIVE_PLUS_0;
	}

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		//
		if(selected)
		{
			if (parent)
			{
				if (parent->inFocus)
				{	
					frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENTSELECTED_PLUS_0);

					if(!itemIcon.empty())
						frameBuffer->paintHintIcon(itemIcon, x + 2, y + 2, item_width - 4, item_height - 4);
				}
				else
				{
					frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENT_PLUS_0 ); //FIXME:

					if(!itemIcon.empty())
						frameBuffer->paintHintIcon(itemIcon, x + 4*ICON_OFFSET, y + 4*ICON_OFFSET, item_width - 8*ICON_OFFSET, item_height - 8*ICON_OFFSET);
				}
			}
			else
			{
				frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENTSELECTED_PLUS_0);

				if(!itemIcon.empty())
					frameBuffer->paintHintIcon(itemIcon, x + 2, y + 2, item_width - 4, item_height - 4);
			}
		}
		else
		{
			// refresh
			frameBuffer->paintBoxRel(x, y, item_width, item_height, COL_MENUCONTENT_PLUS_0);

			if(!itemIcon.empty())
				frameBuffer->paintHintIcon(itemIcon, x + 4*ICON_OFFSET, y + 4*ICON_OFFSET, item_width - 8*ICON_OFFSET, item_height - 8*ICON_OFFSET);
		}

		// locale ???

		// vfd
		if (selected)
		{
			CVFD::getInstance()->showMenuText(0, l_text, -1, true);
		}

		return 0;
	}
	else // standard|classic|extended
	{
		if (itemShadow)
		{
			if (selected)
			{
				if (!paintFrame)
				{
					if (background)
					{
						delete [] background;
						background = NULL;
					}
									
					background = new fb_pixel_t[dx*height];
						
					if (background)
					{
						frameBuffer->saveScreen(x, y, dx, height, background);
					}
				}	
				
				// shadow
				frameBuffer->paintBoxRel(x, y, dx, height, COL_MENUCONTENT_PLUS_6);	
				// itemBox
				frameBuffer->paintBoxRel(x + 2, y + 2, dx - 4, height - 4, bgcolor, NO_RADIUS, CORNER_NONE, itemGradient); 
			}
			else
			{
				if (paintFrame)
					frameBuffer->paintBoxRel(x, y, dx, height, bgcolor);
				else
				{
					if (background)
					{
						frameBuffer->restoreScreen(x, y, dx, height, background);
							
						delete [] background;
						background = NULL;
					}
				}
			}
		}
		else
		{
			if (selected)
			{
				if (!paintFrame)
				{
					if (background)
					{
						delete [] background;
						background = NULL;
					}
									
					background = new fb_pixel_t[dx*height];
						
					if (background)
					{
						frameBuffer->saveScreen(x, y, dx, height, background);
					}
				}	

				frameBuffer->paintBoxRel(x, y, dx, height, bgcolor, NO_RADIUS, CORNER_NONE, itemGradient);
			}
			else
			{
				if (paintFrame) //FIXME: TEST
					frameBuffer->paintBoxRel(x, y, dx, height, bgcolor, NO_RADIUS, CORNER_NONE, NOGRADIENT);
				else
				{
					if (background)
					{
						frameBuffer->restoreScreen(x, y, dx, height, background);
							
						delete [] background;
						background = NULL;
					}
				}
			}
		}
			
	
		// icon (left)
		int icon_w = 0;
		int icon_h = 0;
		int bpp = 0;
		int icon_offset = 0;

		if(widgetType == WIDGET_TYPE_CLASSIC)
		{
			icon_h = ITEM_ICON_H_MINI;
			icon_w = ITEM_ICON_W_MINI;
			
			if (widgetMode == MODE_LISTBOX)
			{
				icon_h = ITEM_ICON_H_MINI*2;
				icon_w = ITEM_ICON_W_MINI;
			}
					
			if (!itemIcon.empty())
			{
				frameBuffer->paintHintIcon(itemIcon.c_str(), x + BORDER_LEFT, y + ((height - icon_h)/2), icon_w, icon_h);
			}
		}
		else // standard|extended
		{
			if (!iconName.empty())
			{
				icon_offset = ICON_OFFSET;
				
				//get icon size
				frameBuffer->getIconSize(iconName.c_str(), &icon_w, &icon_h);
		
				frameBuffer->paintIcon(iconName, x + BORDER_LEFT, y + (height - icon_h)/2, 0, true, icon_w, icon_h);
			}
		}

		// locale|option text
		int l_text_width = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getRenderWidth(l_text, true);

		// local
		if(l_text_width >= dx - BORDER_LEFT - BORDER_RIGHT)
			l_text_width = dx - BORDER_LEFT - BORDER_RIGHT;

		if(nLinesItem)
		{
			// local
			if(l_text != NULL)
			{
					nameFont->RenderString(x + BORDER_LEFT + icon_w + ICON_OFFSET, y + 3 + nameFont->getHeight(), dx - BORDER_RIGHT - BORDER_LEFT - ICON_OFFSET - icon_w, l_text, color, 0, true); // UTF-8
			}

			// option
			if(option_text != NULL)
			{
					optionFont->RenderString(x + BORDER_LEFT + icon_w + ICON_OFFSET, y + height, dx - BORDER_LEFT - BORDER_RIGHT - ICON_OFFSET - icon_w, option_text, color, 0, true);
			}
		}
		else
		{
			// local
			if(l_text != NULL)
			{
				nameFont->RenderString(x + BORDER_LEFT + icon_w + ICON_OFFSET, y + 3 + nameFont->getHeight(), dx - BORDER_RIGHT - BORDER_LEFT - ICON_OFFSET - icon_w, l_text, color, 0, true); // UTF-8
			}

			// option
			std::string Option;
			
			if(option_text != NULL)
			{
				Option = " - ";
				Option += option_text;

				optionFont->RenderString(x + BORDER_LEFT + ICON_OFFSET + l_text_width + ICON_OFFSET, y + (height - optionFont->getHeight())/2 + optionFont->getHeight(), dx - BORDER_LEFT - BORDER_RIGHT - ICON_OFFSET - l_text_width - icon_w, Option.c_str(), COL_COLORED_EVENTS_CHANNELLIST, 0, true);
			}
		}
		
		// vfd
		if (selected)
		{
			CVFD::getInstance()->showMenuText(0, l_text, -1, true);
		}
		
		return y + height;
	}
}

//// ClistBox
ClistBox::ClistBox(const int x, const int y, const int dx, const int dy)
{
	frameBuffer = CFrameBuffer::getInstance();

	selected = -1;
	current_page = 0;
	pos = 0;

	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;
	
	// sanity check
	if(itemBox.iHeight > ((int)frameBuffer->getScreenHeight()))
		itemBox.iHeight = frameBuffer->getScreenHeight();

	// sanity check
	if(itemBox.iWidth > (int)frameBuffer->getScreenWidth())
		itemBox.iWidth = frameBuffer->getScreenWidth();
	
	full_width = itemBox.iWidth;
	full_height = itemBox.iHeight;

	wanted_height = dy;
	wanted_width = dx;
	start_x = x;
	start_y = y;

	hheight = 0;
	fheight = 0;
	footInfoHeight = 0;
	cFrameFootInfoHeight = 0;
	connectLineWidth = 0;
	footInfoMode = FOOT_INFO_MODE;
	
	listmaxshow = 0;

	hbutton_count	= 0;
	hbutton_labels.clear();
	fbutton_count	= 0;
	fbutton_labels.clear();
	fbutton_width = itemBox.iWidth;

	paintDate = false;
	paintTitle = false;
	paint_Foot = false;
	paintFootInfo = false;

	l_name = "";
	iconfile = "";
	tMode = CC_ALIGN_LEFT;

	//
	enableCenter = false;
	inFocus = true;
	shrinkMenu = false;

	//
	itemsPerX = 6;
	itemsPerY = 3;
	maxItemsPerPage = itemsPerX*itemsPerY;

	// head
	headColor = COL_MENUHEAD_PLUS_0;
	headRadius = g_settings.Head_radius;
	headCorner = g_settings.Head_corner;
	headGradient = g_settings.Head_gradient;

	// foot
	footColor = COL_MENUFOOT_PLUS_0;
	footRadius = g_settings.Foot_radius;
	footCorner = g_settings.Foot_corner;
	footGradient = g_settings.Foot_gradient;

	//
	widgetType = WIDGET_TYPE_STANDARD;
	cnt = 0;

	itemType = WIDGET_ITEM_LISTBOX;

	savescreen = false;
	background = NULL;

	textBox = NULL;

	actionKey = "";
	
	paintFrame = true;
	bgcolor = COL_MENUCONTENT_PLUS_0;
	radius = NO_RADIUS;
	corner = CORNER_NONE;
	def_color = false;
	scrolling = true;
	items_background = NULL;
	
	item_height = 0;
	item_width = 0;
	iconOffset = 0;
	items_width = 0;
	items_height = 0;
}

ClistBox::ClistBox(CBox* position)
{
	frameBuffer = CFrameBuffer::getInstance();

	selected = -1;
	current_page = 0;
	pos = 0;

	itemBox = *position;
	
	// sanity check
	if(itemBox.iHeight > ((int)frameBuffer->getScreenHeight()))
		itemBox.iHeight = frameBuffer->getScreenHeight();

	// sanity check
	if(itemBox.iWidth > (int)frameBuffer->getScreenWidth())
		itemBox.iWidth = frameBuffer->getScreenWidth();
	
	full_width = itemBox.iWidth;
	full_height = itemBox.iHeight;

	wanted_height = position->iHeight;
	wanted_width = position->iWidth;
	start_x = position->iX;
	start_y = position->iY;

	hheight = 0;
	fheight = 0;
	footInfoHeight = 0;
	cFrameFootInfoHeight = 0;
	connectLineWidth = 0;
	footInfoMode = FOOT_INFO_MODE;
	
	listmaxshow = 0;

	hbutton_count	= 0;
	hbutton_labels.clear();
	fbutton_count	= 0;
	fbutton_labels.clear();
	fbutton_width = itemBox.iWidth;

	paintDate = false;
	paintTitle = false;
	paint_Foot = false;
	paintFootInfo = false;

	l_name = "";
	iconfile = "";
	tMode = CC_ALIGN_LEFT;

	//
	enableCenter = false;
	inFocus = true;
	shrinkMenu = false;

	//
	itemsPerX = 6;
	itemsPerY = 3;
	maxItemsPerPage = itemsPerX*itemsPerY;

	// head
	headColor = COL_MENUHEAD_PLUS_0;
	headRadius = g_settings.Head_radius;
	headCorner = g_settings.Head_corner;
	headGradient = g_settings.Head_gradient;

	// foot
	footColor = COL_MENUFOOT_PLUS_0;
	footRadius = g_settings.Foot_radius;
	footCorner = g_settings.Foot_corner;
	footGradient = g_settings.Foot_gradient;

	//
	widgetType = WIDGET_TYPE_STANDARD;
	cnt = 0;

	itemType = WIDGET_ITEM_LISTBOX;

	savescreen = false;
	background = NULL;

	textBox = NULL;

	actionKey = "";
	
	paintFrame = true;
	bgcolor = COL_MENUCONTENT_PLUS_0;
	radius = NO_RADIUS;
	corner = CORNER_NONE;
	def_color = false;
	scrolling = true;
	items_background = NULL;
	
	//
	item_height = 0;
	item_width = 0;
	iconOffset = 0;
	items_width = 0;
	items_height = 0;
}

ClistBox::~ClistBox()
{
	dprintf(DEBUG_NORMAL, "ClistBox:: del\n");

	items.clear();
	
	if (items_background)
	{
		delete [] items_background;
		items_background = NULL;
	}
}

void ClistBox::addItem(CMenuItem * menuItem, const bool defaultselected)
{
	if (defaultselected)
		selected = items.size();
	
	items.push_back(menuItem);
	menuItem->setParent(this);
}

bool ClistBox::hasItem()
{
	return !items.empty();
}

void ClistBox::initFrames()
{
	dprintf(DEBUG_NORMAL, "ClistBox::initFrames:\n");
	
	// reinit position
	itemBox.iHeight = wanted_height;
	itemBox.iWidth = wanted_width;
	itemBox.iX = start_x;
	itemBox.iY = start_y;
	cFrameFootInfoHeight = 0;
	connectLineWidth = 0;

	// widgettype forwarded to item 
	for (unsigned int count = 0; count < items.size(); count++) 
	{
		CMenuItem * item = items[count];

		item->widgetType = widgetType;
		item->paintFrame = paintFrame;
	} 

	// head
	if(paintTitle)
	{
		hheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight() + 6;
	}
	
	// foot height
	if(paint_Foot)
	{
		fheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight() + 6;
	}

	// init frames
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		//
		//if(paintFootInfo)
		cFrameFootInfoHeight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight() + 6;
		
		//	
		if(fbutton_count == 0)
		{
			fheight = 0;
		}

		//
		page_start.clear();
		page_start.push_back(0);
		total_pages = 1;

		for (unsigned int i = 0; i < items.size(); i++) 
		{
			if(i == maxItemsPerPage*total_pages)
			{
				page_start.push_back(i);
				total_pages++;
			}
		}

		page_start.push_back(items.size());

		//
		item_width = itemBox.iWidth/itemsPerX;
		item_height = (itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight - 20)/itemsPerY; //

		// HACK:
		for (unsigned int count = 0; count < items.size(); count++) 
		{
			CMenuItem * item = items[count];

			item->item_width = item_width;
			item->item_height = item_height;
		}		
	}
	else 
	{
		// footInfoBox
		if(paintFootInfo)
		{
			if( (widgetType == WIDGET_TYPE_STANDARD) || (widgetType == WIDGET_TYPE_CLASSIC) )
			{
				cFrameFootInfoHeight = footInfoHeight;
				connectLineWidth = CONNECTLINEBOX_WIDTH;
			}
		}

		// calculate some values
		int itemHeightTotal = 0;
		item_height = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight() + 3;
		int heightCurrPage = 0;
		page_start.clear();
		page_start.push_back(0);
		total_pages = 1;
		int heightFirstPage = 0;
	
		for (unsigned int i = 0; i < items.size(); i++) 
		{
			item_height = items[i]->getHeight();
			itemHeightTotal += item_height;
			heightCurrPage += item_height;

			if((heightCurrPage + hheight + fheight + cFrameFootInfoHeight) > itemBox.iHeight)
			{
				page_start.push_back(i);
				//heightFirstPage = heightCurrPage - item_height;
				heightFirstPage = std::max(heightCurrPage - item_height, heightFirstPage); //FIXME:
				total_pages++;
				heightCurrPage = item_height;
			}
		}

		heightFirstPage = std::max(heightCurrPage, heightFirstPage); //FIXME:
		page_start.push_back(items.size());

		// icon offset
		iconOffset = 0;

		for (unsigned int i = 0; i < items.size(); i++) 
		{
			if ((!(items[i]->iconName.empty())) || CRCInput::isNumeric(items[i]->directKey))
			{
				iconOffset = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();
				break;
			}
		}

		// recalculate height
		if(shrinkMenu)
		{
			//listmaxshow = (itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight)/item_height;
			//itemBox.iHeight = hheight + listmaxshow*item_height + fheight + cFrameFootInfoHeight;
			if (hasItem())
				itemBox.iHeight = std::min(itemBox.iHeight, hheight + heightFirstPage + fheight + cFrameFootInfoHeight); //FIXME:
		}
		
		// sanity check
		if(itemBox.iHeight > (int)frameBuffer->getScreenHeight() - 20)
			itemBox.iHeight = frameBuffer->getScreenHeight() - 20;

		// sanity check
		if(itemBox.iWidth > (int)frameBuffer->getScreenWidth() - 20)
			itemBox.iWidth = frameBuffer->getScreenWidth() - 20;
		
		//
		full_height = itemBox.iHeight;
		full_width = itemBox.iWidth;

		//
		if(paintFootInfo)
		{
			if( (widgetType == WIDGET_TYPE_STANDARD) || (widgetType == WIDGET_TYPE_CLASSIC) )
			{
				itemBox.iWidth -= connectLineWidth; //*2 for sanity check
				//itemBox.iX += connectLineWidth;
			}
		}

		// position xy
		if(enableCenter)
		{
			itemBox.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - itemBox.iWidth ) >> 1 );
			itemBox.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - itemBox.iHeight) >> 1 );
		}
	}
}

void ClistBox::paint()
{
	dprintf(DEBUG_NORMAL, "ClistBox::paint: (%s)\n", l_name.c_str());

	//
	initFrames();
	
	if (!paintFrame)
	{
		if (items_background)
		{
			delete [] items_background;
			items_background = NULL;
		}
							
		items_background = new fb_pixel_t[itemBox.iWidth*(itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight)];
							
		if (items_background)
		{
			frameBuffer->saveScreen(itemBox.iX, itemBox.iY + hheight, itemBox.iWidth, itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight, items_background);
		}
	}

	//
	paintHead();
	paintFoot();
	paintItems();
}

void ClistBox::paintItems()
{
	dprintf(DEBUG_NORMAL, "ClistBox::paintItems:\n");

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		item_start_y = itemBox.iY + hheight + 10;
		items_height = itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight - 20;  //TEST
		items_width = itemBox.iWidth;

		// items background
		if (paintFrame)
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY + hheight, itemBox.iWidth, itemBox.iHeight - hheight - fheight, def_color? COL_MENUCONTENT_PLUS_0 : bgcolor, radius, corner);

		// item not currently on screen
		if (selected >= 0)
		{
			while(selected < (int)page_start[current_page])
				current_page--;
		
			while(selected >= (int)page_start[current_page + 1])
				current_page++;
		}

		for (unsigned int i = 0; i < items.size(); i++) 
		{
			CMenuItem * item = items[i];	
			item->init(-1, 0, 0, 0);
		}

		int count = (int)page_start[current_page];

		if(items.size() > 0)
		{
			for (int _y = 0; _y < itemsPerY; _y++)
			{
				for (int _x = 0; _x < itemsPerX; _x++)
				{
					CMenuItem * item = items[count];
					
					item->init(itemBox.iX + _x*item_width, item_start_y + _y*item_height, items_width, iconOffset);

					if((item->isSelectable()) && (selected == -1)) 
					{
						selected = count;
					} 

					if (selected == (signed int)count) 
					{
						paintItemInfo(count);
					}

					item->paint( selected == ((signed int) count));

					count++;

					if ( (count == (int)page_start[current_page + 1]) || (count == (int)items.size()))
					{
						break;
					}
				}

				if ( (count == (int)page_start[current_page + 1]) || (count == (int)items.size()))
				{
					break;
				}		
			}
		}
	}
	else
	{
		item_start_y = itemBox.iY + hheight;
		items_height = itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight; 

		sb_width = 0;
	
		if(total_pages > 1)
			sb_width = scrolling? SCROLLBAR_WIDTH : 0;

		items_width = itemBox.iWidth - sb_width;

		// extended
		if(widgetType == WIDGET_TYPE_EXTENDED)
		{
			items_width = 2*(itemBox.iWidth/3) - sb_width;			
		}

		// item not currently on screen
		if (selected >= 0)
		{
			while(selected < (int)page_start[current_page])
				current_page--;
		
			while(selected >= (int)page_start[current_page + 1])
				current_page++;
		}

		// paint items background
		if (paintFrame) //FIXME:
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY + hheight, itemBox.iWidth, items_height, def_color? COL_MENUCONTENT_PLUS_0 : bgcolor, radius, corner);
		else
		{
			if (items_background)
			{
				frameBuffer->restoreScreen(itemBox.iX, itemBox.iY + hheight, itemBox.iWidth, items_height, items_background);
			}
		}
	
		// paint right scrollBar if we have more then one page
		if(total_pages > 1)
		{
			if (scrolling)
			{
				if(widgetType == WIDGET_TYPE_EXTENDED)
					scrollBar.paint(itemBox.iX + 2*(itemBox.iWidth/3) - SCROLLBAR_WIDTH, itemBox.iY + hheight, itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight, total_pages, current_page);
				else
					scrollBar.paint(itemBox.iX + itemBox.iWidth - SCROLLBAR_WIDTH, itemBox.iY + hheight, itemBox.iHeight - hheight - fheight - cFrameFootInfoHeight, total_pages, current_page);
			}
		}

		// paint items
		int ypos = itemBox.iY + hheight;
		int xpos = itemBox.iX;
	
		for (unsigned int count = 0; count < items.size(); count++) 
		{
			CMenuItem * item = items[count];

			if ((count >= page_start[current_page]) && (count < page_start[current_page + 1])) 
			{
				//
				item->init(xpos, ypos, items_width, iconOffset);
				
			
				if((item->isSelectable()) && (selected == -1)) 
				{
					selected = count;
				} 

				// paint itemInfo
				if (selected == (signed int)count) 
				{
					paintItemInfo(count);
				}

				// paint item
				ypos = item->paint(selected == ((signed int) count));
			} 
			else 
			{
				// x = -1 is a marker which prevents the item from being painted on setActive changes
				item->init(-1, 0, 0, 0);
			}	
		} 
	}
}

void ClistBox::paintHead()
{
	if(paintTitle)
	{
		dprintf(DEBUG_INFO, "ClistBox::paintHead:\n");
		
		if(widgetType == WIDGET_TYPE_FRAME)
		{
			// box
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, hheight, COL_MENUCONTENT_PLUS_0);

			// paint horizontal line top
			frameBuffer->paintHLineRel(itemBox.iX + BORDER_LEFT, itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT, itemBox.iY + hheight - 2, COL_MENUCONTENT_PLUS_5);

			// icon
			int i_w = 0;
			int i_h = 0;

			frameBuffer->getIconSize(iconfile.c_str(), &i_w, &i_h);
			
			if(i_h >= hheight)
			{
				i_h = hheight - 2;
				i_w = i_h*1.67;
			}

			CFrameBuffer::getInstance()->paintIcon(iconfile, itemBox.iX + BORDER_LEFT, itemBox.iY + (hheight - i_h)/2, 0, true, i_w, i_h);

			// Buttons
			int iw[hbutton_count], ih[hbutton_count];
			int xstartPos = itemBox.iX + itemBox.iWidth - BORDER_RIGHT;
			int buttonWidth = 0; //FIXME

			if (hbutton_count)
			{
				for (int i = 0; i < (int)hbutton_count; i++)
				{
					if (hbutton_labels[i].button != NULL)
					{
						frameBuffer->getIconSize(hbutton_labels[i].button, &iw[i], &ih[i]);

						xstartPos -= (iw[i] + ICON_TO_ICON_OFFSET);
						buttonWidth += iw[i];

						CFrameBuffer::getInstance()->paintIcon(hbutton_labels[i].button, xstartPos, itemBox.iY + (hheight - ih[i])/2);
					}
				}
			}

			// paint time/date
			int timestr_len = 0;

			if(paintDate)
			{
				std::string timestr = getNowTimeStr("%d.%m.%Y %H:%M");
		
				timestr_len = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth(timestr.c_str(), true); // UTF-8
	
				g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->RenderString(xstartPos - timestr_len, itemBox.iY + (hheight - g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight(), timestr_len + 1, timestr.c_str(), COL_MENUHEAD, 0, true); 
			}

			// title
			int startPosX = itemBox.iX + BORDER_LEFT + i_w + ICON_OFFSET;
			int stringWidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getRenderWidth(l_name);
			
			if (tMode == CC_ALIGN_CENTER)
				startPosX = itemBox.iX + (itemBox.iWidth >> 1) - (stringWidth >> 1);
		
			g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(startPosX, itemBox.iY + (hheight - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT - i_w - 2*ICON_OFFSET - buttonWidth - (hbutton_count - 1)*ICON_TO_ICON_OFFSET - timestr_len, l_name, COL_MENUHEAD);
		}
		else
		{		
			// paint head
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, hheight, headColor, headRadius, headCorner, headGradient);
		
			//paint icon (left)
			int i_w = 0;
			int i_h = 0;
			
			frameBuffer->getIconSize(iconfile.c_str(), &i_w, &i_h);
			
			// limit icon height
			if(i_h >= hheight)
			{
				i_h = hheight - 2;
				i_w = i_h*1.67;
			}

			CFrameBuffer::getInstance()->paintIcon(iconfile, itemBox.iX + BORDER_LEFT, itemBox.iY + (hheight - i_h)/2, 0, true, i_w, i_h);

			// Buttons
			int iw[hbutton_count], ih[hbutton_count];
			int xstartPos = itemBox.iX + itemBox.iWidth - BORDER_RIGHT;
			int buttonWidth = 0; //FIXME

			if (hbutton_count)
			{
				for (int i = 0; i < (int)hbutton_count; i++)
				{
					if (hbutton_labels[i].button != NULL)
					{
						frameBuffer->getIconSize(hbutton_labels[i].button, &iw[i], &ih[i]);
						xstartPos -= (iw[i] + ICON_TO_ICON_OFFSET);
						buttonWidth += iw[i];

						CFrameBuffer::getInstance()->paintIcon(hbutton_labels[i].button, xstartPos, itemBox.iY + (hheight - ih[i])/2);
					}
				}
			}

			// paint time/date
			int timestr_len = 0;
			if(paintDate)
			{
				std::string timestr = getNowTimeStr("%d.%m.%Y %H:%M");
			
				timestr_len = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth(timestr.c_str(), true); // UTF-8
		
				g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->RenderString(xstartPos - timestr_len, itemBox.iY + (hheight - g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight(), timestr_len + 1, timestr.c_str(), COL_MENUHEAD, 0, true); 
			}
		
			// head title
			int startPosX = itemBox.iX + BORDER_LEFT + i_w + ICON_OFFSET;
			int stringWidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getRenderWidth(l_name);
			
			if (tMode == CC_ALIGN_CENTER)
				startPosX = itemBox.iX + (itemBox.iWidth >> 1) - (stringWidth >> 1);
				
			g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(startPosX, itemBox.iY + (hheight - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), itemBox.iWidth - BORDER_RIGHT - BORDER_RIGHT - i_w - 2*ICON_OFFSET - timestr_len - buttonWidth - (hbutton_count - 1)*ICON_TO_ICON_OFFSET, l_name.c_str(), COL_MENUHEAD, 0, true); // UTF-8
		}		
	}	
}

void ClistBox::paintFoot()
{
	if(paint_Foot)
	{
		dprintf(DEBUG_NORMAL, "ClistBox::paintFoot:\n");
		
		if(widgetType == WIDGET_TYPE_FRAME)
		{
			if(fbutton_count)
			{
				frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY + itemBox.iHeight - fheight, itemBox.iWidth, fheight, COL_MENUCONTENT_PLUS_0);

				// paint horizontal line buttom
				frameBuffer->paintHLineRel(itemBox.iX + BORDER_LEFT, itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT, itemBox.iY + itemBox.iHeight - fheight + 2, COL_MENUCONTENT_PLUS_5);

				// buttons
				int buttonWidth = 0;

				buttonWidth = (itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT)/fbutton_count;
	
				for (int i = 0; i < (int)fbutton_count; i++)
				{
					if (fbutton_labels[i].button != NULL)
					{
						const char * l_option = NULL;
						int iw = 0;
						int ih = 0;

						CFrameBuffer::getInstance()->getIconSize(fbutton_labels[i].button, &iw, &ih);
						int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

						if(fbutton_labels[i].localename != NULL)
							l_option = fbutton_labels[i].localename;
						else
							l_option = g_Locale->getText(fbutton_labels[i].locale);
		
						CFrameBuffer::getInstance()->paintIcon(fbutton_labels[i].button, itemBox.iX + BORDER_LEFT + i*buttonWidth, itemBox.iY + itemBox.iHeight - fheight + (fheight - ih)/2);

						g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(itemBox.iX + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, itemBox.iY + itemBox.iHeight - fheight + f_h + (fheight - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
					}
				}
			}
		}
		else
		{
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY + itemBox.iHeight - cFrameFootInfoHeight - fheight, itemBox.iWidth, fheight, footColor, footRadius, footCorner, footGradient);

			// buttons
			int buttonWidth = 0;

			if (fbutton_count)
				buttonWidth = (itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT)/fbutton_count;
	
			for (int i = 0; i < (int)fbutton_count; i++)
			{
				if (fbutton_labels[i].button != NULL)
				{
					const char * l_option = NULL;
					int iw = 0;
					int ih = 0;

					CFrameBuffer::getInstance()->getIconSize(fbutton_labels[i].button, &iw, &ih);
					int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

					if(fbutton_labels[i].localename != NULL)
						l_option = fbutton_labels[i].localename;
					else
						l_option = g_Locale->getText(fbutton_labels[i].locale);
		
					CFrameBuffer::getInstance()->paintIcon(fbutton_labels[i].button, itemBox.iX + BORDER_LEFT + i*buttonWidth, itemBox.iY + itemBox.iHeight - cFrameFootInfoHeight - fheight + (fheight - ih)/2);

					g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(itemBox.iX + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, itemBox.iY + itemBox.iHeight - cFrameFootInfoHeight - fheight + f_h + (fheight - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
					}
			}
		}
	}
}

void ClistBox::setHeadButtons(const struct button_label *_hbutton_labels, const int _hbutton_count)
{
	if(paintTitle)
	{
		if (_hbutton_count)
		{
			for (int i = 0; i < (int)_hbutton_count; i++)
			{
				hbutton_labels.push_back(_hbutton_labels[i]);
			}
		}

		hbutton_count = hbutton_labels.size();
	}
}

void ClistBox::setFootButtons(const struct button_label* _fbutton_labels, const int _fbutton_count, const int _fbutton_width)
{
	if(paint_Foot)
	{
		if (_fbutton_count)
		{
			for (int i = 0; i < (int)_fbutton_count; i++)
			{
				fbutton_labels.push_back(_fbutton_labels[i]);
			}
		}

		fbutton_count = fbutton_labels.size();	
		fbutton_width = (_fbutton_width == 0)? itemBox.iWidth : _fbutton_width;
	}
}

void ClistBox::paintItemInfo(int pos)
{
	dprintf(DEBUG_NORMAL, "ClistBox::paintItemInfo:\n"); //FIXME:
	
	if(widgetType == WIDGET_TYPE_STANDARD)
	{
		if(paintFootInfo)
		{
			if (footInfoMode == FOOT_INFO_MODE)
			{
				CMenuItem* item = items[pos];

				// detailslines
				itemsLine.setMode(DL_INFO);
				itemsLine.setInfo1(item->info1.c_str());
				itemsLine.setOptionInfo1(item->option_info1.c_str());
				itemsLine.setInfo2(item->info2.c_str());
				itemsLine.setOptionInfo2(item->option_info2.c_str());
					
				itemsLine.paint(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight - cFrameFootInfoHeight, cFrameFootInfoHeight, item->getHeight(), item->getYPosition());
			}
			else if (footInfoMode == FOOT_HINT_MODE)
			{
				CMenuItem* item = items[pos];
	
				// detailslines box
				itemsLine.setMode(DL_HINT);
				itemsLine.setHint(item->itemHint.c_str());
				itemsLine.setIcon(item->itemIcon.c_str());
					
				itemsLine.paint(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight - cFrameFootInfoHeight, cFrameFootInfoHeight, item->getHeight(), item->getYPosition());
			}
		}
	}
	else if(widgetType == WIDGET_TYPE_CLASSIC)
	{
		if(paintFootInfo)
		{
			if (footInfoMode == FOOT_INFO_MODE)
			{
				CMenuItem* item = items[pos];
	
				// detailslines
				itemsLine.setMode(DL_INFO);
				itemsLine.setInfo1(item->info1.c_str());
				itemsLine.setOptionInfo1(item->option_info1.c_str());
				itemsLine.setInfo2(item->info2.c_str());
				itemsLine.setOptionInfo2(item->option_info2.c_str());
					
				itemsLine.paint(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight - cFrameFootInfoHeight, cFrameFootInfoHeight, item->getHeight(), item->getYPosition());
			}
			else if (footInfoMode == FOOT_HINT_MODE)
			{
				CMenuItem* item = items[pos];
	
				// detailslines box
				itemsLine.setMode(DL_HINT);
				itemsLine.setHint(item->itemHint.c_str());
				//itemsLine.setIcon(item->itemIcon.c_str());
				itemsLine.paint(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight - cFrameFootInfoHeight, cFrameFootInfoHeight, item->getHeight(), item->getYPosition());
			}
		}
	}
	else if(widgetType == WIDGET_TYPE_EXTENDED)
	{
		CMenuItem* item = items[pos];

		int iw, ih;

		//if (paintFootInfo)
		//{
		// scale pic
		int p_w = 0;
		int p_h = 0;

		std::string fname = item->itemIcon;

		::scaleImage(fname, &p_w, &p_h);

		if(textBox)
		{
			delete textBox;
			textBox = NULL;
		}
						
		textBox = new CTextBox(itemBox.iX + 2*(itemBox.iWidth/3), itemBox.iY + hheight, (itemBox.iWidth/3), items_height);
				
		textBox->setBackgroundColor(COL_MENUCONTENTDARK_PLUS_0);

		// hint
		textBox->setText(item->itemHint.c_str(), item->itemIcon.c_str(), p_w, p_h, TOP_CENTER);
		textBox->paint();
		//}
	}
	else if(widgetType == WIDGET_TYPE_FRAME)
	{
		//if(paintFootInfo)
		//{
		// refresh
		frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY + itemBox.iHeight - fheight - cFrameFootInfoHeight, itemBox.iWidth, cFrameFootInfoHeight, COL_MENUCONTENT_PLUS_0);

		// refresh horizontal line buttom
		frameBuffer->paintHLineRel(itemBox.iX + BORDER_LEFT, itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT, itemBox.iY + itemBox.iHeight - fheight - cFrameFootInfoHeight + 2, COL_MENUCONTENT_PLUS_5);

		if(items.size() > 0)
		{
			CMenuItem* item = items[pos];
		
			// itemName
			if(!item->itemName.empty())
			{
				g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->RenderString(itemBox.iX + BORDER_LEFT, itemBox.iY + itemBox.iHeight - fheight - cFrameFootInfoHeight + (cFrameFootInfoHeight - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE] ->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->getHeight(), itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT, item->itemName.c_str(), COL_MENUFOOT_INFO);
			}

			// hint
			if(!item->itemHint.empty())
			{
				g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1]->RenderString(itemBox.iX + BORDER_LEFT, itemBox.iY + itemBox.iHeight - fheight, itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT, item->itemHint.c_str(), COL_MENUFOOT_INFO);
			}
		}
		//}
	}	
}

void ClistBox::hideItemInfo()
{
	dprintf(DEBUG_NORMAL, "ClistBox::hideItemInfo:\n");
	
	if (paintFootInfo)
	{
	    	if(widgetType == WIDGET_TYPE_STANDARD || widgetType == WIDGET_TYPE_CLASSIC)
		{
			itemsLine.clear(itemBox.iX, itemBox.iY, itemBox.iWidth + CONNECTLINEBOX_WIDTH, itemBox.iHeight - cFrameFootInfoHeight, cFrameFootInfoHeight);
		} 
	}	 
}

void ClistBox::saveScreen()
{
	dprintf(DEBUG_NORMAL, "ClistBox::saveScreen:\n");
	
	if(!savescreen)
		return;

	if(background)
	{
		delete[] background;
		background = NULL;
	}

	background = new fb_pixel_t[wanted_width*wanted_height];
	
	if(background)
	{
		frameBuffer->saveScreen(itemBox.iX, itemBox.iY, wanted_width, wanted_height, background);
	}
}

void ClistBox::restoreScreen()
{
	dprintf(DEBUG_NORMAL, "ClistBox::restoreScreen:\n");
	
	if(background) 
	{
		if(savescreen)
			frameBuffer->restoreScreen(itemBox.iX, itemBox.iY, wanted_width, wanted_height, background);
	}
}

void ClistBox::enableSaveScreen()
{
	dprintf(DEBUG_NORMAL, "ClistBox::enableSaveScreen:\n");
	
	savescreen = true;
	
	if(!savescreen && background) 
	{
		delete[] background;
		background = NULL;
	}
	
	saveScreen();
}

void ClistBox::hide()
{
	dprintf(DEBUG_NORMAL, "ClistBox::hide: (%s)\n", l_name.c_str());

	if( savescreen && background)
		restoreScreen();
	else
		frameBuffer->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, full_width, full_height);

	hideItemInfo(); 

	if(textBox != NULL)
	{
		delete textBox;
		textBox = NULL;
	}
	
	if (items_background)
	{
		delete [] items_background;
		items_background = NULL;
	}
	
	CFrameBuffer::getInstance()->blit();	
}

void ClistBox::scrollLineDown(const int)
{
	dprintf(DEBUG_NORMAL, "ClistBox::scrollLineDown:\n");
	
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		if(items.size())
		{
			pos = selected + itemsPerX;

			//FIXME:
			if (pos >= (int)items.size())
				pos -= itemsPerX;

			CMenuItem * item = items[pos];

			if ( item->isSelectable() ) 
			{
				if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page]))
				{ 
					// Item is currently on screen
					//clear prev. selected
					items[selected]->paint(false);
					//select new
					item->paint(true);
					paintItemInfo(pos);
					selected = pos;
				} 
			}
		}
	}
	else
	{
		if(items.size())
		{
			//search next / prev selectable item
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = (selected + count)%items.size();

				CMenuItem * item = items[pos];

				if ( item->isSelectable() ) 
				{
					if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page]))
					{ 	
						// Item is currently on screen
						//clear prev. selected
						items[selected]->paint(false);
						//select new
						paintItemInfo(pos);
						item->paint(true);
						selected = pos;
					} 
					else 
					{
						selected = pos;
						paintItems();
					}
					break;
				}
			}
		}
	}
}

void ClistBox::scrollLineUp(const int)
{
	dprintf(DEBUG_NORMAL, "ClistBox::scrollLineUp:\n");
	
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		if(items.size())
		{
			pos = selected - itemsPerX;

			if(pos < 0)
				pos = selected;

			CMenuItem * item = items[pos];

			if ( item->isSelectable() ) 
			{
				if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page]))
				{ 
					// Item is currently on screen
					//clear prev. selected
					items[selected]->paint(false);
					//select new
					item->paint(true);
					paintItemInfo(pos);
					selected = pos;
				}
			}
		}
	}
	else
	{
		if(items.size())
		{
			//search next / prev selectable item
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = selected - count;
				if ( pos < 0 )
					pos += items.size();

				CMenuItem * item = items[pos];

				if ( item->isSelectable() ) 
				{
					if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page]))
					{ 
						// Item is currently on screen
						//clear prev. selected
						items[selected]->paint(false);
						//select new
						paintItemInfo(pos);
						item->paint(true);
						selected = pos;
					} 
					else 
					{
						selected = pos;
						paintItems();
					}
					break;
				}
			}
		}
	}
}

void ClistBox::scrollPageDown(const int)
{
	dprintf(DEBUG_NORMAL, "ClistBox::scrollPageDown:\n");
	
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		if(items.size())
		{
			if(current_page) 
			{
				pos = (int) page_start[current_page] - 1;

				selected = pos;
				paintItems();
			}
		}
	}
	else
	{
		if(items.size())
		{
			pos = (int) page_start[current_page + 1];

			// check pos
			if(pos >= (int) items.size()) 
				pos = items.size() - 1;

			for (unsigned int count = pos ; count < items.size(); count++) 
			{
				CMenuItem * item = items[pos];
				if (item->isSelectable()) 
				{
					if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page])) 
					{
						items[selected]->paint(false);

						// paint new item
						paintItemInfo(pos);
						item->paint(true);
						selected = pos;
					} 
					else 
					{
						selected = pos;
						paintItems();
					}
					break;
				}
				pos++;
			}
		}
	}
}

void ClistBox::scrollPageUp(const int)
{
	dprintf(DEBUG_NORMAL, "ClistBox::scrollPageUp:\n");
	
	if(widgetType == WIDGET_TYPE_FRAME)
	{
		if(items.size())
		{
			pos = (int) page_start[current_page + 1];
			if(pos >= (int) items.size()) 
				pos = items.size() - 1;

			selected = pos;
			paintItems();
		}
	}
	else
	{
		if(items.size())
		{
			if(current_page) 
			{
				pos = (int) page_start[current_page] - 1;
				for (unsigned int count = pos; count > 0; count--) 
				{
					CMenuItem * item = items[pos];
					if ( item->isSelectable() ) 
					{
						if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page])) 
						{
							// prev item
							items[selected]->paint(false);

							// new item
							paintItemInfo(pos);
							item->paint(true);
							selected = pos;
						} 
						else 
						{
							selected = pos;
							paintItems();
						}
						break;
					}
					pos--;
				}
			} 
			else 
			{
				pos = 0;
				for (unsigned int count = 0; count < items.size(); count++) 
				{
					CMenuItem * item = items[pos];
					if (item->isSelectable()) 
					{
						if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page])) 
						{
							// prev item
							items[selected]->paint(false);

							// new item
							paintItemInfo(pos);
							item->paint(true);
							selected = pos;
						} 
						else 
						{
							selected = pos;
							paintItems();
						}
						break;
					}
					pos++;
				}
			}
		}
	}
}

void ClistBox::swipLeft()
{
	dprintf(DEBUG_NORMAL, "ClistBox::swipLeft:\n");

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		if(items.size())
		{
			//search next / prev selectable item
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = selected - count;
				if ( pos < 0 )
					pos += items.size();

				CMenuItem * item = items[pos];

				if ( item->isSelectable() ) 
				{
					if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page]))
					{ 
						// Item is currently on screen
						//clear prev. selected
						items[selected]->paint(false);
						//select new
						item->paint(true);
						paintItemInfo(pos);
						selected = pos;
					}
					else 
					{
						selected = pos;
						paintItems();
					}
								
					break;
				}
			}
		}
	}
	else if (widgetType == WIDGET_TYPE_EXTENDED)
	{
		if(textBox)
			textBox->scrollPageUp(1);
	}
}

void ClistBox::swipRight()
{
	dprintf(DEBUG_NORMAL, "ClistBox::swipRight:\n");

	if(widgetType == WIDGET_TYPE_FRAME)
	{
		if(items.size())
		{
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = (selected + count)%items.size();

				CMenuItem * item = items[pos];

				if ( item->isSelectable() ) 
				{
					if ((pos < (int)page_start[current_page + 1]) && (pos >= (int)page_start[current_page]))
					{ 
						// Item is currently on screen
						//clear prev. selected
						items[selected]->paint(false);
						//select new
						item->paint(true);
						paintItemInfo(pos);
						selected = pos;
					}
					else 
					{
						selected = pos;
						paintItems();
					}
								
					break;
				}
			}
		}
	}
	else if (widgetType == WIDGET_TYPE_EXTENDED)
	{
		if(textBox)
			textBox->scrollPageDown(1);
	}
}

void ClistBox::changeWidgetType()
{
	dprintf(DEBUG_NORMAL, "ClistBox::changeWidgetType:\n");

	if(widget.size())
	{
		hide();

		cnt++;

		if(cnt >= (int)widget.size())
		{
			cnt = 0;
		}
			
		widgetType = widget[cnt];

		paint();
	}
}

//
int ClistBox::oKKeyPressed(CMenuTarget* parent)
{
	dprintf(DEBUG_NORMAL, "ClistBox::okKeyPressed:\n");

	if (hasItem() && selected >= 0 && items[selected]->isSelectable())
		actionKey = items[selected]->actionKey;

	if(parent)
		if (hasItem() && selected >= 0 && items[selected]->isSelectable())
			return items[selected]->exec(parent);
		else
			return RETURN_EXIT;
	else
		return RETURN_EXIT;
}

void ClistBox::integratePlugins(CPlugins::i_type_t integration, const unsigned int shortcut, bool enabled, int imode, int itype, bool i2lines, bool iShadow)
{
	unsigned int number_of_plugins = (unsigned int) g_PluginList->getNumberOfPlugins();

	std::string IconName;
	unsigned int sc = shortcut;

	for (unsigned int count = 0; count < number_of_plugins; count++)
	{
		if ((g_PluginList->getIntegration(count) == integration) && !g_PluginList->isHidden(count))
		{
			//
			IconName = NEUTRINO_ICON_MENUITEM_PLUGIN;

			std::string icon("");
			icon = g_PluginList->getIcon(count);

			if(!icon.empty())
			{
				IconName = PLUGINDIR;
				IconName += "/";
				IconName += g_PluginList->getFileName(count);
				IconName += "/";
				IconName += g_PluginList->getIcon(count);
			}

			//
			neutrino_msg_t dk = (shortcut != RC_nokey) ? CRCInput::convertDigitToKey(sc++) : RC_nokey;

			//FIXME: iconName
			ClistBoxItem *fw_plugin = new ClistBoxItem(g_PluginList->getName(count), enabled, NULL, CPluginsExec::getInstance(), g_PluginList->getFileName(count), dk, NULL, IconName.c_str());

			fw_plugin->setHint(g_PluginList->getDescription(count).c_str());
			fw_plugin->setWidgetMode(imode); //FIXME:
			fw_plugin->setWidgetType(itype);
			if (i2lines)
				fw_plugin->set2lines();
			if (iShadow)
				fw_plugin->enableItemShadow();
			
			fw_plugin->isPlugin = true;
			
			addItem(fw_plugin);
		}
	}
}

/*
void ClistBox::onDirectKeyPressed(neutrino_msg_t msg)
{
	// 
	for (unsigned int i = 0; i < items.size(); i++) 
	{
		CMenuItem * titem = items[i];
			
		if ((titem->directKey != RC_nokey) && (titem->directKey == msg)) 
		{
			if (titem->isSelectable()) 
			{
				items[selected]->paint(false);
				selected = i;

				if (selected > (int)page_start[current_page + 1] || selected < (int)page_start[current_page]) 
				{
					// different page
					paintItems();
				}

				paintItemInfo(selected);
				pos = selected;
				
				//msg = RC_ok;
				actionKey = titem->actionKey;
			} 
			else 
			{
				// swallow-key...
				//handled = true;
			}
			break;
		}
	}
}
*/


