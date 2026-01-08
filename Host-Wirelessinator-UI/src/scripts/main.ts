import { ArduinoServer } from './ArduinoServer'
import * as InoUi from './ArduinoUi'
import * as ConUi from './ConsoleUi'
import * as Log from './log'

ConUi.console_ui_node.command_path.innerText = `Arduino@${ArduinoServer.SERVER_ADDRESS}>`

ConUi.console_ui_node.input_command.addEventListener("keydown", (event) => {
	if (event.key === 'Enter') {
		const command = ConUi.console_ui_node.input_command.value.trim()
		connectAndOrSendCommand(command)
		ConUi.console_ui_node.input_command.value = ""
	}
})

function connectAndOrSendCommand(command: string): void {
	ArduinoServer.setConnectionMode(InoUi.arduino_ui_node.keep_connection_checkbox.checked)

	if (!command) {
		Log.logError("No command provided, skipping")
		return
	}

	if (ArduinoServer.isOpen())
		ArduinoServer.sendCommand(command)
	else
		ArduinoServer.connect(() => { ArduinoServer.sendCommand(command) })
}
