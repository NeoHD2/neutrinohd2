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
	local w = neutrino.CWindow(box.iX, box.iY, box.iWidth, box.iHeight)
	w:setGradient(neutrino.DARK2LIGHT2DARK)

	-- bg
	bg = neutrino.CCImage(neutrino.CONFIGDIR .. "/skin/KravenHD/background/bg_02.png")
	bg:setPosition(box)
	
	--w:addCCItem(bg)
	
	-- title
	title = neutrino.CCLabel()
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_SETTINGS));
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
	list:disablePaintFrame()
	list:disableScrollBar()
	list:enableItemShadow()

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_VIDEO, true, "", neutrino.CVideoSettings(), "")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_VIDEOSETTINGS)
	item1:set2lines()
	item1:enableItemShadow()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_AUDIO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOSETTINGS)
	item2:setActionKey(neutrino.CAudioSettings(), "")
	item2:set2lines()
	item2:enableItemShadow()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_PARENTALLOCK_PARENTALLOCK)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PARENTALLOCKSETTINGS)
	item3:setActionKey(neutrino.CParentalLockSettings(), "")
	item3:set2lines()
	item3:enableItemShadow()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_NETWORK)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_NETWORKSETTINGS)
	item4:setActionKey(neutrino.CNetworkSettings_getInstance(), "");
	item4:set2lines()
	item4:enableItemShadow()

	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_RECORDING)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RECORDINGSETTINGS)
	item5:setActionKey(neutrino.CRecordingSettings(), "");
	item5:set2lines()
	item5:enableItemShadow()

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_STREAMING)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MOVIEPLAYERSETTINGS)
	item6:setActionKey(neutrino.CMoviePlayerSettings(), "");
	item6:set2lines()
	item6:enableItemShadow()
	
	item7 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_OSD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item7:setActionKey(None, "osdsetup");
	item7:set2lines()
	item7:enableItemShadow()
	
	item8 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_LCD)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LCDSETTINGS)
	item8:setActionKey(neutrino.CLCDSettings(), "");
	item8:set2lines()
	item8:enableItemShadow()
	
	item9 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_KEYBINDING)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REMOTECONTROLSETTINGS)
	item9:setActionKey(neutrino.CRemoteControlSettings(), "");
	item9:set2lines()
	item9:enableItemShadow()
	
	item10 = neutrino.ClistBoxItem(neutrino.LOCALE_AUDIOPLAYERSETTINGS_GENERAL)
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOPLAYERSETTINGS)
	item10:setActionKey(neutrino.CAudioPlayerSettings(), "");
	item10:set2lines()
	item10:enableItemShadow()
	
	item11 = neutrino.ClistBoxItem(neutrino.LOCALE_PICTUREVIEWERSETTINGS_GENERAL)
	item11:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PICTUREVIEWERSETTINGS)
	item11:setActionKey(neutrino.CPictureViewerSettings(), "");
	item11:set2lines()
	item11:enableItemShadow()
	
	item12 = neutrino.ClistBoxItem(neutrino.LOCALE_MAINSETTINGS_MISC)
	item12:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MISCSETTINGS)
	item12:setActionKey(None, "miscsetup");
	item12:set2lines()
	item12:enableItemShadow()
	
	item13 = neutrino.ClistBoxItem(neutrino.LOCALE_HDD_SETTINGS)
	item13:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_HDDSETTINGS)
	item13:setActionKey(neutrino.CHDDMenuHandler(), "");
	item13:set2lines()
	item13:enableItemShadow()

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
	m:addItem(timeBox)
	m:addItem(list)
	
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





