<?php
    include "libs/db.php";
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED4.0 -- modules</title>
    <link rel="stylesheet" href="common.css">
    <link rel="stylesheet" href="modules.css">
</head>

<body>
    <h1>LEDs 4.0 module managment</h1>
    <div class="links indent">
        <a href="/">Home</a>
    </div>

    <?php        
        if ($_SERVER["REQUEST_METHOD"] == "POST") {
            $act = $_POST["action"];

            if ($act == "add") {
                $name = $_POST["modname"];
                $addr = $_POST["modaddr"];

                try {
                    $stmt = $db->prepare("INSERT INTO `modules` (`auth_user`, `name`, `address`) VALUES (?, ?, ?)");
                    $stmt->bind_param("sss", $auth_user, $name, $addr);
                    $stmt->execute();
                    
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
                    $stmt->bind_param("i", $module_id);
                    $stmt->execute();
                    
                    // echo "Module id $module_id deleted succesfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] delete failed: ".$db->error);
                }
            }
        }
    ?>

    <h2>Add module</h2>
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

    <h2>Existing modules</h2>
    <table class="indent">
        <tr>
            <th id="th-name">Name</th>
            <th id="th-addr">Address</th>
            <th id="th-del">Delete</th>
        </tr>

        <?php
            try {
                $stmt = $db->prepare("SELECT module_id, name, address FROM `modules` WHERE `auth_user` = ?");
                $stmt->bind_param("s", $auth_user);
                $stmt->execute();
                $stmt->bind_result($module_id, $name, $addr);

                while ($stmt->fetch()) { ?>
                
                    <tr class="modrow">
                        <td><?= $name ?></td>
                        <td><?= $addr ?></td>
                        <td><form method="POST">
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