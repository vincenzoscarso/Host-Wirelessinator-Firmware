import * as Log from './log'
import * as Ino from './ArduinoUi'

class ArduinoServerClass {
    private __socket: WebSocket | null
    private __keep_connection: boolean
    public readonly SERVER_ADDRESS = "192.168.1.250:55555"

    constructor() {
        this.__socket = null
        this.__keep_connection = false
    }

    getConnectionMode() {
        return this.__keep_connection
    }

    setConnectionMode(value: boolean) {
        this.__keep_connection = value
    }

    connect(): Promise<void> {
        if (this.isOpen()) this.__socket!.close()

        this.__socket = new WebSocket(`ws://${this.SERVER_ADDRESS}`)

        this.__socket.onclose = () => {
            Ino.arduino_ui_node.status.innerText = "Disconnected"
            Log.logInfo('Disconnected from the server')
        }

        Log.logInfo("Waiting for server to come online...")

        return new Promise<void>((resolve, reject) => {
            if (!this.__socket) {
                return reject()
            }

            this.__socket.onopen = () => {
                Ino.arduino_ui_node.status.innerText = "Connected"
                Log.logInfo('Connected to the server')
                return resolve()
            }

            this.__socket.onerror = (event) => {
                Log.logError(`WebSocket error: ${event}`)
                return reject()
            }
        })

    }

    sendCommandAndGetOutput(command: string): Promise<any> {
        return new Promise((resolve, reject) => {
            if (!this.isOpen() || !this.__socket) {
                Log.logError("Couldn't send request: socket not initialized")
                return reject()
            }
            
            let request: string
            let header = `Connection: ${this.__keep_connection ? "keep_connection" : "close_connection"}`
            request = header + "\n" + "-- HEADER END --" + "\n" + command

            this.__socket!.send(request)
            Log.logInfo(`Sent request:\n${request}`)

            this.__socket.onmessage = (event) => {
                return resolve(event.data)
            }

            this.__socket.onerror = () => {
                Log.logError(`WebSocket error`)
                return reject()
            }
        })
    }

    async getHostsJson(): Promise<any> {
        try {
            await this.connect();

            const output = await this.sendCommandAndGetOutput("GetHostsJson");

            return output;
        } catch (error) {
            console.error("Error obtaining hosts JSON: ", error);
            throw error;
        }
    }

    isOpen(): boolean {
        return this.__socket?.readyState === WebSocket.OPEN
    }
}

export const ArduinoServer: ArduinoServerClass = new ArduinoServerClass
