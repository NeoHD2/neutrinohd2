
<h1 align="center">
  Welcome to neutrino-HD2 software
</h1>

-------
![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/mainmenu.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/pluginsbrowser.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/moviebrowser.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/movietrailer.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/movieinfowidget.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/mainmenu_1.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/mainmenu_2.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/mainmenu_3.bmp)

![](https://github.com/mohousch/neutrinohd2/blob/master/nhd2-exp/doc/resources/KravenHD.png)

## How to build neutrino-HD2 for PC (x86) ##

* check preqs (debian):

```bash
$:~ sudo apt-get install autoconf libtool libtool-bin g++ gdb libavformat-dev libswscale-dev libopenthreads-dev libglew-dev freeglut3-dev libcurl4-gnutls-dev libfreetype6-dev libid3tag0-dev libmad0-dev libogg-dev libpng12-dev libgif-dev libjpeg-dev libvorbis-dev libflac-dev libblkid-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev libfribidi-dev libass-dev python-dev lua5.2 lua5.2-dev lua-json lua-expat lua-posix lua-socket lua-soap lua-curl
```

```bash
$:~ git clone https://github.com/mohousch/neutrinohd2.git
```
```bash
$:~ cd neutrinohd2
```

```bash
$:~ make
```

* to run neutrino
```bash
$:~ make run
```

* to update neutrinohd2 source code:
```bash
$:~ make update
```

* to clean build:
```bash
$:~ make clean
```

* distclean build:
```bash
$:~ make distclean
```

## how to build neutrino-HD2 for sh4/arm/mips boxes ##
see:
* https://github.com/mohousch/buildsystem-hd2






