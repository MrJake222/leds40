var dname = document.getElementById("divname")

async function name_load() {
    try {
        let resp = await fetch("/name")

        if (!resp.ok) {
            throw Error("response not ok, error: " + resp.statusText)
        }

        let json = await resp.json()
        console.debug("name: ", json)
        let name = json["name"]

        dname.innerHTML = name

        if (name == "") {
            dname.innerHTML = "&lt;no name set&gt;"
        }
    }
    catch (e) {
        console.warn("Failed to load name:", e)
    }
}