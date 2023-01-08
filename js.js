const HTTP = new XMLHttpRequest();
let heightHolder = 0;
const baseURL = 'http://192.168.4.1';
let viewFlag = false;
const calibrate = document.querySelector('button[name="calibrate"]');
const setHeight = document.querySelector('button[name="setHeight"]');
const setMinimum = document.querySelector('button[name="minimum"]');
const setMaximum = document.querySelector('button[name="maximum"]');
const calibrateButtonContainer = document.getElementById('calibrateButtons');
const heightSlider = document.getElementById('height');
const ambient = document.getElementById('ambient');
const target = document.getElementById('target');
const current = document.getElementById('current');
const currentHeight = document.getElementById('currentHeight');
const tempSlider = document.getElementById('tempSlider');
const tempInput = document.getElementById('target');

heightSlider.oninput = function() {
    currentHeight.innerHTML = this.value;
};
calibrate.addEventListener('click', function() {
    viewFlag = (viewFlag) ? false : true;
    if(!viewFlag){
    calibrateButtonContainer.style.display = 'none';
}
else{
    calibrateButtonContainer.style.display = 'flex';
}
});
setMaximum.addEventListener('click', function() {
    postRequest('0','max');
});
setMinimum.addEventListener('click', function() {
    postRequest('0','min');
});
tempSlider.oninput = function() {
    if(this.value > 699){
    target.setAttribute('style', 'color: red;');
        target.innerHTML = 'MAX';
    }
    else{ 
        target.setAttribute('style', 'color: white;');
        target.innerHTML = this.value;  
    }
};
setHeight.onclick = function(){
    postRequest(heightSlider.value,'height');
};
function postRequest(req,route){
    HTTP.open('POST', `${baseURL}/${route}/`);
    const body = JSON.stringify(req);
    HTTP.onload = () => {
        let res = JSON.parse(HTTP.responseText);
        current.textContent = res.current;
        ambient.textContent = res.ambient;
        if(res.height){
        currentHeight.textContent = res.height;
        currentHeight.value = res.height;
        heightSlider.value = res.height;
        }
        if(res.max){
            heightSlider.setAttribute("max",res.max);
        }
        if(res.min){
        heightSlider.setAttribute("min",res.min);
       }
    };
    HTTP.setRequestHeader('Content-Type', 'text/plain');
    HTTP.send(body);
};
function update(){
   postRequest('0','update');
}
setInterval(update, 2000);