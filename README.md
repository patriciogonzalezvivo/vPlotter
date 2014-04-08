# vPlotter
---------------

## Basic principles

http://www.marginallyclever.com/blog/2012/02/drawbot-overvie/
http://2e5.com/plotter/V/design/

## Physical Components

- 2 x [Nema 17 Stepper Motor](https://ultimachine.com/content/kysan-1124090-nema-17-stepper-motor)

- 2 x [Pololu A4988 Stepper Driver](https://ultimachine.com/content/pololu-a4988-stepper-driver-heatsink-kit)

- 2 x [Timing Pulley, GT2, 20 Tooth](https://ultimachine.com/content/timing-pulley-gt2-20-tooth)

- 6m of [Timing Belt, GT2, Custom Length](https://ultimachine.com/content/timing-belt-gt2-custom-length)

- 1 x [Micro Servo](http://www.adafruit.com/products/169) 

- 15x15 inch of Play-wood 

## Software Installation

1. Basic installation

	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install avahi-daemon apache2 php5 php5-json git-core screen htop

2. Install [wiringPi](http://wiringpi.com/download-and-install/) and Apache with PHP-JSON 

	cd ~
	git clone git://git.drogon.net/wiringPi
	cd wiringPi
	./build

3. Link the vPlotter API and Web to an webServer and set right PHP permissions

	cd /var/www
	ln -s /home/pi/openframeworks/apps/vPlotter vPlotter
	cd vPlotter/vPlotterAPI
	./install.sh

### TODO:
	- precompile version of vPlotter inside vPlotterAPI
	- install.sh should install install the vPlotterManager and auto load the GPIOs 

## Precedents

Hektor 
http://hektor.ch/
Viktor 
http://www.we-find-wildness.com/2011/01/juerg-lehni-viktor/
Norweigan Creation's Drawing Machine 
http://www.norwegiancreations.com/2012/04/drawing-machine-part-2/
SADbot 
http://www.instructables.com/id/SADbot-the-Seasonally-Affected-Drawing-robot/
Harvey Moon's The Drawing Machine 
http://www.unanything.com/Drawbot.html 
http://www.kickstarter.com/projects/notever/the-drawing-machine 
http://www.creativeapplications.net/maxmsp/drawing-machine-maxmsp-processing/
Der Kritzler 
http://tinkerlog.com/2011/09/02/der-kritzler/
Polargraph 
http://www.polargraph.co.uk
Tristan Perich's Drawing Machine 
http://tristanperich.com/Art/Machine_Drawings/
Paul Ferragut's Wall Drawing Machine 
http://vimeo.com/20526787 
http://vimeo.com/35647507 
http://vimeo.com/26900180
Douglas Irving Repetto's Giant Painting Machine 
http://music.columbia.edu/~douglas/portfolio/GPM_sanmateo_peoria_sf/ 
http://music.columbia.edu/~douglas/portfolio/GPM_milano/
Sprite Mod's White Board Plotter 
http://spritesmods.com/?art=whiteboard&f=had
DrawBot 
http://www.marginallyclever.com/category/drawbot-my-creations/ 
https://github.com/i-make-robots/DrawBot 
http://web.me.com/sami6877/unanything/Site/Secret_Work/Entries/2010/6/1_Drawbot.html 
http://blog.makezine.com/2010/07/30/drawbot-takes-its-time-to-sketch/
AS220 Lab's Drawbot 
http://www.as220.org/labs/drawbot/instructions.html
Make Magazine's Drawing Machine 
http://makezine.com/makeitlast/ 
http://makermedia.cmail4.com/t/ViewEmail/r/351C80A070878603/6423113868B805CEC9C291422E3DE149 
http://blog.makezine.com/2011/02/24/make-it-last-build-3-rigging-up-your-drawbot/
Muralizer 
http://www.muralizer.com/blog/ 
http://www.muralizer.com/blog/about/ 
http://www.kickstarter.com/projects/1910641777/muralizer-it-prints-on-walls 
http://blog.makezine.com/2009/10/29/muralizer-prints-art-on-the-wall/ 
http://www.kickstarter.com/projects/1910641777/muralizer-it-prints-on-walls
InternBot 
http://jamesprovost.com/blog/introducing-internbot/
