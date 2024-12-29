<?php
    include $_SERVER["DOCUMENT_ROOT"] . "/libs/db.php";
    include $_SERVER["DOCUMENT_ROOT"] . "/libs/leds.php";

    if ($_SERVER["REQUEST_METHOD"] == "GET") {
        die("only POST here");
    }

    $list_id = $_POST["list_id"];
    $color = $_POST["color"];
    $value = $_POST["value"];
    $update_db = $_POST["db"] == "true";
    // echo "list$list_id: $color = $value (db $update_db)";

    // send update to modules
    try {
        $stmt = $db->prepare(
            "SELECT address FROM `modules`
             INNER JOIN `listmod`
             ON `modules`.`module_id` = `listmod`.`module_id`
             WHERE `list_id` = ?;");
        $stmt->bind_param("i", $list_id);
        $stmt->execute();
        $stmt->bind_result($address);
        while ($stmt->fetch())
            update_module($address, $color, $value);
    }
    catch (mysqli_sql_exception $e) {
        echo("[DB] select failed: ".$db->error);
    }

    // send update to db
    if ($update_db) {
        try {
            switch ($color) {
                case "red": $stmt = $db->prepare("UPDATE `lists` SET `red` = ? WHERE `lists`.`list_id` = ?"); break;
                case "grn": $stmt = $db->prepare("UPDATE `lists` SET `grn` = ? WHERE `lists`.`list_id` = ?"); break;
                case "blu": $stmt = $db->prepare("UPDATE `lists` SET `blu` = ? WHERE `lists`.`list_id` = ?"); break;
                default: die("unknown color");
            }
            
            $stmt->bind_param("ii", $value, $list_id);
            $stmt->execute();

            // echo "dbu";
        }
        catch (mysqli_sql_exception $e) {
            echo("[DB] update failed: ".$db->error);
        }
    }
?>