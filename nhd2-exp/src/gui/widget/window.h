/*
	Neutrino-GUI  -   DBoxII-Project
	
	$Id: window.h 2016.12.12 11:43:30 mohousch Exp $

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

#if !defined(WINDOW_H)
#define WINDOW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <driver/framebuffer.h>
#include <system/settings.h>
#include <driver/color.h>

#include <gui/widget/widget_helpers.h>


// window
class CWindow : public CWidgetItem
{
	private:
		CFrameBuffer* frameBuffer;

		//
		int radius;
		int corner;
		fb_pixel_t bgcolor;
		int gradient;
		
		//
		fb_pixel_t * background;
		bool savescreen;
		void saveScreen();
		void restoreScreen();

		//
		bool enableshadow;
		bool centerPos;
		bool paintFrame;
		
		//
		std::vector<CComponent*> CCItems;

	public:
		CWindow(const int x = 0, const int y = 0, const int dx = DEFAULT_XRES, const int dy = DEFAULT_XRES);
		CWindow(CBox* position);
		virtual ~CWindow(){};

		void init();
		void setPosition(const int x, const int y, const int dx, const int dy);
		void setPosition(CBox* position);
		void setColor(fb_pixel_t col){bgcolor = col;};
		void setCorner(int ra, int co){radius = ra; corner = co;};
		void setGradient(int grad){gradient = grad;};
		void enableShadow(void){enableshadow = true;};
		void enableSaveScreen();

		void enableCenterPos(){centerPos = true; init();};
		void disablePaintFrame(){paintFrame = false;};

		void paint(void);
		void hide(void);
		void refresh(void);
		
		inline CBox getWindowsPos(void){return (itemBox);};
		
		//
		void addCCItem(CComponent* CCItem);
		bool hasItem(){return !CCItems.empty();};
		void clearCCItems(){CCItems.clear();};
		void paintCCItems();
		
		void enableRepaint(){rePaint = true;};
		bool update() const {return rePaint;};
};

#endif


