import { ArduinoServer } from './ArduinoServer'
import * as InoUi from './ArduinoUi'
import * as ConUi from './ConsoleUi'
import * as Log from './log'

ConUi.console_ui_node.command_path.innerText = `Arduino@${ArduinoServer.SERVER_ADDRESS}>`

ConUi.console_ui_node.input_command.addEventListener("keydown", (event) => {
	if (event.key === 'Enter') {
		const command = ConUi.console_ui_node.input_command.value.trim()
		handleAndSendCommand(command)
		ConUi.console_ui_node.input_command.value = ""
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
