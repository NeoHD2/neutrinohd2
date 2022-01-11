/*
 * $Id: bouquets.cpp 16.11.2020 mohousch Exp $
 *
 * BouquetManager for zapit - d-box2 linux project
 *
 * (C) 2002 by Simplex    <simplex@berlios.de>,
 *             rasc       <rasc@berlios.de>,
 *             thegoodguy <thegoodguy@berlios.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <map>
#include <set>

#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>    // std::sort
#include <fstream>
#include <iostream>

// tuxbox headers
#include <configfile.h>

#include <bouquets.h>
#include <getservices.h> // LoadServices
#include <sdt.h>
#include <settings.h>
#include <xmlinterface.h>

// system
#include <system/debug.h>
#include <system/helpers.h>

#include <global.h>
#include <neutrino.h>

// curl
#include <curl/curl.h>
#include <curl/easy.h>

#include <dirent.h>

// webtv stuff
#if defined (__USE_FILE_OFFSET64) || defined (_DARWIN_USE_64_BIT_INODE)
typedef struct dirent64 dirent_struct;
#define my_alphasort alphasort64
#define my_scandir scandir64
typedef struct stat64 stat_struct;
#define my_stat stat64
#define my_lstat lstat64
#else
typedef struct dirent dirent_struct;
#define my_alphasort alphasort
#define my_scandir scandir
typedef struct stat stat_struct;
#define my_stat stat
#define my_lstat lstat
#error not using 64 bit file offsets
#endif

extern tallchans allchans;   			//  defined in zapit.cpp
extern CConfigFile config;   			//  defined in zapit.cpp
extern CBouquetManager * g_bouquetManager;	//  defined in zapit.cpp

#define TIMER_START()                                                                   \
        static struct timeval tv, tv2;                                                  \
        static unsigned int msec;                                                       \
        gettimeofday(&tv, NULL)

#define TIMER_STOP(label)                                                               \
        gettimeofday(&tv2, NULL);                                                       \
        msec = ((tv2.tv_sec - tv.tv_sec) * 1000) + ((tv2.tv_usec - tv.tv_usec) / 1000); \
        printf("%s: %u msec\n", label, msec)

#define GET_ATTR(node, name, fmt, arg)                                  \
        do {                                                            \
                char * ptr = xmlGetAttribute(node, name);               \
                if ((ptr == NULL) || (sscanf(ptr, fmt, &arg) <= 0))     \
                        arg = 0;                                        \
        }                                                               \
        while (0)

// CZapitBouquet
CZapitChannel * CZapitBouquet::getChannelByChannelID(const t_channel_id channel_id, const unsigned char serviceType)
{
	CZapitChannel * result = NULL;

	ZapitChannelList * channels = &tvChannels;
	
	switch (serviceType) 
	{
		case ST_RESERVED: // ?
		case ST_DIGITAL_TELEVISION_SERVICE:
		case ST_NVOD_REFERENCE_SERVICE:
		case ST_NVOD_TIME_SHIFTED_SERVICE:
			channels = &tvChannels;
			break;
				
		case ST_DIGITAL_RADIO_SOUND_SERVICE:
			channels = &radioChannels;
			break;

		case ST_WEBTV:
			channels = &webtvChannels;
			break;
	}

	unsigned int i;
	for (i = 0; (i<channels->size()) && ((*channels)[i]->getChannelID() != channel_id); i++){};

	if (i < channels->size())
		result = (*channels)[i];

	if ((serviceType == ST_RESERVED) && (result == NULL)) 
	{
		result = getChannelByChannelID(channel_id, ST_DIGITAL_RADIO_SOUND_SERVICE);
	}

	return result;
}

void CZapitBouquet::sortBouquet(void)
{
	sort(tvChannels.begin(), tvChannels.end(), CmpChannelByChName()); //FIXME:
	sort(radioChannels.begin(), radioChannels.end(), CmpChannelByChName()); //FIXME:
}

void CZapitBouquet::addService(CZapitChannel *newChannel)
{
	switch (newChannel->getServiceType())
	{
		case ST_DIGITAL_TELEVISION_SERVICE:
		case ST_NVOD_REFERENCE_SERVICE:
		case ST_NVOD_TIME_SHIFTED_SERVICE:
			tvChannels.push_back(newChannel);
			break;
			
		case ST_DIGITAL_RADIO_SOUND_SERVICE:
			radioChannels.push_back(newChannel);
			break;

		case ST_WEBTV:
			webtvChannels.push_back(newChannel);
			break;
	}
}

void CZapitBouquet::removeService(CZapitChannel *oldChannel)
{
	if (oldChannel != NULL) 
	{
		ZapitChannelList * channels = &tvChannels;

		switch (oldChannel->getServiceType()) 
		{
			case ST_DIGITAL_TELEVISION_SERVICE:
			case ST_NVOD_REFERENCE_SERVICE:
			case ST_NVOD_TIME_SHIFTED_SERVICE:
				channels = &tvChannels;
				break;

			case ST_DIGITAL_RADIO_SOUND_SERVICE:
				channels = &radioChannels;
				break;
		}
		(*channels).erase(remove(channels->begin(), channels->end(), oldChannel), channels->end());
	}
}

void CZapitBouquet::moveService(const unsigned int oldPosition, const unsigned int newPosition, const unsigned char serviceType)
{
	ZapitChannelList * channels = &tvChannels;

	switch (serviceType) 
	{
		case ST_DIGITAL_TELEVISION_SERVICE:
		case ST_NVOD_REFERENCE_SERVICE:
		case ST_NVOD_TIME_SHIFTED_SERVICE:
			channels = &tvChannels;
			break;
			
		case ST_DIGITAL_RADIO_SOUND_SERVICE:
			channels = &radioChannels;
			break;
	}

	if ((oldPosition < channels->size()) && (newPosition < channels->size())) 
	{
		ZapitChannelList::iterator it = channels->begin();

		advance(it, oldPosition);
		CZapitChannel* tmp = *it;
		channels->erase(it);

		advance(it, newPosition - oldPosition);
		channels->insert(it, tmp);
	}
}

// CBouquetManager
void CBouquetManager::writeBouquetHeader(FILE * bouq_fd, uint32_t i, const char * bouquetName)
{
	fprintf(bouq_fd, "\t<Bouquet name=\"%s\" hidden=\"%d\" locked=\"%d\">\n", bouquetName, Bouquets[i]->bHidden ? 1 : 0, Bouquets[i]->bLocked ? 1 : 0);
}

void CBouquetManager::writeBouquetFooter(FILE * bouq_fd)
{
	fprintf(bouq_fd, "\t</Bouquet>\n");
}

void CBouquetManager::writeBouquetChannels(FILE * bouq_fd, uint32_t i, bool /*bUser*/)
{
	bool write_names = 1;

	for ( unsigned int j = 0; j < Bouquets[i]->tvChannels.size(); j++) 
	{
		if(write_names) 
		{
			fprintf(bouq_fd, "\t\t<S i=\"%x\" n=\"%s\" t=\"%x\" on=\"%x\"/>\n",
				Bouquets[i]->tvChannels[j]->getServiceId(),
				convert_UTF8_To_UTF8_XML(Bouquets[i]->tvChannels[j]->getName().c_str()).c_str(),
				Bouquets[i]->tvChannels[j]->getTransportStreamId(),
				Bouquets[i]->tvChannels[j]->getOriginalNetworkId()
				);
		} 
		else 
		{
			fprintf(bouq_fd, "\t\t<S i=\"%x\" t=\"%x\" on=\"%x\"/>\n",
				Bouquets[i]->tvChannels[j]->getServiceId(),
				Bouquets[i]->tvChannels[j]->getTransportStreamId(),
				Bouquets[i]->tvChannels[j]->getOriginalNetworkId()
				);
		}
	}
	
	for ( unsigned int j = 0; j < Bouquets[i]->radioChannels.size(); j++) 
	{
		if(write_names) 
		{
			fprintf(bouq_fd, "\t\t<S i=\"%x\" n=\"%s\" t=\"%x\" on=\"%x\"/>\n",
				Bouquets[i]->radioChannels[j]->getServiceId(),
				convert_UTF8_To_UTF8_XML(Bouquets[i]->radioChannels[j]->getName().c_str()).c_str(),
				Bouquets[i]->radioChannels[j]->getTransportStreamId(),
				Bouquets[i]->radioChannels[j]->getOriginalNetworkId()
				);
		} 
		else 
		{
			fprintf(bouq_fd, "\t\t<S i=\"%x\" t=\"%x\" on=\"%x\"/>\n",
				Bouquets[i]->radioChannels[j]->getServiceId(),
				Bouquets[i]->radioChannels[j]->getTransportStreamId(),
				Bouquets[i]->radioChannels[j]->getOriginalNetworkId()
				);
		}
	}
}

void CBouquetManager::saveBouquets(void)
{
	FILE * bouq_fd;
	
	bouq_fd = fopen(BOUQUETS_XML, "w");
	fprintf(bouq_fd, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<zapit>\n");
	
	for (unsigned int i = 0; i < Bouquets.size(); i++) 
	{
		if (Bouquets[i] != remainChannels) 
		{
			dprintf(DEBUG_INFO, "CBouquetManager::saveBouquets: name %s user: %d\n", Bouquets[i]->Name.c_str(), Bouquets[i]->bUser);
			
			if(!Bouquets[i]->bUser && !Bouquets[i]->bWebTV)
			{
				writeBouquetHeader(bouq_fd, i, convert_UTF8_To_UTF8_XML(Bouquets[i]->Name.c_str()).c_str());
				writeBouquetChannels(bouq_fd, i);
				writeBouquetFooter(bouq_fd);
			}
		}
	}
	
	fprintf(bouq_fd, "</zapit>\n");
	fdatasync(fileno(bouq_fd));
	fclose(bouq_fd);
	chmod(BOUQUETS_XML, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

void CBouquetManager::saveUBouquets(void)
{
	FILE * ubouq_fd;
	
	ubouq_fd = fopen(UBOUQUETS_XML, "w");
	fprintf(ubouq_fd, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<zapit>\n");
	
	for (unsigned int i = 0; i < Bouquets.size(); i++) 
	{
		if (Bouquets[i] != remainChannels) 
		{
			if(Bouquets[i]->bUser && !Bouquets[i]->bWebTV) 
			{
				writeBouquetHeader(ubouq_fd, i, convert_UTF8_To_UTF8_XML(Bouquets[i]->Name.c_str()).c_str());
				writeBouquetChannels(ubouq_fd, i, true);
				writeBouquetFooter(ubouq_fd);
			}
		}
	}
	
	fprintf(ubouq_fd, "</zapit>\n");
	fdatasync(fileno(ubouq_fd));
	fclose(ubouq_fd);
	chmod(UBOUQUETS_XML, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

void CBouquetManager::saveBouquets(const CZapitClient::bouquetMode bouquetMode, const char * const providerName)
{
	if (bouquetMode == CZapitClient::BM_DELETEBOUQUETS) 
	{
		dprintf(DEBUG_INFO, "CBouquetManager::saveBouquets: removing existing bouquets");
		g_bouquetManager->clearAll();
		unlink(BOUQUETS_XML);
	}
	else if (bouquetMode == CZapitClient::BM_DONTTOUCHBOUQUETS)
	{
		return;
	}
	else if (bouquetMode == CZapitClient::BM_CREATESATELLITEBOUQUET) 
	{
		while (Bouquets.size() > 1) 
		{
			BouquetList::iterator it = Bouquets.begin() + 1;
			Bouquets[0]->tvChannels.insert(Bouquets[0]->tvChannels.end(), (*it)->tvChannels.begin(), (*it)->tvChannels.end());
			Bouquets[0]->radioChannels.insert(Bouquets[0]->radioChannels.end(), (*it)->radioChannels.begin(), (*it)->radioChannels.end());
			delete (*it);
			Bouquets.erase(it);
		}

		if(Bouquets.size() > 0)
			Bouquets[0]->Name = providerName;
	}
	else if (bouquetMode == CZapitClient::BM_UPDATEBOUQUETS) 
	{
		while (!(Bouquets.empty())) 
		{
			CZapitBouquet * bouquet;
			int dest = g_bouquetManager->existsBouquet(Bouquets[0]->Name.c_str());
			
			dprintf(DEBUG_INFO, "CBouquetManager::saveBouquets: dest %d for name %s\n", dest, Bouquets[0]->Name.c_str());

			if(dest == -1) 
			{
				bouquet = g_bouquetManager->addBouquet(Bouquets[0]->Name.c_str(), false);
				dest = g_bouquetManager->existsBouquet(Bouquets[0]->Name.c_str());
			}
			else
				bouquet = g_bouquetManager->Bouquets[dest];

			// list from scanned exist in file
			// tv bouquets
			for(unsigned int i = 0; i < Bouquets[0]->tvChannels.size(); i++) 
			{
				if(!(g_bouquetManager->existsChannelInBouquet(dest, Bouquets[0]->tvChannels[i]->getChannelID()))) 
				{
					bouquet->addService(Bouquets[0]->tvChannels[i]);

					dprintf(DEBUG_INFO, "CBouquetManager::saveBouquets: adding channel %s\n", Bouquets[0]->tvChannels[i]->getName().c_str());
				}
			}
			
			// radio bouquets
			for(unsigned int i = 0; i < Bouquets[0]->radioChannels.size(); i++) 
			{
				if(!(g_bouquetManager->existsChannelInBouquet(dest, Bouquets[0]->radioChannels[i]->getChannelID()))) 
				{
					bouquet->addService(Bouquets[0]->radioChannels[i]);

					dprintf(DEBUG_INFO, "CBouquetManager::saveBouquets: adding channel %s\n", Bouquets[0]->radioChannels[i]->getName().c_str());
				}
			}

			bouquet->sortBouquet();
			delete Bouquets[0];
			Bouquets.erase(Bouquets.begin());
		}
	}
}

void CBouquetManager::sortBouquets(void)
{
	sort(Bouquets.begin(), Bouquets.end(), CmpBouquetByChName()); //FIXME:
}

void CBouquetManager::parseBouquetsXml(const char *fname, bool bUser)
{
	_xmlDocPtr parser = NULL;

	parser = parseXmlFile(fname);

	if (parser == NULL)
		return;

	_xmlNodePtr root = xmlDocGetRootElement(parser);

	_xmlNodePtr search = root->xmlChildrenNode;
	_xmlNodePtr channel_node;

	if (search) 
	{
		t_original_network_id original_network_id;
		t_service_id service_id;
		t_transport_stream_id transport_stream_id;
		t_satellite_position  satellitePosition = 0;
		freq_id_t freq = 0;
		

		dprintf(DEBUG_INFO, "CBouquetManager::parseBouquetsXml: %s\n", fname);

		while ((search = xmlGetNextOccurence(search, "Bouquet")) != NULL) 
		{
			char * name = xmlGetAttribute(search, (char *) "name");
			CZapitBouquet *newBouquet = addBouquet(name, bUser);
			char *hidden = xmlGetAttribute(search, (char *) "hidden");
			char *locked = xmlGetAttribute(search, (char *) "locked");
			newBouquet->bHidden = hidden ? (strcmp(hidden, "1") == 0) : false;
			newBouquet->bLocked = locked ? (strcmp(locked, "1") == 0) : false;
			newBouquet->bFav = (strcmp(name, "Favorites") == 0);
			channel_node = search->xmlChildrenNode;

			while ((channel_node = xmlGetNextOccurence(channel_node, "S")) != NULL) 
			{
				std::string  name1 = xmlGetAttribute(channel_node, (char *) "n");
				GET_ATTR(channel_node, (char *) "i", SCANF_SERVICE_ID_TYPE, service_id);
				GET_ATTR(channel_node, (char *) "on", SCANF_ORIGINAL_NETWORK_ID_TYPE, original_network_id);
				GET_ATTR(channel_node, (char *) "t", SCANF_TRANSPORT_STREAM_ID_TYPE, transport_stream_id);

				// grab satelliteposition and freq from channel map
				for (tallchans_iterator it = allchans.begin(); it != allchans.end(); it++)
				{
					if(it->second.getServiceId() == service_id)
					{
						satellitePosition = it->second.getSatellitePosition();
						freq = it->second.getFreqId();
					}
				}

				CZapitChannel *chan = findChannelByChannelID(CREATE_CHANNEL_ID64);

				if (chan != NULL) 
				{
					if(!bUser)
						chan->pname = (char *) newBouquet->Name.c_str();

					chan->bAlwaysLocked = newBouquet->bLocked;
					newBouquet->addService(chan);
				}

				channel_node = channel_node->xmlNextNode;
			}

			if(!bUser)
				newBouquet->sortBouquet();
			search = search->xmlNextNode;
		}

		dprintf(DEBUG_DEBUG, "CBouquetManager::parseBouquetsXml: found %d bouquets\n", (int)Bouquets.size());
	}

	xmlFreeDoc(parser);
	parser = NULL;
}

void CBouquetManager::makeBouquetfromCurrentservices(const _xmlNodePtr root)
{
	_xmlNodePtr provider = root->xmlChildrenNode;
	
	// TODO: use locales
	CZapitBouquet * newBouquet = addBouquet("Neue Sender");
	newBouquet->bHidden = false;
	newBouquet->bLocked = false;
			
	t_original_network_id original_network_id;
	t_service_id          service_id;
	t_transport_stream_id transport_stream_id;
	t_satellite_position  satellitePosition;
	freq_id_t freq = 0;
	
	while (provider) 
	{	
		_xmlNodePtr transponder = provider->xmlChildrenNode;
		
		while (xmlGetNextOccurence(transponder, "transponder") != NULL) 
		{
			_xmlNodePtr channel_node = transponder->xmlChildrenNode;
			
			while (xmlGetNextOccurence(channel_node, "channel") != NULL) 
			{
				
				if (strncmp(xmlGetAttribute(channel_node, "action"), "remove", 6)) 
				{
					GET_ATTR(provider, "position", SCANF_SATELLITE_POSITION_TYPE, satellitePosition);
					GET_ATTR(transponder, "onid", SCANF_ORIGINAL_NETWORK_ID_TYPE, original_network_id);
					GET_ATTR(transponder, "id", SCANF_TRANSPORT_STREAM_ID_TYPE, transport_stream_id);
					GET_ATTR(channel_node, "service_id", SCANF_SERVICE_ID_TYPE, service_id);
								
					CZapitChannel *chan = findChannelByChannelID(CREATE_CHANNEL_ID64);

					if (chan != NULL)
						newBouquet->addService(chan);
				}
			
				channel_node = channel_node->xmlNextNode;
			}
			transponder = transponder->xmlNextNode;
		}

		provider = provider->xmlNextNode;
	}
}

////
std::string ReadMarkerValue(std::string strLine, const char* strMarkerName)
{
	if (strLine.find(strMarkerName) != std::string::npos)
	{
		strLine = strLine.substr(strLine.find(strMarkerName));
		strLine = strLine.substr(strLine.find_first_of('"')+1);
		strLine = strLine.substr(0,strLine.find_first_of('"'));
		return strLine;
	}

	return std::string("");
}
////
void CBouquetManager::parseWebTVBouquet(std::string filename)
{
	int cnt = 0;	

	dprintf(DEBUG_NORMAL, "CBouquetManager::parseWebTVBouquet: parsing %s\n", filename.c_str());

	_xmlDocPtr parser = NULL;
	
	// check for extension
	bool iptv = false;
	bool webtv = false;
	bool playlist = false;
					
	std::string extension = getFileExt(filename);
						
	if( strcasecmp("tv", extension.c_str()) == 0)
		iptv = true;
	else if( strcasecmp("m3u8", extension.c_str()) == 0)
		playlist = true;
	else if( strcasecmp("m3u", extension.c_str()) == 0)
		playlist = true;
	if( strcasecmp("xml", extension.c_str()) == 0)
		webtv = true;

	std::string name = std::string(rindex(filename.c_str(), '/') + 1);
	removeExtension(name);

	CZapitBouquet *newBouquet = addBouquetIfNotExist(name);
	newBouquet->bWebTV = true;
	
	if(iptv)
	{
		FILE * f = fopen(filename.c_str(), "r");

		std::string title;
		std::string url;
		std::string description;
		
		if(f != NULL)
		{
			while(true)
			{
				t_channel_id id = 0;
				
				char line[1024];
				if (!fgets(line, 1024, f))
					break;
				
				size_t len = strlen(line);
				if (len < 2)
					// Lines with less than one char aren't meaningful
					continue;
				
				// strip newline
				line[--len] = 0;
				
				// strip carriage return (when found)
				if (line[len - 1] == '\r')
					line[len - 1 ] = 0;
				
				if (strncmp(line, "#SERVICE 4097:0:1:0:0:0:0:0:0:0:", 32) == 0)
					url = line + 32;
				else if (strncmp(line, "#DESCRIPTION", 12) == 0)
				{
					int offs = line[12] == ':' ? 14 : 13;
			
					title = line + offs;

					description = "stream";

					if(!url.empty())
					{
						// grab channel id from channellist
						for (tallchans_iterator it = allchans.begin(); it != allchans.end(); it++)
						{
							if(strcasecmp(it->second.getName().c_str(), title.c_str()) == 0)
								id = it->second.getChannelID();
						}

						if(id == 0)
							id = create_channel_id64(0, 0, 0, 0, 0, url.c_str());
					
						pair<map<t_channel_id, CZapitChannel>::iterator, bool> ret;

						ret = allchans.insert (std::pair <t_channel_id, CZapitChannel> (id, CZapitChannel(title, id, url, description)));

						ret.first->second.setServiceType(ST_WEBTV);

						CZapitChannel *chan = findChannelByChannelID(id);

						if (chan != NULL) 
						{
							chan->setName(title);
							chan->setDescription(description);
									
							newBouquet->addService(chan);

							cnt++;
						}
					}
				}
			}
			
			fclose(f);
		}
	}
	else if(webtv)
	{
		if (parser != NULL)
		{
			xmlFreeDoc(parser);
			parser = NULL;
		}

		parser = parseXmlFile(filename.c_str());
		
		if (parser) 
		{
			_xmlNodePtr l0 = NULL;
			_xmlNodePtr l1 = NULL;
			l0 = xmlDocGetRootElement(parser);
			l1 = l0->xmlChildrenNode;
			
			
			if (l1) 
			{
				while ( ((xmlGetNextOccurence(l1, "webtv")) || (xmlGetNextOccurence(l1, "station")))) 
				{
					const char * title;
					const char * url;
					const char * description;
					t_channel_id id = 0;
					const char* xmltv;
					const char* logo;
					const char* epgmap;
					const char* epgid;
					
					// 
					if(xmlGetNextOccurence(l1, "webtv"))
					{
						title = xmlGetAttribute(l1, (const char *)"title");
						url = xmlGetAttribute(l1, (const char *)"url");
						description = xmlGetAttribute(l1, (const char *)"description");
						epgid = xmlGetAttribute(l1, (const char*)"epgid");
						xmltv = xmlGetAttribute(l1, (const char*)"xmltv");
						logo = xmlGetAttribute(l1, (const char*)"logo");
						epgmap = xmlGetAttribute(l1, (const char*)"epgmap");

						if (epgid)
							id = strtoull(epgid, NULL, 16);
							
						if(id == 0)
						{
							// grab channel id from channellist
							for (tallchans_iterator it = allchans.begin(); it != allchans.end(); it++)
							{
								if(strcasecmp(it->second.getName().c_str(), title) == 0)
									id = it->second.getChannelID();
							}
						}

						if(id == 0)
							id = create_channel_id64(0, 0, 0, 0, 0, url);
							
						pair<map<t_channel_id, CZapitChannel>::iterator, bool> ret;
						ret = allchans.insert(std::pair <t_channel_id, CZapitChannel> (id, CZapitChannel(title, id, url, description)));

						ret.first->second.setServiceType(ST_WEBTV);
						
						CZapitChannel * chan = findChannelByChannelID(id);

						if (chan != NULL) 
						{
							chan->setName(title);
							chan->setDescription(description);
							if (xmltv != NULL) chan->setXMLTV(xmltv);
							if (epgmap != NULL) chan->setEPGMap(epgmap);
							if (logo != NULL) chan->setLogo(logo);

							newBouquet->addService(chan);

							cnt++;
						}
					}

					l1 = l1->xmlNextNode;
				}
			}
		}
		
		xmlFreeDoc(parser);
		parser = NULL;
	}
	else if(playlist)
	{
		std::ifstream infile;
		char cLine[1024];

		std::string epg_url = "";
		std::string description = "";
		std::string title = "";
		std::string prefix = "";
		std::string group = "";
		std::string epgid = "";
		std::string alogo = "";
		std::string script = "";
		t_channel_id id = 0;
		CZapitBouquet* pBouquet = NULL;
				
		infile.open(filename.c_str(), std::ifstream::in);

		while (infile.good())
		{
			infile.getline(cLine, sizeof(cLine));
					
			// remove CR
			if(cLine[strlen(cLine) - 1] == '\r')
				cLine[strlen(cLine) - 1] = 0;
				
			std::string strLine = cLine;
			
			if (strLine.empty())
				continue;

			if (strLine.find("#EXTM3U") != std::string::npos)
			{
				epg_url = "";
				epg_url = ReadMarkerValue(strLine, "tvg-url=");
				
				if (!epg_url.empty())
				{
				/*
					if (epg_url.find_first_of(',') != std::string::npos)
					{
						std::vector<std::string> epg_list = ::split(epg_url, ',');
						for (std::vector<std::string>::iterator it_epg = epg_list.begin(); it_epg != epg_list.end(); it_epg++)
							CNeutrinoApp::getInstance()->g_settings_xmltv_xml_auto_pushback((*it_epg));
					}
					else
						CNeutrinoApp::getInstance()->g_settings_xmltv_xml_auto_pushback(epg_url);
				*/
				}
			}
			
			if (strLine.find("#EXTINF") != std::string::npos)
			{
				int iColon = (int)strLine.find_first_of(':');
				int iComma = (int)strLine.find_first_of(',');
				title = "";
				prefix = "";
				group = "";
				description = "";
				alogo = "";
				script = "";
				id = 0;

				if (iColon >= 0 && iComma >= 0 && iComma > iColon)
				{
					iComma++;
					iColon++;
					title = strLine.substr(iComma);
					std::string strInfoLine = strLine.substr(iColon, --iComma - iColon);
					description = ReadMarkerValue(strInfoLine, "tvg-name=");
					prefix = ReadMarkerValue(strInfoLine, "group-prefix=");
					group = ReadMarkerValue(strInfoLine, "group-title=");
					//id = ReadMarkerValue(strInfoLine, "tvg-id=");
					alogo = ReadMarkerValue(strInfoLine, "tvg-logo=");
					script = ReadMarkerValue(strInfoLine, "tvg-script=");
				}
				
				pBouquet = addBouquetIfNotExist("WEBTV");
				pBouquet->bWebTV = true;
			}		
			else if(strlen(cLine) > 0 && cLine[0] != '#')
			{
				char *url = NULL;
				if ((url = strstr(cLine, "http://")) || (url = strstr(cLine, "https://")) || (url = strstr(cLine, "rtmp://")) || (url = strstr(cLine, "rtsp://")) || (url = strstr(cLine, "rtp://")) || (url = strstr(cLine, "mmsh://"))) 
				{
					if (url != NULL) 
					{
						description = "stream";
						
						CZapitBouquet* gBouquet = pBouquet;
						if (!group.empty())
						{
							gBouquet = addBouquetIfNotExist(group);
							gBouquet->bWebTV = true;		
						}
						
						// grab channel id from channellist
						for (tallchans_iterator it = allchans.begin(); it != allchans.end(); it++)
						{
							if(strcasecmp(it->second.getName().c_str(), title.c_str()) == 0)
								id = it->second.getChannelID();
						}

						if(id == 0)
							id = create_channel_id64(0, 0, 0, 0, 0, url);
							
						pair<map<t_channel_id, CZapitChannel>::iterator, bool> ret;
						ret = allchans.insert(std::pair <t_channel_id, CZapitChannel> (id, CZapitChannel(title, id, url, description)));

						ret.first->second.setServiceType(ST_WEBTV);
					
						CZapitChannel * chan = findChannelByChannelID(id);

						if (chan != NULL) 
						{
							chan->setName(title);
							chan->setDescription(description);

							gBouquet->addService(chan);

							cnt++;
						}
					}
				}
			}
		}

		infile.close();
	}

	dprintf(DEBUG_NORMAL, "CBouquetManager::loadWebTVBouquet: load %d WEBTV Channels (allchans:%d)\n", cnt, (int) allchans.size());
}

void CBouquetManager::loadWebTVBouquet(const std::string& dirname)
{
	dprintf(DEBUG_NORMAL, "CBouquetManager::loadWebTVBouquet: %s\n", dirname.c_str());

	dirent_struct **namelist;
	int n;

	n = my_scandir(dirname.c_str(), &namelist, 0, my_alphasort);

	if (n < 0)
	{
		perror(("getServices: scandir: " + dirname).c_str());
		return;
	}
	
	for(int i = 0; i < n; i++)
	{
		std::string file;
		if( (strcmp(namelist[i]->d_name, ".") != 0) && (strcmp(namelist[i]->d_name, "..") != 0) )
		{
			// name
			file = dirname + "/" + namelist[i]->d_name;

			parseWebTVBouquet(file);
		}
		free(namelist[i]);
	}

	free(namelist);
	
	// renum services
    	int webtvcnt = 1;

	for (tallchans::iterator it = allchans.begin(); it != allchans.end(); it++)
	{
		if(it->second.getServiceType() == ST_WEBTV)
		    it->second.number = webtvcnt++;
	}
}


void CBouquetManager::loadBouquets(bool loadCurrentBouquet)
{
	dprintf(DEBUG_NORMAL, "CBouquetManager::loadBouquets:\n");

	clearAll();
	
	// bouquets
	parseBouquetsXml(BOUQUETS_XML, false);
	sortBouquets();

	// ubouquets
	parseBouquetsXml(UBOUQUETS_XML, true);
	
	// current bouquets
	if(loadCurrentBouquet)
	{
		_xmlDocPtr parser = NULL;
		parser = parseXmlFile(CURRENTSERVICES_XML);
		if (parser != NULL)
		{
			dprintf(DEBUG_NORMAL, "reading %s\n", CURRENTSERVICES_XML);

			makeBouquetfromCurrentservices(xmlDocGetRootElement(parser));
			xmlFreeDoc(parser);
			parser = NULL;
		}
	}

	// webTV bouquets
	loadWebTVBouquet(CONFIGDIR "/webtv");

	renumServices();
}

void CBouquetManager::makeRemainingChannelsBouquet(void)
{
	ZapitChannelList unusedChannels;
	set<t_channel_id> chans_processed;
	bool tomake = config.getBool("makeRemainingChannelsBouquet", true);

	int i = 1;  // tv
    	int j = 1;  // radio
    	int k = 1;  //webtv

	for (vector<CZapitBouquet*>::const_iterator it = Bouquets.begin(); it != Bouquets.end(); it++) 
	{
		// tvChannels
		for (vector<CZapitChannel*>::iterator jt = (*it)->tvChannels.begin(); jt != (*it)->tvChannels.end(); jt++) 
		{
			if(tomake) 
				chans_processed.insert((*jt)->getChannelID());
			
			if(!(*jt)->number) 
				(*jt)->number = i++;
			
			if(!(*jt)->pname && !(*it)->bUser) 
				(*jt)->pname = (char *) (*it)->Name.c_str();
		}

		// radioChannels
		for (vector<CZapitChannel*>::iterator jt = (*it) ->radioChannels.begin(); jt != (*it)->radioChannels.end(); jt++) 
		{
			if(tomake) 
				chans_processed.insert((*jt)->getChannelID());
			
			if(!(*jt)->number) 
				(*jt)->number = j++;

			if(!(*jt)->pname && !(*it)->bUser) 
				(*jt)->pname = (char *) (*it)->Name.c_str();
		}

		// webtvChannels
		for (vector<CZapitChannel*>::iterator jt = (*it)->webtvChannels.begin(); jt != (*it)->webtvChannels.end(); jt++) 
		{
			if(!(*jt)->number) 
				(*jt)->number = k++;

			if(!(*jt)->pname && !(*it)->bUser) 
				(*jt)->pname = (char *) (*it)->Name.c_str();
		}
	}

	if(!tomake)
		return;

	// TODO: use locales
	remainChannels = addBouquet((Bouquets.size() == 0) ? "All Channels" : "Other", false); // UTF-8 encoded

	for (tallchans::iterator it = allchans.begin(); it != allchans.end(); it++)
	{
		if(it->second.getServiceType() != ST_WEBTV)
		{
			if (chans_processed.find(it->first) == chans_processed.end())
				unusedChannels.push_back(&(it->second));
		}
	}

	sort(unusedChannels.begin(), unusedChannels.end(), CmpChannelByChName()); //FIXME:

	for (ZapitChannelList::const_iterator it = unusedChannels.begin(); it != unusedChannels.end(); it++) 
	{
		remainChannels->addService(findChannelByChannelID((*it)->getChannelID()));
	}

	if ((remainChannels->tvChannels.empty()) && (remainChannels->radioChannels.empty())) 
	{
		deleteBouquet(remainChannels);
		remainChannels = NULL;
		return;
	}

	// renum remainsChannels
	// tv
	for (vector<CZapitChannel*>::iterator jt = remainChannels->tvChannels.begin(); jt != remainChannels->tvChannels.end(); jt++)
	{
		if(!(*jt)->number) 
			(*jt)->number = i++;
	}

	// radio
	for (vector<CZapitChannel*>::iterator jt = remainChannels->radioChannels.begin(); jt != remainChannels->radioChannels.end(); jt++)
	{
		if(!(*jt)->number) 
			(*jt)->number = j++;
	}
}

void CBouquetManager::renumServices()
{
	if(remainChannels)
		deleteBouquet(remainChannels);

	remainChannels = NULL;
	
	makeRemainingChannelsBouquet();
}

CZapitBouquet * CBouquetManager::addBouquet(const std::string& name, bool ub, bool myfav, bool iswebtv)
{
	CZapitBouquet * newBouquet = new CZapitBouquet(myfav ? "Favorites" : name);
	newBouquet->bUser = ub;
	newBouquet->bFav = myfav;
	newBouquet->bWebTV = iswebtv;

	if(ub) 
	{
		BouquetList::iterator it;
		for(it = Bouquets.begin(); it != Bouquets.end(); it++)
			if(!(*it)->bUser)
				break;
		Bouquets.insert(it, newBouquet);
	} 
	else
		Bouquets.push_back(newBouquet);

	return newBouquet;
}

CZapitBouquet* CBouquetManager::addBouquetIfNotExist(const std::string& name)
{
	CZapitBouquet* bouquet = NULL;

	int bouquetId = existsBouquet(name.c_str(), true);
	if (bouquetId == -1)
		bouquet = addBouquet(name, false);
	else
		bouquet = Bouquets[bouquetId];

	return bouquet;
}

void CBouquetManager::deleteBouquet(const unsigned int id)
{
	if (id < Bouquets.size() && Bouquets[id] != remainChannels)
		deleteBouquet(Bouquets[id]);
}

void CBouquetManager::deleteBouquet(const CZapitBouquet* bouquet)
{
	if (bouquet != NULL) 
	{
		BouquetList::iterator it = find(Bouquets.begin(), Bouquets.end(), bouquet);

		if (it != Bouquets.end()) 
		{
			Bouquets.erase(it);
			delete bouquet;
		}
	}
}

// -- Find Bouquet-Name, if BQ exists   (2002-04-02 rasc)
// -- Return: Bouqet-ID (found: 0..n)  or -1 (Bouquet does not exist)
int CBouquetManager::existsBouquet(char const * const name, bool ignore_user)
{
	for (unsigned int i = 0; i < Bouquets.size(); i++) 
	{
		//if (Bouquets[i]->Name == name)
		if ((!ignore_user || !Bouquets[i]->bUser) && (Bouquets[i]->Name == name))
			return (int)i;
	}
	
	return -1;
}

int CBouquetManager::existsUBouquet(char const * const name, bool myfav)
{
	unsigned int i;

	for (i = 0; i < Bouquets.size(); i++) 
	{
		if(myfav) 
		{
			if (Bouquets[i]->bFav)
				return (int)i;
		}
		else if (Bouquets[i]->bUser && (Bouquets[i]->Name == name))
			return (int)i;
	}

	return -1;
}

// -- Check if channel exists in BQ   (2002-04-05 rasc)
// -- Return: True/false
bool CBouquetManager::existsChannelInBouquet( unsigned int bq_id, const t_channel_id channel_id)
{
	bool     status = false;
	CZapitChannel  *ch = NULL;

	if (bq_id <= Bouquets.size()) 
	{
		// query TV-Channels  && Radio channels
		ch = Bouquets[bq_id]->getChannelByChannelID(channel_id, 0);

		if (ch)  
			status = true;
	}

	return status;
}

void CBouquetManager::moveBouquet(const unsigned int oldId, const unsigned int newId)
{
	if ((oldId < Bouquets.size()) && (newId < Bouquets.size())) 
	{
		BouquetList::iterator it = Bouquets.begin();

		advance(it, oldId);
		CZapitBouquet* tmp = *it;
		Bouquets.erase(it);

		advance(it, newId - oldId);
		Bouquets.insert(it, tmp);
	}
}

void CBouquetManager::clearAll()
{
	for (unsigned int i = 0; i < Bouquets.size(); i++)
		delete Bouquets[i];

	Bouquets.clear();
	remainChannels = NULL;
}

CZapitChannel *CBouquetManager::findChannelByChannelID(const t_channel_id channel_id)
{
	tallchans_iterator itChannel = allchans.find(channel_id);
	if (itChannel != allchans.end())
		return &(itChannel->second);

	return NULL;
}

CZapitChannel *CBouquetManager::findChannelByName(std::string name, const t_service_id sid)
{
	for (tallchans_iterator itChannel = allchans.begin(); itChannel != allchans.end(); ++itChannel) 
	{
		if( (itChannel->second.getName().length() == name.length() && !strcasecmp(itChannel->second.getName().c_str(), name.c_str()) ) && (itChannel->second.getServiceId() == sid) ) 
		{
			return &itChannel->second;
		}
	}
	
	return NULL;
}

//// ChannelIterator
CBouquetManager::ChannelIterator::ChannelIterator(CBouquetManager *owner, const CZapitClient::channelsMode Mode)
{
	Owner = owner;
	mode = Mode;

	if (Owner->Bouquets.size() == 0)
		c = -2;
	else 
	{
		b = 0;
		c = -1; 
		(*this)++;
	}
}

CBouquetManager::ChannelIterator CBouquetManager::ChannelIterator::operator ++(int)
{
	if (c != -2)  // we can add if it's not the end marker
	{
		c++;
		if ((unsigned int) c >= getBouquet()->size()) 
		{
			for (b++; b < Owner->Bouquets.size(); b++)
				if (getBouquet()->size() != 0) 
				{
					c = 0;
					goto end;
				}
			c = -2;
		}
	}

 end:
	return(*this);
}

CZapitChannel* CBouquetManager::ChannelIterator::operator *()
{
	return (*getBouquet())[c];               // returns junk if we are an end marker !!
}

CBouquetManager::ChannelIterator CBouquetManager::ChannelIterator::FindChannelNr(const unsigned int channel)
{
	c = channel;
	for (b = 0; b < Owner->Bouquets.size(); b++)
		if (getBouquet()->size() > (unsigned int)c)
			goto end;
		else
			c -= getBouquet()->size();
	c = -2;
 end:
	return (*this);
}

int CBouquetManager::ChannelIterator::getLowestChannelNumberWithChannelID(const t_channel_id channel_id)
{
	int i = 0;

	for (b = 0; b < Owner->Bouquets.size(); b++)
		for (c = 0; (unsigned int) c < getBouquet()->size(); c++, i++)
			if ((**this)->getChannelID() == channel_id)
			    return (**this)->number -1;
			    //return i;
	return -1; // not found
}

int CBouquetManager::ChannelIterator::getNrofFirstChannelofBouquet(const unsigned int bouquet_nr)
{
	if (bouquet_nr >= Owner->Bouquets.size())
		return -1;  // not found

	int i = 0;

	for (b = 0; b < bouquet_nr; b++)
		i += getBouquet()->size();

	return i;
}



