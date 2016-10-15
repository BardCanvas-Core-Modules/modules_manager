<?php
/**
* Modules manager cache purge
* 
* @package    HNG2
* @subpackage modules_manager
* @author     Alejandro Caballero - lava.caballero@gmail.com
*/

use hng2_base\config;

include "../config.php";
include "../includes/bootstrap.inc";
header("Content-Type: text/plain; charset=utf-8");

if( $account->level < config::ADMIN_USER_LEVEL ) throw_fake_401();

$force_regeneration = true;
$avoid_postinits    = true;
include ROOTPATH . "/includes/modules_autoloader.inc";

echo "OK";
