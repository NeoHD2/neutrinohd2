-- nmp mainmenu

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINMENU_HEAD, neutrino.NEUTRINO_ICON_BUTTON_SETUP, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()
	
	item1 = neutrino.CMenuForwarder("TV-Radio Switch")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TVRADIO_SWITCH)
	item1:setHint(" switch TV/Radio modus")
	item1:setDirectKey(neutrino.RC_red)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)
	item1:setActionKey(None, "tvradioswitch")

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_TIMERLIST_NAME)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)
	item2:setHint(neutrino.LOCALE_HELPTEXT_TIMERLIST)
	item2:setActionKey(neutrino.CTimerList(), "")
	item2:setDirectKey(neutrino.RC_yellow)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_MEDIAPLAYER)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MEDIAPLAYER)
	item3:setHint(neutrino.LOCALE_HELPTEXT_MEDIAPLAYER)
	item3:setActionKey(None, "mediaplayer")
	item3:setDirectKey(neutrino.RC_blue)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_FEATURES)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FEATURES)
	item4:setHint(neutrino.LOCALE_HELPTEXT_FEATURES)
	item4:setActionKey(None, "features");
	item4:setDirectKey(neutrino.RC_1)

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SETTINGS)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SETTINGS)
	item5:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item5:setActionKey(None, "mainsetup");
	item5:setDirectKey(neutrino.RC_2)

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SERVICE)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SERVICE)
	item6:setHint(neutrino.LOCALE_HELPTEXT_SERVICE)
	item6:setActionKey(None, "service");
	item6:setDirectKey(neutrino.RC_3)
	
	item7 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SLEEPTIMER)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item7:setHint(neutrino.LOCALE_HELPTEXT_SLEEPTIMER)
	item7:setActionKey(neutrino.CSleepTimerWidget(), "")
	item7:setIconName(neutrino.NEUTRINO_ICON_BUTTON_TIMER)
	
	item8 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_REBOOT)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REBOOT)
	item8:setHint(neutrino.LOCALE_HELPTEXT_MAINSETTINGS)
	item8:setActionKey(None, "reboot")
	item8:setDirectKey(neutrino.RC_4)
	
	item9 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINMENU_SHUTDOWN)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SHUTDOWN)
	item9:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item9:setActionKey(None, "shutdown")
	item9:setDirectKey(neutrino.RC_standby)
	item9:setIconName(neutrino.NEUTRINO_ICON_BUTTON_POWER)
	
	item10 = neutrino.CMenuForwarder("Information")
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_BOXINFO)
	item10:setHint(neutrino.LOCALE_HELPTEXT_BOXINFO)
	item10:setActionKey(None, "information");
	item10:setDirectKey(neutrino.RC_info)
	item10:setIconName(neutrino.NEUTRINO_ICON_BUTTON_HELP)

	m:addItem(item1)
	m:addItem(item2)
	m:addItem(item3)
	m:addItem(item4)
	m:addItem(neutrino.CMenuSeparator(neutrino.LINE))
	m:addItem(item5)
	m:addItem(item6)
	m:addItem(neutrino.CMenuSeparator(neutrino.LINE))
	m:addItem(item7)
	m:addItem(item8)
	m:addItem(item9)
	m:addItem(neutrino.CMenuSeparator(neutrino.LINE))
	m:addItem(item10)
	
	if selected < 0 then
		selected = 0
	end

	m:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = m:getSelected()
	 
	local actionKey = m:getActionKey()
	
	if actionKey == "epgmenu" then
		neutrino.CNeutrinoApp_getInstance():startSkin("red")
	end
	if actionKey == "service" then
		neutrino.CNeutrinoApp_getInstance():startSkin("service")
	end
	if actionKey == "mainsetup" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mainsetup")
	end
	if actionKey == "tvradioswitch" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "tvradioswitch")
	end
	if actionKey == "mediaplayer" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mediaplayer")
	end
	if actionKey == "features" then
		neutrino.CNeutrinoApp_getInstance():startSkin("blue")
	end
	if actionKey == "powermenu" then
		neutrino.CNeutrinoApp_getInstance():startSkin("powermenu")
	end
	if actionKey == "information" then
		neutrino.CNeutrinoApp_getInstance():startSkin("information")
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





