-- information

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
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, "Information", neutrino.NEUTRINO_ICON_BUTTON_EPG);
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

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_STREAMINFO_HEAD)
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_SLEEPTIMER)
	item1:setActionKey(neutrino.CStreamInfo2Handler(), "")

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_DBOXINFO)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_RESTART)
	item2:setActionKey(neutrino.CDBoxInfoWidget(), "")

	item3 = neutrino.CMenuForwarder(neutrino.LOCALE_SERVICEMENU_IMAGEINFO)
	item3:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_STANDBY)
	item3:setActionKey(neutrino.CImageInfo(), "")

	list:addItem(item1)
	list:addItem(item2)
	list:addItem(item3)
	list:integratePlugins(2);
	
	m:addItem(head)
	m:addItem(list)
	m:addItem(foot)

	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





