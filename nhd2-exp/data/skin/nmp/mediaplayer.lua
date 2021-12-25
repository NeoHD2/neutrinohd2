-- mediaplayer

local selected = 0

-- main
function main()
	local ret = neutrino.RETURN_REPAINT
	
	local m = neutrino.CMenuWidget(neutrino.LOCALE_MAINMENU_MEDIAPLAYER, neutrino.NEUTRINO_ICON_MULTIMEDIA, 600)
	m:enablePaintFootInfo()
	m:enableShrinkMenu()

	m:integratePlugins(4);

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





