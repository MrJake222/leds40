function set_up_status(loading, radioOn, radioOff, isOn) {
    loading.classList.remove("loading")
    loading.style.color = isOn ? "green" : "red"
    loading.innerHTML = isOn ? "ON" : "OFF"
    radioOn.checked = isOn
    radioOff.checked = !isOn
}

function set_up_error(loading, text) {
    loading.style.animation = "none"
    loading.style.color = "red"
    loading.innerHTML = text
}

async function load_status() {
    console.info("loading status")
    
    let loading_ap  = document.getElementById("loading_mode_ap")
    let loading_sta = document.getElementById("loading_mode_sta")

    try {
        let resp = await fetch("/wifi_status")
        console.debug("response status: ", resp)

        if (!resp.ok) {
            throw Error("response not ok, error: " + resp.statusText)
        }

        let json = await resp.json()
        console.debug("Status: ", json)

        let mode = json["mode"]
        if (!["off", "ap", "sta", "ap_sta"].includes(mode)) {
            throw Error("unknown mode, error: " + resp.statusText)
        }

        let ap = (mode == "ap" || mode == "ap_sta")
        let sta = (mode == "sta" || mode == "ap_sta")

        set_up_status(loading_ap,  document.getElementById("radio_ap_on"),  document.getElementById("radio_ap_off"),  ap )
        set_up_status(loading_sta, document.getElementById("radio_sta_on"), document.getElementById("radio_sta_off"), sta)

        if (ap) {
            loading_ap.innerHTML += "<br>Name: " + json["ap_ssid"]
            loading_ap.innerHTML += "<br>IP: " + json["ap_ip"]
        }

        if (sta) {
            loading_sta.innerHTML += "<br>" + json["sta_status_string"]
            if (json["sta_status_string"].includes("Connected")) {
                loading_sta.innerHTML += "<br>IP: " + json["sta_ip"]
            } else {
                loading_sta.style.color = "red"
            }
        }
    }
    catch (e) {
        console.error("failed to load status", e)
        set_up_error(loading_ap, "Failed, see console for details")
        set_up_error(loading_sta, "Failed, see console for details")
    }
}

async function load_name() {
    console.info("loading name")
    
    let loading_name = document.getElementById("loading_name")
    let text_name    = document.getElementById("text_name")

    try {
        let resp = await fetch("/name")
        console.debug("response name: ", resp)

        if (!resp.ok) {
            throw Error("response not ok, error: " + resp.statusText)
        }

        let json = await resp.json()
        console.debug("Name: ", json)

        let name = json["name"]
        loading_name.innerHTML = name
        text_name.value = name

        loading_name.classList.remove("loading")

        if (name == "") {
            loading_name.classList.add("yellow")
            loading_name.innerHTML = "no name set"
        }
    }
    catch (e) {
        console.error("failed to load name", e)
        set_up_error(loading_name, "Failed, see console for details")
    }
}

async function load_mac() {
    console.info("loading mac")
    
    let loading_mac = document.getElementById("loading_sta_mac")

    try {
        let resp = await fetch("/mac")
        console.debug("response mac: ", resp)

        if (!resp.ok) {
            throw Error("response not ok, error: " + resp.statusText)
        }

        let json = await resp.json()
        console.debug("Mac: ", json)

        let mac = json["mac"]
        loading_mac.innerHTML = mac
        loading_mac.classList.remove("loading")
    }
    catch (e) {
        console.error("failed to load mac", e)
        set_up_error(loading_mac, "Failed, see console for details")
    }
}