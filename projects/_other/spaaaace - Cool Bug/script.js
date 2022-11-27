

function mouseStillHandler() {
	document.body.style.cursor = 'none';
}
function mouseMoveHandler() {
	document.body.style.cursor = 'auto'
}


var timeout;
window.onload = (event) => {
	
	document.body.style.cursor = 'none';
	document.body.onmousemove = function(){
		mouseMoveHandler();
		clearTimeout(timeout);
		timeout = setTimeout(mouseStillHandler, 500);
	}
	var loadTime = window.performance.timing.domContentLoadedEventEnd- window.performance.timing.navigationStart;
	console.log("page loaded in " + loadTime/1000 + " Seconds")
	
};


function gameLoaded() {
	console.log('game scripts loaded');
}


//document.addEventListener("mousemove", mouseMoveHandler); 
