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
    $force_regeneration = true;
    $avoid_postinits    = true;
    include ROOTPATH . "/includes/modules_autoloader.inc";
}
else
{
    $settings->set("modules:modules_manager.disable_cache", "true");
    $files = glob("{$config->datafiles_location}/cache/always_on*.dat");
        foreach($files as $file) @unlink($file);
    $files = glob("{$config->datafiles_location}/cache/editable_prefs*.dat");
        foreach($files as $file) @unlink($file);
    $files = glob("{$config->datafiles_location}/cache/modules*.dat");
        foreach($files as $file) @unlink($file);
}

echo "OK";
