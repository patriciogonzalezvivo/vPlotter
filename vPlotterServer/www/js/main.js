var rotate = 0;
var scale = 1;
var file;

//	DRAG&DROP ZONE
//-----------------------------------------------------------
//
this.uploadPlace =  document.getElementById('dropbox');
this.uploadPlace.addEventListener("dragover", function(event) {
	event.stopPropagation();
	event.preventDefault();
	document.getElementById('dropbox').className="over";
	// var ok = event.dataTransfer && event.dataTransfer.types && event.dataTransfer.types.indexOf('Files') >= 0;
	// this.uploadPlace.className = ok ? 'over' : 'not-available';
},true);

this.uploadPlace.addEventListener("dragleave", function(event) {
	event.stopPropagation();
	event.preventDefault();
	document.getElementById('dropbox').className="not-over";
},true);

//	On File Drop
this.uploadPlace.addEventListener("drop", function(event) {
	event.preventDefault();
	var dt = event.dataTransfer;
	file = dt.files[0];

	//	Make Preview
	var preview = new FileReader();
	preview.addEventListener('loadend', function() {
		var bin = preview.result;

		document.getElementById('OSCbox').style.display = 'none';
		document.getElementById('dropbox').style.display = 'none';
		var displayDiv = document.getElementById('preview');
		displayDiv.style.display = 'block';
		var buttonsDiv = document.getElementById('buttons');
		buttonsDiv.style.display = 'block';

		//	Add Image to Preview
		var img = document.createElement("img");
		img.id = 'addedIMG';
		img.className = 'addedIMG';
		img.file = file;
		img.src = bin;
		displayDiv.appendChild(img);

		// var scaleSlider = document.getElementById('scaleSlider');
	}, false);
	preview.readAsDataURL(file);
},false);


//	ADD TO QUEUE
//------------------------------------------------------------
//
function addOSCToQueue(){
	console.log("ADD OSC STREAM to queue");
	var url = '/addOsc';
	var data = new FormData();
	data.append('rotate', 0);
	data.append('scale', 100);
	data.append('file', 'OSC');
	var xhr = new XMLHttpRequest();
	xhr.open('POST', url, true);
	xhr.send(data);
}

function addFileToQueue() {

	// Once the process of reading file
	this.loadEnd = function() {
		var url = '/addFile';
		var data = new FormData();
		data.append('rotate', rotate);
		data.append('scale', scale*100);
		data.append('file', file);
		var xhr = new XMLHttpRequest();
		xhr.open('POST', url, true);
		xhr.onload = function () {
			// do something to response
			console.log(this.responseText);
		};
		xhr.send(data);

		location.reload();
	};

	// Loading errors
	this.loadError = function(event) {
		switch(event.target.error.code) {
			case event.target.error.NOT_FOUND_ERR:
				console.log('File not found!');
			break;
			case event.target.error.NOT_READABLE_ERR:
				console.log('File not readable!');
			break;
			case event.target.error.ABORT_ERR:
				console.log('Abort');
			break;
			default:
				console.log('Unknown read error');
		}
	};

	var reader = new FileReader();
	reader.addEventListener('loadend', this.loadEnd, false);
	reader.addEventListener('error', this.loadError, false);
	reader.readAsBinaryString(file);
}

function OnSliderChanged(slider){
	if (slider.id === "rotateNumber"){
		rotate = slider.value;
	} else if (slider.id === "scaleSlider"){
		scale = slider.value*0.01;
	}
	var image = document.getElementById('addedIMG');
	image.style.webkitTransform = "rotate("+rotate+"deg) scale("+scale+","+scale+")";
}
