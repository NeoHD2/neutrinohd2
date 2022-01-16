-- osdsetup

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local fb = neutrino.CFrameBuffer_getInstance()
	local box = neutrino.CBox()
	
	box.iX = fb:getScreenX(true)
	box.iY = fb:getScreenY(true)
	box.iWidth = 700
	box.iHeight = fb:getScreenHeight(true)
	
	local m = neutrino.CWidget(box)
	m:paintMainFrame(true)
	m:setCorner(neutrino.NO_RADIUS, neutrino.CORNER_NONE)
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINSETTINGS_OSD), neutrino.NEUTRINO_ICON_COLORS);
	head:setHAlign(neutrino.CC_ALIGN_CENTER)
	head:setRadius('8')
	head:setCorner(neutrino.CORNER_ALL)
	
	local foot = neutrino.CFooters(box.iX + 30, box.iY + 100 + box.iHeight - 200 + 10, box.iWidth - 60, 40)
	foot:setRadius('8')
	foot:setCorner(neutrino.CORNER_ALL)
	
	local btninfo = neutrino.button_label_struct()

	btninfo.button = neutrino.NEUTRINO_ICON_INFO
	btninfo.locale = neutrino.NONEXISTANT_LOCALE
	btninfo.localename = ""
	foot:setButtons(btninfo)
	
	local list = neutrino.ClistBox(box.iX + 30, box.iY + 100, box.iWidth - 60, box.iHeight - 200)

	list:setWidgetType(neutrino.WIDGET_TYPE_CLASSIC)
	list:paintScrollBar(false)

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_SKIN_SKIN, true, "", neutrino.CSkinManager())
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_THEMES)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_COLORMENU_THEMESELECT)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_THEMES)
	item2:setActionKey(neutrino.CThemes(), "")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_COLORMENU_MENUCOLORS)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MENUCOLORS)
	item3:setActionKey(neutrino.COSDMenuColorSettings(), "")

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_COLORSTATUSBAR_HEAD)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_INFOBARCOLORS)
	item4:setActionKey(neutrino.COSDInfoBarColorSettings(), "");

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_LANGUAGE)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LANGUAGE)
	item5:setActionKey(neutrino.CLanguageSettings(), "");

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_EPGPLUS_SELECT_FONT_NAME)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FONT)
	item6:setActionKey(neutrino.CFontSettings(), "");
	
	item7 = neutrino.CMenuForwarder(neutrino.LOCALE_TIMING_HEAD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDTIMING)
	item7:setActionKey(neutrino.COSDTimingSettings(), "");
	
	item8 = neutrino.CMenuForwarder(neutrino.LOCALE_VIDEOMENU_SCREENSETUP)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCREENSETUP)
	item8:setActionKey(neutrino.CScreenSetup(), "");
	
	item9 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_MISC)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item9:setActionKey(neutrino.COSDDiverses(), "");
	
	item10 = neutrino.CMenuForwarder("Skin Style selection")
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item10:setActionKey(neutrino.CSkinSettings(), "");

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
	
	m:addItem(head)
	m:addItem(list)
	m:addItem(foot)
	
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





