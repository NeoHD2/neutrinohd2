-- mainmenu

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local fb = neutrino.CFrameBuffer_getInstance()
	local box = neutrino.CBox()
	
	box.iX = fb:getScreenX(true)
	box.iY = fb:getScreenY(true)
	box.iWidth = 700 --(fb:getScreenWidth(true) - 100)/2
	box.iHeight = fb:getScreenHeight(true)
	
	local m = neutrino.CWidget(box)
	m:enablePaintFrame()
	m:setCorner(neutrino.NO_RADIUS, neutrino.CORNER_NONE)
	
	local head = neutrino.CHeaders(box.iX + 30, box.iY + 50, box.iWidth - 60, 40, neutrino.g_Locale:getText(neutrino.LOCALE_MAINMENU_FEATURES), neutrino.NEUTRINO_ICON_FEATURES);
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

	item1 = neutrino.CMenuForwarder(neutrino.LOCALE_TIMERLIST_NAME, true, "", neutrino.CTimerList())
	item1:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_TIMERLIST)

	item2 = neutrino.CMenuForwarder(neutrino.LOCALE_USERMENU_ITEM_PLUGINS)
	item2:setItemIcon(neutrino.NEUTRINO_ICON_MENUITEM_PLUGIN)
	item2:setActionKey(neutrino.CPluginList(), "")

	list:addItem(item1)
	list:addItem(item2)
	list:integratePlugins(0x40);
	
	m:addItem(head)
	m:addItem(list)
	m:addItem(foot)
	
	if selected < 0 then
		selected = 0
	end

	list:setSelected(selected)
	
	m:addKey(neutrino.RC_blue, None, "plugins")

	ret = m:exec(None, "")
	
	selected = list:getSelected()
	 
	local actionKey = list:getActionKey()
	local key = m:getActionKey()
	
	if key == "plugins" then
		neutrino.CNeutrinoApp_getInstance():exec(None, "plugins")
	end
	
	if m:getExitPressed() ~= true and ret == neutrino.RETURN_REPAINT then
		main()
	end

	return ret
end

main()





