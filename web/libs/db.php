<?php
    function db_connect() {
        $db_srv = "localhost";
        $sb_usr = "leds";
        $db_pwd = "leDS40EsP82sixsix";
        $db_dbn = "leds";
        return mysqli_connect($db_srv, $sb_usr, $db_pwd, $db_dbn);
    }

    try {
        $db = db_connect();
    }
    catch (mysqli_sql_exception $e) {
        die("[DB] connection failed: ".mysqli_connect_error());
    }

    $auth_user = $_SERVER["AUTHELIA_USER"];
?>