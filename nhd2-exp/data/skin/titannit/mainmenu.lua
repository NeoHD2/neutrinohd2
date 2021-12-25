-- mainmenu

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
	m:enablePaintFrame()
	m:setCorner(neutrino.NO_RADIUS, neutrino.CORNER_NONE)
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_HEAD), neutrino.NEUTRINO_ICON_BUTTON_SETUP);
	--head:setMode(neutrino.CC_ALIGN_CENTER)
	head:enablePaintDate()
	head:setFormat("%d.%m.%Y %H:%M:%S")
	head:setRadius('8')
	head:setCorner(neutrino.CORNER_ALL)
	
	local foot = neutrino.CFooters(box.iX + 30, box.iY + 100 + box.iHeight - 200 + 10, box.iWidth - 60, 40)
	foot:setCorner(neutrino.CORNER_ALL)
	foot:setRadius('8')
	
	local btninfo = neutrino.button_label_struct()

	btninfo.button = neutrino.NEUTRINO_ICON_INFO
	btninfo.locale = neutrino.NONEXISTANT_LOCALE
	btninfo.localename = ""
	foot:setButtons(btninfo)
	
	local list = neutrino.ClistBox(box.iX + 30, box.iY + 100, box.iWidth - 60, box.iHeight - 200)

	list:setWidgetType(neutrino.WIDGET_TYPE_CLASSIC)
	list:disableScrollBar()

	item1 = neutrino.CMenuForwarder("EPG / Timer", true, "", None, "epgmenu")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)
	item1:setHint(neutrino.LOCALE_HELPTEXT_TIMERLIST)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_FEATURES)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FEATURES)
	item2:setHint(neutrino.LOCALE_HELPTEXT_FEATURES)
	item2:setActionKey(None, "features")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SERVICE)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SERVICE)
	item3:setHint(neutrino.LOCALE_HELPTEXT_SERVICE)
	item3:setActionKey(None, "service")

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SETTINGS)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SETTINGS)
	item4:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item4:setActionKey(None, "mainsetup");
	item4:setDirectKey(neutrino.RC_2)

	item5 = neutrino.CMenuForwarder("Information")
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_BOXINFO)
	item5:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item5:setActionKey(None, "information");

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_POWERMENU)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_POWERMENU)
	item6:setHint(neutrino.LOCALE_HELPTEXT_POWERMENU)
	item6:setActionKey(None, "powermenu");
	
	item7 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_MEDIAPLAYER)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MEDIAPLAYER)
	item7:setHint(neutrino.LOCALE_HELPTEXT_POWERMENU)
	item7:setActionKey(None, "mediaplayer");

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	list:addItem(item7)
	
	m:addItem(head)
	m:addItem(list)
	m:addItem(foot)
	
	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	 
	local actionKey = list:getActionKey()
	
	if actionKey == "epgmenu" then
		neutrino.CNeutrinoApp_getInstance():startSkin("red")
	end
	if actionKey == "service" then
		neutrino.CNeutrinoApp_getInstance():startSkin("service")
	end
	if actionKey == "mainsetup" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mainsetup")
	end
	if actionKey == "mediaplayer" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mediaplayer")
	end
	if actionKey == "features" then
		neutrino.CNeutrinoApp_getInstance():startSkin("blue")
	end
	if actionKey == "powermenu" then
		neutrino.CNeutrinoApp_getInstance():startSkin("powermenu")
	end
	if actionKey == "information" then
		neutrino.CNeutrinoApp_getInstance():startSkin("information")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





