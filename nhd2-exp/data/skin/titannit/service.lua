-- service

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
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_SERVICE), neutrino.NEUTRINO_ICON_UPDATE);
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

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_SCANTS)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SCANSETTINGS)
	item1:setActionKey(neutrino.CTunerSetup(), "")

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_BOUQUETEDITOR_NAME)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FEATURES)
	item2:setActionKey(neutrino.CBEBouquetWidget(), "")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_RELOAD)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RELOADCHANNELS)
	item3:setHint(neutrino.LOCALE_HELPTEXT_RELOADCHANNELS)
	item3:setActionKey(None, "reloadchannels");

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_CAM_SETTINGS)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_CICAM)
	item4:setHint(neutrino.LOCALE_HELPTEXT_CAM)
	item4:setActionKey(neutrino.CCAMMenuHandler(), "");

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item5:setActionKey(neutrino.CImageInfo(), "")

	item6 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_UPDATE)
	item6:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SOFTUPDATE)
	item6:setActionKey(neutrino.CUpdateSettings(), "")

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	list:addItem(item6)
	
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
	
	if actionKey == "reloadchannels" then
		neutrino.CNeutrinoApp_getInstance():exec(None, 'reloadchannels')
	end

	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end
	
	return ret
end

main()





