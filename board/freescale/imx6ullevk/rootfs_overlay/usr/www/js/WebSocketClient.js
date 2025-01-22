import { Logger } from './Logger.js';

class WebSocketClient {
    constructor(wsUri, options = {}) {
        this.wsUri = wsUri;
        this.reconnectInterval = options.reconnectInterval || 30000; // Default to 30 seconds
        this.websocket = null;
        this.messageCallbacks = [];
        this.messageQueue = [];
        this.logger = new Logger('log-output');
        this.isConnected = false;

        // Attempt to connect immediately upon instantiation
        this.connect();
    }

    connect() {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.logger.info("WebSocket is already open.");
            return Promise.resolve(this.websocket);
        }

        return new Promise((resolve, reject) => {
            this.websocket = new WebSocket(this.wsUri, 'WEBSOCKET_SIMPLE_PROTOCOL');

            this.websocket.onopen = () => {
                this.isConnected = true;
                this.logger.info("Connected to WebSocket server.");
                this.processQueuedMessages(); // Send any queued messages.
                resolve(this.websocket);
            };

            this.websocket.onerror = (error) => {
                this.logger.error(`WebSocket error observed: ${error.message}`);
                this.handleReconnection().catch(err => this.logger.error(`Reconnection failed: ${err.message}`));
                reject(error);
            };

            this.websocket.onclose = (event) => {
                this.isConnected = false;
                this.logger.info(`WebSocket closed. Code: ${event.code}, Reason: ${event.reason}`);
                this.handleReconnection().catch(err => this.logger.error(`Reconnection failed: ${err.message}`));
                reject(new Error('WebSocket connection closed'));
            };

            this.websocket.onmessage = this.receiveMessage.bind(this); // Use the method for receiving messages
        });
    }

    disconnect() {
        if (this.websocket && this.websocket.readyState === WebSocket.OPEN) {
            this.websocket.close();
            this.isConnected = false;
            this.logger.info("WebSocket disconnected.");
        }
    }

    sendMessage(type, data = null) {
        return new Promise((resolve, reject) => {
            if (!this.isConnected) {
                this.queueMessage({ type, data });
                this.handleReconnection().catch(err => this.logger.error(`Reconnection failed: ${err.message}`));
                reject(new Error("WebSocket is not open."));
                return;
            }

            try {
                const message = { type, data };
                this.websocket.send(JSON.stringify(message));
                this.logger.info(`Sent message: ${JSON.stringify(message)}`);
                resolve();
            } catch (error) {
                this.logger.error("An error occurred while sending the message.");
                reject(error);
            }
        });
    }

    receiveMessage(event) {
        try {
            const message = JSON.parse(event.data);
            this.logger.info(`Received message: ${JSON.stringify(message)}`);

            // Notify callbacks with the received message
            this.messageCallbacks.forEach(callback => callback(message));
        } catch (error) {
            this.logger.error(`Failed to parse incoming message: ${error.message}`);
        }
    }

    onMessage(callback) {
        this.messageCallbacks.push(callback);
    }

    queueMessage(data) {
        this.messageQueue.push(data);
    }

    processQueuedMessages() {
        while (this.messageQueue.length > 0) {
            const data = this.messageQueue.shift();
            this.sendMessage(data).catch(err => this.logger.error(err));
        }
    }

    handleReconnection() {
        return new Promise((resolve, reject) => {
            setTimeout(() => {
                this.logger.info("Attempting to reconnect...");
                this.connect()
                    .then(resolve)
                    .catch(err => {
                        this.logger.error(`Reconnection failed: ${err.message}`);
                        reject(err);
                    });
            }, this.reconnectInterval);
        });
    }
}

export { WebSocketClient };
