<?php
$csv_filename = 'config.csv';
$txt_filename = 'config.txt';
$txt = '';
$csv = array();

$file = fopen($csv_filename, 'r');
while (($line = fgetcsv($file)) !== FALSE) {
  $csv = $line;
}
fclose($file);

if(isset($_GET['motorsDistance'])) {
	$txt = '-d '.$_GET["motorsDistance"]; 
	$csv[0] = $_GET["motorsDistance"];
}

if(isset($_GET['pulleyRadius'])) {
	$txt = $txt.' -pr '.$_GET["pulleyRadius"];
	$csv[1] = $_GET["pulleyRadius"];
}

if(isset($_GET['stepsPerRotation'])) {
	$txt = $txt.' -spr '.$_GET["stepsPerRotation"];
	$csv[2] = $_GET["stepsPerRotation"];
}

if(isset($_GET['stepDelay'])) {
	$txt = $txt.' -sd '.$_GET["stepDelay"];
	$csv[3] = $_GET["stepDelay"];
}

if(isset($_GET['penDownAngle'])) {
	$txt = $txt.' -pd '.$_GET["penDownAngle"];
	$csv[4] = $_GET["penDownAngle"];
}

if(isset($_GET['penUpAngle'])) {
	$txt = $txt.' -pu '.$_GET["penUpAngle"];
	$csv[5] = $_GET["penUpAngle"];	
}

if(isset($_GET['penDelay'])) {
	$txt = $txt.' -delay '.$_GET["penDelay"];
	$csv[6] = $_GET["penDelay"];
}

$fc = fopen($txt_filename,'w');
fwrite($fc,$txt);
fclose($fc);

$string='';
$index=0;
foreach ($csv as $element) {
	if($index == 0){
		$string=$element;
	} else {
		$string=$string.','.$element;
	}
	$index++;
}
$fcc = fopen($csv_filename,'w');
fwrite($fcc,$string);
fclose($fcc);

header("Location: ../vPlotterWeb/config.html");
exit;

?>
