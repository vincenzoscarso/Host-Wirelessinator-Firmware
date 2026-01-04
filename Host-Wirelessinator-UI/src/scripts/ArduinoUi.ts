import { get } from './utils'

export const arduino_ui_node = {
	arduino_status: get<HTMLSpanElement>("arduino-status"),
	arduino_keep_connection_checkbox: get<HTMLInputElement>("arduino-keep-connection-checkbox")
}