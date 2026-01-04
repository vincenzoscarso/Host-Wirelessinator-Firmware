import { get } from './utils'

export const console_ui_node = {
    window: get<HTMLDivElement>("console-window"),
    content: get<HTMLDivElement>("console-content"),
    command_path: get<HTMLSpanElement>("console-command-path"),
    input_command: get<HTMLInputElement>("console-command"),
    clear_button: get<HTMLButtonElement>("console-clear-button"),
    hide_button: get<HTMLButtonElement>("console-hide-button"),
    show_button: get<HTMLButtonElement>("console-show-button")
}

console_ui_node.window.addEventListener("click", () => { __focusCommandInput() })
console_ui_node.clear_button.addEventListener("click", () => { __clearConsole() })
console_ui_node.show_button.addEventListener("click", () => { __showConsole(); __focusCommandInput() })
console_ui_node.hide_button.addEventListener("click", () => { __hideConsole() })

function __clearConsole() {
    console_ui_node.content.innerText = ""
}

function __showConsole() {
    console_ui_node.window.style.display = "block"
    console_ui_node.show_button.style.display = "none"
}

function __hideConsole() {
    console_ui_node.window.style.display = "none"
    console_ui_node.show_button.style.display = "block"
}

function __focusCommandInput() {
    console_ui_node.input_command.focus()
}

export function appendToConsole(str: string) {
    console_ui_node.content.innerText += str
}
