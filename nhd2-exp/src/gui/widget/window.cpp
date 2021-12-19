/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: window.cpp 2016.12.12 11:43:30 mohousch Exp $

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

#include <gui/widget/window.h>
#include <system/debug.h>


CWindow::CWindow(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;

	centerPos = false;

	init();
}

CWindow::CWindow(CBox* position)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox = *position;

	centerPos = false;

	init();
}

void CWindow::init()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	frameBuffer = CFrameBuffer::getInstance();

	radius = NO_RADIUS;
	corner = CORNER_NONE;
	bgcolor = COL_MENUCONTENT_PLUS_0;
	gradient = NOGRADIENT;

	enableshadow = false;
	paintFrame = true;

	savescreen = false;
	background = NULL;

	// sanity check
	if(itemBox.iHeight > ((int)frameBuffer->getScreenHeight(true)))
		itemBox.iHeight = frameBuffer->getScreenHeight(true);

	// sanity check
	if(itemBox.iWidth > (int)frameBuffer->getScreenWidth(true))
		itemBox.iWidth = frameBuffer->getScreenWidth(true);

	if(centerPos)
	{
		itemBox.iX = frameBuffer->getScreenX() + ((frameBuffer->getScreenWidth() - itemBox.iWidth) >> 1 );
		itemBox.iY = frameBuffer->getScreenY() + ((frameBuffer->getScreenHeight() - itemBox.iHeight) >> 1 );
	}
	
	//
	itemType = WIDGET_ITEM_WINDOW;
}

void CWindow::saveScreen()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);

	if (savescreen)
	{
		if(background)
		{
			delete[] background;
			background = NULL;
		}

		background = new fb_pixel_t[itemBox.iWidth*itemBox.iHeight];
		
		if(background)
		{
			frameBuffer->saveScreen(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, background);
		}
	}
}

void CWindow::restoreScreen()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	if(background) 
	{
		frameBuffer->restoreScreen(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, background);
	}

	delete[] background;
	background = NULL;
}

void CWindow::enableSaveScreen()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	savescreen = true;
	
	saveScreen();
}

void CWindow::setPosition(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;
	
	init();
}

void CWindow::setPosition(CBox* position)
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	itemBox = *position;
	
	init();
}

void CWindow::paint()
{
	dprintf(DEBUG_NORMAL, "CWindow::%s\n", __FUNCTION__);

	if (paintFrame)
	{
		if(enableshadow)
		{
			// shadow
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, COL_MENUCONTENT_PLUS_6);

			// window Box
			frameBuffer->paintBoxRel(itemBox.iX + 1, itemBox.iY + 1, itemBox.iWidth - 2, itemBox.iHeight - 2, bgcolor, NO_RADIUS, CORNER_NONE, gradient);
		}
		else
		{
			frameBuffer->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, bgcolor, radius, corner, gradient);
		}
	}
		
	//CCItems
	if (hasItem())
	{
		paintCCItems();
	}
}

void CWindow::hide()
{
	dprintf(DEBUG_DEBUG, "CWindow::%s\n", __FUNCTION__);
	
	if( savescreen && background)
		restoreScreen();
	else
		frameBuffer->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);
		
	//
	if (hasItem())
	{
		for (unsigned int count = 0; count < (unsigned int)CCItems.size(); count++) 
		{
			CComponent *CCItem = CCItems[count];

			CCItem->hide();
		}
	}
		
	CFrameBuffer::getInstance()->blit();
}

//
void CWindow::addCCItem(CComponent* CCItem)
{
	CCItems.push_back(CCItem);
	CCItem->setParent(this);
}

void CWindow::paintCCItems()
{
	dprintf(DEBUG_NORMAL, "CWindow::paintCCItems:\n");

	for (unsigned int count = 0; count < (unsigned int)CCItems.size(); count++) 
	{
		CComponent *CCItem = CCItems[count];

		CCItem->paint();
	}
}

void CWindow::refresh(void)
{
	for (unsigned int count = 0; count < (unsigned int)CCItems.size(); count++) 
	{
		if (CCItems[count]->update())
		{
			CCItems[count]->refresh();
		}
	}
}

