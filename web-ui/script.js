const NODE_ARDUINO_STATUS = document.getElementById("arduino-status")
const NODE_ARDUINO_KEEP_CONNECTION_CHECKBOX = document.getElementById("arduino-keep-connection-checkbox")

const NODE_CONSOLE_SHOW_BUTTON = document.getElementById("console-show-button")
const NODE_CONSOLE_WINDOW = document.getElementById("console-window")
const NODE_CONSOLE_CONTENT = document.getElementById("console-content")
const NODE_CONSOLE_COMMAND_PATH = document.getElementById("console-command-path")
const NODE_CONSOLE_INPUT_COMMAND = document.getElementById("console-command")

const SERVER_ADDRESS = "192.168.1.10:55555"
/** @type {WebSocket} */
let socket
let is_keep_connection_enabled

/*========================================================
    Getters and Setters (temporary until class creation)
 =========================================================*/

function getConnectionMode() {
    return is_keep_connection_enabled;
}

function setConnectionMode(value) {
    is_keep_connection_enabled = value
}

/*====================
    Arduino handling
  ====================*/

function connectToServer(callback) {
    if (socket) {
        socket.close()
    }

    socket = new WebSocket(`ws://${SERVER_ADDRESS}`)

    socket.addEventListener('open', function () {
        logInfo('Connected to the server')
        callback()
    })

    socket.addEventListener('message', function (event) {
        logArduino(event.data)
    })

    socket.addEventListener('error', function (event) {
        logError('WebSocket error:', event.type)
    })

    socket.addEventListener('close', function () {
        logInfo('Disconnected from the server')
    })

    logInfo("Waiting for server to come online...")
}

function sendCommand(command, keep_connection) {
    if (!socket || socket.readyState !== WebSocket.OPEN) {
        logError("Socket is not open. Command not sent.")
    }

    let request

    if (keep_connection) {
        request = "Connection: keep_connection" + "\n"
    } else {
        request = "Connection: close_connection" + "\n"
    }
    request += "-- HEADER END --" + "\n"
    request += command

    socket.send(request)

    logInfo(`Sent request:\n${request}`)
}

/*====================
    Console handling
  ====================*/

NODE_CONSOLE_SHOW_BUTTON.addEventListener("click", () => {
    NODE_CONSOLE_INPUT_COMMAND.focus()
})

NODE_CONSOLE_WINDOW.addEventListener("click", () => {
    NODE_CONSOLE_INPUT_COMMAND.focus()
})

NODE_CONSOLE_COMMAND_PATH.innerText = `Arduino@${SERVER_ADDRESS}>`

NODE_CONSOLE_INPUT_COMMAND.addEventListener('keydown', (event) => {
    if (event.key === 'Enter') {
        setConnectionMode(NODE_ARDUINO_KEEP_CONNECTION_CHECKBOX.checked)
        const command = NODE_CONSOLE_INPUT_COMMAND.value.trim();

        if (!command) {
            logError("No command provided, skipping")
            return
        }

        if (socket && socket.readyState === WebSocket.OPEN) {
            sendCommand(command, is_keep_connection_enabled);
        } else {
            connectToServer(() => {
                sendCommand(command, is_keep_connection_enabled);
            });
        }

        NODE_CONSOLE_INPUT_COMMAND.value = "";
    }
})

function clearConsole() {
    NODE_CONSOLE_CONTENT.innerText = ""
}

function showConsole() {
    NODE_CONSOLE_WINDOW.style.display = "block"
    NODE_CONSOLE_SHOW_BUTTON.style.display = "none"
}

function hideConsole() {
    NODE_CONSOLE_WINDOW.style.display = "none"
    NODE_CONSOLE_SHOW_BUTTON.style.display = "block"
}

/*===========
    Logging
  ===========*/

function logInfo(message) {
    header = "[SYSTEM][I]: "
    log(header, message)
}

function logWarning(message) {
    header = "[SYSTEM][W]: "
    log(header, message)
}

function logError(message) {
    header = "[SYSTEM][E]: "
    log(header, message)
}

function logArduino(message) {
    header = "[ARDUINO]: "
    log(header, message)
}

function log(header, message) {
    NODE_CONSOLE_CONTENT.innerText += header + message + '\n'
    console.log(header + message)
}
