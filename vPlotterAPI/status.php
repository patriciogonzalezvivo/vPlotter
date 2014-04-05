<?php
	$config_file = 'config.csv';
	$printing_file = 'print.txt';
	$queue_file = 'queue.csv';

	//	OPTIONS
	//
	$csv = array();
	$file = fopen($config_file, 'r');
	while (($line = fgetcsv($file)) !== FALSE) {
  		$csv = $line;
	}
	fclose($file);
	$status['options']['motorsDistance'] = $csv[0];
	$status['options']['pulleyRadius'] = $csv[1];
	$status['options']['stepsPerRotation'] = $csv[2];
	$status['options']['stepDelay'] = $csv[3];
	$status['options']['penDownAngle'] = $csv[4];
	$status['options']['penUpAngle'] = $csv[5];
	$status['options']['penDelay'] = $csv[6];


	//	PRINTING STATUS
	//
	$status['printing'] = preg_replace('~[\r\n]+~', '', file($printing_file));

	//	QUEUE
	//
	// $queue = file($queue_file);
	$elements = [];
	$csv_queue = fopen($queue_file, 'r');
	$line_num = 0;
	while (($line = fgetcsv($csv_queue)) !== FALSE) {
  		$elements[$line_num]['file'] = preg_replace('~[\r\n]+~', '', $line[0]);
  		$elements[$line_num]['rotate'] = $line[1];
  		$elements[$line_num]['scale'] = $line[2];
  		$line_num = $line_num + 1;
	}

	// foreach ($queue as $line_num => $line) {
	//  	$elements[$line_num] = preg_replace('~[\r\n]+~', '', $line);
	// }
	$status['queue'] = $elements;

	//	Answer the call on XML or Json acording to demand
	//
	if (isset($_GET['xml'])){
		header("Content-type: text/xml; charset=utf-8");

		function array_to_xml($array, &$xml) {
    		foreach($array as $key => $value) {
        		if(is_array($value)) {
            		if(!is_numeric($key)){
                		$subnode = $xml->addChild("$key");
                		array_to_xml($value, $subnode);
            		} else {
                		array_to_xml($value, $xml);
            		}
        		} else {
            		$xml->addChild("$key","$value");
        		}
    		}
		}

		$xml = new SimpleXMLElement("<?xml version=\"1.0\"?><vPlotter></vPlotter>");
		$node = $xml->addChild('status');
		// function call to convert array to xml
		array_to_xml($status, $node);
		// display XML to screen
		echo $xml->asXML();
	} else {
		header('Content-Type: application/json');
		echo json_encode($status);
	}
?>
