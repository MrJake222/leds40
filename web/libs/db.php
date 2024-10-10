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
    $auth_user = $_SERVER["AUTHELIA_USER"];
?>
