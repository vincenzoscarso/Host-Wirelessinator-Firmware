import { ConsoleUI } from "./ConsoleUi"

export function logInfo(message: string) {
	let header = "[SYSTEM][I]: "
	__log(header, message)
}

export function logWarning(message: string) {
	let header = "[SYSTEM][W]: "
	__log(header, message)
}

export function logError(message: string) {
	let header = "[SYSTEM][E]: "
	__log(header, message)
}

export function logArduino(message: string) {
	let header = "[ARDUINO]: "
	__log(header, message)
}

function __log(header: string, message: string) {
	ConsoleUI.appendToConsole(header + message + '\n')
	console.log(header + message)
}
