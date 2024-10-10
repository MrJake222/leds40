let FIELDS = ["ssid", "bssid", "enc", "channel", "rssi", "hidden"]

function clear_networks(networks_table) {
    let nodes = networks_table.children
    while (nodes.length > 1) {
        networks_table.removeChild(nodes[1]);
    }
}

async function load_networks() {

    let section = document.getElementById("sta-config")
    let networks_table = document.getElementById("wifi-networks")
    let loading = document.getElementById("loading-sta")

    document.getElementById("radio_sta_on").checked = true

    clear_networks(networks_table)
    console.debug("loading networks")

    loading.style.display = "block"
    section.style.display = "block"

    try {
        let resp = await fetch("/wifi_scan")
        console.debug("response scan: ", resp)

        if (!resp.ok) {
            throw Error("response not ok, error: " + resp.statusText)
        }

        let json = await resp.json()
        console.debug("Networks: ", json)

        json = json.sort((a,b) => b["rssi"] - a["rssi"])

        json.forEach(net => {
            let row = document.createElement("tr")
            networks_table.appendChild(row)

            // radio
            let cell = document.createElement("td")
            row.appendChild(cell)
            cell.className = "radio"

            let radio = document.createElement("input")
            cell.appendChild(radio)
            radio.type = "radio"
            radio.name = "ssid"
            radio.value = net["ssid"]
            radio.checked = net["connected"]
            // end radio

            FIELDS.forEach(f => {
                let cell = document.createElement("td")
                row.appendChild(cell)

                cell.className = f
                
                if (f == "hidden") {
                    cell.innerHTML = net[f] ? "yes" : "-"
                } else {
                    cell.innerHTML = net[f]
                }

                if (f == "rssi") {
                    x = parseInt(net[f])
                    cell.innerHTML += ` (${Math.min(Math.max(2 * (x + 100), 0), 100)}%)`
                }
            })
        })

        loading.style.display = "none"
    }
    catch (e) {
        console.error("failed to load networks")
        console.error("error: " + e)
        loading.innerHTML = "Failed to load networks:<br>" + e
        loading.style.color = "red"
        loading.style.animation = "none"
    }

}