-- osdsetup

local selected = 0
local nmp = "nmp"

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINSETTINGS_OSD, neutrino.NEUTRINO_ICON_COLORS, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_SKIN_SKIN, true, "", neutrino.CSkinManager())
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_THEMES)
	item1:setHint(neutrino.LOCALE_SKIN_SKIN)
	item1:setIconName(neutrino.NEUTRINO_ICON_BUTTON_RED)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_COLORMENU_THEMESELECT)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_THEMES)
	item2:setActionKey(neutrino.CThemes(), "")
	item2:setHint(neutrino.LOCALE_COLORMENU_THEMESELECT)
	item2:setIconName(neutrino.NEUTRINO_ICON_BUTTON_GREEN)

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_COLORMENU_MENUCOLORS)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_MENUCOLORS)
	item3:setActionKey(neutrino.COSDMenuColorSettings(), "")
	item3:setHint(neutrino.LOCALE_COLORMENU_MENUCOLORS)
	item3:setIconName(neutrino.NEUTRINO_ICON_BUTTON_YELLOW)

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_COLORSTATUSBAR_HEAD)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_INFOBARCOLORS)
	item4:setActionKey(neutrino.COSDInfoBarColorSettings(), "")
	item4:setHint(neutrino.LOCALE_COLORSTATUSBAR_HEAD)
	item4:setIconName(neutrino.NEUTRINO_ICON_BUTTON_BLUE)

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_LANGUAGE)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_LANGUAGE)
	item5:setActionKey(neutrino.CLanguageSettings(), "")
	item5:setHint(neutrino.LOCALE_MAINSETTINGS_LANGUAGE)
	item5:setDirectKey(neutrino.RC_1)

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_EPGPLUS_SELECT_FONT_NAME)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FONT)
	item6:setActionKey(neutrino.CFontSettings(), "")
	item6:setHint(neutrino.LOCALE_EPGPLUS_SELECT_FONT_NAME)
	item6:setDirectKey(neutrino.RC_2)
	
	item7 = neutrino.CMenuForwarder(neutrino.LOCALE_TIMING_HEAD)
	item7:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDTIMING)
	item7:setActionKey(neutrino.COSDTimingSettings(), "")
	item7:setHint(neutrino.LOCALE_COLORMENU_THEMESELECT)
	item7:setDirectKey(neutrino.RC_3)
	
	item8 = neutrino.CMenuForwarder(neutrino.LOCALE_VIDEOMENU_SCREENSETUP)
	item8:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCREENSETUP)
	item8:setActionKey(neutrino.CScreenSetup(), "")
	item8:setHint(neutrino.LOCALE_TIMING_HEAD)
	item8:setDirectKey(neutrino.RC_4)
	
	item9 = neutrino.CMenuForwarder(neutrino.LOCALE_MAINSETTINGS_MISC)
	item9:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item9:setActionKey(neutrino.COSDDiverses(), "")
	item9:setHint(neutrino.LOCALE_MAINSETTINGS_MISC)
	item9:setDirectKey(neutrino.RC_5)
	
	still= ""
	if nmp == "nmp" then
		still = "nmp"
	elseif nmp == "ddt" then
		still = "ddt"
	elseif nmp == "ni" then
		still = "ni"
	end
	item10 = neutrino.CMenuForwarder("NMP Still:", true, still)
	item10:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_OSDSETTINGS)
	item10:setActionKey(None, "nmp")
	item10:setHint("select style: nmp, ddt or ni")

	m:addItem(item1)
	m:addItem(item2)
	m:addItem(item3)
	m:addItem(item4)
	m:addItem(item5)
	m:addItem(item6)
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
	
	if actionKey == "nmp" then
		if nmp == "nmp" then
			nmp = "ddt"
			neutrino.CFrameBuffer_getInstance():setIconBasePath("/home/mohousch/Downloads/ddt_icons/")
			neutrino.CFrameBuffer_getInstance():setHintBasePath("/home/mohousch/Downloads/ddt_icons/")
			neutrino.CNeutrinoApp_getInstance():readSkinConfig(neutrino.CONFIGDIR .. "/skin/nmp/nmp.config")
		elseif nmp == "ddt" then
			nmp = "ni"
			neutrino.CFrameBuffer_getInstance():setIconBasePath("/home/mohousch/Downloads/ni_icons/")
			neutrino.CFrameBuffer_getInstance():setHintBasePath("/home/mohousch/Downloads/ni_icons/")
			neutrino.CNeutrinoApp_getInstance():readSkinConfig(neutrino.CONFIGDIR .. "/skin/nmp/ni.config")
		elseif nmp == "ni" then
			nmp = "nmp"
			neutrino.CFrameBuffer_getInstance():setIconBasePath("/home/mohousch/Downloads/tuxbox_icons/")
			neutrino.CFrameBuffer_getInstance():setHintBasePath("/home/mohousch/Downloads/tuxbox_icons/")
			neutrino.CNeutrinoApp_getInstance():readSkinConfig(neutrino.CONFIGDIR .. "/skin/nmp/nmp.config")
		end
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





