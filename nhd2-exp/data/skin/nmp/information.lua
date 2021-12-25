-- information

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget("Information", neutrino.NEUTRINO_ICON_INFO, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_STREAMINFO_HEAD)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item1:setActionKey(neutrino.CStreamInfo2Handler(), "")
	item1:setHint(neutrino.LOCALE_STREAMINFO_HEAD)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_DBOXINFO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RESTART)
	item2:setActionKey(neutrino.CDBoxInfoWidget(), "")
	item2:setHint(neutrino.LOCALE_DBOXINFO)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_GREEN)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item3:setActionKey(neutrino.CImageInfo(), "")
	item3:setHint(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)

	m:addItem(item1)
	m:addItem(item2)
	m:addItem(item3)
	m:integratePlugins(2, neutrino.RC_1);

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





