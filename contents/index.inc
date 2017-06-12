<?php
/**
 * @var array  $errors
 * @var array  $messages
 * @var module $this_module
 */

use hng2_base\module;

# Module re-scanning, including uninstalled ones

$_all_modules = array();

$module_dirs_pattern = ROOTPATH . "/*";
$available_modules   = glob($module_dirs_pattern, GLOB_ONLYDIR);
foreach($available_modules as $this_module_dir)
{
    $module_info_file = "$this_module_dir/module_info.xml";
    if( ! file_exists($module_info_file) ) continue;
    
    $module_keyname = basename($this_module_dir);
    $module         = new module($module_info_file);
    $sorting_key    = empty($module->language->display_name) ? $module_keyname : trim($module->language->display_name);
    
    $_all_modules[$sorting_key] = $module;
}

ksort($_all_modules);
$resorted_modules = array();
foreach($_all_modules as $display_name => $module) $resorted_modules[$module->name] = $module;
$_all_modules = $resorted_modules;
unset( $resorted_modules );

$current_module->load_extensions("index", "pre_rendering");
?>

<script type="text/javascript" src="media/functions~v1.2.6-2.js"></script>

<h1>
    <button class="pull-right" onclick="reload_self()">
        <span class="fa fa-refresh fa-fw"></span>
        <?=  $language->reload ?>
    </button>
    
    <?= $current_module->language->page_title ?>
    
    <? if( $settings->get("modules:modules_manager.disable_cache") == "true" ): ?>
        <button onclick="change_caching_status('enabled')">
            <span class="fa fa-toggle-off"></span>
            <?= $current_module->language->buttons->toggle_cache->enable ?>
        </button>
    <? else: ?>
        <button onclick="change_caching_status('disabled')">
            <span class="fa fa-toggle-on"></span>
            <?= $current_module->language->buttons->toggle_cache->disable ?>
        </button>
        <button onclick="purge_modules_cache()" title="<?= $current_module->language->buttons->purge_cache->title ?>">
            <span class="fa fa-refresh"></span>
            <?= $current_module->language->buttons->purge_cache->caption ?>
        </button>
    <? endif; ?>
    
    <? $current_module->load_extensions("index", "extra_heading_buttons"); ?>
</h1>

<? $current_module->load_extensions("index", "pre_tabs_bar"); ?>

<? $selected_tab = trim(stripslashes($_GET["group"])); ?>
<div class="tabs_bar">
    <span id="flag_tabs">
        <?
        if( empty($selected_tab) ) $selected_tab = "all_modules";
        foreach($current_module->language->filter_tabs->tab as $tab):
            $active = trim($tab["for"]) == $selected_tab ? "state_active" : "" ?>
            <span class="tab framed_content <?= $active ?> inlined" data-for="<?= $tab["for"] ?>" style="cursor: pointer"
                  onmouseover="$(this).toggleClass('state_hover', true)"
                  onmouseout="$(this).toggleClass('state_hover', false)"
                  onclick="toggle_modules('<?= $tab["for"] ?>')">
                <?= $tab ?>
            </span>
        <? endforeach; ?>
    </span>
    
    <span id="modules_filter" style="display: inline-block; white-space: nowrap;">
        <input type="text" name="filter" style="width: 150px; padding-right: 26px;"
               placeholder="<?= $current_module->language->search ?>"
               value="<?= trim(stripslashes($_GET["filter"])) ?>"
               onkeyup="filter_modules()" onchange="filter_modules()">
        <img src="<?= $config->full_root_path ?>/media/backspace.png" onclick="clear_modules_filter()"
             style="cursor: pointer; position: absolute; margin-left: -24px; margin-top: 2px;">
    </span>
    
    <?
    $all_groups = array();
    foreach($_all_modules as $module_name => $this_module)
    {
        $key = trim($this_module->group);
        if( isset($all_groups[$key]) ) continue;
        
        $val = ucwords(str_replace("_", " ", trim($this_module->group)));
        $all_groups[$key] = $val;
    }
    asort($all_groups);
    ?>
    <select id="group_filter" onchange="toggle_modules( this.options[this.selectedIndex].value )">
        <option <? if( $selected_tab == "all_modules" ) echo "selected"; ?> value="all_modules"><?= $current_module->language->filter_tabs->all_groups ?></option>
        <? foreach($all_groups as $key => $val): ?>
            <option <? if($selected_tab == "group_{$key}") echo "selected"; ?> value="group_<?= $key ?>"><?= $val ?></option>
        <? endforeach; ?>
    </select>
</div>

<? $current_module->load_extensions("index", "pre_modules_listing"); ?>

<div class="table_wrapper">
    <table id="modules_listing" class="nav_table no_highlight" data-filter="all_modules">
        <tbody>
        
        <? include __DIR__ . "/index_modules.inc"; ?>
        
        </tbody>
    </table>
</div>

<? $current_module->load_extensions("index", "post_modules_listing"); ?>

<? if( count($infos_per_handler) > 0 ): ?>
    <script type="text/javascript">
        <? foreach($infos_per_handler as $info_handler => $echoes): if( $echoes > 0 ) continue; ?>
            $('.info_section_trigger[data-trigger-for="<?= $info_handler ?>"]').hide();
        <? endforeach; ?>
    </script>
<? endif; ?>