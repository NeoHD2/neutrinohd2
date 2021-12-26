-- mediaplayer

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
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_MEDIAPLAYER), neutrino.NEUTRINO_ICON_MULTIMEDIA);
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
	
	local list = neutrino.ClistBox(box.iX + 50, box.iY + 100, box.iWidth - 100, box.iHeight - 200)

	list:setWidgetType(neutrino.WIDGET_TYPE_CLASSIC)
	list:disableScrollBar()

	list:integratePlugins(4);
	
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





