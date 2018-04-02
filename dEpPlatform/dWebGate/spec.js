if (typeof module !== 'undefined') 
	require('push.js'); 
var testServer = "localhost"; 
var testPort = 9030; 


describe('client', function() { 
	var client = this; 
	var connected = false; 
	var subscribed = false; 
 	var messageReceived = false; 
 

 	function onConnect() { 
 		console.log("connected"); 
 		connected = true; 
 	}; 
 

 	function onSubscribe() { 
 		console.log("subscribed"); 
 		subscribed = true; 
 	}; 
 

 	function messageArrived(response) { 
 		console.log("messageArrived"); 
 		messageReceived = true; 
 		//reponse.invocationContext.onMessageArrived = null; 
 	}; 
 

 	it('should create a new client', function() { 
 		client = new Messaging.Client(testServer, testPort, "testclientid"); 
 		client.onMessageArrived = messageArrived; 
 

 		expect(client).not.toBe(null); 
 	}); 
 

 	it('should connect to a server', function() { 
 		runs(function() { 
 			client.connect({onSuccess:onConnect}); 
 		}); 
 

 		waitsFor(function() { 
 			return connected; 
 		}, "the client should connect", 2000); 
 

 		runs(function() { 
 			expect(connected).toBe(true); 
 		}); 
 	}); 
 

 	it('should subscribe to a topic', function() { 
 		runs(function() { 
 			client.subscribe("/World", {onSuccess:onSubscribe}); 
 		}); 
 

 		waitsFor(function() { 
 			return subscribed; 
 		}, "the client should subscribe", 2000); 
 

 		runs(function() { 
 			expect(subscribed).toBe(true); 
 		}); 
 	}); 
 

 	it('should send and receive a message', function() { 
 		runs(function() { 
 			message = new Messaging.Message("Hello"); 
 			message.destinationName = "/World"; 
 			client.send(message);  
 		}) 
 

 		waitsFor(function() { 
 			return messageReceived; 
 		}, "the client should send and receive a message", 2000); 
 

 		runs(function() { 
 			expect(messageReceived).toBe(true); 
 		}) 
 	}); 
 }) 
