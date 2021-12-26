-- miscsetup

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
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MISCSETTINGS_HEAD), neutrino.NEUTRINO_ICON_SETTINGS);
	head:setHAlign(neutrino.CC_ALIGN_CENTER)
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

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_GENERAL, true, "", neutrino.CGeneralSettings(), "video")
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_GENERALSETTINGS)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_CHANNELLIST)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_CHANNELLISTSETTINGS)
	item2:setActionKey(neutrino.CChannelListSettings(), "")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_EPG_HEAD)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_EPGSETTINGS)
	item3:setActionKey(neutrino.CEPGSettings(), "")

	item4 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_FILEBROWSER)
	item4:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_FILEBROWSERSETTINGS)
	item4:setActionKey(neutrino.CFileBrowserSettings(), "");

	item5 = neutrino.CMenuForwarder(neutrino.LOCALE_MISCSETTINGS_ZAPIT)
	item5:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STARTCHANNELSETTINGS)
	item5:setActionKey(neutrino.CZapitSetup(), "");

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:addItem(item4)
	list:addItem(item5)
	
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
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





