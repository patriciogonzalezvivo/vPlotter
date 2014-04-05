# Remote RASPISTILL-API
---------------------

Take remotes pictures with your RaspberryCam

## 1. Setup

You need to buy:

- [RaspberryCam](http://www.adafruit.com/products/1367)

You need to install:

- Apache Server with PHP: 

	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install avahi-daemon apache2 php5

- Enable camera kernel module:

	sudo raspi-config

Navigate to “camera” and select “enable”.

![enable_cam](http://www.raspberrypi.org/wp-content/uploads/2013/05/image-2.jpg)
 
Camera device need 777 priviladge **( WARNING: this is dangerous )**
 
	sudo chmod 777 /dev/vchiq


- Copy the PHP code in his own special folder

Go to apache folder and link to the ```raspistillAPI/``` folder to copy the PHP code that is going to take and deliver the photo

	cd /var/www
	sudo ln -s /[your_dir_path]/raspistillAPI raspistillAPI

The folder where is store the images needs all (777) privileges **( WARNING: this is dangerous )**

	sudo chown www-data /[your_dir_path]/raspistillAPI/
	sudo chgrp www-data /[your_dir_path]/raspistillAPI/
	
After this copy the index.php in that folder.

## 2. Use

Go to a navigator and type:

	http://raspberrypi.local/raspistillAPI/?width=640&height=480

You should see a photo.
If that's the case try compiling the OF project and be ready to ask for images on demand using ```raspistill``` API

	width	: Set image width <size>
	
	height	: Set image height <size>
	
	quality	: Set jpeg quality <0 to 100>
	
	sharpness	: Set image sharpness (-100 to 100)
	
	contrast	: Set image contrast (-100 to 100)
	
	brightness	: Set image brightness (0 to 100)
	
	saturation	: Set image saturation (-100 to 100)
	
	ISO	: Set capture ISO
	
	ev	: Set EV compensation
	
	exposure	: Set exposure mode: off, auto, night, nightpreview, backlight, spotlight, sports, snow,beach,	verylong,	fixedfps,	antishake,	fireworks

	awb	: Set AWB mode: off, auto, sun, cloud, shade, tungsten, fluorescent, incandescent, flash, horizon
	
	imxfx	: Set image effect: none, negative, solarise, sketch, denoise, emboss, oilpaint, hatch, gpen, pastel, watercolour, film, blur, saturation, colourswap, washedout, posterise, colourpoint, colourbalance, cartoon
	
	colfx	: Set colour effect (U:V)
	
	metering	: Set metering mode: average, spot, backlit, matrix
	
	rotation	: Set image rotation (0-359)
	
	hflip	: Set horizontal flip
	
	vflip	: Set vertical flip


# vPlotter
---------------

## 1. Setup

You need to buy:

- 2 x [Nema 17 Stepper Motor](https://ultimachine.com/content/kysan-1124090-nema-17-stepper-motor)

- 2 x [Pololu A4988 Stepper Driver](https://ultimachine.com/content/pololu-a4988-stepper-driver-heatsink-kit)

- 2 x [Timing Pulley, GT2, 20 Tooth](https://ultimachine.com/content/timing-pulley-gt2-20-tooth)

- 6m of [Timing Belt, GT2, Custom Length](https://ultimachine.com/content/timing-belt-gt2-custom-length)

- 1 x [Micro Servo](http://www.adafruit.com/products/169) 

- 15x15 inch of Play-wood 

You need to install:

- [WiringPI](http://wiringpi.com/download-and-install/)
  
	cd ~
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install avahi-daemon apache2 php5 php5-json git-core
	git clone git://git.drogon.net/wiringPi
	cd wiringPi
	./build



