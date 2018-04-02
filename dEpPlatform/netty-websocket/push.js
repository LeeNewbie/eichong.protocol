Messaging = (function(global) {

	var VERSION = [ 0x03 ];// �汾��

	// ��Ϣ����
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

	// ������Ϣ
	var WireMessage = function(type) {
		this.type = type;
	};

	// ��Ϣ����
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
		// connect�ж�
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
	 * ��Ϣ����
	 */
	function decodeMessage(input) {
		var byte1 = input[0]; // messageType
		var msgType = byte1 >> 4; // λ��
		var pos = 1;
		var version = input[pos++]; // �汾��
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

	// �����ϲ�
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

	// Client��������
	PushClient.prototype.host;
	PushClient.prototype.port;
	PushClient.prototype.clientId;
	PushClient.prototype.token;

	// socket����
	PushClient.prototype.socket;
	// ����״̬
	PushClient.prototype.connected = false;
	// ���Ӳ���
	PushClient.prototype.connectOptions;
	// ���ӳ�ʱ�����
	PushClient.prototype._connectTimeout = null;
	// ����
	PushClient.prototype._heartbeat = null;

	/**
	 * ���Ӳ���
	 * 
	 * @param connectOptions:���Ӳ���
	 */
	PushClient.prototype.connect = function(connectOptions) {
		// �жϵ�ǰ�Ƿ�Ϊ����״̬
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
		this._connectTimeout = new ConnectTimeout(this, window, connectOptions.timeout, this._disconnected,  [ERROR.CONNECT_TIMEOUT.code, format(ERROR.CONNECT_TIMEOUT)]);
	};

	/**
	 * ���Ӳ���
	 * 
	 * @param host:������ַ
	 * @param port:�����˿�
	 */
	PushClient.prototype._doConnect = function(host, port) {
		var wsurl = [ "ws://", host, ":", port, "/push" ].join("");
		this.connected = false;
		this.socket = new WebSocket(wsurl);
		this.socket.binaryType = 'arraybuffer';
		// ���ӳɹ��ص�
		this.socket.onopen = scope(this.on_socket_open_handler, this);
		this.socket.onmessage = scope(this.on_socket_message_handler, this);
		this.socket.onerror = scope(this.on_socket_error_handler, this);
		this.socket.onclose = scope(this.on_socket_close_hander, this);

		this._heartbeat = new HeartBeat(this, window,
				this.connectOptions.keepAliveInterval);

	};

	/**
	 * ����socket���ӵ���
	 */
	PushClient.prototype.on_socket_open_handler = function() {
		var wireMessage = new WireMessage(MESSAGE_TYPE.CONNECT);
		wireMessage.clientId = this.clientId;
		wireMessage.token = this.token;
		this._socket_send(wireMessage);
	};
	PushClient.prototype.send_message = function(message) {
		var wireMessage = new WireMessage(MESSAGE_TYPE.SUBSCRIBE);
		wireMessage.clientId = this.clientId;
		wireMessage.token = this.token;
		wireMessage.message = message;
		this._socket_send(wireMessage);
	};

	/**
	 * �������Ӻ�ص�����Ϣ
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
		// ���ӳɹ���Ӧ
		case MESSAGE_TYPE.CONNACK:
			this._connectTimeout.cancel();
			if (wireMessage.returnCode === 0) {
				console.log('msgType=' + msgType + ' ��ǰ�û�:' + this.clientId
						+ '��Websocket���������ӳɹ�!');
				this.connected = true;
			}
			// ����ص���������
			if (this.connectOptions.onSuccess) {
				this.connectOptions.onSuccess({
					'retCode' : 'OK',
					'msg' : 'Websocket���ӳɹ�!'
				});
			}
			break;
		// ������Ӧ
		case MESSAGE_TYPE.PINGRESP:
			console.log('msgType=' + msgType + ' ��ǰ�û�:' + this.clientId
					+ "��Websocket��������������!");
			this._heartbeat.reset();
			break;
		default:
			;
		}
	};

	/**
	 * socketͨ�Ŵ���
	 * 
	 * @param error
	 */
	PushClient.prototype.on_socket_error_handler = function(error) {
		alert('b');
	};

	/**
	 * socket�ر�
	 */
	PushClient.prototype.on_socket_close_hander = function() {
		alert('c');
	};

	/**
	 * ��������
	 * 
	 * @param wireMessage:��Ϣ����
	 */
	PushClient.prototype._socket_send = function(wireMessage) {
		this.socket.send(wireMessage.encode());
		this._heartbeat.reset();
	};

	/**
	 * �ر�����
	 * 
	 * @param errorText:������Ϣ
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
	 * ����
	 */
	var HeartBeat = function(client, window, keepAliveInterval) {
		if (!keepAliveInterval) {
			keepAliveInterval = 10; // ���10��û��������Ӧ,����Ϊ���ӳ���ʧ��
		}
		this._client = client; // �ͻ���
		this._window = window; // ��ǰwindow
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
	 * ��ʱ���
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
		// ȡ������
		this.cancel = function() {
			this._window.clearTimeout(this.timeout);
		};
	};

	return {
		Client : PushClient
	};
})(window);

// ����UTF8���㳤��
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

// ת��
function writeString(input, utf8Length, buffer, offset) {
	offset = writeUint16(utf8Length, buffer, offset);
	stringToUTF8(input, buffer, offset);
	return offset + utf8Length;
}

// �������
function writeUint16(input, buffer, offset) {
	buffer[offset++] = input >> 8;
	buffer[offset++] = input % 256;
	return offset;
}

// �ַ���ת��ΪUTF8
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
