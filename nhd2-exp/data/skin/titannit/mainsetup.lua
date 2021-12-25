-- mainsetup
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
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINSETTINGS_HEAD), neutrino.NEUTRINO_ICON_SETTINGS);
	head:setMode(neutrino.CC_ALIGN_CENTER)
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

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_VIDEO, true, "", neutrino.CVideoSettings(), "")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_VIDEOSETTINGS)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_AUDIO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOSETTINGS)
	item2:setActionKey(neutrino.CAudioSettings(), "")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_PARENTALLOCK_PARENTALLOCK)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PARENTALLOCKSETTINGS)
	item3:setActionKey(neutrino.CParentalLockSettings(), "")

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_NETWORK)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_NETWORKSETTINGS)
	item4:setActionKey(neutrino.CNetworkSettings_getInstance(), "");

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_RECORDING)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RECORDINGSETTINGS)
	item5:setActionKey(neutrino.CRecordingSettings(), "");

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_STREAMING)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MOVIEPLAYERSETTINGS)
	item6:setActionKey(neutrino.CMoviePlayerSettings(), "");
	
	item7 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_OSD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item7:setActionKey(None, "osdsetup");
	
	item8 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_LCD)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LCDSETTINGS)
	item8:setActionKey(neutrino.CLCDSettings(), "");
	
	item9 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_KEYBINDING)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REMOTECONTROLSETTINGS)
	item9:setActionKey(neutrino.CRemoteControlSettings(), "");
	
	item10 = neutrino.CMenuForwarder(neutrino.LOCALE_AUDIOPLAYERSETTINGS_GENERAL)
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOPLAYERSETTINGS)
	item10:setActionKey(neutrino.CAudioPlayerSettings(), "");
	
	item11 = neutrino.CMenuForwarder(neutrino.LOCALE_PICTUREVIEWERSETTINGS_GENERAL)
	item11:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PICTUREVIEWERSETTINGS)
	item11:setActionKey(neutrino.CPictureViewerSettings(), "");
	
	item12 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_MISC)
	item12:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MISCSETTINGS)
	item12:setActionKey(None, "miscsetup");
	
	item13 = neutrino.CMenuForwarder(neutrino.LOCALE_HDD_SETTINGS)
	item13:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_HDDSETTINGS)
	item13:setActionKey(neutrino.CHDDMenuHandler(), "");

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





