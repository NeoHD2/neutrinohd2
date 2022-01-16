--epgmenu

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
	bg = neutrino.CCImage(neutrino.CONFIGDIR .. "/skins/KravenHD/background/bg_02.png")
	bg:setPosition(box)
	
	--w:addCCItem(bg)
	
	-- title
	title = neutrino.CCLabel()
	title:setText("Timer / EPG");
	title:setPosition(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, 50);
	title:setHAlign(neutrino.CC_ALIGN_CENTER)
	
	w:addCCItem(title)
	
	-- logo
	logo = neutrino.CCImage(neutrino.CONFIGDIR .. "/skins/KravenHD/background/logo.png")
	logo:setPosition(box.iX + box.iWidth/2, box.iY + 50 + 100, box.iWidth/2, 300)
	
	w:addCCItem(logo)
	
	-- time
	local timeBox = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50 + 400, box.iWidth/2, 100)
	timeBox:paintMainFrame(false)
	timeBox:enableRepaint()
	
	time = neutrino.CCTime()
	time:setFormat("%d.%m.%Y %H:%M:%S")
	time:setPosition(box.iX + box.iWidth/2, box.iY + 50 + 400, box.iWidth/2, 100);
	time:enableRepaint()
	
	timeBox:addCCItem(time)
	
	local list = neutrino.ClistBox(box.iX + 50, box.iY + 50, box.iWidth/2 - 100, box.iHeight - 100)
	list:paintMainFrame(false)
	list:paintItemShadow(true)

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_EPGMENU_EVENTLIST)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item1:setActionKey(neutrino.CEventListHandler(), "")
	item1:set2lines()
	item1:enableItemShadow()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_EPGMENU_EVENTINFO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RESTART)
	item2:setActionKey(neutrino.CEPGDataHandler(), "")
	item2:set2lines()
	item2:enableItemShadow()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_EPGMENU_EPGPLUS)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item3:setActionKey(neutrino.CEPGplusHandler(), "")
	item3:set2lines()
	item3:enableItemShadow()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_EPGMENU_STREAMINFO)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REBOOT)
	item4:setActionKey(neutrino.CStreamInfo2Handler(), "")
	item4:set2lines()
	item4:enableItemShadow()
	
	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_TIMERLIST_NAME)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)
	item5:setActionKey(neutrino.CTimerList(), "")
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
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





