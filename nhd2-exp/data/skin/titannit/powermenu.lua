-- powermenu
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
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_POWERMENU), neutrino.NEUTRINO_ICON_BUTTON_POWER);
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
	list:disableScrollBar()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SLEEPTIMER)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item1:setActionKey(neutrino.CSleepTimerWidget(), "")

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_RESTART)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RESTART)
	item2:setHint(neutrino.LOCALE_HELPTEXT_FEATURES)
	item2:setActionKey(None, "restart")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_STANDBY)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item3:setHint(neutrino.LOCALE_HELPTEXT_SERVICE)
	item3:setActionKey(None, "standby")

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_REBOOT)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REBOOT)
	item4:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item4:setActionKey(None, "reboot")

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SHUTDOWN)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SHUTDOWN)
	item5:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item5:setActionKey(None, "shutdown")

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	
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
	
	if actionKey == "restart" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "restart")
	end
	if actionKey == "standby" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "standby")
	end
	if actionKey == "reboot" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "reboot")
	end
	if actionKey == "shutdown" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "shutdown")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





