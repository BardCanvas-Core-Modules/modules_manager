<?php
/**
 * Modules manager admin actions
 *
 * @package    HNG2
 * @subpackage modules_manager
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_base\config;
use hng2_base\module;
use hng2_cache\disk_cache;

include "../config.php";
include "../includes/bootstrap.inc";

if( $account->level < config::ADMIN_USER_LEVEL ) throw_fake_401();

if( empty($_REQUEST["do_module_name"]) ) die($current_module->language->task_messages->module_not_provided);

if( ! in_array($_REQUEST["install_action"], array("install", "uninstall", "enable", "disable")) )
    die($current_module->language->task_messages->invalid_action);

$do_module_name        = trim(stripslashes($_REQUEST["do_module_name"]));
$module_install_action = trim($_REQUEST["install_action"]);

if( ! is_file(ROOTPATH . "/$do_module_name/module_info.xml"))
    die($current_module->language->task_messages->module_not_found);

$errors       = array();
$messages     = array();
$update_cache = false;

include __DIR__ . "/actions/$module_install_action.inc";

if( $update_cache )
{
    $modules_cache = new disk_cache("{$config->datafiles_location}/cache/modules.dat");
    $module = new module(ROOTPATH . "/{$do_module_name}/module_info.xml");
    $modules_cache->set($do_module_name, $module->serialize());
}

if( count($messages) > 0 && count($errors) == 0 )
{
    send_notification($account->id_account, "success", implode("\n", $messages));
    
    die("OK");
}

if( count($messages) > 0 && count($errors) > 0 )
{
    $output = array();
    $output[] = $current_module->language->messages->issues_detected . "\n";
    $output[] = $current_module->language->messages->successfull_actions;
    $output   = array_merge($output, $messages);
    $output[] = "\n" . $current_module->language->messages->failed_actions;
    $output   = array_merge($output, $errors);
    $output[] = "\n" . $current_module->language->messages->please_review;
    
    die( implode("\n", $output) );
}

die( implode("\n", $errors) );
