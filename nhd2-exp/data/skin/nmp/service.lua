-- service

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINMENU_SERVICE, neutrino.NEUTRINO_ICON_UPDATE, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_SCANTS)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCANSETTINGS)
	item1:setActionKey(neutrino.CTunerSetup(), "")
	item1:setHint(neutrino.LOCALE_SERVICEMENU_SCANTS)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_BOUQUETEDITOR_NAME)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FEATURES)
	item2:setActionKey(neutrino.CBEBouquetWidget(), "")
	item2:setHint(neutrino.LOCALE_BOUQUETEDITOR_NAME)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_GREEN)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_RELOAD)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RELOADCHANNELS)
	item3:setHint(neutrino.LOCALE_HELPTEXT_RELOADCHANNELS)
	item3:setActionKey(None, "reloadchannels");
	item3:setHint(neutrino.LOCALE_SERVICEMENU_RELOAD)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_CAM_SETTINGS)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_CICAM)
	item4:setHint(neutrino.LOCALE_HELPTEXT_CAM)
	item4:setActionKey(neutrino.CCAMMenuHandler(), "");
	item4:setHint(neutrino.LOCALE_CAM_SETTINGS)
	item4:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item5:setActionKey(neutrino.CImageInfo(), "")
	item5:setHint(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item5:setDirectKey(neutrino.RC_1)

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_UPDATE)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SOFTUPDATE)
	item6:setActionKey(neutrino.CUpdateSettings(), "")
	item6:setHint(neutrino.LOCALE_SERVICEMENU_UPDATE)
	item6:setDirectKey(neutrino.RC_2)

	m:addItem(item1)
	m:addItem(item2)
	m:addItem(item3)
	m:addItem(item4)
	m:addItem(item5)
	m:addItem(item6)

	if selected < 0 then
		selected = 0
	end

	m:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = m:getSelected()
	
	local actionKey = m:getActionKey()
	
	if actionKey == "reloadchannels" then
		neutrino.CNeutrinoApp_getInstance():exec(None, 'reloadchannels')
	end

	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end
	
	return ret
end

main()





