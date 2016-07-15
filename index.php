<?php
/**
* Modules manager admin index
* 
* @package    HNG2
* @subpackage modules_manager
* @author     Alejandro Caballero - lava.caballero@gmail.com
*/

include "../config.php";
include "../includes/bootstrap.inc";

if( ! $account->_is_admin ) throw_fake_404();

$template->page_contents_include = "index.nav.inc";
$template->set_page_title($current_module->language->page_title);
include "{$template->abspath}/admin.php";
