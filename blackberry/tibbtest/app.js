// this sets the background color of the master UIView (when there are no windows/tab groups on it)
//Titanium.UI.setBackgroundColor('#000');

//
// create root window
//
var win1 = Titanium.UI.createWindow({  
    backgroundColor:'#F00'
});

var label1 = Ti.UI.createLabel({
	textAlign : 'center',
	font: {fontSize:12},
	color:'green',
	top: 200
});


var client = Ti.Network.createHTTPClient({
validatesSecureCertificate: true,
timeout: 100,
     onLoad : function(e) {
             Ti.API.info("onLoad is called!");
     },
     onError : function(e) {
             Ti.API.info("onError is called!");
     },
});

Ti.API.info("Timeout: " + client.getTimeout());
Ti.API.info("Sec Sert: " + client.getValidatesSecureCertificate());

var url = "http://www.appcelerator.com";

client.open("GET", url);
client.send();


win1.add(label1);
win1.open();