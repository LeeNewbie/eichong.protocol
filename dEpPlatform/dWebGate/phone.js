Messaging = (function(global) {

	var HEAD_FLAG1 = [ 0x45 ];
	var HEAD_FLAG2 = [ 0x43 ];

	// 消息类型
	var MESSAGE_TYPE = {
		D_CONNET_EP : 1,
		D_NEW_CONNET_EP : 20,
		D_VERSION_CONNET_EP : 21,
		D_HEART : 2,
		D_START_CHARGE : 10,
		D_STOP_CHARGE : 11,
		D_CONSUME_RECORD : 12
	};

	// 传输消息
    var WireMessage = function (type, options) { 	
        this.type = type;
 		options.epCode="3301021010000008";
		options.epGunNo="1";
		options.accountid="1501";
		options.checkCode="3301021010000000";
        for(name in options) {
            if (options.hasOwnProperty(name)) {
                this[name] = options[name];
            }
        }
    };

    var validate = function(obj, keys) {
        for(key in obj) {
        	if (obj.hasOwnProperty(key) && keys.hasOwnProperty(key)) {
        	  if (typeof obj[key] !== keys[key])
        		 throw new Error(format(ERROR.INVALID_TYPE, [typeof obj[key], key]));
        	} else {	
            	var errorStr = "Unknown property, " + key + ". Valid properties are:";
            	for (key in keys)
            		if (keys.hasOwnProperty(key))
            		    errorStr = errorStr+" "+key;
            	throw new Error(errorStr);
            }
        }
    };
    var ERROR = {
    	CONNECT_TIMEOUT: {code:1, text:"AMQJSC0001E Connect timed out."},
        SUBSCRIBE_TIMEOUT: {code:2, text:"AMQJS0002E Subscribe timed out."}, 
        UNSUBSCRIBE_TIMEOUT: {code:3, text:"AMQJS0003E Unsubscribe timed out."},
        PING_TIMEOUT: {code:4, text:"AMQJS0004E Ping timed out."},
        INTERNAL_ERROR: {code:5, text:"AMQJS0005E Internal error."},
        CONNACK_RETURNCODE: {code:6, text:"AMQJS0006E Bad Connack return code:{0} {1}."},
        SOCKET_ERROR: {code:7, text:"AMQJS0007E Socket error:{0}."},
        SOCKET_CLOSE: {code:8, text:"AMQJS0008I Socket closed."},
        MALFORMED_UTF: {code:9, text:"AMQJS0009E Malformed UTF data:{0} {1} {2}."},
        UNSUPPORTED: {code:10, text:"AMQJS0010E {0} is not supported by this browser."},
        INVALID_STATE: {code:11, text:"AMQJS0011E Invalid state {0}."},
        INVALID_TYPE: {code:12, text:"AMQJS0012E Invalid type {0} for {1}."},
        INVALID_ARGUMENT: {code:13, text:"AMQJS0013E Invalid argument {0} for {1}."},
        UNSUPPORTED_OPERATION: {code:14, text:"AMQJS0014E Unsupported operation."},
        INVALID_STORED_DATA: {code:15, text:"AMQJS0015E Invalid data in local storage key={0} value={1}."}
    };
    var format = function(error, substitutions) {
    	var text = error.text;
    	if (substitutions) {
    	  for (var i=0; i<substitutions.length; i++) {
    		field = "{"+i+"}";
    		start = text.indexOf(field);
    		if(start > 0) {
    			var part1 = text.substring(0,start);
    			var part2 = text.substring(start+field.length);
    			text = part1+substitutions[i]+part2;
    		}
    	  }
    	}
    	return text;
    };

	// 消息编码
	WireMessage.prototype.encode = function() {
		//var cmd = ((this.type & 0x0F) << 4);
		var cmd = this.type;
		var remLength = 0;
		remLength += HEAD_FLAG1.length;
		remLength += HEAD_FLAG2.length;
		switch (this.type) {
		case MESSAGE_TYPE.D_CONNET_EP:
		case MESSAGE_TYPE.D_NEW_CONNET_EP:
		case MESSAGE_TYPE.D_VERSION_CONNET_EP:
			remLength += UTF8Length(this.epCode) + 2;
			remLength += UTF8Length(this.epGunNo) + 2;
			remLength += UTF8Length(this.accountid) + 2;
			remLength += UTF8Length(this.checkCode) + 2;
			break;
		case MESSAGE_TYPE.D_START_CHARGE:
			remLength += UTF8Length(this.fronzeAmt) + 2;
			remLength += UTF8Length(this.chargeType) + 2;
			break;
		case MESSAGE_TYPE.D_HEART:
			break;
		case MESSAGE_TYPE.D_STOP_CHARGE:
			break;
		case MESSAGE_TYPE.D_START_CHARGE:
			remLength += UTF8Length(this.chargeOrder) + 2;
			remLength += UTF8Length(this.result) + 2;
			break;
		default:
			;
		}
		var buffer = new ArrayBuffer(remLength + 1);
		var byteStream = new Uint8Array(buffer);
		var pos = 0;
		byteStream.set(HEAD_FLAG1, pos);
		pos += HEAD_FLAG1.length;
		byteStream.set(HEAD_FLAG2, pos);
		pos += HEAD_FLAG2.length;
		byteStream.set(remLength, pos);
		pos += remLength.length;
		byteStream.set(cmd, pos);
		pos += cmd.length;
		// connect判断
		switch (this.type) {
		case MESSAGE_TYPE.D_CONNET_EP:
		case MESSAGE_TYPE.D_NEW_CONNET_EP:
		case MESSAGE_TYPE.D_VERSION_CONNET_EP:
			pos = writeString(this.epCode, UTF8Length(this.epCode),
					byteStream, pos);
			pos = writeString(this.epGunNo, UTF8Length(this.epGunNo), byteStream,
					pos);
			pos = writeString(this.accountid, UTF8Length(this.accountid),
					byteStream, pos);
			pos = writeString(this.checkCode, UTF8Length(this.checkCode),
					byteStream, pos);
			break;
		case MESSAGE_TYPE.D_START_CHARGE:
			pos = writeString(this.fronzeAmt, UTF8Length(this.fronzeAmt),
					byteStream, pos);
			pos = writeString(this.chargeType, UTF8Length(this.chargeType), byteStream,
					pos);
			break;
		case MESSAGE_TYPE.D_HEART:
			break;
		case MESSAGE_TYPE.D_STOP_CHARGE:
			break;
		case MESSAGE_TYPE.D_START_CHARGE:
			pos = writeString(this.chargeOrder, UTF8Length(this.chargeOrder),
					byteStream, pos);
			pos = writeString(this.result, UTF8Length(this.result), byteStream,
					pos);
			break;
		default:
			;
		}
		return buffer;
	};

	/**
	 * 消息解码
	 */
	function decodeMessage(input) {
		//var byte1 = input[0]; // messageType
		//var msgType = byte1 >> 4; // 位移
		var pos = 0;
		var version = input[pos++];
		var wireMessage = new WireMessage(msgType);
		wireMessage.version = version;
		switch (msgType) {
		case MESSAGE_TYPE.CONNACK:
			wireMessage.returnCode = input[pos++];
			break;
		default:
			;
		}
		return wireMessage;
	}

	// 参数合并
	var scope = function(f, scope) {
		return function() {
			return f.apply(scope, arguments);
		};
	};

	/**
	 * PushClient
	 */
	var PushClient = function(host, port) {
		if (!("WebSocket" in global && global["WebSocket"] !== null)) {
			throw new Error('Not Support WebSocket!');
		}
        if (!("localStorage" in global && global["localStorage"] !== null)) {
         	throw new Error(format(ERROR.UNSUPPORTED, ["localStorage"]));
        }
		if (!("ArrayBuffer" in global && global["ArrayBuffer"] !== null)) {
			throw new Error('Not Support ArrayBuffer!');
		}
		this.host = host;
		this.port = port;
        this._localKey=host+":"+port+":";

		for(key in localStorage)
        	if (   key.indexOf("Sent:"+this._localKey) == 0  		    
        	    || key.indexOf("Received:"+this._localKey) == 0)
        	this.restore(key);
	};

	// Client公共参数
	PushClient.prototype.host;
	PushClient.prototype.port;

	// socket对象
	PushClient.prototype.socket;
	// 连接状态
	PushClient.prototype.connected = false;
	// 连接参数
	PushClient.prototype.connectOptions = {};
	// 连接超时检测句柄
	PushClient.prototype._connectTimeout = null;
	// 心跳
	PushClient.prototype._heartbeat = null;

    PushClient.prototype.store = function(prefix, wireMessage) {
    	storedMessage = {type:wireMessage.type, messageIdentifier:wireMessage.messageIdentifier, version:1};
    	
    	switch(wireMessage.type) {
	      case MESSAGE_TYPE.PUBLISH:
	    	  if(wireMessage.pubRecReceived)
	    		  storedMessage.pubRecReceived = true;
	    	  
	    	  // Convert the payload to a hex string.
	    	  storedMessage.payloadMessage = {};
	    	  var hex = "";
	          var messageBytes = wireMessage.payloadMessage.payloadBytes;
	          for (var i=0; i<messageBytes.length; i++) {
	            if (messageBytes[i] <= 0xF)
	              hex = hex+"0"+messageBytes[i].toString(16);
	            else 
	              hex = hex+messageBytes[i].toString(16);
	          }
	    	  storedMessage.payloadMessage.payloadHex = hex;
	    	  
	    	  storedMessage.payloadMessage.qos = wireMessage.payloadMessage.qos;
	    	  storedMessage.payloadMessage.destinationName = wireMessage.payloadMessage.destinationName;
	    	  if (wireMessage.payloadMessage.duplicate) 
	    		  storedMessage.payloadMessage.duplicate = true;
	    	  if (wireMessage.payloadMessage.retained) 
	    		  storedMessage.payloadMessage.retained = true;	   
	    	  
	    	  // Add a sequence number to sent messages.
	    	  if ( prefix.indexOf("Sent:") == 0 ) {
	    		  if ( wireMessage.sequence === undefined )
	    		      wireMessage.sequence = ++this._sequence;
	    		  storedMessage.sequence = wireMessage.sequence;
	    	  }
	          break;    
	          
	        default:
	        	throw Error(format(ERROR.INVALID_STORED_DATA, [key, storedMessage]));
  	    }
    	localStorage.setItem(prefix+this._localKey+wireMessage.messageIdentifier, JSON.stringify(storedMessage));
    };
   
	PushClient.prototype.restore = function(key) {    	
    	var value = localStorage.getItem(key);
    	var storedMessage = JSON.parse(value);
    	
    	var wireMessage = new WireMessage(storedMessage.type, storedMessage);
    	
    	switch(storedMessage.type) {
	      case MESSAGE_TYPE.PUBLISH:
	    	  // Replace the payload message with a Message object.
	    	  var hex = storedMessage.payloadMessage.payloadHex;
	    	  var buffer = new ArrayBuffer((hex.length)/2);
              var byteStream = new Uint8Array(buffer); 
              var i = 0;
              while (hex.length >= 2) { 
            	  var x = parseInt(hex.substring(0, 2), 16);
	              hex = hex.substring(2, hex.length);
	              byteStream[i++] = x;
	          }
              var payloadMessage = new Messaging.Message(byteStream);
	      	  
	    	  payloadMessage.qos = storedMessage.payloadMessage.qos;
	    	  payloadMessage.destinationName = storedMessage.payloadMessage.destinationName;
              if (storedMessage.payloadMessage.duplicate) 
	    		  payloadMessage.duplicate = true;
	    	  if (storedMessage.payloadMessage.retained) 
	    		  payloadMessage.retained = true;	 
	    	  wireMessage.payloadMessage = payloadMessage;
              
	          break;    
	          
	        default:
	          throw Error(format(ERROR.INVALID_STORED_DATA, [key, value]));
	    }
    	    		    	
    	if (key.indexOf("Sent:"+this._localKey) == 0) {      
    		this._sentMessages[wireMessage.messageIdentifier] = wireMessage;    		    
    	} else if (key.indexOf("Received:"+this._localKey) == 0) {
    		this._receivedMessages[wireMessage.messageIdentifier] = wireMessage;
    	}
    };

	/**
	 * 连接操作
	 * 
	 * @param connectOptions:连接参数
	 */
	PushClient.prototype.connect = function(connectOptions) {
		// 判断当前是否为连接状态
		if (this.connected) {
			throw new Error('The Connect Status Is Connect!');
		}
		if (this.socket) {
			throw new Error('The Socket Status Is Socket!');
		}
		this.connectOptions = connectOptions;
		debug = connectOptions.debug;
		if (connectOptions.hosts) {
			this._doConnect(connectOptions.hosts[0], connectOptions.ports[0]);
		} else {
			this._doConnect(this.host, this.port);
		}
	};

	/**
	 * 连接操作
	 * 
	 * @param host:主机地址
	 * @param port:主机端口
	 */
	PushClient.prototype._doConnect = function(host, port) {
		var wsurl = [ "ws://", host, ":", port, "/ws" ].join("");
		this.connected = false;
		this.socket = new WebSocket(wsurl);
		this.socket.binaryType = 'arraybuffer';
		// 连接成功回调
		this.socket.onopen = scope(this.on_socket_open_handler, this);
		this.socket.onmessage = scope(this.on_socket_message_handler, this);
		this.socket.onerror = scope(this.on_socket_error_handler, this);
		this.socket.onclose = scope(this.on_socket_close_hander, this);

		this._heartbeat = new HeartBeat(this, window,
				this.connectOptions.keepAliveInterval);
		this._connectTimeout = new ConnectTimeout(this, window,
				this.connectOptions.timeout, this._disconnected,
				'Client Connect Server Timeout!');

	};

	/**
	 * 建立socket连接调用
	 */
	PushClient.prototype.on_socket_open_handler = function() {
		var wireMessage = new WireMessage(MESSAGE_TYPE.D_CONNET_EP, this.connectOptions);
		//wireMessage.clientId = this.clientId;
		//wireMessage.token = this.token;
		this._socket_send(wireMessage);
		alert("连接成功");
	};

	/**
	 * 建立连接后回调的消息
	 * 
	 * @param event
	 */
	PushClient.prototype.on_socket_message_handler = function(event) {
		var byteArray = new Uint8Array(event.data);
		try {
			var wireMessage = decodeMessage(byteArray);
		} catch (error) {
			this._disconnected('Internal error.');
			return;
		}
		var msgType = wireMessage.type;
		switch (msgType) {
		// 连接成功响应
		case MESSAGE_TYPE.CONNACK:
			this._connectTimeout.cancel();
			if (wireMessage.returnCode === 0) {
				console.log('msgType=' + msgType + ' 当前用户:' + this.clientId
						+ '与Websocket服务器连接成功!');
				this.connected = true;
			}
			// 如果回调函数存在
			if (this.connectOptions.onSuccess) {
				this.connectOptions.onSuccess({
					'retCode' : 'OK',
					'msg' : 'Websocket连接成功!'
				});
			}
			break;
		// 心跳响应
		case MESSAGE_TYPE.PINGRESP:
			console.log('msgType=' + msgType + ' 当前用户:' + this.clientId
					+ "与Websocket服务器心跳正常!");
			this._heartbeat.reset();
			break;
		default:
			;
		}
	};

	/**
	 * socket通信错误
	 * 
	 * @param error
	 */
	PushClient.prototype.on_socket_error_handler = function(error) {
		alert(error);
	};

	/**
	 * socket关闭
	 */
	PushClient.prototype.on_socket_close_hander = function() {
		alert('c');
	};

	/**
	 * 发送数据
	 * 
	 * @param wireMessage:消息数据
	 */
	PushClient.prototype._socket_send = function(wireMessage) {
		this.socket.send(wireMessage.encode());
		this._heartbeat.reset();
	};

	/**
	 * 关闭连接
	 * 
	 * @param errorText:错误消息
	 */
	PushClient.prototype._disconnected = function(errorText) {
		console.log("errorText:" + errorText);
		this._heartbeat.cancel();
		if (this.socket) {
			// Cancel all socket callbacks so that they cannot be driven again
			// by this socket.
			this.socket.onopen = null;
			this.socket.onmessage = null;
			this.socket.onerror = null;
			this.socket.onclose = null;
			if (this.socket.readyState === 1)
				this.socket.close();
			delete this.socket;
		}
		if (this.connected) {
			this.connected = false;
		}
	};

	/**
	 * 心跳
	 */
	var HeartBeat = function(client, window, keepAliveInterval) {
		if (!keepAliveInterval) {
			keepAliveInterval = 10; // 如果10秒没有心跳响应,则认为连接出现失败
		}
		this._client = client; // 客户端
		this._window = window; // 当前window
		this._keepAliveInterval = keepAliveInterval * 1000;
		this.isReset = false;

		var doTimeout = function(pinger) {
			return function() {
				return doPing.apply(pinger);
			};
		};

		var doPing = function() {
			if (!this.isReset) {
				this._client._disconnected('Client Ping Server Timeout');
			} else {
				this.isReset = false;
				var pingReq = new WireMessage(MESSAGE_TYPE.PINGREQ);
				this._client.socket.send(pingReq.encode());
				this.timeout = this._window.setTimeout(doTimeout(this),
						this._keepAliveInterval);
			}
		};

		this.reset = function() {
			this.isReset = true;
			this._window.clearTimeout(this.timeout);
			if (this._keepAliveInterval > 0)
				this.timeout = setTimeout(doTimeout(this),
						this._keepAliveInterval);
		};

		this.cancel = function() {
			this._window.clearTimeout(this.timeout);
		};
	};

	/**
	 * 超时检测
	 */
	var ConnectTimeout = function(client, window, timeoutSeconds, action, args) {
		this._window = window;
		if (!timeoutSeconds) {
			timeoutSeconds = 30;
		}
		var doTimeout = function(action, client, args) {
			return function() {
				return action.apply(client, args);
			};
		};
		this.timeout = setTimeout(doTimeout(action, client, args),
				timeoutSeconds * 1000);
		// 取消连接
		this.cancel = function() {
			this._window.clearTimeout(this.timeout);
		};
	};

	return {
		Client : PushClient
	};
})(window);

// 采用UTF8计算长度
function UTF8Length(input) {
	var output = 0;
	for ( var i = 0; i < input.length; i++) {
		var charCode = input.charCodeAt(i);
		if (charCode > 0x7FF) {
			// Surrogate pair means its a 4 byte character
			if (0xD800 <= charCode && charCode <= 0xDBFF) {
				i++;
				output++;
			}
			output += 3;
		} else if (charCode > 0x7F)
			output += 2;
		else
			output++;
	}
	return output;
}

// 转码
function writeString(input, utf8Length, buffer, offset) {
	offset = writeUint16(utf8Length, buffer, offset);
	stringToUTF8(input, buffer, offset);
	return offset + utf8Length;
}

// 算出长度
function writeUint16(input, buffer, offset) {
	buffer[offset++] = input >> 8;
	buffer[offset++] = input % 256;
	return offset;
}

// 字符串转码为UTF8
function stringToUTF8(input, output, start) {
	var pos = start;
	for ( var i = 0; i < input.length; i++) {
		var charCode = input.charCodeAt(i);
		// Check for a surrogate pair.
		if (0xD800 <= charCode && charCode <= 0xDBFF) {
			lowCharCode = input.charCodeAt(++i);
			charCode = ((charCode - 0xD800) << 10) + (lowCharCode - 0xDC00)
					+ 0x10000;
		}
		if (charCode <= 0x7F) {
			output[pos++] = charCode;
		} else if (charCode <= 0x7FF) {
			output[pos++] = charCode >> 6 & 0x1F | 0xC0;
			output[pos++] = charCode & 0x3F | 0x80;
		} else if (charCode <= 0xFFFF) {
			output[pos++] = charCode >> 12 & 0x0F | 0xE0;
			output[pos++] = charCode >> 6 & 0x3F | 0x80;
			output[pos++] = charCode & 0x3F | 0x80;
		} else {
			output[pos++] = charCode >> 18 & 0x07 | 0xF0;
			output[pos++] = charCode >> 12 & 0x3F | 0x80;
			output[pos++] = charCode >> 6 & 0x3F | 0x80;
			output[pos++] = charCode & 0x3F | 0x80;
		}
	}
	return output;
}
