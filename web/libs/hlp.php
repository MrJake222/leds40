<?php
    function mainpage() {
        // strip first part of the url

        $full = $_SERVER['SERVER_NAME'];
        $pos = strpos($full, ".");
        $main = substr($full, $pos+1);

        return "https://".$main;
    }
?>