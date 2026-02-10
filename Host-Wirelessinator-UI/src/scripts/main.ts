import { ArduinoServer } from './ArduinoServer'
import { ConsoleUI } from "./ConsoleUi"
import { HostsUI } from './HostsUi'
import * as InoUi from './ArduinoUi'
import * as Log from './log'

ConsoleUI.setCommandPathValue(`Arduino@${ArduinoServer.SERVER_ADDRESS}>`)
ConsoleUI.setInputKeyDownEventListener((event) => {
	if (event.key === 'Enter') {
		const command = ConsoleUI.getInputCommandValue().trim()
		handleAndSendCommand(command)
		ConsoleUI.clearInputCommandValue()
	}
})

async function handleAndSendCommand(command: string): Promise<void> {
	ArduinoServer.setConnectionMode(InoUi.arduino_ui_node.keep_connection_checkbox.checked)
	
	if (!command) {
		Log.logError("No command provided, skipping")
		return
	}
	
	let output: any
	if (ArduinoServer.isOpen()) {
		output = await sendCommand(command)
	}
	else {
		output = await connectAndSendCommand(command)
	}
	
	Log.logArduino(output)
}

async function connectAndSendCommand(command: string): Promise<any> {
	try {
		await ArduinoServer.connect()
		return await ArduinoServer.sendCommandAndGetOutput(command)
	} catch (error) {
		Log.logError(`Error during command execution: ${error}`)
	}
}

async function sendCommand(command: string): Promise<any> {
	try {
		return await ArduinoServer.sendCommandAndGetOutput(command)
	} catch (error) {
		Log.logError(`Error during command execution: ${error}`)
	}
}

HostsUI.renderHosts(await ArduinoServer.getHostsJson(), handleAndSendCommand)
