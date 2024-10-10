function count_checked(inputs) {
    return [...inputs]
        .map(c => c.checked ? 1 : 0)
        .reduce((acc, cur) => acc + cur, 0);
}

function setcolor_verify(ev) {
    let radios = document.querySelectorAll("#verify-chk input[type=radio]");
    let checkboxes = document.querySelectorAll("#verify-chk input[type=checkbox]");
    let checked_radios = count_checked(radios);
    let checked_boxes = count_checked(checkboxes);

    if (checked_radios < 1) {
        alert("No presets available");
        ev.preventDefault();
    }

    if (checked_boxes < 1) {
        if (checkboxes.length > 0) {
            checkboxes[0].setCustomValidity("Select at least one");
            checkboxes[0].reportValidity();
            checkboxes[0].setCustomValidity("");
        }
        else {
            alert("No lists available");
        }

        ev.preventDefault();
    }
}

function delete_prompt(ev, name) {
    let yes = confirm(`Do you really want to delete ${name}?`);
    if (!yes) {
        ev.preventDefault();
    }
}