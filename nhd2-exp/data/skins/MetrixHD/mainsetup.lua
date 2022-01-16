-- mainsetup
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
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_SETTINGS));
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
	list:paintScrollBar(false)

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_VIDEO, true, "", neutrino.CVideoSettings(), "")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_VIDEOSETTINGS)
	item1:set2lines()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_AUDIO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOSETTINGS)
	item2:setActionKey(neutrino.CAudioSettings(), "")
	item2:set2lines()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_PARENTALLOCK_PARENTALLOCK)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PARENTALLOCKSETTINGS)
	item3:setActionKey(neutrino.CParentalLockSettings(), "")
	item3:set2lines()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_NETWORK)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_NETWORKSETTINGS)
	item4:setActionKey(neutrino.CNetworkSettings_getInstance(), "");
	item4:set2lines()

	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_RECORDING)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RECORDINGSETTINGS)
	item5:setActionKey(neutrino.CRecordingSettings(), "");
	item5:set2lines()

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_STREAMING)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MOVIEPLAYERSETTINGS)
	item6:setActionKey(neutrino.CMoviePlayerSettings(), "");
	item6:set2lines()
	
	item7 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_OSD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item7:setActionKey(None, "osdsetup");
	item7:set2lines()
	
	item8 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_LCD)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LCDSETTINGS)
	item8:setActionKey(neutrino.CLCDSettings(), "");
	item8:set2lines()
	
	item9 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_KEYBINDING)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REMOTECONTROLSETTINGS)
	item9:setActionKey(neutrino.CRemoteControlSettings(), "");
	item9:set2lines()
	
	item10 = neutrino.ClistBoxItem(neutrino.LOCALE_AUDIOPLAYERSETTINGS_GENERAL)
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOPLAYERSETTINGS)
	item10:setActionKey(neutrino.CAudioPlayerSettings(), "");
	item10:set2lines()
	
	item11 = neutrino.ClistBoxItem(neutrino.LOCALE_PICTUREVIEWERSETTINGS_GENERAL)
	item11:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PICTUREVIEWERSETTINGS)
	item11:setActionKey(neutrino.CPictureViewerSettings(), "");
	item11:set2lines()
	
	item12 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_MISC)
	item12:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MISCSETTINGS)
	item12:setActionKey(None, "miscsetup");
	item12:set2lines()
	
	item13 = neutrino.ClistBoxItem(neutrino.LOCALE_HDD_SETTINGS)
	item13:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_HDDSETTINGS)
	item13:setActionKey(neutrino.CHDDMenuHandler(), "");
	item13:set2lines()

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
	list:addItem(item11)
	list:addItem(item12)
	list:addItem(item13)
	
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
	
	if actionKey == "miscsetup" then
		neutrino.CNeutrinoApp_getInstance():startSkin("miscsetup")
	end
	if actionKey == "osdsetup" then
		neutrino.CNeutrinoApp_getInstance():startSkin("osdsetup")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





