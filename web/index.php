<?php
    include $_SERVER["DOCUMENT_ROOT"] . "/libs/db.php";
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED4.0</title>
    <link rel="stylesheet" href="/common.css">
    <link rel="stylesheet" href="index.css">
</head>

<body>
    <h1>LED4.0</h1>
    <div class="links indent">

        <?php if ($admin) { ?>
            <a href="/mgmt/rooms.php" class="admin">Manage rooms</a>
        <?php } ?>

    </div>

    <div class="rooms indent">
        <?php
            try {
                $stmt = $db->prepare("SELECT room_id, name FROM `rooms`");
                $stmt->execute();
                $stmt->bind_result($room_id, $name);

                while ($stmt->fetch()) { ?>
                
                        <a href="/room/room.php?room_id=<?= $room_id ?>"><?= $name ?></a>

                <?php }

            }
            catch (mysqli_sql_exception $e) {
                echo("[DB] select failed: ".$db->error);
            }
        ?>
    </div>

</body>
</html>