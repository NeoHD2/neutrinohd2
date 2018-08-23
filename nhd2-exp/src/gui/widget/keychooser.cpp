/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: keychooser.cpp 2013/10/12 mohousch Exp $

	Copyright (C) 2001 Steffen Hehn 'McClean'
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
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <global.h>
#include <neutrino.h>

#include <gui/color.h>
#include <driver/screen_max.h>

#include <system/debug.h>

#include <gui/widget/keychooser.h>


class CKeyValue : public CMenuSeparator
{
	std::string the_text;
	public:
		int         keyvalue;

		CKeyValue() : CMenuSeparator(CMenuSeparator::STRING, LOCALE_KEYCHOOSERMENU_CURRENTKEY)
		{
		};

		virtual const char * getString(void)
		{
			the_text  = g_Locale->getText(LOCALE_KEYCHOOSERMENU_CURRENTKEY);
			the_text += ": ";
			the_text += CRCInput::getKeyName(keyvalue);
			return the_text.c_str();
		};
};

CKeyChooser::CKeyChooser(int * const Key, const neutrino_locale_t Title, const std::string & Icon) : CMenuWidget(Title, Icon)
{
	frameBuffer = CFrameBuffer::getInstance();
	key = Key;
	keyChooser = new CKeyChooserItem(LOCALE_KEYCHOOSER_HEAD, key);
	keyDeleter = new CKeyChooserItemNoKey(key);

	addItem(new CKeyValue());
	addItem(new CMenuSeparator(CMenuSeparator::LINE));
	addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	addItem(new CMenuSeparator(CMenuSeparator::LINE));
	addItem(new CMenuForwarder(LOCALE_KEYCHOOSERMENU_SETNEW , true, NULL, keyChooser));
	addItem(new CMenuForwarder(LOCALE_KEYCHOOSERMENU_SETNONE, true, NULL, keyDeleter));
}

CKeyChooser::CKeyChooser(int * const Key, const char * const Title, const std::string & Icon) : CMenuWidget(Title, Icon)
{
	frameBuffer = CFrameBuffer::getInstance();
	key = Key;
	keyChooser = new CKeyChooserItem(LOCALE_KEYCHOOSER_HEAD, key);
	keyDeleter = new CKeyChooserItemNoKey(key);

	addItem(new CKeyValue());
	addItem(new CMenuSeparator(CMenuSeparator::LINE));
	addItem(new CMenuForwarder(LOCALE_MENU_BACK, true, NULL, NULL, NULL, CRCInput::RC_nokey, NEUTRINO_ICON_BUTTON_LEFT));
	addItem(new CMenuSeparator(CMenuSeparator::LINE));
	addItem(new CMenuForwarder(LOCALE_KEYCHOOSERMENU_SETNEW , true, NULL, keyChooser));
	addItem(new CMenuForwarder(LOCALE_KEYCHOOSERMENU_SETNONE, true, NULL, keyDeleter));
}

CKeyChooser::~CKeyChooser()
{
	delete keyChooser;
	delete keyDeleter;
}

void CKeyChooser::paint()
{
	(((CKeyValue *)(items[0]))->keyvalue) = *key;

	CMenuWidget::paint();
}

CKeyChooserItem::CKeyChooserItem(const neutrino_locale_t Name, int * Key)
{
	name = g_Locale->getText(Name);
	key = Key;
}

CKeyChooserItem::CKeyChooserItem(const char * const Name, int * Key)
{
	name = Name;
	key = Key;
}

int CKeyChooserItem::exec(CMenuTarget* parent, const std::string &)
{
	dprintf(DEBUG_NORMAL, "CKeyChooserItem::exec\n");

	neutrino_msg_t      msg;
	neutrino_msg_data_t data;

	unsigned long long timeoutEnd;

	int res = menu_return::RETURN_REPAINT;

	if (parent)
		parent->hide();

	paint();
	
	CFrameBuffer::getInstance()->blit();

	g_RCInput->clearRCMsg();

	timeoutEnd = CRCInput::calcTimeoutEnd(g_settings.timing[SNeutrinoSettings::TIMING_MENU] == 0 ? 0xFFFF : g_settings.timing[SNeutrinoSettings::TIMING_MENU]);

 get_Message:
	g_RCInput->getMsgAbsoluteTimeout( &msg, &data, &timeoutEnd );
	
	if (msg != CRCInput::RC_timeout)
	{
		if ((msg > 0) && (msg <= CRCInput::RC_MaxRC))
			*key = msg;
		else if (CNeutrinoApp::getInstance()->handleMsg(msg, data) & messages_return::cancel_all)
			res = menu_return::RETURN_EXIT_ALL;
		else
			goto get_Message;
	}

	hide();
	return res;
}

void CKeyChooserItem::hide()
{
	m_cBoxWindow.hide();
	CFrameBuffer::getInstance()->blit();
}

void CKeyChooserItem::paint()
{
	dprintf(DEBUG_DEBUG, "CKeyChooserItem::paint\n");

	int hheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight();
	int mheight = g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->getHeight();

	CFrameBuffer * frameBuffer = CFrameBuffer::getInstance();

	m_cBox.iWidth = w_max(MENU_WIDTH, 0);
	m_cBox.iHeight = h_max(hheight +  2*mheight, 0);
	m_cBox.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - m_cBox.iWidth) >> 1);
	m_cBox.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - m_cBox.iHeight) >> 1);

	//box
	m_cBoxWindow.setDimension(&m_cBox);
	//m_cBoxWindow.enableShadow();
	m_cBoxWindow.enableSaveScreen();
	m_cBoxWindow.setColor(COL_MENUCONTENT_PLUS_0);
	m_cBoxWindow.setCorner(RADIUS_MID, CORNER_ALL);
	m_cBoxWindow.paint();
	

	//head 
	m_cTitle.iWidth = m_cBox.iWidth;
	m_cTitle.iHeight = hheight;
	m_cTitle.iX = m_cBox.iX;
	m_cTitle.iY = m_cBox.iY;

	m_cTitleWindow.setDimension(&m_cTitle);
	m_cTitleWindow.setColor(COL_MENUHEAD_PLUS_0);
	m_cTitleWindow.setCorner(RADIUS_MID, CORNER_TOP);
	m_cTitleWindow.setGradient(g_settings.Head_gradient);
	m_cTitleWindow.paint();

	// icon
	int iw, ih;
	frameBuffer->getIconSize(NEUTRINO_ICON_KEYBINDING, &iw, &ih);
	frameBuffer->paintIcon(NEUTRINO_ICON_KEYBINDING, m_cTitle.iX + BORDER_LEFT, m_cTitle.iY + (hheight - ih)/2);

	// title
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(m_cTitle.iX + BORDER_LEFT + iw + BORDER_LEFT, m_cTitle.iY + hheight, m_cTitle.iWidth, name.c_str(), COL_MENUHEAD, 0, true); // UTF-8

	//paint msg...
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(m_cBox.iX + BORDER_LEFT, m_cBox.iY + hheight + mheight, m_cBox.iWidth, g_Locale->getText(LOCALE_KEYCHOOSER_TEXT1), COL_MENUCONTENT, 0, true); // UTF-8

	g_Font[SNeutrinoSettings::FONT_TYPE_MENU]->RenderString(m_cBox.iX + BORDER_LEFT, m_cBox.iY + hheight + mheight* 2, m_cBox.iWidth, g_Locale->getText(LOCALE_KEYCHOOSER_TEXT2), COL_MENUCONTENT, 0, true); // UTF-8
}
