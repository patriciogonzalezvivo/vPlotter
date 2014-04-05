<?php
        header('Content-Type: image/jpeg');

        // Notes:
        //
        //      - camera device need 777 priviladge ( WARNING: this is dangerous )
        //
        //      sudo chmod 777 /dev/vchiq
        //
        //      - the folder where is store the iamges needs all 777 priviladge ( WARNING: this is dangerous )
        //
        //      sudo chown www-data rpicam
        //      sudo chgrp www-data rpicam
        //

        $width = "";
        if(isset($_GET['width'])) {
                // Set image width <size>
                $width = " -w ".$_GET['width'];
        }

        $height = "";
        if(isset($_GET['height'])) {
                // Set image height <size>
                $height = " -h ".$_GET['height'];
        }

        $quality = "";   
        if(isset($_GET['quality'])) {
                // Set jpeg quality <0 to 100>
                $quality = " -q ".$_GET['quality'];
        }

        $sharpness = "";  
        if(isset($_GET['sharpness'])) {
                // Set image sharpness (-100 to 100)
                $sharpness = " -sh ".$_GET['sharpness'];
        }

        $contrast = "";  
        if(isset($_GET['contrast'])) {
                // Set image contrast (-100 to 100)
                $contrast = " -co ".$_GET['contrast'];
        }

        $brightness = ""; 
        if(isset($_GET['brightness'])) {
                // Set image brightness (0 to 100)
                $brightness = " -br ".$_GET['brightness'];
        }

        $saturation = "";  
        if(isset($_GET['saturation'])) {
                // Set image saturation (-100 to 100)
                $saturation = " -sa ".$_GET['saturation'];
        }

        $exposure = "";
        if(isset($_GET['exposure'])) {
                // Set exposure mode:
                //
                //      off,auto,night,nightpreview,backlight,spotlight,sports,snow,beach,verylong,fixedfps,antishake,fireworks
                $exposure = " -ex ".$_GET['exposure'];
        }

        $awb = "";
        if(isset($_GET['awb'])) {
                // Set AWB mode:
                //
                //      off,auto,sun,cloud,shade,tungsten,fluorescent,incandescent,flash,horizon
                $awb = " -awb ".$_GET['awb'];
        }

        $imxfx = "";
        if(isset($_GET['imxfx'])) {
                // Set image effect:
                //
                //      none,negative,solarise,sketch,denoise,emboss,oilpaint,hatch,gpen,pastel,watercolour,film,blur,saturation,colourswap,washedout,posterise,colourpoint,colourbalance,cartoon
                $imxfx = " -ifx ".$_GET['imxfx'];
        }

        $colfx = "";
        if(isset($_GET['colfx'])) {
                // Set colour effect (U:V)
                $colfx = " -cfx ".$_GET['colfx'];
        }

        $metering = "";
        if(isset($_GET['metering'])) {
                // Set metering mode:
                //
                //      average,spot,backlit,matrix
                $metering = " -mm ".$_GET['metering'];
        }

        $rotation = "";   
        if(isset($_GET['rotation'])) {
                // Set image rotation (0-359)
                $rotation = " -rot ".$_GET['rotation'];
        }

        $hflip = "";
        if(isset($_GET['hflip'])) {
                // Set horizontal flip
                $hflip = " -hf";
        }

        $vflip = "";
        if(isset($_GET['vflip'])) {
                // Set horizontal flip
                $vflip = " -vf";
        }

        $command = "raspistill -t 1 -n".$width.$height.$quality.$sharpness.$contrast.$brightness.$saturation.$exposure.$awb.$imxfx.$colfx.$metering.$rotation.$hflip.$vflip." -o image.jpg";

        $last_line = system($command);   
        readfile('image.jpg');     
?>
<!doctype>