<?php
    function send_post($address, $body) {
        $ch = curl_init();

        curl_setopt($ch, CURLOPT_URL, $address."/leds");
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $body);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

        $result = curl_exec($ch);
        if (curl_errno($ch)) {
            echo "[curl] ".curl_error($ch)."<br>";
        }
        curl_close($ch);
    }

    function update_module($address, $color, $value) {
        switch ($color) {
            case "red": $body = "r=$value"; break;
            case "grn": $body = "g=$value"; break;
            case "blu": $body = "b=$value"; break;
            default: die("unknown color");
        }

        send_post($address, $body);
    }

    function update_module_rgb($address, $red, $grn, $blu) {
        $body  = "r=$red&";
        $body .= "g=$grn&";
        $body .= "b=$blu";

        send_post($address, $body);
    }
?>