function delete_prompt(ev, name) {
    let yes = confirm(`Do you really want to delete ${name}?`);
    if (!yes) {
        ev.preventDefault();
    }
}