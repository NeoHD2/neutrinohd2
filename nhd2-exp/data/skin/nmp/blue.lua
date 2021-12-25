-- mainmenu

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINMENU_FEATURES, neutrino.NEUTRINO_ICON_FEATURES, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_TIMERLIST_NAME, true, "", neutrino.CTimerList())
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)
	item1:setHint(neutrino.LOCALE_TIMERLIST_NAME)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_USERMENU_ITEM_PLUGINS)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PLUGIN)
	item2:setActionKey(neutrino.CPluginList(), "")
	item2:setHint(neutrino.LOCALE_USERMENU_ITEM_PLUGINS)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)

	m:addItem(item1)
	m:addItem(item2)
	m:integratePlugins(0x40, neutrino.RC_1);
	
	if selected < 0 then
		selected = 0
	end

	m:setSelected(selected)
	
	m:addKey(neutrino.RC_blue, None, "plugins")

	ret = m:exec(None, "")
	
	selected = m:getSelected()
	 
	local actionKey = m:getActionKey()
	local key = m:getActionKey()
	
	if actionKey == "plugins" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "plugins")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





