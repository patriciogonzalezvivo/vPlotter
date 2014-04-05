<?php
$upload_folder = 'data';

$rotate = 0;
if(isset($_GET['rotate'])){
	$rotate = $_GET['rotate'];
}

$scale = 100;
if(isset($_GET['scale'])){
	$scale = $_GET['scale'];
}

function addToQueue($newItem,$rotate,$scale){
	$txt_filename = 'queue.txt';
	$csv_filename = 'queue.csv';
	
	// TXT file
	//
	$txt_queue = "";
	$ftq = fopen($txt_filename,'r');
	if(filesize($txt_filename)>0){
    	$txt_queue = fread($ftq,filesize($txt_filename));
        fclose($ftq);
    }
	$fq = fopen($txt_filename,'w');

	if($scale != 100.0){
		$txt_queue = $txt_queue.' -s '.$scale;	
	}

	if($rotate != 0.0){
		$txt_queue = $txt_queue.' -r '.$rotate;	
	}

	fwrite($fq,$txt_queue.' -i '.$newItem."\n");
	fclose($fq); 

	//	CSV file
	//
	$csv_queue = "";
	$fcq = fopen($csv_filename,'r');
	if(filesize($csv_filename)>0){
    	$csv_queue = fread($fcq,filesize($csv_filename));
        fclose($fcq);
    }
	$fqf = fopen($csv_filename,'w');
	fwrite($fqf,$csv_queue.$newItem.",".$rotate.",".$scale."\n");
	fclose($fqf); 
}

// If the browser supports sendAsBinary () can use the array $ _FILES
if(count($_FILES)>0) { 
	if( move_uploaded_file( $_FILES['upload']['tmp_name'] , $upload_folder.'/'.$_FILES['upload']['name'] ) ) {
		addToQueue($_FILES['upload']['name'], $rotate, $scale );
	}
	exit();
} else if(isset($_GET['up'])) {
	// If the browser does not support sendAsBinary ()
	if(isset($_GET['base64'])) {
		$content = base64_decode(file_get_contents('php://input'));
	} else {
		$content = file_get_contents('php://input');
	}

	$headers = getallheaders();
	$headers = array_change_key_case($headers, CASE_UPPER);
	
	if(file_put_contents($upload_folder.'/'.$headers['UP-FILENAME'], $content)) { 
		addToQueue( $headers['UP-FILENAME'], $rotate, $scale );
	}
	exit();
}
?>
