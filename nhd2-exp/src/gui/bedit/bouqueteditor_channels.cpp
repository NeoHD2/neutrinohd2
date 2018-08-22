/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: bouqueteditor_channels.cpp 2013/10/12 mohousch Exp $

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

#include <gui/widget/buttons.h>
#include <gui/widget/icons.h>
#include <gui/widget/messagebox.h>
#include <gui/widget/items2detailsline.h>

#include <gui/bedit/bouqueteditor_channels.h>

#include <global.h>
#include <neutrino.h>

#include <driver/fontrenderer.h>
#include <driver/screen_max.h>
#include <gui/bedit/bouqueteditor_chanselect.h>
#include <gui/widget/buttons.h>
#include <gui/widget/icons.h>

//
#include <client/zapitclient.h>
#include <bouquets.h>
#include <satconfig.h>
#include <getservices.h>

#include <system/debug.h>


extern tallchans allchans;			// defined in zapit.cpp
extern CBouquetManager * g_bouquetManager;	// defined in zapit.cpp

CBEChannelWidget::CBEChannelWidget(const std::string & Caption, unsigned int Bouquet)
{
	frameBuffer = CFrameBuffer::getInstance();
	selected = 0;
	
	state = beDefault;
	caption = Caption;
	bouquet = Bouquet;
	mode = CZapitClient::MODE_TV;

	listBox = NULL;
	item = NULL;

	// box	
	cFrameBox.iWidth = w_max ( (frameBuffer->getScreenWidth() / 20 * 17), (frameBuffer->getScreenWidth() / 20 ));
	cFrameBox.iHeight = h_max ( (frameBuffer->getScreenHeight() / 20 * 20), (frameBuffer->getScreenHeight() / 20));
	
	cFrameBox.iX = frameBuffer->getScreenX() + (frameBuffer->getScreenWidth() - cFrameBox.iWidth) / 2;
	cFrameBox.iY = frameBuffer->getScreenY() + (frameBuffer->getScreenHeight() - cFrameBox.iHeight) / 2;

	listBox = new ClistBoxEntry(&cFrameBox);
}

#define BUTTONS_COUNT 4
const struct button_label CBEChannelWidgetButtons[BUTTONS_COUNT] =
{
	{ NEUTRINO_ICON_BUTTON_RED   , LOCALE_BOUQUETEDITOR_DELETE, NULL     },
	{ NEUTRINO_ICON_BUTTON_GREEN , LOCALE_BOUQUETEDITOR_ADD, NULL        },
	{ NEUTRINO_ICON_BUTTON_YELLOW, LOCALE_BOUQUETEDITOR_MOVE, NULL       },
	{ NEUTRINO_ICON_BUTTON_BLUE  , LOCALE_BOUQUETEDITOR_SWITCHMODE, NULL }
};

void CBEChannelWidget::paint(bool reinit)
{
	dprintf(DEBUG_NORMAL, "CBEChannelWidget::paint:\n");

	for (unsigned int count = 0; count < Channels->size(); count++)
	{
		item = new ClistBoxEntryItem((*Channels)[count]->getName().c_str(), true);

		if(state == beMoving && count == selected)
			item->setIconName(NEUTRINO_ICON_BUTTON_YELLOW);

		// scrambled
		std::string scrambled_icon = "";
		if((*Channels)[count]->scrambled)
			scrambled_icon = NEUTRINO_ICON_SCRAMBLED;

		item->setIcon1(scrambled_icon.c_str());

		// hd/uhd
		std::string hd_icon = "";
		if((*Channels)[count]->isHD())
			hd_icon = NEUTRINO_ICON_HD;
		else if((*Channels)[count]->isUHD()) 
			hd_icon = NEUTRINO_ICON_UHD,

		item->setIcon2(hd_icon.c_str()); 

		// info1
		char buf[128];
		transponder_id_t ct = (*Channels)[count]->getTransponderId();
		transponder_list_t::iterator tpI = transponders.find(ct);
		int len = snprintf(buf, sizeof(buf), "%d ", (*Channels)[count]->getFreqId());

		// satname
		sat_iterator_t sit = satellitePositions.find((*Channels)[count]->getSatellitePosition());
		
		if(sit != satellitePositions.end()) 
		{
			snprintf(&buf[len], sizeof(buf) - len, "(%s)\n", sit->second.name.c_str());
		}

		item->setInfo1(buf);

		listBox->addItem(item);
	}

	listBox->setTitle(caption.c_str());
	listBox->enablePaintHead();
	listBox->enablePaintDate();
	listBox->enablePaintFoot();
	listBox->setFooterButtons(CBEChannelWidgetButtons, BUTTONS_COUNT);
	listBox->enablePaintFootInfo();

	//
	listBox->setSelected(selected);
	listBox->paint(reinit);
}

void CBEChannelWidget::hide()
{
	listBox->hide();
}

int CBEChannelWidget::exec(CMenuTarget* parent, const std::string &/*actionKey*/)
{
	dprintf(DEBUG_NORMAL, "CBEChannelWidget::exec:\n");

	neutrino_msg_t      msg;
	neutrino_msg_data_t data;

	int res = menu_return::RETURN_REPAINT;

	if (parent)
		parent->hide();

	Channels = mode == CZapitClient::MODE_TV ? &(g_bouquetManager->Bouquets[bouquet]->tvChannels) : &(g_bouquetManager->Bouquets[bouquet]->radioChannels);
	
	paint();
	frameBuffer->blit();	

	channelsChanged = false;

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(1*1000*1000, false);

	unsigned long long timeoutEnd = CRCInput::calcTimeoutEnd(g_settings.timing[SNeutrinoSettings::TIMING_EPG]);

	bool loop = true;
	while (loop)
	{
		g_RCInput->getMsgAbsoluteTimeout( &msg, &data, &timeoutEnd );

		if ( msg <= CRCInput::RC_MaxRC )
			timeoutEnd = CRCInput::calcTimeoutEnd(g_settings.timing[SNeutrinoSettings::TIMING_EPG]);

		if ((msg == CRCInput::RC_timeout) || (msg == (neutrino_msg_t)g_settings.key_channelList_cancel))
		{
			if (state == beDefault)
			{
				loop = false;
			}
			else if (state == beMoving)
			{
				cancelMoveChannel();
			}
		}
		else if (msg == CRCInput::RC_page_up)
		{
			if (state == beDefault)
				listBox->scrollPageUp();
			else if(state == beMoving)
			{
				selected = listBox->getSelected();
				int next_selected = selected - listBox->getListMaxShow();

				if (next_selected < 0)
					next_selected = 0;

				internalMoveChannel(selected, next_selected);
			}
		}
		else if (msg == CRCInput::RC_page_down)
		{
			if (state == beDefault)
				listBox->scrollPageDown();
			else if(state == beMoving)
			{
				selected = listBox->getSelected();

				int next_selected = selected + listBox->getListMaxShow();

				if (next_selected > Channels->size())
					next_selected = Channels->size();

				internalMoveChannel(selected, next_selected);
			}
		}
		else if (msg == CRCInput::RC_up)
		{
			if (!(Channels->empty()))
			{
				selected = listBox->getSelected();

				int prev_selected = selected;
				int next_selected = selected - 1;
				if (next_selected < 0)
					next_selected = 0;

				if (state == beDefault)
				{
					listBox->scrollLineUp();
				}
				else if (state == beMoving)
				{
					internalMoveChannel(prev_selected, next_selected);
				}
			}
		}
		else if (msg == CRCInput::RC_down)
		{
			selected = listBox->getSelected();

			int prev_selected = selected;
			int next_selected = selected + 1;
				if (next_selected > Channels->size())
					next_selected = Channels->size();

			if (state == beDefault)
			{
				listBox->scrollLineDown();
			}
			else if (state == beMoving)
			{
				internalMoveChannel(prev_selected, next_selected);
			}
		}
		else if(msg == CRCInput::RC_red)
		{
			selected = listBox->getSelected();

			if (state == beDefault)
				deleteChannel();
		}
		else if(msg == CRCInput::RC_green)
		{
			selected = listBox->getSelected();

			if (state == beDefault)
				addChannel();
		}
		else if(msg == CRCInput::RC_yellow)
		{
			selected = listBox->getSelected();

			if (state == beDefault)
				beginMoveChannel();

			listBox->clearItems();
			paint(false);
		}
		else if(msg == CRCInput::RC_blue)
		{
			if (state == beDefault)
			{
				if (mode == CZapitClient::MODE_TV)
					mode = CZapitClient::MODE_RADIO;
				else
					mode = CZapitClient::MODE_TV;

				Channels = mode == CZapitClient::MODE_TV ? &(g_bouquetManager->Bouquets[bouquet]->tvChannels) : &(g_bouquetManager->Bouquets[bouquet]->radioChannels);

				selected = 0;

				listBox->clearItems();
				paint(false);
			}
		}
		else if(msg == CRCInput::RC_ok)
		{
			selected = listBox->getSelected();
			if (state == beDefault)
			{
				if (selected < Channels->size()) /* Channels.size() might be 0 */
					g_Zapit->zapTo_serviceID((*Channels)[selected]->channel_id);

			} 
			else if (state == beMoving) 
			{
				finishMoveChannel();
			}
		}
		else if( CRCInput::isNumeric(msg) )
		{
			if (state == beDefault)
			{
				//kein pushback - wenn man versehentlich wo draufkommt is die edit-arbeit umsonst
				//selected = oldselected;
				//g_RCInput->postMsg( msg, data );
				//loop=false;
			}
			else if (state == beMoving)
			{
				cancelMoveChannel();
			}
		}
		else if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
		{
			listBox->paintHead();
		}
		else
		{
			CNeutrinoApp::getInstance()->handleMsg( msg, data );
		}

		frameBuffer->blit();	
	}
	
	hide();

	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;

	delete listBox;
	listBox = NULL;
	
	return res;
}

void CBEChannelWidget::deleteChannel()
{
	if (selected >= Channels->size()) /* Channels.size() might be 0 */
		return;

	if (MessageBox(LOCALE_FILEBROWSER_DELETE, (*Channels)[selected]->getName(), CMessageBox::mbrNo, CMessageBox::mbYes|CMessageBox::mbNo)!=CMessageBox::mbrYes)
		return;

	g_bouquetManager->Bouquets[bouquet]->removeService((*Channels)[selected]->channel_id);

	Channels = mode == CZapitClient::MODE_TV ? &(g_bouquetManager->Bouquets[bouquet]->tvChannels) : &(g_bouquetManager->Bouquets[bouquet]->radioChannels);

	if (selected >= Channels->size())
		selected = Channels->empty() ? 0 : (Channels->size() - 1);
	
	channelsChanged = true;

	listBox->clearItems();
	paint(false);
}

void CBEChannelWidget::addChannel()
{
	CBEChannelSelectWidget * channelSelectWidget = new CBEChannelSelectWidget(caption, bouquet, mode);

	channelSelectWidget->exec(this, "");
	
	if (channelSelectWidget->hasChanged())
	{
		channelsChanged = true;
		Channels = mode == CZapitClient::MODE_TV ? &(g_bouquetManager->Bouquets[bouquet]->tvChannels) : &(g_bouquetManager->Bouquets[bouquet]->radioChannels);
	}
	
	delete channelSelectWidget;

	listBox->clearItems();
	paint(false);
}

void CBEChannelWidget::beginMoveChannel()
{
	state = beMoving;
	origPosition = selected;
	newPosition = selected;
}

void CBEChannelWidget::finishMoveChannel()
{
	state = beDefault;

	listBox->clearItems();
	paint(false);
}

void CBEChannelWidget::cancelMoveChannel()
{
	state = beDefault;
	internalMoveChannel(newPosition, origPosition);
	channelsChanged = false;
}

void CBEChannelWidget::internalMoveChannel(unsigned int fromPosition, unsigned int toPosition)
{
	if ( (int) toPosition == -1 ) 
		return;
	
	if (toPosition == Channels->size()) 
		return;

	g_bouquetManager->Bouquets[bouquet]->moveService(fromPosition, toPosition, mode == CZapitClient::MODE_TV ? 1 : 2);

	channelsChanged = true;
	Channels = mode == CZapitClient::MODE_TV ? &(g_bouquetManager->Bouquets[bouquet]->tvChannels) : &(g_bouquetManager->Bouquets[bouquet]->radioChannels);

	selected = toPosition;
	newPosition = toPosition;

	listBox->clearItems();
	paint(false);
}

bool CBEChannelWidget::hasChanged()
{
	return (channelsChanged);
}
