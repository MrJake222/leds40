<?php
    include "libs/db.php";
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED4.0 rooms</title>
    <link rel="stylesheet" href="common.css">
    <link rel="stylesheet" href="mgmt.css">
    <script src="verify.js"></script>
</head>

<body>
    <h1>LED4.0 rooms</h1>
    <div class="links indent">
        <a href="/">Home</a>
    </div>

    <?php        
        if ($_SERVER["REQUEST_METHOD"] == "POST") {
            $act = $_POST["action"];

            if ($act == "add") {
                $name = $_POST["name"];

                try {
                    $stmt = $db->prepare("INSERT INTO `rooms` (`name`) VALUES (?)");
                    $stmt->execute([$name]);
                    
                    // echo "Room $name($addr) was added successfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] insert failed: ".$db->error);
                }
            }

            else if ($act == "del") {
                $room_id = $_POST["room_id"];

                try {
                    $stmt = $db->prepare("DELETE FROM `rooms` WHERE `room_id` = ?");
                    $stmt->execute([$room_id]);
                    
                    // echo "Room id $room_id deleted succesfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] delete failed: ".$db->error);
                }
            }
        }
    ?>

    <h2>Existing rooms</h2>
    <table class="indent">
        <tr>
            <th id="th-name">Name</th>
            <th id="th-del">Delete</th>
        </tr>

        <?php
            try {
                $stmt = $db->prepare("SELECT room_id, name FROM `rooms`");
                $stmt->execute();
                $stmt->bind_result($room_id, $name);

                while ($stmt->fetch()) { ?>
                
                    <tr class="row">
                        <td><?= $name ?></td>
                        <td><form method="POST" onsubmit="delete_prompt(event, 'room <?= $name ?>')">
                            <input type="hidden" name="action" value="del"/>
                            <input type="hidden" name="room_id" value="<?= $room_id ?>"/>
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

    <h2>Add new room</h2>
    <form class="indent" method="POST">
        <input type="hidden" name="action" value="add"/>
        <table>
            <tr>
                <td class="fieldname">Name</td>
                <td><input type="text" name="name"/></td>
            </tr>
            <tr>
                <td class="fieldname">&nbsp;</td>
                <td><input type="submit" value="Add" class="btnadd"/></td>
            </tr>
        </table>
    </form>

    
</body>
</html>