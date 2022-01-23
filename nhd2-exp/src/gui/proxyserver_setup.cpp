/*
	$port: proxyserver_setup.cpp,v 1.2 2010/03/02 20:11:42 tuxbox-cvs Exp $

	proxyserver_setup menue - Neutrino-GUI

	Copyright (C) 2001 Steffen Hehn 'McClean'
	and some other guys
	Homepage: http://dbox.cyberphoria.org/

	Copyright (C) 2010 T. Graf 'dbt'
	Homepage: http://www.dbox2-tuning.net/

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

#include "gui/proxyserver_setup.h"

#include <global.h>
#include <neutrino.h>

#include <gui/widget/stringinput.h>

#include <driver/screen_max.h>
#include <system/debug.h>


int CProxySetup::exec(CMenuTarget * parent, const std::string &/*actionKey*/)
{
	dprintf(DEBUG_DEBUG, "CProxySetup::exec\n");
	
	int   res = RETURN_REPAINT;

	if (parent)
		parent->hide();

	res = showProxySetup();
	
	return res;
}

/* shows entries for proxy settings */
int CProxySetup::showProxySetup()
{
	dprintf(DEBUG_DEBUG, "CProxySetup::showProxySetup\n");

	//init
	CMenuWidget * mn = new CMenuWidget(_("Proxyserver"), NEUTRINO_ICON_NETWORK);

	mn->setWidgetMode(MODE_SETUP);
	mn->enableShrinkMenu();

	CStringInputSMS * softUpdate_proxy = new CStringInputSMS(_("Hostname"), g_settings.softupdate_proxyserver, MAX_INPUT_CHARS, _("enter proxyserver name or ip, use host:port"), _("a empty entry means no proxy"), "abcdefghijklmnopqrstuvwxyz0123456789-.: ");
	mn->addItem(new CMenuForwarder(_("Hostname"), true, g_settings.softupdate_proxyserver, softUpdate_proxy, NULL, RC_red, NEUTRINO_ICON_BUTTON_RED));

	CStringInputSMS * softUpdate_proxyuser = new CStringInputSMS(_("Username"), g_settings.softupdate_proxyusername, MAX_INPUT_CHARS, _("enter the proxyserver username"), _("a empty entry means no proxy-auth"), "abcdefghijklmnopqrstuvwxyz0123456789!""§$%&/()=?-. ");
	mn->addItem(new CMenuForwarder(_("Username"), true, g_settings.softupdate_proxyusername, softUpdate_proxyuser, NULL, RC_green, NEUTRINO_ICON_BUTTON_GREEN));

	CStringInputSMS * softUpdate_proxypass = new CStringInputSMS(_("Password"), g_settings.softupdate_proxypassword, MAX_INPUT_CHARS, _("enter the proxyserver password"), _("a empty entry means no proxy-auth"), "abcdefghijklmnopqrstuvwxyz0123456789!""§$%&/()=?-. ");
	mn->addItem(new CMenuForwarder(_("Password"), true, g_settings.softupdate_proxypassword, softUpdate_proxypass, NULL, RC_yellow, NEUTRINO_ICON_BUTTON_YELLOW));

	int res = mn->exec(NULL, "");
	mn->hide();
	delete mn;
	return res;
}

