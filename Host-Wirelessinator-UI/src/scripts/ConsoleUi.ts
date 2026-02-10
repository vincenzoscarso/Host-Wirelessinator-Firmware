import { get } from './utils'

class ConsoleUIClass {
    private window = get<HTMLDivElement>("console-window")
    private content = get<HTMLDivElement>("console-content")
    private command_path = get<HTMLSpanElement>("console-command-path")
    private input_command = get<HTMLInputElement>("console-command")
    private clear_button = get<HTMLButtonElement>("console-clear-button")
    private hide_button = get<HTMLButtonElement>("console-hide-button")
    private show_button = get<HTMLButtonElement>("console-show-button")

    constructor() {
        this.window.addEventListener("click", () => { this.__focusCommandInput() })
        this.clear_button.addEventListener("click", () => { this.__clearConsole() })
        this.show_button.addEventListener("click", () => { this.__showConsole(); this.__focusCommandInput() })
        this.hide_button.addEventListener("click", () => { this.__hideConsole() })
    }

    public appendToConsole(str: string) {
        this.content.innerText += str
    }

    public setCommandPathValue(string: string) {
        this.command_path.innerText = string
    }

    public getInputCommandValue(): string {
        return this.input_command.value
    }

    public clearInputCommandValue() {
        this.input_command.value = ""
    }

    public setInputKeyDownEventListener(listener: (event: any) => any) {
        this.input_command.addEventListener("keydown", listener)
    }

    private __clearConsole() {
        this.content.innerText = ""
    }

    private __showConsole() {
        this.window.style.display = "block"
        this.show_button.style.display = "none"
    }

    private __hideConsole() {
        this.window.style.display = "none"
        this.show_button.style.display = "block"
    }

    private __focusCommandInput() {
        this.input_command.focus()
    }
}

export const ConsoleUI: ConsoleUIClass = new ConsoleUIClass