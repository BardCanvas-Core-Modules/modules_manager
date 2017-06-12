<?php
/**
* Modules manager admin index
* 
* @package    HNG2
* @subpackage modules_manager
* @author     Alejandro Caballero - lava.caballero@gmail.com
*/

use hng2_base\config;

include "../config.php";
include "../includes/bootstrap.inc";

if( $account->level < config::ADMIN_USER_LEVEL ) throw_fake_401();

$template->page_contents_include = "contents/index.inc";
$template->set_page_title($current_module->language->page_title);
include "{$template->abspath}/admin.php";
