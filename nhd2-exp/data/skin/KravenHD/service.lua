--[[
neutrino lua skin
]]

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
	title:setText(neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_SERVICE));
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
	list:enableItemShadow()

	item1 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_SCANTS)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCANSETTINGS)
	item1:setActionKey(neutrino.CTunerSetup(), "")
	item1:set2lines()
	item1:enableItemShadow()

	item2 = neutrino.ClistBoxItem(neutrino.LOCALE_BOUQUETEDITOR_NAME)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FEATURES)
	item2:setActionKey(neutrino.CBEBouquetWidget(), "")
	item2:set2lines()
	item2:enableItemShadow()

	item3 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_RELOAD)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RELOADCHANNELS)
	item3:setHint(neutrino.LOCALE_HELPTEXT_RELOADCHANNELS)
	item3:setActionKey(None, "reloadchannels");
	item3:set2lines()
	item3:enableItemShadow()

	item4 = neutrino.ClistBoxItem(neutrino.LOCALE_CAM_SETTINGS)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_CICAM)
	item4:setHint(neutrino.LOCALE_HELPTEXT_CAM)
	item4:setActionKey(neutrino.CCAMMenuHandler(), "");
	item4:set2lines()
	item4:enableItemShadow()

	item5 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item5:setActionKey(neutrino.CImageInfo(), "")
	item5:set2lines()
	item5:enableItemShadow()

	item6 = neutrino.ClistBoxItem(neutrino.LOCALE_SERVICEMENU_UPDATE)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SOFTUPDATE)
	item6:setActionKey(neutrino.CUpdateSettings(), "")
	item6:set2lines()
	item6:enableItemShadow()

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	
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
	
	if actionKey == "service" then
		neutrino.CNeutrinoApp_getInstance():startSkin("service")
	end
	if actionKey == "mediaplayer" then
		neutrino.CNeutrinoApp_getInstance():startSkin("mediaplayer")
	end
	if actionKey == "features" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "plugins")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





