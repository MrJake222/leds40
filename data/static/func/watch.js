var sred = document.getElementById("slider-red")
var sgrn = document.getElementById("slider-green")
var sblu = document.getElementById("slider-blue")

async function load() {
    let resp = await fetch("/leds")
    if (!resp.ok) {
        throw Error("response mode not ok, error: " + resp.statusText)
    }

    let json = await resp.json()
    console.debug("leds: ", json)

    sred.value = json["r"]
    sgrn.value = json["g"]
    sblu.value = json["b"]
}

async function send(name, val) {
    console.log(name, val)

    await fetch("/leds", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
        body: name + "=" + val
    });
}

sred.oninput = (ev) => { send("r", ev.target.value); }
sgrn.oninput = (ev) => { send("g", ev.target.value); }
sblu.oninput = (ev) => { send("b", ev.target.value); }
window.onload = load