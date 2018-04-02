Messaging = (function(global) {

	var VERSION = [ 0x03 ];// 版本号

	// 消息类型
	var MESSAGE_TYPE = {
		CONNECT : 1,
		CONNACK : 2,
		PUBLISH : 3,
		SUBSCRIBE : 8,
		SUBACK : 9,
		UNSUBSCRIBE : 10,
		UNSUBACK : 11,
		PINGREQ : 12,
		PINGRESP : 13,
		DISCONNECT : 14
	};

	// 传输消息
	var WireMessage = function(type) {
		this.type = type;
	};

	// 消息编码
	WireMessage.prototype.encode = function() {
		var first = ((this.type & 0x0F) << 4);
		var remLength = 0;
		remLength += VERSION.length;
		switch (this.type) {
		case MESSAGE_TYPE.CONNECT:
			remLength += UTF8Length(this.clientId) + 2;
			remLength += UTF8Length(this.token) + 2;
			break;
		default:
			;
		}
		var pos = 1;
		var buffer = new ArrayBuffer(remLength + pos);
		var byteStream = new Uint8Array(buffer);
		byteStream[0] = first;// messageType;
		byteStream.set(VERSION, pos); // version
		pos += VERSION.length;
		// connect判断
		switch (this.type) {
		case MESSAGE_TYPE.CONNECT:
			pos = writeString(this.clientId, UTF8Length(this.clientId),
					byteStream, pos);
			pos = writeString(this.token, UTF8Length(this.token), byteStream,
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
		var byte1 = input[0]; // messageType
		var msgType = byte1 >> 4; // 位移
		var pos = 1;
		var version = input[pos++]; // 版本号
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
	var PushClient = function(host, port, clientId, token) {
		if (!("WebSocket" in global && global["WebSocket"] !== null)) {
			throw new Error('Not Support WebSocket!');
		}
		if (!("ArrayBuffer" in global && global["ArrayBuffer"] !== null)) {
			throw new Error('Not Support ArrayBuffer!');
		}
		this.host = host;
		this.port = port;
		this.clientId = clientId;
		this.token = token;
	};

	// Client公共参数
	PushClient.prototype.host;
	PushClient.prototype.port;
	PushClient.prototype.clientId;
	PushClient.prototype.token;

	// socket对象
	PushClient.prototype.socket;
	// 连接状态
	PushClient.prototype.connected = false;
	// 连接参数
	PushClient.prototype.connectOptions;
	// 连接超时检测句柄
	PushClient.prototype._connectTimeout = null;
	// 心跳
	PushClient.prototype._heartbeat = null;

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
		//var wireMessage = new WireMessage(MESSAGE_TYPE.CONNECT);
		//wireMessage.clientId = this.clientId;
		//wireMessage.token = this.token;
		//this._socket_send(wireMessage);
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
