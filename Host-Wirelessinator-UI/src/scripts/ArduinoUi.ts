import { get } from './utils'

export const arduino_ui_node = {
	status: get<HTMLSpanElement>("arduino-status"),
	keep_connection_checkbox: get<HTMLInputElement>("arduino-keep-connection-checkbox")
}
