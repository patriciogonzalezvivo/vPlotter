var STATUS_JSON = 'status.json';

function request(url,callback){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = ( function(myxhr){
    return function(){
      callback(myxhr);
    };
  })(xhr);
  xhr.open('GET',url, true);
  xhr.send('');
}

function checkStatus(){
  request(STATUS_JSON, function(result){
      var rawJson = result.response;
      var status = JSON.parse(rawJson||null);
      if (status !== null){
        var queue = status.queue;

        var oldStatus = document.getElementById('queue');
        if(oldStatus.childNodes.length !== queue.length){
          while (oldStatus.firstChild) {
            oldStatus.removeChild(oldStatus.firstChild);
          }

          for(var i=0;i<queue.length;i++){

            if(queue[i].file === 'OSC'){
              var OSCimg = document.createElement('img');
              if( status.printing === true && i === 0 ){
                OSCimg.className = "printing_image";
              } else {
                OSCimg.className = "queue_image";
              }
              OSCimg.src = "images/osc.png";
              document.getElementById('queue').appendChild(OSCimg);
            } else {
              var img = document.createElement('img');
              if( status.printing === true && i === 0 ){
                img.className = "printing_image";
              } else {
                img.className = "queue_image";
              }
              img.src = "data/"+queue[i].file;
              var rotate = queue[i].rotate;
              var scale = queue[i].scale*0.01;
              img.style.webkitTransform = "rotate("+rotate+"deg) scale("+scale+","+scale+")";
              document.getElementById('queue').appendChild(img);
            }
          }
        }
      }
   });
}

checkStatus();
setInterval(checkStatus,1000);
