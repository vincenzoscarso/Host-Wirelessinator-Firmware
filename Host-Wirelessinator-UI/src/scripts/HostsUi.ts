import { get } from './utils'
import * as Log from './log'

class HostsUIClass {
    private hosts_container = get<HTMLFieldSetElement>("hosts-container")

    public renderHosts(hosts_json_string: any, commandHandler: (command: string) => void) {
        try {
            const hosts_json = JSON.parse(hosts_json_string)
            const number_of_hosts = hosts_json["number_of_hosts"]

            for (let i = 0; i < number_of_hosts; i++) {
                const host_html = this.__getHostHtml(hosts_json[`${i}`])
                this.__appendHost(host_html)
            }

            this.hosts_container.addEventListener('click', (event) => {
                const target = event.target as HTMLElement;

                if (target.classList.contains('boot-shutdown-button')) {
                    const hostName = target.getAttribute('data-hostname');
                    commandHandler(`Boot ${hostName}`);
                }
            });
        } catch (error) {
            const error_str = error instanceof Error ? error.message : String(error)
            Log.logError(error_str)
        }
    }

    public removeAllHosts() {
        this.hosts_container.innerHTML = ''
    }

    private __getHostHtml(host_json: any) {
        let host_name: string = host_json.name
        let host_type: string = host_json.type

        const host_html = `
        <div class="host">
            <img src="/${host_type}.png" alt="${host_type}">

            <div class="details-container">
                <div id="host-name">${host_name}</div>
                <div id="host-type">${host_type}</div>
            </div>

            <div class="button-container">
                <button class="boot-shutdown-button" data-hostname="${host_name}">Boot/Shutdown</button>
            </div>
        </div>
        `

        return host_html
    }

    private __appendHost(host_html: string) {
        this.hosts_container.innerHTML += host_html
    }
}

export const HostsUI: HostsUIClass = new HostsUIClass