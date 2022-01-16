-- mainmenu

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
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_FEATURES));
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

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_TIMERLIST_NAME, true, "", neutrino.CTimerList())
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)
	item1:set2lines()
	item1:enableItemShadow()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_USERMENU_ITEM_PLUGINS)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PLUGIN)
	item2:setActionKey(neutrino.CPluginList(), "")
	item2:set2lines()
	item2:enableItemShadow()

	list:addItem(item1)
	list:addItem(item2)
	list:integratePlugins(0x40, 0, true, neutrino.MODE_MENU, neutrino.WIDGET_TYPE_CLASSIC, true, true);
	
	m:addItem(w)
	m:addItem(timeBox)
	m:addItem(list)
	
	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)
	
	m:addKey(neutrino.RC_blue, None, "plugins")

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	 
	local actionKey = m:getActionKey()
	
	if actionKey == "plugins" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "plugins")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





