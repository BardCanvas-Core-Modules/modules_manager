<?php
/**
 * @var bool $update_cache
 * @var array $errors
 * @var array $messages
 */

#
# Prechecks
#

if( $settings->get("modules:$do_module_name.installed") != "true" )
{
    $errors[] = $current_module->language->task_messages->not_installed;
    
    return;
}

if( $settings->get("modules:$do_module_name.enabled") == "true" )
{
    $errors[] = $current_module->language->task_messages->already_enabled;
    
    return;
}

#
# Enable
#

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
$update_cache = true;
$messages[]   = $current_module->language->task_messages->enabled_ok;
# $messages[]   = $current_module->language->task_messages->all_ops_ok;
