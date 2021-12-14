/*
	$Id: widget.cpp 12.03.2020 mohousch Exp $


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

#include <driver/rcinput.h>
#include <gui/widget/widget.h>
#include <gui/widget/framebox.h>
#include <system/debug.h>


CWidget::CWidget(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_NORMAL, "CWidget::\n");
	
	frameBuffer = CFrameBuffer::getInstance();

	mainFrameBox.iX = x;
	mainFrameBox.iY = y;
	mainFrameBox.iWidth = dx;
	mainFrameBox.iHeight = dy;

	savescreen = false;
	background = NULL;

	enableCenter = false;

	timeout = 0;
	selected = -1;
	sec_timer_interval = 1;

	//
	paintFrame = false;
	backgroundColor = COL_MENUCONTENT_PLUS_0;
	radius = NO_RADIUS;
	corner = CORNER_NONE;

	actionKey = "";
}

CWidget::CWidget(CBox *position)
{
	dprintf(DEBUG_NORMAL, "CWidget::\n");
	
	frameBuffer = CFrameBuffer::getInstance();

	mainFrameBox = *position;

	savescreen = false;
	background = NULL;

	enableCenter = false;

	timeout = 0;
	selected = -1;
	sec_timer_interval = 1;

	//
	paintFrame = false;
	backgroundColor = COL_MENUCONTENT_PLUS_0;
	radius = RADIUS_MID;
	corner = CORNER_ALL;

	actionKey = "";
}

CWidget::~CWidget()
{
	dprintf(DEBUG_NORMAL, "CWidget:: del\n");

	items.clear();
}

void CWidget::addItem(CWidgetItem *widgetItem, const bool defaultselected)
{
	if (defaultselected)
		selected = items.size();
	
	items.push_back(widgetItem);
	widgetItem->setParent(this);
}

bool CWidget::hasItem()
{
	return !items.empty();
}

int CWidget::getItemsCount()
{
	return items.size();
}

void CWidget::clearItems(void)
{
	return items.clear();
}

void CWidget::initFrames()
{
	dprintf(DEBUG_NORMAL, "CWidget::initFrames\n");
	
	// sanity check
	if(mainFrameBox.iHeight > ((int)frameBuffer->getScreenHeight(true)))
		mainFrameBox.iHeight = frameBuffer->getScreenHeight(true);

	// sanity check
	if(mainFrameBox.iWidth > (int)frameBuffer->getScreenWidth(true))
		mainFrameBox.iWidth = frameBuffer->getScreenWidth(true);

	if(enableCenter)
	{
		mainFrameBox.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - mainFrameBox.iWidth) >> 1 );
		mainFrameBox.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - mainFrameBox.iHeight) >> 1 );
	}

	if(savescreen) 
		saveScreen();
}

void CWidget::paintItems()
{
	dprintf(DEBUG_NORMAL, "CWidget:: paintItems\n");

	for (int i = 0; i < items.size(); i++)
	{
		if( (items[i]->isSelectable()) && (selected == -1)) 
		{
			selected = i;
		}

		items[i]->paint();
	}

}

void CWidget::paint()
{
	dprintf(DEBUG_NORMAL, "CWidget:: paint\n");
	
	//
	initFrames();

	// paint mainFrame
	if (paintFrame)
		frameBuffer->paintBoxRel(mainFrameBox.iX, mainFrameBox.iY, mainFrameBox.iWidth, mainFrameBox.iHeight, backgroundColor, radius, corner);

	// paint items
	paintItems();
}

void CWidget::saveScreen()
{
	dprintf(DEBUG_NORMAL, "CWidget::saveScreen\n");
	
	if(!savescreen)
		return;

	if(background)
	{
		delete[] background;
		background = NULL;
	}

	background = new fb_pixel_t[mainFrameBox.iWidth*mainFrameBox.iHeight];
	
	if(background)
	{
		frameBuffer->saveScreen(mainFrameBox.iX, mainFrameBox.iY, mainFrameBox.iWidth, mainFrameBox.iHeight, background);
	}
}

void CWidget::restoreScreen()
{
	dprintf(DEBUG_NORMAL, "CWidget::restoreScreen\n");
	
	if(background) 
	{
		if(savescreen)
			frameBuffer->restoreScreen(mainFrameBox.iX, mainFrameBox.iY, mainFrameBox.iWidth, mainFrameBox.iHeight, background);
	}
}

void CWidget::enableSaveScreen()
{
	savescreen = true;
	
	if(!savescreen && background) 
	{
		delete[] background;
		background = NULL;
	}
}

void CWidget::hide()
{
	dprintf(DEBUG_NORMAL, "CWidget:: hide\n");

	if (hasItem())
	{
		for(unsigned int i = 0; i < items.size(); i++)
		{
			items[i]->hide();
		}
	}		

	if( savescreen && background)
	{
		restoreScreen();
	}
	else
	{
		frameBuffer->paintBackgroundBoxRel(mainFrameBox.iX, mainFrameBox.iY, mainFrameBox.iWidth, mainFrameBox.iHeight);
	}

	frameBuffer->blit();
}

void CWidget::addKey(neutrino_msg_t key, CMenuTarget *menue, const std::string & action)
{
	dprintf(DEBUG_DEBUG, "CWidget::addKey: %s\n", action.c_str());
	
	keyActionMap[key].menue = menue;
	keyActionMap[key].action = action;
}

int CWidget::exec(CMenuTarget *parent, const std::string &)
{
	dprintf(DEBUG_NORMAL, "CWidget:: exec:\n");

	retval = RETURN_REPAINT;
	pos = 0;
	exit_pressed = false;

	if (parent)
		parent->hide();

	// set in focus
	if (hasItem() && items.size() > 1)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if((items[i]->isSelectable()) && (items[i]->inFocus == true))
			{
				selected = i;
				break;
			}
		}
	}

	paint();
	CFrameBuffer::getInstance()->blit();

	// add sec timer
	sec_timer_id = g_RCInput->addTimer(sec_timer_interval*1000*1000, false);
	
	uint64_t timeoutEnd = CRCInput::calcTimeoutEnd(timeout == 0 ? 0xFFFF : timeout);

	//control loop
	do {
		g_RCInput->getMsgAbsoluteTimeout(&msg, &data, &timeoutEnd);
		
		int handled = false;

		dprintf(DEBUG_DEBUG, "CWidget::exec: msg:%s\n", CRCInput::getSpecialKeyName(msg));

		if ( msg <= RC_MaxRC ) 
		{
			timeoutEnd = CRCInput::calcTimeoutEnd(timeout == 0 ? 0xFFFF : timeout);

			// keymap
			std::map<neutrino_msg_t, keyAction>::iterator it = keyActionMap.find(msg);
			
			if (it != keyActionMap.end()) 
			{
				actionKey = it->second.action;

				if (it->second.menue != NULL)
				{
					int rv = it->second.menue->exec(this, it->second.action);

					//FIXME:review this
					switch ( rv ) 
					{
						case RETURN_EXIT_ALL:
							retval = RETURN_EXIT_ALL; //fall through
						case RETURN_EXIT:
							msg = RC_timeout;
							break;
						case RETURN_REPAINT:
							//hide(); // for lua???
							//initFrames();
							paint();
							break;
					}
				}
				else
				{
					selected = -1;
					handled = true;

					break;
				}

				frameBuffer->blit();
				continue;
			}
		}

		if (!handled) 
		{
			if ( (msg == NeutrinoMessages::EVT_TIMER) && (data == sec_timer_id) )
			{
				// update time
				for (unsigned int i = 0; i < items.size(); i++)
				{
					if( (items[i]->itemType == WIDGET_ITEM_HEAD) /*&& (items[i]->paintDate)*/ )
					{
						items[i]->paint();
					}
				}
			} 

			switch (msg) 
			{
				case (NeutrinoMessages::EVT_TIMER):
					if ( CNeutrinoApp::getInstance()->handleMsg( msg, data ) & messages_return::cancel_all ) 
					{
						retval = RETURN_EXIT_ALL;
						msg = RC_timeout;
					}
					break;

				//
				case (RC_up):
					onUpKeyPressed();
					break;

				case (RC_down):
					onDownKeyPressed();
					break;

				case (RC_right):
					onRightKeyPressed();
					break;

				case (RC_left):
					onLeftKeyPressed();
					break;

				case (RC_page_up):
					onPageUpKeyPressed();
					break;

				case (RC_page_down):
					onPageDownKeyPressed();
					break;

				case (RC_yellow):
					onYellowKeyPressed();
					break;

				case (RC_home):
					onHomeKeyPressed();
					break;

				case (RC_ok):
					onOKKeyPressed();
					break;
					
				case (RC_timeout):
					exit_pressed = true;
					selected = -1;
					break;

				default:
					if ( CNeutrinoApp::getInstance()->handleMsg( msg, data ) & messages_return::cancel_all ) 
					{
						retval = RETURN_EXIT_ALL;
						msg = RC_timeout;
					}
			}

			if ( msg <= RC_MaxRC )
			{
				// recalculate timeout for RC-Tasten
				timeoutEnd = CRCInput::calcTimeoutEnd(timeout == 0 ? 0xFFFF : timeout);
			}
		}
		
		frameBuffer->blit();
	}
	while ( msg != RC_timeout );

	dprintf(DEBUG_NORMAL, "CWidget: retval: (%d) selected:%d\n", retval, selected);
	
	hide();

	//
	g_RCInput->killTimer(sec_timer_id);
	sec_timer_id = 0;	

	// vfd
	if(!parent)
	{
		if(CNeutrinoApp::getInstance()->getMode() == NeutrinoMessages::mode_webtv)
			CVFD::getInstance()->setMode(CVFD::MODE_WEBTV);
		else
			CVFD::getInstance()->setMode(CVFD::MODE_TVRADIO);
	}
	
	return retval;
}

// events
void CWidget::onOKKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onOKKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
		if (items[selected]->hasItem() && items[selected]->isSelectable())
		{
			int rv = items[selected]->oKKeyPressed(this);

			actionKey = items[selected]->getActionKey();

			//FIXME:review this
			switch ( rv ) 
			{
				case RETURN_EXIT_ALL:
					retval = RETURN_EXIT_ALL; //fall through
				case RETURN_EXIT:
					msg = RC_timeout;
					break;
				case RETURN_REPAINT:
					//hide(); // for lua???
					//initFrames();
					paint();
					break;
			}
		}
	}
}

void CWidget::onHomeKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onHomeKeyPressed\n");
	
	exit_pressed = true;
	dprintf(DEBUG_NORMAL, "CWidget::exec: exit_pressed\n");
	msg = RC_timeout;
	
	if (hasItem())
	{
		for (unsigned int count = 0; count < items.size(); count++) 
		{
			items[count]->homeKeyPressed();
		}
	}

	selected = -1;
}

void CWidget::onYellowKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onYellowKeyPressed\n");
	
	if(hasItem())
	{
		for (unsigned int count = 1; count < items.size(); count++) 
		{
			pos = (selected + count)%items.size();

			CWidgetItem * item = items[pos];

			if(item->isSelectable() && item->hasItem())
			{
				items[selected]->setOutFocus();
				items[selected]->paint();

				selected = pos;

				item->setOutFocus(false);
				item->paint();

				break;
			}
		}
	}
}

void CWidget::onUpKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onUpKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
#if 0
		if( ((items[selected]->itemType == WIDGET_ITEM_FRAMEBOX) ) /*|| ((items[selected]->itemType == WIDGET_ITEM_LISTBOX) && (items[selected]->getWidgetType() == WIDGET_TYPE_FRAME) )*/ )
		{
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = (selected - count)%items.size();

				if ( pos < 0 )
					pos += items.size();

				CWidgetItem * item = items[pos];

				if(item->isSelectable() && item->hasItem())
				{
					items[selected]->setOutFocus();
					items[selected]->paint();

					item->setOutFocus(false);
					item->paint();

					selected = pos;

					break;
				}
			}
		}
		else
#endif
			items[selected]->scrollLineUp();
	}
}

void CWidget::onDownKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onDownKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
#if 0
		if( ((items[selected]->itemType == WIDGET_ITEM_FRAMEBOX) ) /*|| ((items[selected]->itemType == WIDGET_ITEM_LISTBOX) && (items[selected]->getWidgetType() == WIDGET_TYPE_FRAME) )*/ )
		{
			//onYellowKeyPressed();
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = (selected + count)%items.size();

				if (pos >= (int)items.size())
					pos = 0;

				printf("CWidget::onDownKeyPressed: (pos:%d) (selected:%d)\n", pos, selected);

				CWidgetItem * item = items[pos];

				if(item->isSelectable())
				{
					items[selected]->setOutFocus();
					items[selected]->paint();

					item->setOutFocus(false);
					item->paint();

					selected = pos;

					break;
				}
			}
		}
		else
#endif
		{
			items[selected]->scrollLineDown();
		}
	}
}

void CWidget::onRightKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onRightKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
		if( (items[selected]->itemType == WIDGET_ITEM_LISTBOX) && ( (items[selected]->getWidgetType() != WIDGET_TYPE_FRAME) && (items[selected]->getWidgetType() != WIDGET_TYPE_EXTENDED)) )
		{
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = (selected + count)%items.size();

				CWidgetItem * item = items[pos];

				if(item->isSelectable() && item->hasItem())
				{
					items[selected]->setOutFocus();
					items[selected]->paint();

					selected = pos;

					item->setOutFocus(false);
					item->paint();

					break;
				}
			}
		}
		else
			items[selected]->swipRight();
	}
}

void CWidget::onLeftKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onLeftKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
		if( (items[selected]->itemType == WIDGET_ITEM_LISTBOX) && ((items[selected]->getWidgetType() != WIDGET_TYPE_FRAME) && (items[selected]->getWidgetType() != WIDGET_TYPE_EXTENDED)) )
		{
			for (unsigned int count = 1; count < items.size(); count++) 
			{
				pos = (selected - count)%items.size();

				CWidgetItem * item = items[pos];

				if(item->isSelectable() && item->hasItem())
				{
					items[selected]->setOutFocus();
					items[selected]->paint();

					selected = pos;

					item->setOutFocus(false);
					item->paint();

					break;
				}
			}
		}
		else
			items[selected]->swipLeft();
	}
}

void CWidget::onPageUpKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onPageUpKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
		items[selected]->scrollPageUp();
	}
}

void CWidget::onPageDownKeyPressed()
{
	dprintf(DEBUG_NORMAL, "CWidget::onPageDownKeyPressed\n");
	
	if(hasItem() && selected >= 0)
	{
		items[selected]->scrollPageDown();
	}
}



