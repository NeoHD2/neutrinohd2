--[[
	plutotv-vod.lua v1.11

	Copyright (C) 2021 TangoCash
	License: WTFPLv2
]]

plugin_title = "Pluto TV VOD"

json = require "json"

catlist = {}

itemlist = {}
itemlist_details = {}

episodelist = {}
episodelist_details = {}

playback_details = {}

mode = 0;

re_url= ""

function getdata(Url, File)
	if Url == nil then 
		return nil 
	end

	local data = nil

	data = neutrino.getUrlAnswer(Url, "Mozilla/5.0")

	return data
end

-- UTF8 in Umlaute wandeln
function conv_utf8(_string)
	if _string ~= nil then
		_string = string.gsub(_string,"\\u0026","&");
		_string = string.gsub(_string,"\\u00a0"," ");
		_string = string.gsub(_string,"\\u00b4","´");
		_string = string.gsub(_string,"\\u00c4","Ä");
		_string = string.gsub(_string,"\\u00d6","Ö");
		_string = string.gsub(_string,"\\u00dc","Ü");
		_string = string.gsub(_string,"\\u00df","ß");
		_string = string.gsub(_string,"\\u00e1","á");
		_string = string.gsub(_string,"\\u00e4","ä");
		_string = string.gsub(_string,"\\u00e8","è");
		_string = string.gsub(_string,"\\u00e9","é");
		_string = string.gsub(_string,"\\u00f4","ô");
		_string = string.gsub(_string,"\\u00f6","ö");
		_string = string.gsub(_string,"\\u00fb","û");
		_string = string.gsub(_string,"\\u00fc","ü");
		_string = string.gsub(_string,"\\u2013","–");
		_string = string.gsub(_string,"\\u201c","“");
		_string = string.gsub(_string,"\\u201e","„");
		_string = string.gsub(_string,"\\u2026","…");
		_string = string.gsub(_string,"&#038;","&");
		_string = string.gsub(_string,"&#8211;","–");
		_string = string.gsub(_string,"&#8212;","—");
		_string = string.gsub(_string,"&#8216;","‘");
		_string = string.gsub(_string,"&#8217;","’");
		_string = string.gsub(_string,"&#8230;","…");
		_string = string.gsub(_string,"&#8243;","″");
		_string = string.gsub(_string,"<[^>]*>","");
		_string = string.gsub(_string,"\\/","/");
		_string = string.gsub(_string,"\\n","");
	end
	return _string
end

function gen_ids() -- Generation of a random sid 
	local a = string.format("%x", math.random(1000000000,9999999999)) 
	local b = string.format("%x", math.random(1000,9999)) 
	local c = string.format("%x", math.random(1000,9999)) 
	local d = string.format("%x", math.random(10000000000000,99999999999999))
	local id = tostring(a) .. '-' .. tostring(b) .. '-' .. tostring(c) .. '-' .. tostring(d)
	return id
end

function getVideoData(url) -- Generate stream address and evaluate it according to the best resolution
	http = "http://service-stitcher-ipv4.clusters.pluto.tv/stitch/hls/episode/"
	token = "?advertisingId=&appName=web&appVersion=unknown&appStoreUrl=&architecture=&buildVersion=&clientTime=0&deviceDNT=0&deviceId=" .. gen_ids() .. "&deviceMake=Chrome&deviceModel=web&deviceType=web&deviceVersion=unknown&includeExtendedEvents=false&sid=" .. gen_ids() .. "&userId=&serverSideAds=true"

	local data = getdata(http .. url .."/master.m3u8" ..token) -- Calling the generated master.m3u8
	local count = 0
	if data then
		local res = 0
		for band, url2 in data:gmatch(',BANDWIDTH=(%d+).-\n(%d+.-m3u8)') do
			if band and url2 then
				local nr = tonumber(band)
				if nr > res then
					res=nr
					re_url = http .. url .. "/" .. url2 .. token 
				end
			end
		end
	end
end

function get_cat()
	local hint = neutrino.CHintBox(plugin_title, "loading...", neutrino.HINTBOX_WIDTH, neutrino.PLUGINDIR .. "/plutotv/plutotv.png")
	hint:paint()
	
	local r = false

	local c_data = getdata("http://api.pluto.tv/v3/vod/categories?includeItems=true&deviceType=web")
	if c_data then
		local jd = json:decode(c_data)
		if jd then
			for i = 1, jd.totalCategories do
				if jd.categories[i] then
					table.insert(catlist, i, jd.categories[i].name)
					itemlist_details = {}
					for k = 1, #jd.categories[i].items do
						local _duration = 0
						if jd.categories[i].items[k].duration then
							_duration = tonumber(jd.categories[i].items[k].duration) / 1000 / 60
						end
						
						itemlist_details[k] =
						{
							cat  = i;
							item = k;
							name = jd.categories[i].items[k].name;
							desc = jd.categories[i].items[k].description;
							uuid = jd.categories[i].items[k]._id;
							type = jd.categories[i].items[k].type;
							duration = _duration;
							cover = jd.categories[i].items[k].covers[1].url;
							rating = jd.categories[i].items[k].rating;
							genre = jd.categories[i].items[k].genre;
						}
					end
					table.insert(itemlist, i , itemlist_details)
				end
			end
		end
	end
	hint:hide()
end

function cat_menu(_id)
	neutrino.CFileHelpers():createDir("/tmp/plutotv")

	local cm = neutrino.CMenuWidget(catlist[tonumber(_id)], neutrino.PLUGINDIR .. "/plutotv/plutotv.png")
	cm:setWidgetType(neutrino.WIDGET_TYPE_FRAME)
	cm:enablePaintFootInfo()
	cm:setItemsPerPage(6, 2)
	cm:enablePaintDate()
	
	local red = neutrino.button_label_struct()
	red.button = neutrino.NEUTRINO_ICON_BUTTON_RED
	red.locale = neutrino.NONEXISTANT_LOCALE
	red.localename = "Download"

	local green = neutrino.button_label_struct()
	green.button = neutrino.NEUTRINO_ICON_BUTTON_GREEN
	green.locale = neutrino.NONEXISTANT_LOCALE
	green.localename = "Info"
	
	cm:setFooterButtons(red)
	cm:setFooterButtons(green)
	
	cm:addKey(neutrino.RC_red, null, "rec")
	cm:addKey(neutrino.RC_green, null, "info")
	
	for cat, itemlist_detail in pairs (itemlist) do
		if cat == tonumber(_id) then
			local count = 1
			for item, item_detail in pairs(itemlist_detail) do	
				tfile = neutrino.DATADIR .. "/neutrino/icons/nopreview.jpg"
					
				if item_detail.cover ~= nil then
					tfile = "/tmp/plutotv/" .. conv_utf8(item_detail.name) .. ".jpg"
					
					if neutrino.file_exists(tfile) ~= true then
						neutrino.downloadUrl(conv_utf8(item_detail.cover) .. "?h=640&w=480", tfile, "Mozilla/5.0")
					end
				end
					
				playback_details[item] = 
				{
					uuid = item_detail.uuid;
					name = item_detail.name;
					desc = item_detail.desc;
					--cover = item_detail.cover;
					cover = tfile;
					type = item_detail.type;
					duration = item_detail.duration;
					rating = item_detail.rating;
					genre = item_detail.genre;
				}
					
				item = neutrino.ClistBoxItem(conv_utf8(item_detail.name))
				item:setItemIcon(tfile)
				item:setHint(item_detail.desc)
				
				if item_detail.type == "movie" then
					item:setActionKey(null, "info");
				else
					item:setActionKey(null, "season_menu");
				end
					
				cm:addItem(item)
			end
		end
	end
	
	cm:exec(null, "")
	
	local selected = cm:getSelected()
	
	if selected < 0 then
		selected = 0
	end
	
	local actionKey = cm:getActionKey()
	
	if actionKey == "info" then
		title = playback_details[selected + 1].name
		info1 = playback_details[selected + 1].desc
		cover = playback_details[selected + 1].cover
		
		getVideoData(playback_details[selected + 1].uuid)
		
		file = re_url

		movieWidget = neutrino.CMovieInfoWidget()
		movieWidget:setMovie(file, title, info1, "", cover)

		movieWidget:exec(null, "")
	end
	if actionKey == "season_menu" then
		season_menu(playback_details[selected + 1].uuid)
	end
	if actionKey == "rec" then
		httpTool = neutrino.CHTTPTool()
		httpTool:setTitle("Pluto TV VOD downlaoding...")
		
		getVideoData(playback_details[selected + 1].uuid)
		
		config = neutrino.CConfigFile('\t')

		config:loadConfig(neutrino.CONFIGDIR .. "/neutrino.conf")

		local PATH = config:getString("network_nfs_recordingdir") .. "/" .. conv_utf8(playback_details[selected + 1].name) .. ".mp4"
		
		if httpTool:downloadFile(re_url, PATH, 100) == true then
			local infoString = ''
			local m_movieInfo = neutrino.CMovieInfo()

			m_movieInfo:saveMovieInfo(PATH, conv_utf8(playback_details[selected + 1].name), conv_utf8(playback_details[selected + 1].desc), "");
			
			local tfile = config:getString("network_nfs_recordingdir") .. "/" .. playback_details[selected + 1].name .. ".jpg"
			
			neutrino.CFileHelpers():copyFile(playback_details[selected + 1].cover, tfile);
		end
	end
	
	if cm:getExitPressed() ~= true then
		cat_menu(_id)
	end
	
	neutrino.CFileHelpers():removeDir("/tmp/plutotv")
end

function season_menu(_id)
	local h = neutrino.CHintBox(plugin_title, "Suche Episoden ...")
	h:paint()
	local seasons = 1

	local c_data = getdata("http://api.pluto.tv/v3/vod/series/".. _id .."/seasons?includeItems=true&deviceType=web")
	if c_data then
		local jd = json:decode(c_data)
		if jd then
			sm = neutrino.CMenuWidget(jd.name, neutrino.PLUGINDIR .. "/plutotv/plutotv.png")
			
			episodelist = {}
			local count = 1
			for i=1, #jd.seasons do
				item = neutrino.ClistBoxItem("Season "..i)
				item:setActionKey(null, "episode_menu")
				
				sm:addItem(item)
				
				seasons = i
				episodelist_details = {}
				for k = 1, #jd.seasons[i].episodes do
					episodelist_details[k] =
					{
						title = jd.name;
						season = i;
						episode = k;
						name = i.."x"..string.format("%02d",k).." - ".. jd.seasons[i].episodes[k].name;
						desc = jd.seasons[i].episodes[k].description;
						duration = tonumber(jd.seasons[i].episodes[k].duration) / 1000 / 60;
						uuid = jd.seasons[i].episodes[k]._id;
						cover = jd.seasons[i].episodes[k].covers[1].url;
						type = jd.seasons[i].episodes[k].type;
						rating = jd.seasons[i].episodes[k].rating;
						genre = jd.seasons[i].episodes[k].genre;
					}
				end
				table.insert(episodelist, i, episodelist_details)
			end
		end
	end
	
	sm:exec(null, "")
	
	local selected = sm:getSelected()
	local actionKey = sm:getActionKey()
	
	if actionKey == "episode_menu" then
		episode_menu(selected + 1)
	end
	
	if sm:getExitPressed() ~= true then
		season_menu(_id)
	end
	
	h:hide()
end

function episode_menu(s)
	neutrino.CFileHelpers():createDir("/tmp/plutotv")
	
	local em = neutrino.CMenuWidget(episodelist[tonumber(s)][1].title .. " - Season "..s, neutrino.PLUGINDIR .. "/plutotv/plutotv.png")
	em:setWidgetType(neutrino.WIDGET_TYPE_FRAME)
	em:enablePaintFootInfo()
	em:setItemsPerPage(6, 2)
	em:enablePaintDate()
	
	local red = neutrino.button_label_struct()
	red.button = neutrino.NEUTRINO_ICON_BUTTON_RED
	red.locale = neutrino.NONEXISTANT_LOCALE
	red.localename = "Download"

	local green = neutrino.button_label_struct()
	green.button = neutrino.NEUTRINO_ICON_BUTTON_GREEN
	green.locale = neutrino.NONEXISTANT_LOCALE
	green.localename = "Info"
	
	em:setFooterButtons(red)
	em:setFooterButtons(green)
	
	em:addKey(neutrino.RC_red, null, "rec")
	em:addKey(neutrino.RC_green, null, "info")
	
	for season, episodelist_detail in pairs (episodelist) do
		if season == tonumber(s) then
			local count = 1
			for episode, episode_detail in pairs(episodelist_detail) do
				local tfile = neutrino.DATADIR .. "/neutrino/icons/nopreview.jpg"
					
				if episode_detail.cover ~= nil then
					tfile = "/tmp/plutotv/" .. conv_utf8(episode_detail.name) .. ".jpg"
					
					if neutrino.file_exists(tfile) ~= true then
						neutrino.downloadUrl(conv_utf8(episode_detail.cover) .. "?h=640&w=480", tfile, "Mozilla/5.0")
					end
				end
				
				playback_details[episode] = 
				{
					uuid = episode_detail.uuid;
					name = episode_detail.name;
					desc = episode_detail.desc;
					--cover = episode_detail.cover;
					cover = tfile;
					type =  episode_detail.type;
					duration = episode_detail.duration;
					rating = episode_detail.rating;
					genre = episode_detail.genre;
					title = episodelist[tonumber(s)][1].title;
					eptitle = episode_detail.name;
				}
				
				item = neutrino.ClistBoxItem(episode_detail.name)
				item:setItemIcon(tfile)
				item:setHint(episode_detail.desc)
				item:setActionKey(null, "info")
				
				em:addItem(item)
			end
		end
	end
	
	em:exec(null, "")
	
	local selected = em:getSelected()
	local actionKey = em:getActionKey()
	
	if actionKey == "info" then
		title = playback_details[selected + 1].name
		info1 = playback_details[selected + 1].desc
		cover = playback_details[selected + 1].cover
		
		getVideoData(playback_details[selected + 1].uuid)
		
		file = re_url

		movieWidget = neutrino.CMovieInfoWidget()
		movieWidget:setMovie(file, title, info1, "", cover)

		movieWidget:exec(null, "")
	end
	if actionKey == "rec" then
		httpTool = neutrino.CHTTPTool()
		httpTool:setTitle("Pluto TV VOD downlaoding...")
		
		getVideoData(playback_details[selected + 1].uuid)
		
		config = neutrino.CConfigFile('\t')

		config:loadConfig(neutrino.CONFIGDIR .. "/neutrino.conf")

		local PATH = config:getString("network_nfs_recordingdir") .. "/" .. conv_utf8(playback_details[selected + 1].name) .. ".mp4"
		
		if httpTool:downloadFile(re_url, PATH, 100) == true then
			local infoString = ''
			local m_movieInfo = neutrino.CMovieInfo()

			m_movieInfo:saveMovieInfo(PATH, conv_utf8(playback_details[selected + 1].name), conv_utf8(playback_details[selected + 1].desc), "");
			
			local tfile = config:getString("network_nfs_recordingdir") .. "/" .. playback_details[selected + 1].name .. ".jpg"
			
			neutrino.CFileHelpers():copyFile(playback_details[selected + 1].cover, tfile);
		end
	end
	
	if em:getExitPressed() ~= true then
		episode_menu(s)
	end
	
	neutrino.CFileHelpers():removeDir("/tmp/plutotv")
end

--Menü anzeigen
--function MainMenue()
function main()
	get_cat();
	
	local m = neutrino.CMenuWidget(plugin_title, neutrino.PLUGINDIR .. "/plutotv/plutotv.png")
	m:enableShrinkMenu()
	m:enablePaintDate()
	
	for _id,_name in pairs(catlist) do
		item = neutrino.ClistBoxItem(conv_utf8(_name))
		item:setActionKey(null, "cat_menu")
		
		m:addItem(item)
	end
	
	m:exec(null, "")
	
	local selected = m:getSelected()
	local actionKey = m:getActionKey()
	
	if selected < 0 then
		selected = 0
	end
	
	if actionKey == "cat_menu" then
		cat_menu(selected + 1)
	end
	
	if m:getExitPressed() ~= true then
		main()
	end
end

main()

