function clamp(val, min, max) { return Math.min(Math.max(val, min), max) }

async function set_leds(list_id, color, value, db) {
    let resp = await fetch("/room/sliders.php", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
        body: `list_id=${list_id}&color=${color}&value=${value}&db=${db}`
    });

    let text = await resp.text()
    if (text != "")
        alert(text)
}

function set_color(name, color, value) {
    let [r,g,b,a] = name.style.background.match(/[0-9\.]+/g)
    
    switch (color) {
        case "red": r = value; break;
        case "grn": g = value; break;
        case "blu": b = value; break;
    }

    name.style.background = `rgba(${r}, ${g}, ${b}, ${a})`
}

document.querySelectorAll(".list").forEach(list => {
    let name = list.querySelector(".name")
    let content = list.querySelector(".content")
    let sliders = content.querySelector(".sliders")
    let savecolor = content.querySelector(".savecolor")

    let list_id = sliders.querySelector("input[name=list_id]").value;

    ["red", "grn", "blu"].forEach(color => {
        let range  = sliders.querySelector(`.${color} input[type=range]`)
        let number = sliders.querySelector(`.${color} input[type=number]`)
        let form   = savecolor.querySelector(`form input[name=${color}]`)
        
        // continuous range changes -> update other fields / set leds
        range.oninput = async () => {
            number.value = range.value;
            form.value = range.value;
            set_color(name, color, range.value);
            await set_leds(list_id, color, range.value, false);
        }

        // button release -> update db
        range.onchange = async () => {
            set_color(name, color, range.value);
            await set_leds(list_id, color, range.value, true);
        }

        // on input focus loss -> update other fields / set leds / save to db
        number.onchange = async () => {
            if (number.value === "") return;
            number.value = clamp(number.value, 0, 255);
            range.value = number.value;
            form.value = number.value;
            set_color(name, color, number.value);
            await set_leds(list_id, color, number.value, true);
        }
    })
})