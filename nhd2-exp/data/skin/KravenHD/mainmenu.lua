--[[
neutrino lua skin
]]

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
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_HEAD));
	title:setPosition(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, 50);
	title:setHAlign(neutrino.CC_ALIGN_CENTER)
	
	w:addCCItem(title)
	
	-- logo
	logo = neutrino.CCImage(neutrino.CONFIGDIR .. "/skin/KravenHD/background/logo.png")
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

	item1 = neutrino.ClistBoxItem("EPG / Timer")
	item1:setHint(neutrino.LOCALE_HELPTEXT_TIMERLIST)
	item1:set2lines()
	item1:enableItemShadow()
	item1:setActionKey(None, "red")

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_FEATURES)
	item2:setHint(neutrino.LOCALE_HELPTEXT_FEATURES)
	item2:setActionKey(None, "blue")
	item2:set2lines()
	item2:enableItemShadow()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_SERVICE)
	item3:setHint(neutrino.LOCALE_HELPTEXT_SERVICE)
	item3:setActionKey(None, "service")
	item3:set2lines()
	item3:enableItemShadow()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_SETTINGS)
	item4:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item4:setActionKey(None, "mainsetup");
	item4:set2lines()
	item4:enableItemShadow()

	item5 = neutrino.ClistBoxItem("Information")
	item5:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item5:set2lines()
	item5:enableItemShadow()
	item5:setActionKey(None, "information")

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_POWERMENU)
	item6:setHint(neutrino.LOCALE_HELPTEXT_POWERMENU)
	item6:setActionKey(None, "powermenu");
	item6:set2lines()
	item6:enableItemShadow()
	
	item7 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_MEDIAPLAYER)
	item7:setHint(neutrino.LOCALE_HELPTEXT_POWERMENU)
	item7:setActionKey(None, "mediaplayer");
	item7:set2lines()
	item7:enableItemShadow()

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	list:addItem(item7)
	
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
	
	if actionKey == "mainsetup" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mainsetup")
	end
	if actionKey == "service" then
		neutrino.CNeutrinoApp_getInstance():startSkin("service")
	end
	if actionKey == "mediaplayer" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mediaplayer")
	end
	if actionKey == "blue" then
		neutrino.CNeutrinoApp_getInstance():startSkin("blue")
	end
	if actionKey == "powermenu" then
		neutrino.CNeutrinoApp_getInstance():startSkin("powermenu")
	end
	if actionKey == "information" then
		neutrino.CNeutrinoApp_getInstance():startSkin("information")
	end
	if actionKey == "red" then
		neutrino.CNeutrinoApp_getInstance():startSkin("red")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





