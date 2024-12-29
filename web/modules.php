<?php
    include "libs/db.php";
    include 'libs/hlp.php';

    $room_id = $_GET["room_id"];
    $room_name = get_room_name($db, $room_id);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED4.0 modules</title>
    <link rel="stylesheet" href="common.css">
    <link rel="stylesheet" href="mgmt.css">
    <script src="verify.js"></script>
</head>

<body>
    <?php
        $room_id = $_GET["room_id"];
        if (!isset($room_id)) {
            die("Please select room");
        }
    ?>

    <h1>LED4.0 modules</h1>
    <div class="links indent">
        <a href="room.php?room_id=<?= $room_id ?>">Back to room</a>
    </div>

    <?php        
        if ($_SERVER["REQUEST_METHOD"] == "POST") {
            $act = $_POST["action"];

            if ($act == "add") {
                $name = $_POST["modname"];
                $addr = $_POST["modaddr"];

                try {
                    $stmt = $db->prepare("INSERT INTO `modules` (`room_id`, `name`, `address`) VALUES (?, ?, ?)");
                    $stmt->execute([$room_id, $name, $addr]);
                    
                    // echo "Module $name($addr) was added successfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] insert failed: ".$db->error);
                }
            }

            else if ($act == "del") {
                $module_id = $_POST["module_id"];

                try {
                    $stmt = $db->prepare("DELETE FROM `modules` WHERE `module_id` = ?");
                    $stmt->execute([$module_id]);
                    
                    // echo "Module id $module_id deleted succesfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] delete failed: ".$db->error);
                }
            }
        }
    ?>

    <h2><?= $room_name ?></h2>
    <h3>Add module</h3>
    <form class="indent" method="POST">
        <input type="hidden" name="action" value="add"/>
        <table>
            <tr>
                <td class="fieldname">Name</td>
                <td><input type="text" name="modname"/></td>
            </tr>
            <tr>
                <td class="fieldname">Address</td>
                <td><input type="text" name="modaddr" value=".leds.k21a.mrwski.eu"/></td>
            </tr>
            <tr>
                <td class="fieldname">&nbsp;</td>
                <td><input type="submit" value="Add" class="btnadd"/></td>
            </tr>
        </table>
    </form>

    <h3>Existing modules</h3>
    <table class="indent">
        <tr>
            <th id="th-name">Name</th>
            <th id="th-addr">Address</th>
            <th id="th-del">Delete</th>
        </tr>

        <?php
            try {
                $stmt = $db->prepare("SELECT module_id, name, address FROM `modules` WHERE `room_id` = ?");
                $stmt->execute([$room_id]);
                $stmt->bind_result($module_id, $name, $addr);

                while ($stmt->fetch()) { ?>
                
                    <tr class="row">
                        <td><?= $name ?></td>
                        <td><?= $addr ?></td>
                        <td><form method="POST" onsubmit="delete_prompt(event, 'module <?= $name ?>')">
                            <input type="hidden" name="action" value="del"/>
                            <input type="hidden" name="module_id" value="<?= $module_id ?>"/>
                            <input type="submit" value="X" class="btndel">
                        </form></td>
                    </tr>

                <?php }

            }
            catch (mysqli_sql_exception $e) {
                echo("[DB] select failed: ".$db->error);
            }
        ?>
    </table>
</body>
</html>