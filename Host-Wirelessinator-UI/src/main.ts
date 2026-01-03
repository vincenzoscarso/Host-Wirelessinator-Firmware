const NODE_ARDUINO_STATUS = get<HTMLSpanElement>("arduino-status")
const NODE_ARDUINO_KEEP_CONNECTION_CHECKBOX = get<HTMLInputElement>("arduino-keep-connection-checkbox")

const NODE_CONSOLE_SHOW_BUTTON = get<HTMLButtonElement>("console-show-button")
const NODE_CONSOLE_WINDOW = get<HTMLDivElement>("console-window")
const NODE_CONSOLE_CONTENT = get<HTMLDivElement>("console-content")
const NODE_CONSOLE_COMMAND_PATH = get<HTMLSpanElement>("console-command-path")
const NODE_CONSOLE_INPUT_COMMAND = get<HTMLInputElement>("console-command")
const NODE_CONSOLE_CLEAR_BUTTON = get<HTMLButtonElement>("console-clear-button")
const NODE_CONSOLE_HIDE_BUTTON = get<HTMLButtonElement>("console-hide-button")

const SERVER_ADDRESS = "192.168.1.10:55555"
let socket: WebSocket
let is_keep_connection_enabled: boolean

/*========================================================
	Getters and Setters (temporary until class creation)
 =========================================================*/

function getConnectionMode() {
	return is_keep_connection_enabled;
}

function setConnectionMode(value: boolean) {
	is_keep_connection_enabled = value
}

/*====================
	Arduino handling
  ====================*/

function connectToServer(callback: () => void) {
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
		logError(`WebSocket error: ${event.target}`)
	})

	socket.addEventListener('close', function () {
		logInfo('Disconnected from the server')
	})

	logInfo("Waiting for server to come online...")
}

function sendCommand(command: string, keep_connection: boolean) {
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
	showConsole()
	NODE_CONSOLE_INPUT_COMMAND.focus()
})

NODE_CONSOLE_WINDOW.addEventListener("click", () => {
	NODE_CONSOLE_INPUT_COMMAND.focus()
})

NODE_CONSOLE_CLEAR_BUTTON.addEventListener("click", () => {
	clearConsole()
})

NODE_CONSOLE_HIDE_BUTTON.addEventListener("click", () => {
	hideConsole()
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

function logInfo(message: string) {
	let header = "[SYSTEM][I]: "
	log(header, message)
}

function logWarning(message: string) {
	let header = "[SYSTEM][W]: "
	log(header, message)
}

function logError(message: string) {
	let header = "[SYSTEM][E]: "
	log(header, message)
}

function logArduino(message: string) {
	let header = "[ARDUINO]: "
	log(header, message)
}

function log(header: string, message: string) {
	NODE_CONSOLE_CONTENT.innerText += header + message + '\n'
	console.log(header + message)
}

/*=========
	Utils
  =========*/

function get<T extends HTMLElement>(id: string): T {
	const element = document.getElementById(id);
	if (!element) throw new Error(`Element '${id}' not found`);
	return element as T;
}
