<?php
    function mainpage() {
        // strip first part of the url

        $full = $_SERVER['SERVER_NAME'];
        $pos = strpos($full, ".");
        $main = substr($full, $pos+1);

        return "https://".$main;
    }

    function get_room_name($db, $room_id) {
        if (isset($room_id)) {
            try {
                $stmt = $db->prepare("SELECT name FROM `rooms` WHERE room_id = ?");
                $stmt->execute([$room_id]);
                $r = $stmt->get_result()->fetch_all(MYSQLI_ASSOC);
                return $r[0]["name"];
            }
            catch (mysqli_sql_exception $e) {
                echo("[DB] select failed: ".$db->error);
            }
        }
        return "";
    }
?>