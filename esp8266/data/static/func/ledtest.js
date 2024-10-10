function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}


async function step(slider, name) {
    for (let v=0; v<=255; v+=63) {
        await led_set(name, v)
        await sleep(200)
    }
}

async function selftest() {
    await led_set_all(0, 0, 0)

    await step(sred, "r")
    await led_set_all(0, 0, 0)
    await sleep(500)
    
    await step(sgrn, "g")
    await led_set_all(0, 0, 0)
    await sleep(500)

    await step(sblu, "b")
    await led_set_all(0, 0, 0)
    await sleep(500)

    await led_set_all(255, 255, 255)
    await sleep(500)
    await led_set_all(0, 0, 0)
    await sleep(500)
}