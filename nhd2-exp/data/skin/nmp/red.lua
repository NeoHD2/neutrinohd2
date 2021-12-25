--epgmenu

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget("EPG / Timer", neutrino.NEUTRINO_ICON_TIMER, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_EPGMENU_EVENTLIST)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item1:setActionKey(neutrino.CEventListHandler(), "")
	item1:setHint(neutrino.LOCALE_EPGMENU_EVENTLIST)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)
	item1:setDirectKey(neutrino.RC_red)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_EPGMENU_EVENTINFO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RESTART)
	item2:setActionKey(neutrino.CEPGDataHandler(), "")
	item2:setHint(neutrino.LOCALE_EPGMENU_EVENTINFO)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_GREEN)
	item2:setDirectKey(neutrino.RC_green)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_EPGMENU_EPGPLUS)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item3:setActionKey(neutrino.CEPGplusHandler(), "")
	item3:setHint(neutrino.LOCALE_EPGMENU_EPGPLUS)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)
	item3:setDirectKey(neutrino.RC_yellow)

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_EPGMENU_STREAMINFO)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_REBOOT)
	item4:setActionKey(neutrino.CStreamInfo2Handler(), "")
	item4:setHint(neutrino.LOCALE_EPGMENU_STREAMINFO)
	item4:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)
	item4:setDirectKey(neutrino.RC_blue)
	
	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_TIMERLIST_NAME)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)
	item5:setActionKey(neutrino.CTimerList(), "")
	item5:setHint(neutrino.LOCALE_TIMERLIST_NAME)
	item5:setIconName(neutrino.NEUTRINO_ICON_BUTTON_TIMER)

	m:addItem(item1)
	m:addItem(item2)
	m:addItem(item3)
	m:addItem(item4)
	m:addItem(item5)

	if selected < 0 then
		selected = 0
	end

	m:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = m:getSelected()
	
	local actionKey = m:getActionKey()
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





