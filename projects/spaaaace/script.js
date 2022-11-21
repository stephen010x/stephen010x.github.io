

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
};


//document.addEventListener("mousemove", mouseMoveHandler); 
