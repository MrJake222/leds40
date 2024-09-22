var sred = document.getElementById("slider-red")
var sgrn = document.getElementById("slider-green")
var sblu = document.getElementById("slider-blue")

async function led_send(name, val) {
    console.log(name, val)

    await fetch("/leds", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
        body: name + "=" + val
    });
}

async function led_send_all(r, g, b) {
    console.log("rgb", r, g, b)

    await fetch("/leds", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
        body:   "r=" + r + "&" +
                "g=" + g + "&" +
                "b=" + b
    });
}

// sets slider and sends to module
async function led_set(name, val) {
    await led_send(name, val);
    switch (name) {
        case "r": sred.value = val; break;
        case "g": sgrn.value = val; break;
        case "b": sblu.value = val; break;
    }
}

async function led_set_all(r, g, b) {
    await led_send_all(r, g, b);
    sred.value = r;
    sgrn.value = g;
    sblu.value = b;
}

async function led_load() {
    try {
        let resp = await fetch("/leds")
        if (!resp.ok) {
            throw Error("response not ok, error: " + resp.statusText)
        }

        let json = await resp.json()
        console.debug("leds: ", json)

        sred.value = json["r"]
        sgrn.value = json["g"]
        sblu.value = json["b"]
    }
    catch (e) {
        console.warn("Failed to load led:", e)
    }

    sred.oninput = (ev) => { led_send("r", ev.target.value); }
    sgrn.oninput = (ev) => { led_send("g", ev.target.value); }
    sblu.oninput = (ev) => { led_send("b", ev.target.value); }
}