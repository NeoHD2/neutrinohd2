-- mainsetup
local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINMENU_SETTINGS, neutrino.NEUTRINO_ICON_SETTINGS, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_VIDEO, true, "", neutrino.CVideoSettings(), "")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_VIDEOSETTINGS)
	item1:setHint(neutrino.LOCALE_MAINSETTINGS_VIDEO)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_AUDIO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOSETTINGS)
	item2:setActionKey(neutrino.CAudioSettings(), "")
	item2:setHint(neutrino.LOCALE_MAINSETTINGS_AUDIO)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_GREEN)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_PARENTALLOCK_PARENTALLOCK)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PARENTALLOCKSETTINGS)
	item3:setActionKey(neutrino.CParentalLockSettings(), "")
	item3:setHint(neutrino.LOCALE_PARENTALLOCK_PARENTALLOCK)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_NETWORK)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_NETWORKSETTINGS)
	item4:setActionKey(neutrino.CNetworkSettings_getInstance(), "")
	item4:setHint(neutrino.LOCALE_MAINSETTINGS_NETWORK)
	item4:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_RECORDING)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RECORDINGSETTINGS)
	item5:setActionKey(neutrino.CRecordingSettings(), "");
	item5:setHint(neutrino.LOCALE_MAINSETTINGS_RECORDING)
	item5:setDirectKey(neutrino.RC_1)

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_STREAMING)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MOVIEPLAYERSETTINGS)
	item6:setActionKey(neutrino.CMoviePlayerSettings(), "");
	item6:setHint(neutrino.LOCALE_MAINSETTINGS_STREAMING)
	item6:setDirectKey(neutrino.RC_2)
	
	item7 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_OSD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item7:setActionKey(None, "osdsetup");
	item7:setHint(neutrino.LOCALE_MAINSETTINGS_STREAMING)
	item7:setDirectKey(neutrino.RC_3)
	
	item8 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_LCD)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LCDSETTINGS)
	item8:setActionKey(neutrino.CLCDSettings(), "");
	item8:setHint(neutrino.LOCALE_MAINSETTINGS_LCD)
	item8:setDirectKey(neutrino.RC_4)
	
	item9 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_KEYBINDING)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REMOTECONTROLSETTINGS)
	item9:setActionKey(neutrino.CRemoteControlSettings(), "");
	item9:setHint(neutrino.LOCALE_MAINSETTINGS_KEYBINDING)
	item9:setDirectKey(neutrino.RC_5)
	
	item10 = neutrino.CMenuForwarder(neutrino.LOCALE_AUDIOPLAYERSETTINGS_GENERAL)
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_AUDIOPLAYERSETTINGS)
	item10:setActionKey(neutrino.CAudioPlayerSettings(), "");
	item10:setHint(neutrino.LOCALE_AUDIOPLAYERSETTINGS_GENERAL)
	item10:setDirectKey(neutrino.RC_6)
	
	item11 = neutrino.CMenuForwarder(neutrino.LOCALE_PICTUREVIEWERSETTINGS_GENERAL)
	item11:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PICTUREVIEWERSETTINGS)
	item11:setActionKey(neutrino.CPictureViewerSettings(), "");
	item11:setHint(neutrino.LOCALE_PICTUREVIEWERSETTINGS_GENERAL)
	item11:setDirectKey(neutrino.RC_7)
	
	item12 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_MISC)
	item12:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MISCSETTINGS)
	item12:setActionKey(None, "miscsetup");
	item12:setHint(neutrino.LOCALE_MAINSETTINGS_MISC)
	item12:setDirectKey(neutrino.RC_8)
	
	item13 = neutrino.CMenuForwarder(neutrino.LOCALE_HDD_SETTINGS)
	item13:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_HDDSETTINGS)
	item13:setActionKey(neutrino.CHDDMenuHandler(), "");
	item13:setHint(neutrino.LOCALE_HDD_SETTINGS)
	item13:setDirectKey(neutrino.RC_9)

	m:addItem(item1)
	m:addItem(item2)
	m:addItem(item3)
	m:addItem(item4)
	m:addItem(item5)
	m:addItem(item6)
	m:addItem(item7)
	m:addItem(item8)
	m:addItem(item9)
	m:addItem(item10)
	m:addItem(item11)
	m:addItem(item12)
	m:addItem(item13)
	
	if selected < 0 then
		selected = 0
	end

	m:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = m:getSelected()
	 
	local actionKey = m:getActionKey()
	
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





