const setHeight = document.querySelector('button[name="setHeight"]');
        const heightSlider = document.getElementById('height');
        const ambient = document.getElementById('ambient');
        const target = document.getElementById('target');
        const current = document.getElementById('current');
        const currentHeight = document.getElementById('currentHeight');
        heightSlider.oninput = function() {
            currentHeight.innerHTML = this.value;
            postRequest(this.value,'height')
        };
        const tempSlider = document.getElementById('tempSlider');
        const tempInput = document.getElementById('target');
        tempSlider.oninput = function() {
            if(this.value > 699){
            target.setAttribute('style', 'color: red;');
                target.innerHTML = 'MAX';
                console.log(target.innerHTML)
            }
            else{ 
                target.setAttribute('style', 'color: white;');
                target.innerHTML = this.value;  
            }
        };
        setHeight.onclick = function(){
            postRequest(heightSlider.value,'height');
            console.log(heightSlider.value)
        }
        const HTTP = new XMLHttpRequest();
        const baseURL = 'http://192.168.4.1';
        function postRequest(req,route){
            HTTP.open('POST', `${baseURL}/${route}/`);
            const body = JSON.stringify(req);
            HTTP.onload = () => {
                let res = JSON.parse(HTTP.responseText);
                console.log(HTTP.responseText);
                current.textContent = res.current;
                ambient.textContent = res.ambient;
                currentHeight.textContent = res.height;
                currentHeight.value = res.height;
                heightSlider.setAttribute("min",res.min);
                heightSlider.setAttribute("max",res.max);
            };
            HTTP.setRequestHeader('Content-Type', 'text/plain');
            HTTP.send(body);
            
            
       
        };
        function update(){
           postRequest('0','update');
        }
        setInterval(update, 2000);