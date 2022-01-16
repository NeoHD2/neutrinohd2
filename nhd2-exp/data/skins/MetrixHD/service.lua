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
	local w = neutrino.CWindow(box.iX + 60, box.iY + 50, box.iWidth/2 - 120, box.iHeight - 100)
	w:setColor(neutrino.convertSetupColor2Color(15,15,15,0))
	
	local w2 = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, box.iHeight/2)
	w2:setColor(neutrino.convertSetupColor2Color(0,25,80,20))
	
	-- title
	title = neutrino.CCLabel()
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_SERVICE));
	title:setPosition(box.iX + box.iWidth/2, box.iY + 50, box.iWidth/2, 50);
	title:setHAlign(neutrino.CC_ALIGN_CENTER)
	
	w2:addCCItem(title)
	
	-- logo
	logo = neutrino.CCImage(neutrino.CONFIGDIR .. "/skins/MetrixHD/picon_default.png")
	logo:setPosition(box.iX + box.iWidth/2, box.iY + 50 + 50 + 20, box.iWidth/2, 240)
	
	w2:addCCItem(logo)
	
	-- date
	local dateBox = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50, box.iWidth/2, (box.iHeight/2 - 150)/2)
	dateBox:setColor(neutrino.convertSetupColor2Color(15,15,15,15))
	
	date = neutrino.CCTime(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50, box.iWidth/2, (box.iHeight/2 - 150)/2)
	date:setFormat("%A %d.%m.%Y")
	
	dateBox:addCCItem(date)
	
	-- time
	local timeBox = neutrino.CWindow(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50 + (box.iHeight/2 - 150)/2, box.iWidth/2, (box.iHeight/2 - 150)/2)
	timeBox:setColor(neutrino.convertSetupColor2Color(15,15,15,15))
	timeBox:enableRepaint()
	
	time = neutrino.CCTime(box.iX + box.iWidth/2, box.iY + 50 + box.iHeight/2 + 50 + (box.iHeight/2 - 150)/2, box.iWidth/2, (box.iHeight/2 - 150)/2)
	time:setFormat("%H:%M:%S")
	time:enableRepaint()
	
	timeBox:addCCItem(time)
	
	local list = neutrino.ClistBox(box.iX + 50, box.iY + 60, box.iWidth/2 - 100, box.iHeight - 120)
	list:paintMainFrame(false)

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_SCANTS)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCANSETTINGS)
	item1:setActionKey(neutrino.CTunerSetup(), "")
	item1:set2lines()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_BOUQUETEDITOR_NAME)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FEATURES)
	item2:setActionKey(neutrino.CBEBouquetWidget(), "")
	item2:set2lines()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_RELOAD)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RELOADCHANNELS)
	item3:setHint(neutrino.LOCALE_HELPTEXT_RELOADCHANNELS)
	item3:setActionKey(None, "reloadchannels");
	item3:set2lines()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_CAM_SETTINGS)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_CICAM)
	item4:setHint(neutrino.LOCALE_HELPTEXT_CAM)
	item4:setActionKey(neutrino.CCAMMenuHandler(), "");
	item4:set2lines()

	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item5:setActionKey(neutrino.CImageInfo(), "")
	item5:set2lines()

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_UPDATE)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SOFTUPDATE)
	item6:setActionKey(neutrino.CUpdateSettings(), "")
	item6:set2lines()

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	
	m:addItem(w)
	m:addItem(dateBox)
	m:addItem(timeBox)
	m:addItem(list)
	m:addItem(w2)

	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	
	local actionKey = list:getActionKey()
	
	if actionKey == "reloadchannels" then
		neutrino.CServiceMenu():exec(None, 'reloadchannels')
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





