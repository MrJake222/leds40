<?php
    function db_connect() {
        $db_srv = "db";
        $sb_usr = "leds";
        $db_pwd = getenv("DB_PWD");
        $db_dbn = "leds";

        try {
            return mysqli_connect($db_srv, $sb_usr, $db_pwd, $db_dbn);
        }
        catch (mysqli_sql_exception $e) {
            echo "srv: ".$db_srv."<br>";
            echo "usr: ".$sb_usr."<br>";
            //echo "pwd: ".$db_pwd."<br>";
            echo "dbn: ".$db_dbn."<br>";
            die("[DB] connection failed: ".mysqli_connect_error());
        }
    }

    $db = db_connect();
    // $auth_user = $_SERVER["AUTHELIA_USER"];
    $admin = true;

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
