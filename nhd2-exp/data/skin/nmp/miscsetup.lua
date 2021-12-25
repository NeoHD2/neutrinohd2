-- miscsetup

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINSETTINGS_MISC, neutrino.NEUTRINO_ICON_BUTTON_SETUP, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_GENERAL, true, "", neutrino.CGeneralSettings(), "video")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_GENERALSETTINGS)
	item1:setHint(neutrino.LOCALE_MISCSETTINGS_GENERAL)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_CHANNELLIST)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_CHANNELLISTSETTINGS)
	item2:setActionKey(neutrino.CChannelListSettings(), "")
	item2:setHint(neutrino.LOCALE_MISCSETTINGS_CHANNELLIST)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_GREEN)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_EPG_HEAD)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_EPGSETTINGS)
	item3:setActionKey(neutrino.CEPGSettings(), "")
	item3:setHint(neutrino.LOCALE_MISCSETTINGS_EPG_HEAD)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_FILEBROWSER)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FILEBROWSERSETTINGS)
	item4:setActionKey(neutrino.CFileBrowserSettings(), "");
	item4:setHint(neutrino.LOCALE_MISCSETTINGS_EPG_HEAD)
	item4:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_ZAPIT)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STARTCHANNELSETTINGS)
	item5:setActionKey(neutrino.CZapitSetup(), "");
	item5:setHint(neutrino.LOCALE_MISCSETTINGS_EPG_HEAD)
	item5:setDirectKey(neutrino.RC_1)

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
	 
	local actionKey = list:getActionKey()
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





