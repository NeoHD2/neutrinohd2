/*
 * $Id: widget_helpers.cpp 27.02.2019 mohousch Exp $
 *
 * (C) 2003 by thegoodguy <thegoodguy@berlios.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <global.h>

#include <driver/color.h>
#include <system/settings.h>
#include <system/debug.h>

#include <gui/widget/widget_helpers.h>
#include <gui/widget/textbox.h>

#include <video_cs.h>


extern cVideo * videoDecoder;

// progressbar
CProgressBar::CProgressBar(int r, int g, int b, bool inv)
{
	frameBuffer = CFrameBuffer::getInstance();
	
	inverse = inv;
	
	red = (double) r;
	green = (double) g;
	yellow = (double) b;
	
	percent = 255;
	
	cc_type = CC_PROGRESSBAR;
}

void CProgressBar::setPosition(const int x, const int y, const int w, const int h)
{
	cCBox.iWidth = w;
	cCBox.iHeight = h;
	cCBox.iX = x;
	cCBox.iY = y;
	
	double div = (double) 100;
	
	if (cCBox.iWidth)
		div = (double) 100 / (double) cCBox.iWidth;

	red = (double) red / (double) div;
	green = (double) green / (double) div;
	yellow = (double) yellow / (double) div;
}

void CProgressBar::paintPCR(unsigned char pcr)
{
	int i, siglen;
	unsigned int posx;
	unsigned int posy;
	unsigned int xpos;
	unsigned int ypos;

	double div;
	uint32_t rgb;
	
	int b = 0;
	
	i = 0;
	xpos = cCBox.iX;
	ypos = cCBox.iY;

	// body
	frameBuffer->paintBoxRel(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight, COL_MENUCONTENT_PLUS_2, NO_RADIUS, CORNER_ALL, g_settings.progressbar_gradient);	//fill passive
	
	if (pcr != percent) 
	{
		if(percent == 255) 
			percent = 0;

		div = (double) 100 / (double) cCBox.iWidth;
		siglen = (double) pcr / (double) div;
		posx = xpos;
		posy = ypos;
		int maxi = siglen;
		int total = cCBox.iWidth;
		int step = 255/total;

		if (pcr > percent) 
		{
			if(g_settings.progressbar_color == 0)
			{
				//red
				for (i = 0; (i < red) && (i < maxi); i++) 
				{
					step = 255/red;

					if(inverse) 
						rgb = COL_GREEN_PLUS_0 + ((unsigned char)(step*i) << 16); // adding red
					else
						rgb = COL_RED_PLUS_0 + ((unsigned char)(step*i) <<  8); // adding green
				
					frameBuffer->paintBoxRel(posx + i, posy, 1, cCBox.iHeight, rgb, NO_RADIUS, CORNER_ALL, g_settings.progressbar_gradient);
				}
	
				//yellow
				for (; (i < yellow) && (i < maxi); i++) 
				{
					step = 255/yellow/2;

					if(inverse) 
						rgb = COL_YELLOW_PLUS_0 - (((unsigned char)step*(b++)) <<  8); // removing green
					else
						rgb = COL_YELLOW_PLUS_0 - ((unsigned char)(step*(b++)) << 16); // removing red
	
					frameBuffer->paintBoxRel(posx + i, posy, 1, cCBox.iHeight, rgb, NO_RADIUS, CORNER_ALL, g_settings.progressbar_gradient);
				}

				//green
				for (; (i < green) && (i < maxi); i++) 
				{
					step = 255/green;

					if(inverse) 
						rgb = COL_YELLOW_PLUS_0 - ((unsigned char) (step*(b++)) <<  8); // removing green
					else
						rgb = COL_YELLOW_PLUS_0 - ((unsigned char) (step*(b++)) << 16); // removing red
				
					frameBuffer->paintBoxRel (posx + i, posy, 1, cCBox.iHeight, rgb, NO_RADIUS, CORNER_ALL, g_settings.progressbar_gradient);
				}
			}
			else
			{
				for(; (i < maxi); i++) 
				{
					frameBuffer->paintBoxRel(posx + i, posy, 1, cCBox.iHeight, COL_MENUCONTENT_PLUS_6, NO_RADIUS, CORNER_ALL, g_settings.progressbar_gradient);
				}
			}
		}
		
		percent = pcr;
	}
}

void CProgressBar::reset()
{
  	percent = 255;
}

// CButtons
void CButtons::setButtons(const struct button_label *button_label, const int button_count)
{
	if (button_count)
	{
		for (int i = 0; i < button_count; i++)
		{
			buttons.push_back(button_label[i]);
		}
	}

	count = buttons.size();	
}

void CButtons::paint()
{
	int buttonWidth = 0;

	count = buttons.size();

	if(count)
	{
		buttonWidth = (cCBox.iWidth - BORDER_LEFT - BORDER_RIGHT)/count;
	
		for (int i = 0; i < count; i++)
		{
			if (buttons[i].button != NULL)
			{
				const char * l_option = NULL;
				int iw = 0;
				int ih = 0;

				CFrameBuffer::getInstance()->getIconSize(buttons[i].button, &iw, &ih);
				int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

				if(buttons[i].localename != NULL)
					l_option = buttons[i].localename;
				else
					l_option = g_Locale->getText(buttons[i].locale);
		
				CFrameBuffer::getInstance()->paintIcon(buttons[i].button, cCBox.iX + BORDER_LEFT + i*buttonWidth, cCBox.iY + (cCBox.iHeight - ih)/2);

				g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(cCBox.iX + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, cCBox.iY + f_h + (cCBox.iHeight - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
			}
		}
	}
}

//
void CButtons::paintFootButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label * const content)
{
	int iw, ih;

	int buttonWidth = 0;
	
	if(count)
	{
		//
		buttonWidth = (dx - BORDER_LEFT - BORDER_RIGHT)/count;

		for (int i = 0; i < count; i++)
		{
			if(content[i].button != NULL)
			{
				std::string l_option("");

				l_option.clear();

				frameBuffer->getIconSize(content[i].button, &iw, &ih);
				int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

				if(content[i].localename != NULL)
					l_option = content[i].localename;
				else
					l_option = g_Locale->getText(content[i].locale);
		
				frameBuffer->paintIcon(content[i].button, x + BORDER_LEFT + i*buttonWidth, y + (dy - ih)/2);

				g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(x + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, y + f_h + (dy - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
			}
		}
	}
}

// head buttons (right)
void CButtons::paintHeadButtons(const int x, const int y, const int dx, const int dy, const unsigned int count, const struct button_label * const content)
{
	int iw[count], ih[count];
	int startx = x + dx - BORDER_RIGHT;
	
	for (int i = 0; i < count; i++)
	{
		if(content[i].button != NULL)
		{
			frameBuffer->getIconSize(content[i].button, &iw[i], &ih[i]);
		
			startx -= (iw[i] + ICON_TO_ICON_OFFSET);

			frameBuffer->paintIcon(content[i].button, startx, y + (dy - ih[i])/2);
		}
	}
}

// scrollBar
void CScrollBar::paint(const int x, const int y, const int dy, const int NrOfPages, const int CurrentPage)
{
	// scrollBar
	CBox cFrameScrollBar;
	CWindow cScrollBarWindow;

	cFrameScrollBar.iX = x;
	cFrameScrollBar.iY = y;
	cFrameScrollBar.iWidth = SCROLLBAR_WIDTH;
	cFrameScrollBar.iHeight = dy;


	cScrollBarWindow.setPosition(&cFrameScrollBar);
	cScrollBarWindow.setColor(COL_MENUCONTENT_PLUS_1);
	cScrollBarWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cScrollBarWindow.paint();
		
	// scrollBar slider
	CBox cFrameSlider;
	CWindow cSliderWindow;	

	cFrameSlider.iX = cFrameScrollBar.iX + 2;
	cFrameSlider.iY = cFrameScrollBar.iY + CurrentPage*(cFrameScrollBar.iHeight/NrOfPages);
	cFrameSlider.iWidth = cFrameScrollBar.iWidth - 4;
	cFrameSlider.iHeight = cFrameScrollBar.iHeight/NrOfPages;

	cSliderWindow.setPosition(&cFrameSlider);
	cSliderWindow.setColor(COL_MENUCONTENT_PLUS_3);
	cSliderWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cSliderWindow.paint();
}

void CScrollBar::paint(CBox* position, const int NrOfPages, const int CurrentPage)
{
	// scrollBar
	CBox cFrameScrollBar;
	CWindow cScrollBarWindow;

	cFrameScrollBar = *position;

	cScrollBarWindow.setPosition(&cFrameScrollBar);
	cScrollBarWindow.setColor(COL_MENUCONTENT_PLUS_1);
	cScrollBarWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cScrollBarWindow.paint();
		
	// scrollBar slider
	CBox cFrameSlider;
	CWindow cSliderWindow;	

	cFrameSlider.iX = cFrameScrollBar.iX + 2;
	cFrameSlider.iY = cFrameScrollBar.iY + CurrentPage*(cFrameScrollBar.iHeight/NrOfPages);
	cFrameSlider.iWidth = cFrameScrollBar.iWidth - 4;
	cFrameSlider.iHeight = cFrameScrollBar.iHeight/NrOfPages;

	cSliderWindow.setPosition(&cFrameSlider);
	cSliderWindow.setColor(COL_MENUCONTENT_PLUS_3);
	cSliderWindow.setCorner(NO_RADIUS, CORNER_ALL);
	cSliderWindow.paint();
}

// detailsLine
CItems2DetailsLine::CItems2DetailsLine()
{
	frameBuffer = CFrameBuffer::getInstance(); 
	paintLines = true; 
	mode = DL_INFO; 
	info1 = "";
	option_info1 = "";
	info2 = "";
	option_info2 = "";
	hint = "";
	icon = "";
	
	cc_type = CC_DETAILSLINE;
}

CItems2DetailsLine::~CItems2DetailsLine()
{
	info1.clear();
	option_info1.clear();
	info2.clear();
	option_info2.clear();
	hint.clear();
	icon.clear();
}

void CItems2DetailsLine::paint(int x, int y, int width, int height, int info_height, int iheight, int iy)
{
	dprintf(DEBUG_INFO, "\nCItems2DetailsLine::paint: x:%d y:%d width:%d height:%d info_height:%d iheight:%d iy:%d\n", x, y, width, height, info_height, iheight, iy);
	
	int xpos  = x - CONNECTLINEBOX_WIDTH;
	int ypos1 = iy;
	int ypos2 = y + height;
	int ypos1a = ypos1 + (iheight/2) - 2;
	int ypos2a = ypos2 + (info_height/2) - 2;
	
	fb_pixel_t col1 = COL_MENUCONTENT_PLUS_6;
	fb_pixel_t col2 = COL_MENUFOOT_INFO_PLUS_0;

	if (paintLines)
	{
		// clear infoBox
		frameBuffer->paintBackgroundBoxRel(xpos, y, CONNECTLINEBOX_WIDTH, height + info_height);

		//
		int fh = iheight > 10 ? iheight - 10 : 5;
			
		// vertical line connected to item	
		frameBuffer->paintBoxRel(xpos + CONNECTLINEBOX_WIDTH - 4, ypos1 + 5, 4, fh, col1);
		CFrameBuffer::getInstance()->paintBoxRel(xpos + CONNECTLINEBOX_WIDTH - 4, ypos1 + 5, 1, fh, col2);
			
		// vertical line connected to infobox	
		frameBuffer->paintBoxRel(xpos + CONNECTLINEBOX_WIDTH - 4, ypos2 + 7, 4, info_height - 14, col1);
		frameBuffer->paintBoxRel(xpos + CONNECTLINEBOX_WIDTH - 4, ypos2 + 7, 1, info_height - 14, col2);			

		// vertical line
		frameBuffer->paintBoxRel(xpos, ypos1a, 4, ypos2a - ypos1a, col1);
		frameBuffer->paintBoxRel(xpos, ypos1a, 1, ypos2a - ypos1a + 4, col2);		

		// Hline (item)
		frameBuffer->paintBoxRel(xpos, ypos1a, 12, 4, col1);
		frameBuffer->paintBoxRel(xpos, ypos1a, 1, 4, col2);
			
		// Hline (infobox)
		frameBuffer->paintBoxRel(xpos, ypos2a, 12, 4, col1);
		frameBuffer->paintBoxRel(xpos, ypos2a, 1, 4, col2);
	}

	// info box shadow
	frameBuffer->paintBoxRel(x, ypos2, width, info_height, col1);

	// infoBox
	frameBuffer->paintBoxRel(x + 2, ypos2 + 2, width - 4, info_height - 4, COL_MENUFOOT_INFO_PLUS_0/*, NO_RADIUS, CORNER_NONE, g_settings.Foot_Info_gradient*/);
	
	//
	int DLx = x +2;
	int DLy = ypos2 + 2;
	int DLwidth = width - 4;
	int DLheight = info_height - 4;
	
	if (mode == DL_INFO)
	{
		// option_info1
		int l_ow1 = 0;
		if(!option_info1.empty())
		{
			l_ow1 = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->getRenderWidth(option_info1.c_str());

			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->RenderString(DLx + DLwidth - BORDER_RIGHT - l_ow1, DLy + (DLheight/2 - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->getHeight(), DLwidth - BORDER_LEFT - BORDER_RIGHT - l_ow1, option_info1.c_str(), COL_MENUFOOT_INFO, 0, true);
		}

		// info1
		int l_w1 = 0;
		if(!info1.empty())
		{
			l_w1 = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->getRenderWidth(info1.c_str());

			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->RenderString(DLx + BORDER_LEFT, DLy + (DLheight/2 - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST]->getHeight(), DLwidth - BORDER_LEFT - BORDER_RIGHT - l_ow1, info1.c_str(), COL_MENUFOOT_INFO, 0, true);
		}

		// option_info2
		int l_ow2 = 0;
		if(!option_info2.empty())
		{
			l_ow2 = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getRenderWidth(option_info2.c_str());

			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->RenderString(DLx + DLwidth - BORDER_RIGHT - l_ow2, DLy + DLheight/2 + (DLheight/2 - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_NUMBER]->getHeight(), DLwidth - BORDER_LEFT - BORDER_RIGHT - l_ow2, option_info2.c_str(), COL_MENUFOOT_INFO, 0, true);
		}

		// info2
		int l_w2 = 0;
		if(!info2.empty())
		{
			l_w2 = g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->getRenderWidth(info2.c_str());

			g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->RenderString (DLx + BORDER_LEFT, DLy + DLheight/2 + (DLheight/2 - g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_CHANNELLIST_DESCR]->getHeight(), DLwidth - BORDER_LEFT - BORDER_RIGHT - l_ow2, info2.c_str(), COL_MENUFOOT_INFO, 0, true); // UTF-8
		}
	}
	else if (mode == DL_HINT)
	{	
		CTextBox Dline(DLx, DLy, DLwidth, DLheight);
		Dline.disablePaintFrame();
		Dline.setMode(AUTO_WIDTH);

		// Hint
		if(!hint.empty())
		{
			Dline.setText(hint.c_str(), !icon.empty()? icon.c_str() : NEUTRINO_ICON_MENUITEM_NOPREVIEW, 100, DLheight - 8, TOP_LEFT);
		}
					
		Dline.paint();
	}	
}


void CItems2DetailsLine::clear(int x, int y, int width, int height, int info_height)
{ 
	// lines
	frameBuffer->paintBackgroundBoxRel(x - CONNECTLINEBOX_WIDTH, y, CONNECTLINEBOX_WIDTH, height + info_height);

	// info box
	frameBuffer->paintBackgroundBoxRel(x, y + height, width, info_height);
}

// Hline
CHline::CHline()
{
	frameBuffer = CFrameBuffer::getInstance();
	color = COL_MENUCONTENTDARK_PLUS_0; 
	cc_type = CC_HLINE;
}

// Vline
CVline::CVline()
{
	frameBuffer = CFrameBuffer::getInstance();
	color = COL_MENUCONTENTDARK_PLUS_0; 
	cc_type = CC_VLINE;
}

// CFrameLine
CFrameLine::CFrameLine()
{
	frameBuffer = CFrameBuffer::getInstance();
	color = COL_WHITE; 
	cc_type = CC_FRAMELINE;
}

// CLabel
CLabel::CLabel()
{
	frameBuffer = CFrameBuffer::getInstance();
	color = COL_MENUCONTENT;
	paintBG = false; 
	utf8 = true; 
	font = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE];
	height = font->getHeight();
	width = 0;
	halign = CC_ALIGN_LEFT;
	
	cc_type = CC_LABEL;
}

void CLabel::paint()
{
	int stringWidth = font->getRenderWidth(label);
	
	if (stringWidth > cCBox.iWidth)
		stringWidth = cCBox.iWidth;
		
	int startPosX = cCBox.iX;
	
	if (halign == CC_ALIGN_CENTER)
		startPosX = cCBox.iX + (cCBox.iWidth >> 1) - (stringWidth >> 1);
	else if (halign == CC_ALIGN_RIGHT)
		startPosX = cCBox.iX + cCBox.iWidth - stringWidth;
	
	font->RenderString(startPosX, cCBox.iY + height + (cCBox.iHeight - height)/2, cCBox.iWidth, label, color, 0, utf8, paintBG);
}

//
CText::CText()
{
	font = g_Font[SNeutrinoSettings::FONT_TYPE_EPG_INFO1];
	mode = AUTO_WIDTH;
	
	cc_type = CC_TEXT;
}

void CText::paint()
{
	CTextBox textBox;
	textBox.setPosition(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight);

	textBox.disablePaintFrame();
	textBox.setMode(mode);
	textBox.setFontText(font);

	// caption
	if(!Text.empty())
	{
		textBox.setText(Text.c_str());
	}
	
	textBox.paint();
}

// grid
CGrid::CGrid(const int x, const int y, const int dx, const int dy)
{
	cCBox.iX = x;
	cCBox.iY = y;
	cCBox.iWidth = dx;
	cCBox.iHeight = dy;

	init();
}

CGrid::CGrid(CBox* position)
{
	cCBox = *position;

	init();
}

void CGrid::init()
{
	frameBuffer = CFrameBuffer::getInstance();

	rgb = COL_NOBEL;
	inter_frame = 15;
	
	//
	cc_type = CC_GRID;
}

void CGrid::setPosition(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_DEBUG, "CGrid::%s\n", __FUNCTION__);
	
	cCBox.iX = x;
	cCBox.iY = y;
	cCBox.iWidth = dx;
	cCBox.iHeight = dy;
}

void CGrid::setPosition(CBox* position)
{
	dprintf(DEBUG_DEBUG, "CGrid::%s\n", __FUNCTION__);
	
	cCBox = *position;
}

void CGrid::paint()
{
	// hlines grid
	for(int count = 0; count < cCBox.iHeight; count += inter_frame)
		frameBuffer->paintHLine(cCBox.iX, cCBox.iX + cCBox.iWidth, cCBox.iY + count, rgb );

	// vlines grid
	for(int count = 0; count < cCBox.iWidth; count += inter_frame)
		frameBuffer->paintVLine(cCBox.iX + count, cCBox.iY, cCBox.iY + cCBox.iHeight, rgb );
}

void CGrid::hide()
{
	frameBuffer->paintBackgroundBoxRel(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight);
	
	CFrameBuffer::getInstance()->blit();
}

// pig
CPig::CPig(const int x, const int y, const int dx, const int dy)
{
	cCBox.iX = x;
	cCBox.iY = y;
	cCBox.iWidth = dx;
	cCBox.iHeight = dy;

	init();
}

CPig::CPig(CBox* position)
{
	cCBox = *position;

	init();
}

void CPig::init()
{
	frameBuffer = CFrameBuffer::getInstance();
	
	//
	cc_type = CC_PIG;
}

void CPig::setPosition(const int x, const int y, const int dx, const int dy)
{
	dprintf(DEBUG_DEBUG, "CPig::%s\n", __FUNCTION__);
	
	cCBox.iX = x;
	cCBox.iY = y;
	cCBox.iWidth = dx;
	cCBox.iHeight = dy;
}

void CPig::setPosition(CBox* position)
{
	dprintf(DEBUG_DEBUG, "CPig::%s\n", __FUNCTION__);
	
	cCBox = *position;
}

void CPig::paint()
{
	frameBuffer->paintBackgroundBoxRel(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight);	
		

	if(videoDecoder)
		videoDecoder->Pig(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight);	
}

void CPig::hide()
{
	if(videoDecoder)  
		videoDecoder->Pig(-1, -1, -1, -1);

	frameBuffer->paintBackgroundBoxRel(cCBox.iX, cCBox.iY, cCBox.iWidth, cCBox.iHeight);
	
	CFrameBuffer::getInstance()->blit();
}

//// widget items
// headers
CHeaders::CHeaders(const int x, const int y, const int dx, const int dy, const char * const title, const char * const icon)
{
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;

	htitle = title;
	hicon = icon;

	bgcolor = COL_MENUHEAD_PLUS_0;
	radius = RADIUS_MID;
	corner = CORNER_TOP;
	gradient = g_settings.Head_gradient;

	paintDate = false;
	hbutton_count	= 0;
	hbutton_labels.clear();
	
	tMode = CC_ALIGN_LEFT;

	itemType = WIDGET_ITEM_HEAD;
}

CHeaders::CHeaders(CBox position, const char * const title, const char * const icon)
{
	itemBox = position;

	htitle = title;
	hicon = icon;

	bgcolor = COL_MENUHEAD_PLUS_0;
	radius = RADIUS_MID;
	corner = CORNER_TOP;
	gradient = g_settings.Head_gradient;

	paintDate = false;
	hbutton_count	= 0;
	hbutton_labels.clear();
	
	tMode = CC_ALIGN_LEFT;

	itemType = WIDGET_ITEM_HEAD;
}

void CHeaders::setButtons(const struct button_label* _hbutton_labels, const int _hbutton_count)		
{
	if (_hbutton_count)
	{
		for (int i = 0; i < _hbutton_count; i++)
		{
			hbutton_labels.push_back(_hbutton_labels[i]);
		}
	}

	hbutton_count = hbutton_labels.size();
}		

void CHeaders::paint()
{
	dprintf(DEBUG_INFO, "CHeaders::paint:\n");
	
	// box
	CFrameBuffer::getInstance()->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, bgcolor, radius, corner, gradient);

	// left icon
	int i_w = 0;
	int i_h = 0;

	if(hicon != NULL)
	{
		CFrameBuffer::getInstance()->getIconSize(hicon, &i_w, &i_h);

		// scale icon
		if(i_h >= itemBox.iHeight)
		{
			i_h = itemBox.iHeight - 2;
			i_w = i_h*1.67;
		}

		CFrameBuffer::getInstance()->paintIcon(hicon, itemBox.iX + BORDER_LEFT, itemBox.iY + (itemBox.iHeight - i_h)/2, 0, true, i_w, i_h);
	}

	// right buttons
	hbutton_count = hbutton_labels.size();

	int iw[hbutton_count], ih[hbutton_count];
	int startx = itemBox.iX + itemBox.iWidth - BORDER_RIGHT;
	int buttonWidth = 0;

	if(hbutton_count)
	{
		for (int i = 0; i < hbutton_count; i++)
		{
			if (hbutton_labels[i].button != NULL)
			{
				CFrameBuffer::getInstance()->getIconSize(hbutton_labels[i].button, &iw[i], &ih[i]);
		
				startx -= (iw[i] + ICON_TO_ICON_OFFSET);
				buttonWidth += iw[i];

				CFrameBuffer::getInstance()->paintIcon(hbutton_labels[i].button, startx, itemBox.iY + (itemBox.iHeight - ih[i])/2);
			}
		}
	}

	// paint time/date
	int timestr_len = 0;
	if(paintDate)
	{
		std::string timestr = getNowTimeStr("%d.%m.%Y %H:%M");
		
		timestr_len = g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getRenderWidth(timestr.c_str(), true); // UTF-8
	
		g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->RenderString(startx - timestr_len, itemBox.iY + (itemBox.iHeight - g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_EVENTLIST_ITEMLARGE]->getHeight(), timestr_len + 1, timestr.c_str(), COL_MENUHEAD, 0, true); 
	}
	
	int startPosX = itemBox.iX + BORDER_LEFT + i_w + ICON_OFFSET;
	int stringWidth = g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getRenderWidth(htitle);
	
	if (tMode == CC_ALIGN_CENTER)
		startPosX = itemBox.iX + (itemBox.iWidth >> 1) - (stringWidth >> 1);

	// title
	g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->RenderString(startPosX, itemBox.iY + (itemBox.iHeight - g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight())/2 + g_Font[SNeutrinoSettings::FONT_TYPE_MENU_TITLE]->getHeight(), itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT - i_w - 2*ICON_OFFSET - buttonWidth - (hbutton_count - 1)*ICON_TO_ICON_OFFSET - timestr_len, htitle, COL_MENUHEAD);
}

void CHeaders::hide()
{
	dprintf(DEBUG_NORMAL, "CHeaders::hide:\n");
	
	CFrameBuffer::getInstance()->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);
}

// footers
CFooters::CFooters(int x, int y, int dx, int dy)
{
	itemBox.iX = x;
	itemBox.iY = y;
	itemBox.iWidth = dx;
	itemBox.iHeight = dy;

	fbuttons.clear();
	fcount = 0;

	fbgcolor = COL_MENUFOOT_PLUS_0;
	fradius = RADIUS_MID;
	fcorner = CORNER_BOTTOM;
	fgradient = g_settings.Foot_gradient;

	itemType = WIDGET_ITEM_FOOT;
}

CFooters::CFooters(CBox position)
{
	itemBox = position;

	fbuttons.clear();
	fcount = 0;

	fbgcolor = COL_MENUFOOT_PLUS_0;
	fradius = RADIUS_MID;
	fcorner = CORNER_BOTTOM;
	fgradient = g_settings.Foot_gradient;

	itemType = WIDGET_ITEM_FOOT;
}

void CFooters::setButtons(const struct button_label *button_label, const int button_count)
{
	if (button_count)
	{
		for (int i = 0; i < button_count; i++)
		{
			fbuttons.push_back(button_label[i]);
		}
	}

	fcount = fbuttons.size();	
}

void CFooters::paint()
{
	dprintf(DEBUG_NORMAL, "CFooters::paint:\n");
	
	// box
	CFrameBuffer::getInstance()->paintBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight, fbgcolor, fradius, fcorner, fgradient);

	int buttonWidth = 0;

	fcount = fbuttons.size();

	if(fcount)
	{
		buttonWidth = (itemBox.iWidth - BORDER_LEFT - BORDER_RIGHT)/fcount;
	
		for (int i = 0; i < fcount; i++)
		{
			if (fbuttons[i].button != NULL)
			{
				const char * l_option = NULL;
				int iw = 0;
				int ih = 0;

				CFrameBuffer::getInstance()->getIconSize(fbuttons[i].button, &iw, &ih);
				int f_h = g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->getHeight();

				if(fbuttons[i].localename != NULL)
					l_option = fbuttons[i].localename;
				else
					l_option = g_Locale->getText(fbuttons[i].locale);
		
				CFrameBuffer::getInstance()->paintIcon(fbuttons[i].button, itemBox.iX + BORDER_LEFT + i*buttonWidth, itemBox.iY + (itemBox.iHeight - ih)/2);

				g_Font[SNeutrinoSettings::FONT_TYPE_INFOBAR_SMALL]->RenderString(itemBox.iX + BORDER_LEFT + iw + ICON_OFFSET + i*buttonWidth, itemBox.iY + f_h + (itemBox.iHeight - f_h)/2, buttonWidth - iw - ICON_OFFSET, l_option, COL_MENUFOOT, 0, true); // UTF-8
			}
		}
	}
}

void CFooters::hide()
{
	dprintf(DEBUG_NORMAL, "CFooters::hide:\n");
	
	CFrameBuffer::getInstance()->paintBackgroundBoxRel(itemBox.iX, itemBox.iY, itemBox.iWidth, itemBox.iHeight);
}

