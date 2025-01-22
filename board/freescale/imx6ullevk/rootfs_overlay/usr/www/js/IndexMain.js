import { WebSocketClient } from './WebSocketClient.js';
import { Logger } from './Logger.js';

const messagesDiv = document.getElementById('messages');
const logOutputDiv = document.getElementById('log-output');
const logger = new Logger(logOutputDiv);
let wsManager;

// Utility function to make elements resizable with min size constraints
function makeResizable(div) {
    const resizeHandle = document.createElement("div");
    resizeHandle.classList.add("resize-handle");
    div.appendChild(resizeHandle);

    let startX, startY, startWidth, startHeight;

    resizeHandle.onmousedown = function(e) {
        e.preventDefault();
        startX = e.clientX;
        startY = e.clientY;
        startWidth = div.offsetWidth;
        startHeight = div.offsetHeight;

        document.onmousemove = function(e) {
            const newWidth = Math.max(startWidth + (e.clientX - startX), parseInt(div.style.minWidth));
            const newHeight = Math.max(startHeight + (e.clientY - startY), parseInt(div.style.minHeight));
            div.style.width = `${newWidth}px`;
            div.style.height = `${newHeight}px`;
        };

        document.onmouseup = function() {
            document.onmousemove = document.onmouseup = null;
        }
    }
}

async function initializeWebSocket() {
    try {
        if (!wsManager) {
            wsManager = new WebSocketClient('ws://10.10.0.221:8080');

            // Setup message handling after successfully connecting
            wsManager.onMessage = async (message) => {
                try {
                    const parsedMessage = JSON.parse(message);
                    if (parsedMessage.type === 'MQTT_GET_CONFIG_RESPONSE') {
                        logger.info("MQTT configuration received successfully.");
                        const config = parsedMessage.payload;
                        console.log("Received MQTT config:", config);
                        // Update UI or internal state with the received configuration
                    } else {
                        appendMessage(`Received: ${message}`);
                    }
                } catch (error) {
                    logger.error(`Failed to parse message: ${error.message}`);
                    appendMessage(`Received malformed message: ${message}`);
                }
            };
        }

        await wsManager.connect();
        logger.info("WebSocket connection established.");
    } catch (error) {
        logger.error(`Failed to connect to WebSocket server: ${error.message}`);
    }
}

// Common function for sending messages and handling UI updates
async function handleSendMessage(type, data, buttonId) {
    const button = document.getElementById(buttonId);
    try {
        button.disabled = true;
        logger.info(`Attempting to send message: ${JSON.stringify(data)}`);
        await wsManager.sendMessage(type, data); // Only pass data
        appendMessage(`Sent message: ${JSON.stringify(data)}`, 'info');
    } catch (error) {
        logger.error(`Failed to send message: ${error.message}`);
        appendMessage(`Failed to send message: ${error.message}`, 'error');
    } finally {
        button.disabled = false;
    }
}

// Function to send SIMPLE_TEXT message as a JSON object
async function sendTextMessage() {
    const textMessage = document.getElementById('textMessageInput').value.trim();

    if (textMessage) {
        const data = { message: textMessage };
        try {
            await handleSendMessage("SIMPLE_TEXT", data, 'sendTextButton');
            document.getElementById('textMessageInput').value = '';
            document.getElementById('textMessageInput').focus();
        } catch (error) {
            logger.error(`Failed to send SIMPLE_TEXT message: ${error.message}`);
            appendMessage(`Failed to send SIMPLE_TEXT: ${error.message}`, 'error');
        }
    } else {
        logger.warn("Please enter a message to send.");
        appendMessage("Please enter a message to send.", 'warn');
    }
}

// Function to publish MQTT event
async function publishMqttEvent() {
    const topic = document.getElementById('topicSelect').value;
    const index = parseInt(document.getElementById('indexSelect').value, 10);
    const state = document.getElementById('stateSelect').value;

    if (!topic || isNaN(index) || !state) {
        logger.warn("Please select a topic, an index, and enter a state.");
        appendMessage("Please select a topic, an index, and enter a state.", 'warn');
        return;
    }

    const data = { topic, index, state };
    await handleSendMessage('MQTT_PUBLISH_EVENT', data, 'publishButton');
}

async function requestMqttConfig() {
    try {
        logger.info("Requesting MQTT Configuration...");
        const data = {};
        await wsManager.sendMessage("GET_MQTT_CONFIG", data, 'requestMqttConfigButton');
    } catch (error) {
        logger.error(`Failed to send GET_MQTT_CONFIG message: ${error.message}`);
        appendMessage(`Failed to send GET_MQTT_CONFIG message: ${error.message}`, 'error');
    }
}

// Append a message to the messages output pane with log level
function appendMessage(text, level = 'info') {
    const messageElement = document.createElement('div');
    messageElement.className = `message log-level-${level}`;
    messageElement.textContent = text;
    messagesDiv.appendChild(messageElement);
    messagesDiv.scrollTop = messagesDiv.scrollHeight;
}

// Clear the content of an output pane
function clearOutputPane(div) {
    while (div.firstChild) {
        div.removeChild(div.firstChild);
    }
}

window.addEventListener('load', async () => {
    await initializeWebSocket();

    makeResizable(messagesDiv);
    makeResizable(logOutputDiv);

    // Add event listeners for click events
    document.getElementById('clearMessagesButton').addEventListener('click', () => clearOutputPane(messagesDiv));
    document.getElementById('sendTextButton').addEventListener('click', sendTextMessage);
    document.getElementById('requestMqttConfigButton').addEventListener('click', requestMqttConfig);
    document.getElementById('publishButton').addEventListener('click', publishMqttEvent);
    document.getElementById('clearLogButton').addEventListener('click', () => clearOutputPane(logOutputDiv));
});

window.addEventListener('beforeunload', () => {
    if (wsManager) {
        wsManager.disconnect();
    }
});
