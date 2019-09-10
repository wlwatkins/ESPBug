function getFile(adr, callback, timeout, method, onTimeout, onError){
	if(adr === undefined) return;
	if(callback === undefined) callback = function(){};
	if(timeout === undefined) timeout = 8000;
	if(method === undefined) method = "GET";
	if(onTimeout === undefined) {
		onTimeout = function(){
			console.log("ERROR: timeout loading file "+adr);
		};
	}
	if(onError === undefined){
		onError = function(){
			console.log("ERROR: loading file: "+adr);
		};
	}


	var request = new XMLHttpRequest();

	request.open(method, encodeURI(adr), true);
	request.timeout = timeout;
	request.ontimeout = onTimeout;
  request.onerror = onError;
	request.overrideMimeType("application/json");
	request.onreadystatechange = function() {
		if(this.readyState == 4){
			if(this.status == 200){
				callback(this.responseText);
			}
		}
	};

	request.send();
	console.log(adr);
}
