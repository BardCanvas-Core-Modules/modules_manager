<?php
/**
 * @var bool $update_cache
 * @var array $errors
 * @var array $messages
 */

#
# Precheck
#

if( $settings->get("modules:$do_module_name.installed") == "true" )
    die( $current_module->language->task_messages->already_installed );

#
# Install
#

if( file_exists(ROOTPATH . "/$do_module_name/module_install.inc") )
{
    include ROOTPATH . "/$do_module_name/module_install.inc";
    if( count($errors) > 0 )
    {
        $errors[] = $current_module->language->task_messages->installed_ko;
        
        return;
    }
}

$settings->set("modules:$do_module_name.installed", "true");
$update_cache = true;
$messages[] = $current_module->language->task_messages->installed_ok;

#
# Enable
#

$module_install_action = "enable";
if( file_exists(ROOTPATH . "/$do_module_name/module_install.inc") )
{
    include ROOTPATH . "/$do_module_name/module_install.inc";
    if( count($errors) > 0 )
    {
        $errors[] = $current_module->language->task_messages->enabled_ko;
    
        return;
    }
}

$settings->set("modules:$do_module_name.enabled", "true");
$messages[] = $current_module->language->task_messages->enabled_ok;
# $messages[]   = $current_module->language->task_messages->all_ops_ok;
