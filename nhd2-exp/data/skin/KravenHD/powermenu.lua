-- powermenu
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
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_POWERMENU));
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
	list:paintMainFrame(false)
	list:paintScrollBar(false)
	list:paintItemShadow(true)

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_SLEEPTIMER)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item1:setActionKey(neutrino.CSleepTimerWidget(), "")
	item1:set2lines()
	item1:enableItemShadow()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_RESTART)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RESTART)
	item2:setHint(neutrino.LOCALE_HELPTEXT_FEATURES)
	item2:setActionKey(None, "restart")
	item2:set2lines()
	item2:enableItemShadow()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_STANDBY)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item3:setHint(neutrino.LOCALE_HELPTEXT_SERVICE)
	item3:setActionKey(None, "standby")
	item3:set2lines()
	item3:enableItemShadow()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_REBOOT)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REBOOT)
	item4:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item4:setActionKey(None, "reboot")
	item4:set2lines()
	item4:enableItemShadow()

	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_SHUTDOWN)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SHUTDOWN)
	item5:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item5:setActionKey(None, "shutdown")
	item5:set2lines()
	item5:enableItemShadow()

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	
	m:addItem(w)
	m:addItem(timeBox)
	m:addItem(list)

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





