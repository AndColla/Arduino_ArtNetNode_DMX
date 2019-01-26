void startWebserver() {
  webserver.setDefaultCommand(&webIndex);
  webserver.addCommand("json", &webJson);
  webserver.begin();
}

void webIndex(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET) {
    /* store the HTML in program memory using the P macro */
    P(message) =
"<!DOCTYPE html><html> <head> <title>Arduino Art-Net Node</title> <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'/> <style>*{margin: 0;}header{width: 100%; padding: 20px; background: rgb(45,45,55); margin-bottom: 50px;}header > h1{color: white;}header > h2{color: rgb(100, 100, 100);}body{background: rgb(25,25,35); color: white; font-family: Verdana;}input{display: inline; padding: 10px; margin: 5px auto; -webkit-appearance: none; outline: 0; border: none;}.container{background: rgb(45,45,55); padding: 20px; width: 80%; margin: 25px auto 0px auto; text-align: center;}.container > h2{color: white; padding-bottom: 20px;}.ipInput{display: inline;}.ipByte{-webkit-appearance: none; outline: 0; border: none; width: 2em; text-align: center; padding: 5px;}.ipByte::-webkit-outer-spin-button, .ipByte::-webkit-inner-spin-button{-webkit-appearance: none; margin: 0;}.ipDot{margin: auto -2px auto -2px;}.button{margin-top: 20px; margin-bottom: 0px; padding: 10px 180px; font-weight: 700; font-size: 18px; background: #4CAF50; color: white;}.spacer{margin-left: 20px;}.rebootContainer{width: 80%; margin: auto;}.reboot{font-weight: 700; background: #F44336; color: white; font-size: 20px; margin-top: 20px; padding: 15px 50px;}.right{text-align: right;}.left{text-align: left;}table{margin: auto}tr{height: 37px;}</style> </head> <body> <header> <h1>Arduino Art-Net Node</h1> <h2>v0.1</h2> </header> <div class='container'> <h2>IP Configuration</h3> <form method='post'> <table> <tr> <td class='right'>IP Address:</td><td class='left'> <div class='ipInput'> <input type='number' class='ipByte' name='ip0' id='ip0' min='0' max='255'> <span class='ipDot'>.</span> <input type='number' class='ipByte' name='ip1' id='ip1' min='0' max='255'> <span class='ipDot'>.</span> <input type='number' class='ipByte' name='ip2' id='ip2' min='0' max='255'> <span class='ipDot'>.</span> <input type='number' class='ipByte' name='ip3' id='ip3' min='0' max='255'> </div></td></tr><tr> <td class='right'>Subnet Mask:</td><td class='left'> <div class='ipInput'> <input type='number' class='ipByte' name='mask0' id='mask0' min='0' max='255'> <span class='ipDot'>.</span> <input type='number' class='ipByte' name='maks1' id='mask1' min='0' max='255'> <span class='ipDot'>.</span> <input type='number' class='ipByte' name='mask2' id='mask2' min='0' max='255'> <span class='ipDot'>.</span> <input type='number' class='ipByte' name='mask3' id='mask3' min='0' max='255'> </div></td></tr><tr> <td class='right'>Mac Address:</td><td class='left' id='macAddress'></td></tr></table> <input type='submit' name='ip' class='button' value='Save'> </form> </div><div class='container'> <h2>Art-Net Configuration</h3> <form method='post'> <div> <span>Net:</span> <input type='number' class='ipByte' name='net' id='net' min='0' max='255'> <span class='spacer'></span> <span>Subnet:</span> <input type='number' class='ipByte' name='subnet' id='subnet' min='0' max='255'> <span class='spacer'></span> <span>Universe:</span> <input type='number' class='ipByte' name='universe' id='universe' min='0' max='255'> </div><input type='submit' name='artnet' class='button' value='Save'> </form> </div><div class='rebootContainer'> <form method='post'> <input type='submit' name='reboot' class='reboot' value='Reboot Device'> </form> </div><script>(function(){var xhttp=new XMLHttpRequest(); xhttp.onreadystatechange=function(){if (this.readyState==4 && this.status==200){var res=JSON.parse(this.responseText); var ip=res.ip.split('.'); for (var i=0; i < 4; i++){document.getElementById('ip'+i).value=ip[i];}var mask=res.mask.split('.'); for (var i=0; i < 4; i++){document.getElementById('mask'+i).value=mask[i];}document.getElementById('net').value=res.net; document.getElementById('subnet').value=res.subnet; document.getElementById('universe').value=res.universe; document.getElementById('macAddress').textContent=res.mac.toUpperCase();}}; xhttp.open('GET', 'json', true); xhttp.send();})(); </script> </body></html>";
    server.printP(message);
  }
}

void webJson(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET) {
    P(json1) = "{\"ip\": \"";
    P(json2) = "\",\"mask\": \"";
    P(json3) = "\",\"mac\": \"";
    P(json4) = "\",\"net\": ";
    P(json5) = ",\"subnet\": ";
    P(json6) = ",\"universe\": ";
    P(json7) = "}";
    P(dot) = ".";
    P(colon) = ":";

    server.printP(json1);
    for (int i=0; i<3; i++) {
      server.print(ip[i]);
      server.printP(dot);
    }
    server.print(ip[3]);
    
    server.printP(json2);
    for (int i=0; i<3; i++) {
      server.print(subnetMask[i]);
      server.printP(dot);
    }
    server.print(subnetMask[3]);

    server.printP(json3);
    for (int i=0; i<5; i++) {
      server.printf("%02x", mac[i]);
      server.printP(colon);
    }
    server.printf("%02x", mac[5]);
    
    server.printP(json4);
    server.print(artnetNet);
    
    server.printP(json5);
    server.print(artnetSubnet);
    
    server.printP(json6);
    server.print(artnetUniverse);
    
    server.printP(json7);
  }
}