-- osdsetup

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local fb = neutrino.CFrameBuffer_getInstance()
	local box = neutrino.CBox()
	
	box.iX = fb:getScreenX()
	box.iY = fb:getScreenY()
	box.iWidth = fb:getScreenWidth()
	box.iHeight = fb:getScreenHeight()
	
	local m = neutrino.CWidget(box)
	local w = neutrino.CWindow(box.iX, box.iY, box.iWidth, box.iHeight)
	w:setGradient(neutrino.DARK2LIGHT2DARK)

	-- bg
	bg = neutrino.CCImage(neutrino.CONFIGDIR .. "/skin/KravenHD/background/bg_02.png")
	bg:setPosition(box)
	
	--w:addCCItem(bg)
	
	-- title
	title = neutrino.CCLabel()
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINSETTINGS_OSD));
	title:setPosition(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, 50);
	title:setHAlign(neutrino.CC_ALIGN_CENTER)
	
	w:addCCItem(title)
	
	-- logo
	logo = neutrino.CCImage(neutrino.CONFIGDIR .. "/skin/KravenHD/background/logo.png")
	logo:setPosition(box.iX + box.iWidth/2, box.iY + 50 + 100, box.iWidth/2, 300)
	
	w:addCCItem(logo)
	
	-- time
	local timeBox = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50 + 400, box.iWidth/2, 100)
	timeBox:disablePaintFrame()
	timeBox:enableRepaint()
	
	time = neutrino.CCTime()
	time:setFormat("%d.%m.%Y %H:%M:%S")
	time:setPosition(box.iX + box.iWidth/2, box.iY + 50 + 400, box.iWidth/2, 100);
	time:enableRepaint()
	
	timeBox:addCCItem(time)
	
	local list = neutrino.ClistBox(box.iX + 50, box.iY + 50, box.iWidth/2 - 100, box.iHeight - 100)
	list:disablePaintFrame()
	list:disableScrollBar()
	list:enableItemShadow()


	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_SKIN_SKIN, true, "", neutrino.CSkinManager())
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_THEMES)
	item1:set2lines()
	item1:enableItemShadow()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_COLORMENU_THEMESELECT)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_THEMES)
	item2:setActionKey(neutrino.CThemes(), "")
	item2:set2lines()
	item2:enableItemShadow()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_COLORMENU_MENUCOLORS)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MENUCOLORS)
	item3:setActionKey(neutrino.COSDMenuColorSettings(), "")
	item3:set2lines()
	item3:enableItemShadow()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_COLORSTATUSBAR_HEAD)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_INFOBARCOLORS)
	item4:setActionKey(neutrino.COSDInfoBarColorSettings(), "");
	item4:set2lines()
	item4:enableItemShadow()

	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_LANGUAGE)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LANGUAGE)
	item5:setActionKey(neutrino.CLanguageSettings(), "");
	item5:set2lines()
	item5:enableItemShadow()

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_EPGPLUS_SELECT_FONT_NAME)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FONT)
	item6:setActionKey(neutrino.CFontSettings(), "");
	item6:set2lines()
	item6:enableItemShadow()
	
	item7 = neutrino.ClistBoxItem(neutrino.LOCALE_TIMING_HEAD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDTIMING)
	item7:setActionKey(neutrino.COSDTimingSettings(), "");
	item7:set2lines()
	item7:enableItemShadow()
	
	item8 = neutrino.ClistBoxItem(neutrino.LOCALE_VIDEOMENU_SCREENSETUP)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCREENSETUP)
	item8:setActionKey(neutrino.CScreenSetup(), "");
	item8:set2lines()
	item8:enableItemShadow()
	
	item9 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_MISC)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item9:setActionKey(neutrino.COSDDiverses(), "");
	item9:set2lines()
	item9:enableItemShadow()
	
	item10 = neutrino.ClistBoxItem("Skin Style selection")
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item10:setActionKey(neutrino.CSkinSettings(), "");
	item10:set2lines()
	item10:enableItemShadow()

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	list:addItem(item7)
	list:addItem(item8)
	list:addItem(item9)
	list:addItem(item10)
	
	m:addItem(w)
	m:addItem(timeBox)
	m:addItem(list)
	
	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





