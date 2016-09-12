<?php
/**
 * Modules manager cache enabler/disabler
 * 
 * @package    HNG2
 * @subpackage modules_manager
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *             
 * @param "new_status" enabled|disabled
 */

use hng2_base\config;

include "../config.php";
include "../includes/bootstrap.inc";
header("Content-Type: text/plain; charset=utf-8");

if( $account->level < config::ADMIN_USER_LEVEL ) throw_fake_401();

if( ! in_array($_GET["new_status"], array("enabled", "disabled")) )
    die( $current_module->language->task_messages->invalid_mode_specified );

if( $_GET["new_status"] == "enabled" )
{
    $settings->set("modules:modules_manager.disable_cache", "");
}
else
{
    $settings->set("modules:modules_manager.disable_cache", "true");
    @unlink( "{$config->datafiles_location}/cache/modules.dat" );
}

echo "OK";
