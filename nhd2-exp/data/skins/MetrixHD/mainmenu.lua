--[[
openATV MetrixHD mainmenu
]]

local selected = 0

function main()
	local ret = neutrino.RETURN_REPAINT
	
	local fb = neutrino.CFrameBuffer_getInstance()
	local box = neutrino.CBox()
	
	box.iX = fb:getScreenX()
	box.iY = fb:getScreenY()
	box.iWidth = fb:getScreenWidth()
	box.iHeight = fb:getScreenHeight()
	
	local config = neutrino.CConfigFile('\t')
	config:loadConfig(neutrino.CONFIGDIR .. "/skin/MetrixHD/MetrixHD.config")
	
	local m = neutrino.CWidget(box)
	m.id = neutrino.WIDGET_MAINMENU
	m.name = "mainmenu"
	
	-- rightWindow / logo 
	local w2 = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, box.iHeight/2)
	w2:setColor(neutrino.convertSetupColor2Color(config:getInt32("menu_Content_Selected_red"),config:getInt32("menu_Content_Selected_green"),config:getInt32("menu_Content_Selected_blue"),config:getInt32("menu_Content_Selected_alpha")))
	--w2:setGradient(neutrino.DARK2LIGHT2DARK)
	
	-- title
	title = neutrino.CCLabel()
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_HEAD));
	title:setPosition(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, 50);
	title:setHAlign(neutrino.CC_ALIGN_CENTER)
	title:setColor(neutrino.COL_MENUHEAD)
	
	w2:addCCItem(title)
	
	-- logo
	--logo = neutrino.CCImage(neutrino.CONFIGDIR .. "/skin/MetrixHD/picon_default.png")
	logo = neutrino.CCImage(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/menu_epg_refresh.png")
	logo:setPosition(box.iX + box.iWidth/2, box.iY + 50 + 50 + 20, box.iWidth/2, 240)
	
	w2:addCCItem(logo)
	
	-- date
	local dateBox = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50, box.iWidth/2, (box.iHeight/2 - 150)/2)
	--dateBox:setColor(neutrino.convertSetupColor2Color(15,15,15,15))
	--dateBox:setGradient(neutrino.DARK2LIGHT)
	
	date = neutrino.CCTime(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50, box.iWidth/2, (box.iHeight/2 - 150)/2)
	date:setFormat("%A %d.%m.%Y")
	date:setColor(neutrino.COL_MENUHEAD)
	
	dateBox:addCCItem(date)
	
	-- time
	local timeBox = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50 + (box.iHeight/2 - 150)/2, box.iWidth/2, (box.iHeight/2 - 150)/2)
	--timeBox:setGradient(neutrino.LIGHT2DARK)
	timeBox:enableRepaint()
	
	time = neutrino.CCTime(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50 + (box.iHeight/2 - 150)/2, box.iWidth/2, (box.iHeight/2 - 150)/2)
	time:setFormat("%H:%M:%S")
	time:enableRepaint()
	
	timeBox:addCCItem(time)
	
	local w = neutrino.CWindow(box.iX + 60, box.iY + 50, box.iWidth/2 - 120, box.iHeight - 100)
	local list = neutrino.ClistBox(box.iX + 50, box.iY + 60, box.iWidth/2 - 100, box.iHeight - 120)
	list:paintMainFrame(false)
	list:enablePaintFootInfo(0)
	list:setFootInfoMode(neutrino.FOOT_HINTICON_MODE)
	list:setItemInfoPos(box.iX + box.iWidth/2, box.iY + 50 + 50 + 20, box.iWidth/2, 240)
	list:paintItemInfoFrame(true)	list:setItemInfoColor(neutrino.convertSetupColor2Color(config:getInt32("menu_Content_Selected_red"),config:getInt32("menu_Content_Selected_green"),config:getInt32("menu_Content_Selected_blue"),config:getInt32("menu_Content_Selected_alpha")))
	--list:setItemInfoScaling(true)
	

	item1 = neutrino.ClistBoxItem("EPG / Timer")
	item1:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/timerepg.png")
	item1:setHint(neutrino.LOCALE_HELPTEXT_TIMERLIST)
	item1:set2lines()
	item1:setActionKey(None, "red")

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_FEATURES)
	item2:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/features.png")
	item2:setHint(neutrino.LOCALE_HELPTEXT_FEATURES)
	item2:setActionKey(None, "blue")
	item2:set2lines()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_SERVICE)
	item3:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/service.png")
	item3:setHint(neutrino.LOCALE_HELPTEXT_SERVICE)
	item3:setActionKey(None, "service")
	item3:set2lines()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_SETTINGS)
	item4:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/mainsetup.png")
	item4:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item4:setActionKey(None, "mainsetup");
	item4:set2lines()

	item5 = neutrino.ClistBoxItem("Information")
	item5:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/information.png")
	item5:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item5:set2lines()
	item5:setActionKey(None, "information")

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_POWERMENU)
	item6:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/power.png")
	item6:setHint(neutrino.LOCALE_HELPTEXT_POWERMENU)
	item6:setActionKey(None, "powermenu");
	item6:set2lines()
	
	item7 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINMENU_MEDIAPLAYER)
	item7:setItemIcon(neutrino.CONFIGDIR .. "/skin/MetrixHD/buttons/mediaplayer.png")
	item7:setHint(neutrino.LOCALE_HELPTEXT_POWERMENU)
	item7:setActionKey(None, "mediaplayer");
	item7:set2lines()

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	list:addItem(item7)
	
	m:addItem(w)
	m:addItem(dateBox)
	m:addItem(timeBox)
	m:addItem(w2)
	m:addItem(list)

	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)
	
	m:addKey(neutrino.RC_setup, None, "RC_setup")

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	
	local actionKey = list:getActionKey()
	
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
	if actionKey == "mainsetup" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mainsetup")
	end
	
	if m:getExitPressed() ~= true then
		main()
	end

	return ret
end

main()





