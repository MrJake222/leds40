<?php
    include 'libs/db.php';
    include 'libs/leds.php';
    include 'libs/hlp.php';
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED4.0</title>
    <link rel="stylesheet" href="common.css">
    <link rel="stylesheet" href="index.css">

    <script src="verify.js"></script>
    <script src="sliders.js" defer></script>
</head>

<body>
    <h1>LEDs 4.0</h1>
    <div class="links indent">
        <a href="/modules.php">Manage modules</a>
        <a href="/login/logout?rd=<?= mainpage() ?>" class="logout">Logout</a>
    </div>

    <?php
        if ($_SERVER['REQUEST_METHOD'] == "POST") {
            $act = $_POST["action"];

            if ($act == "addlist") {
                $name = $_POST["name"];

                try {
                    $stmt = $db->prepare("INSERT INTO `lists` (`auth_user`, `name`) VALUES (?, ?)");
                    $stmt->bind_param("ss", $auth_user, $name);
                    $stmt->execute();
                    
                    // echo "List added successfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] insert failed: ".$db->error);
                }
            }

            else if ($act == "dellist") {
                $list_id = $_POST["list_id"];

                try {
                    $stmt = $db->prepare("DELETE FROM `lists` WHERE `list_id` = ?");
                    $stmt->bind_param("i", $list_id);
                    $stmt->execute();
                    
                    // echo "List deleted succesfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] delete failed: ".$db->error);
                }
            }

            else if ($act == "addlistmod") {
                $list_id = $_POST["list_id"];
                $module_id = $_POST["module_id"];

                try {
                    $stmt = $db->prepare("INSERT INTO `listmod` (`list_id`, `module_id`) VALUES (?, ?)");
                    $stmt->bind_param("ii", $list_id, $module_id);
                    $stmt->execute();
                    
                    // echo "Mapping added successfully.";
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] insert failed: ".$db->error);
                }
            }

            else if ($act == "dellistmod") {
                $listmod_id = $_POST["listmod_id"];

                try {
                    $stmt = $db->prepare("DELETE FROM `listmod` WHERE `listmod_id` = ?");
                    $stmt->bind_param("i", $listmod_id);
                    $stmt->execute();
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] delete failed: ".$db->error);
                }
            }

            else if ($act == "savecolor") {
                $name = $_POST["name"];
                $red = $_POST["red"];
                $grn = $_POST["grn"];
                $blu = $_POST["blu"];

                try {
                    $stmt = $db->prepare("INSERT INTO `colors` (`auth_user`, `name`, `red`, `grn`, `blu`) VALUES (?, ?, ?, ?, ?)");
                    $stmt->bind_param("ssiii", $auth_user, $name, $red, $grn, $blu);
                    $stmt->execute();
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] insert failed: ".$db->error);
                }
            }

            else if ($act == "setcolor") {
                $color_id = $_POST["color_id"];
                $list_ids = $_POST["list_ids"];

                try {
                    // grab RGB from preset
                    $stmt = $db->prepare("SELECT `red`, `grn`, `blu` FROM `colors` WHERE `color_id` = ?");
                    $stmt->bind_result($red, $grn, $blu);
                    $stmt->execute([$color_id]);
                    $stmt->fetch();
                    $stmt->close();

                    // send update to all modules on selected lists
                    $placeholders = str_repeat('?,', count($list_ids) - 1) . '?';
                    $stmt = $db->prepare(
                        "SELECT `address` FROM `modules`
                         INNER JOIN `listmod`
                         ON `modules`.`module_id` = `listmod`.`module_id`
                         WHERE `list_id` IN ($placeholders);");
                    $stmt->bind_result($address);
                    $stmt->execute($list_ids);
                    while ($stmt->fetch())
                        update_module_rgb($address, $red, $grn, $blu);

                    // update color values in db
                    $stmt = $db->prepare(
                        "UPDATE `lists`
                        SET `red` = ?, `grn` = ?, `blu` = ?
                        WHERE `list_id` IN ($placeholders);");
                    $stmt->execute(array_merge([$red, $grn, $blu], $list_ids));

                    // delete old user selections
                    $stmt = $db->prepare("DELETE FROM `listsel` WHERE `auth_user` = ?;");
                    $stmt->execute([$auth_user]);
                    foreach($list_ids as $list_id) {
                        // insert new selections
                        $stmt = $db->prepare("INSERT INTO `listsel` (`auth_user`, `list_id`) VALUES (?, ?);");
                        $stmt->execute([$auth_user, $list_id]);
                    }
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] select/update failed: ".$db->error);
                }
            }

            else if ($act == "delcolor") {
                $color_id = $_POST["color_id"];

                try {
                    $stmt = $db->prepare("DELETE FROM `colors` WHERE `color_id` = ?");
                    $stmt->execute([$color_id]);
                }
                catch (mysqli_sql_exception $e) {
                    echo("[DB] delete failed: ".$db->error);
                }
            }
        }
    ?>

    <h2>Presets</h2>
    <?php
        function get_ids($list) { return $list["list_id"]; }
        function get_all_selected_list_ids($auth_user) {
            global $db;
            $stmt = $db->prepare("SELECT list_id FROM `listsel` WHERE `auth_user` = ?");
            $stmt->execute([$auth_user]);
            $result = $stmt->get_result()->fetch_all(MYSQLI_ASSOC);
            $list_ids = array_map("get_ids", $result);
            return $list_ids;
        }
    ?>
    <!-- used to avoid nesting color delete form -->
    <form id="setcolor" method='POST' onsubmit="setcolor_verify(event)">
        <input type="hidden" name="action" value="setcolor">
    </form>
    <div class="indent set fg">
        <div class="set-tables" id="verify-chk">
            <table>
                <tr>
                    <th colspan="3">Presets</th>
                </tr>
                
                <?php
                    try {
                        // iterate over colors
                        $stmt = $db->prepare("SELECT color_id, name, red, grn, blu FROM `colors` WHERE `auth_user` = ?");
                        $stmt->bind_param("s", $auth_user);
                        $stmt->execute();
                        $stmt->bind_result($color_id, $name, $red, $grn, $blu);

                        while ($stmt->fetch()) { ?>

                            <tr>
                                <td>
                                    <form method='POST' onsubmit="delete_prompt(event, 'preset <?= $name ?>')">
                                        <input type="hidden" name="action" value="delcolor">
                                        <input type="hidden" name="color_id" value="<?= $color_id ?>">
                                        <input type="submit" value="X">
                                    </form>
                                </td>

                                <td class="set-divider-color">
                                    <input id="radio<?= $color_id ?>" type="radio" name="color_id" value="<?= $color_id ?>"form="setcolor" required>
                                </td>

                                <td>
                                    <label for="radio<?= $color_id ?>" style="color: rgb(<?= $red ?>, <?= $grn ?>, <?= $blu ?>)"> <?= $name ?> </label>
                                </td>
                            </tr>

                        <?php }
                ?>
            </table>

            <table>
                <tr>
                    <th colspan="3">Lists</th>
                </tr>

                <?php
                        // get selected lists
                        $selected_list_ids = get_all_selected_list_ids($auth_user);

                        // iterate over lists
                        $stmt = $db->prepare("SELECT list_id, name FROM `lists` WHERE `auth_user` = ?");
                        $stmt->bind_param("s", $auth_user);
                        $stmt->execute();
                        $stmt->bind_result($list_id, $name);

                        while ($stmt->fetch()) { ?>

                            <tr>
                                <td class="set-divider-list">
                                    <input id="chk<?= $list_id ?>" type="checkbox" name="list_ids[]" value="<?= $list_id ?>" form="setcolor"
                                        <?php if (in_array($list_id, $selected_list_ids)) echo "checked"; ?> >
                                </td>

                                <td>
                                    <label for="chk<?= $list_id ?>" class="set-name"> <?= $name ?> </label>
                                </td>
                            </tr>

                        <?php }
                    }
                    catch (mysqli_sql_exception $e) {
                        echo("[DB] select failed: ".$db->error);
                    }
                ?>
            </table>
        </div>
        <input type="submit" value="Apply" form="setcolor" class="applyall">
    </div>

    <h2>Lists</h2>
    <h3>New List</h3>
    <form method="POST" class="indent">
        <input type="hidden" name="action" value="addlist">
        <input type="text" name="name" placeholder="New list name" required>
        <input type="submit" value="Add">
    </form>

    <h3>Existing lists</h3>
    <div class="lists indent">
        <?php
            function module_table_on_list($list_id) { ?>
                <?php
                    // second connection to fetch nested results
                    $db2 = db_connect();
                    $stmt = $db2->prepare("
                        SELECT listmod_id, modules.module_id, name FROM `listmod`
                        INNER JOIN `modules`
                        ON `modules`.`module_id` = `listmod`.`module_id`
                        WHERE `list_id` = ?;");

                    $stmt->bind_param("i", $list_id);
                    $stmt->execute();
                    $stmt->bind_result($listmod_id, $module_id, $modname);
                    $module_ids_on_list = array(); ?>

                    <?php while ($stmt->fetch()) {
                        $module_ids_on_list[] = $module_id; ?>

                        <tr>
                            <td><?= $modname ?></td>
                            <td class="right"><form method="POST">
                                <input type="hidden" name="action" value="dellistmod">
                                <input type="hidden" name="listmod_id" value="<?= $listmod_id ?>">
                                <input type="submit" value="X">
                            </form></td>
                        </tr>

                    <?php }

                    $db2->close();
                    return $module_ids_on_list;
                ?>
            <?php }

            function get_all_modules($auth_user) {
                global $db;
                $stmt = $db->prepare("SELECT module_id, name FROM `modules` WHERE `auth_user` = ?");
                $stmt->bind_param("s", $auth_user);
                $stmt->execute();
                $modules = $stmt->get_result()->fetch_all(MYSQLI_ASSOC);

                return $modules;
            }

            try {
                // fetch all modules for dropdowns (select tag)
                $modules = get_all_modules($auth_user);

                // iterate over lists
                $stmt = $db->prepare("SELECT list_id, name, red, grn, blu FROM `lists` WHERE `auth_user` = ?");
                $stmt->bind_param("s", $auth_user);
                $stmt->execute();
                $stmt->bind_result($list_id, $name, $red, $grn, $blu);

                while ($stmt->fetch()) { ?>

                    <div class='list fg'>
                        <div class='name' style="background: rgba(<?= $red ?>, <?= $grn ?>, <?= $blu ?>, 0.3)"><?= $name ?></div>

                        <form method="POST" class="dellist" onsubmit="delete_prompt(event, 'list <?= $name ?>')">
                            <input type="hidden" name="action" value="dellist">
                            <input type="hidden" name="list_id" value="<?= $list_id ?>">
                            <input type="submit" value="X">
                        </form>
                        
                        <div class="content">
                            <h3>Modules</h3>
                            <table class="modules section">                      
                                <?php
                                    // list all modules belonging to this list (in table format) and return ids
                                    $module_ids_on_list = module_table_on_list($list_id);

                                    // create filtered list
                                    $module_ids_not_on_list = array_filter($modules, function($module) {
                                        global $module_ids_on_list;
                                        return !in_array($module["module_id"], $module_ids_on_list);
                                    });

                                    // Add module dropdown only if there are unadded modules
                                    if (count($module_ids_not_on_list) > 0) { ?>
                                        <tr>
                                            <form method="POST" class="addlistmod">
                                                <input type="hidden" name="action" value="addlistmod">
                                                <input type="hidden" name="list_id" value="<?= $list_id ?>">
                                                
                                                <td>
                                                    <select name="module_id">
                                                        <?php
                                                            foreach ($module_ids_not_on_list as $mod) {
                                                                $module_id = $mod["module_id"];
                                                                $module_name = $mod["name"];
                                                                echo "<option value=$module_id>$module_name</option>";
                                                            }
                                                        ?>
                                                    </select>
                                                </td>

                                                <td class="right">
                                                    <input type="submit" value="Add">
                                                </td>
                                            </form>
                                        </tr>
                                    <?php }
                                ?>
                            </table>

                            <h3>RGB</h3>
                            <div class="sliders section">
                                <!-- for JS, update leds colors / db entires -->
                                <input type="hidden" name="list_id" value="<?= $list_id ?>">   
                                
                                <div class="slider red">
                                    <div class="name">R</div>
                                    <input type="range"  min="0" max="255" value="<?= $red ?>">
                                    <input type="number" min="0" max="255" value="<?= $red ?>" class="right">
                                </div>
                                <div class="slider grn">
                                    <div class="name">G</div>
                                    <input type="range"  min="0" max="255" value="<?= $grn ?>">
                                    <input type="number" min="0" max="255" value="<?= $grn ?>" class="right">
                                </div>
                                <div class="slider blu">
                                    <div class="name">B</div>
                                    <input type="range"  min="0" max="255" value="<?= $blu ?>">
                                    <input type="number" min="0" max="255" value="<?= $blu ?>" class="right">
                                </div>
                            </div>

                            <form method="POST" class="savecolor section">
                                <!-- Create preset (no list_id, presets are external to lists) -->
                                <input type="hidden" name="action" value="savecolor">
                                <input type="text" name="name" placeholder="Preset name" required>
                                <input type="hidden" name="red" value="<?= $red ?>">
                                <input type="hidden" name="grn" value="<?= $grn ?>">
                                <input type="hidden" name="blu" value="<?= $blu ?>">
                                <input type="submit" value="Create preset">
                            </form>
                        </div>
                    </div>
                     
                <?php }

            }
            catch (mysqli_sql_exception $e) {
                echo("[DB] select failed: ".$db->error);
            }
        ?>
    </div>
</body>
</html>