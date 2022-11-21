

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
	console.log("page loaded")
};
document.getElementById("game").onload = (event) => {
	console.log("game scripts loaded")
}


//document.addEventListener("mousemove", mouseMoveHandler); 
