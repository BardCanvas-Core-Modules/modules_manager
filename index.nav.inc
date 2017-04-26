<?
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
</h1>

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

<div class="table_wrapper">
    <table id="modules_listing" class="nav_table no_highlight" data-filter="all_modules">
        <tbody>

        <? $infos_per_handler = array(); ?>
        <? foreach($_all_modules as $module_name => $this_module):
            
            $tr_state = "";
            if( ! $this_module->installed  ) $tr_state = "new";
            elseif( $this_module->installed && ! $this_module->enabled ) $tr_state = "disabled";
            
            $filter_classes = "all_modules group_{$this_module->group} ";
            if( $this_module->installed ) $filter_classes .= " flag_installed";
            else                          $filter_classes .= " flag_not_installed";
            if( $this_module->enabled   ) $filter_classes .= " flag_enabled";
            else                          $filter_classes .= " flag_disabled";
            ?>
            
            <tr class="module <?= $tr_state ?> <?= $filter_classes ?>" valign="top">
                
                <td width="30%">
                    <p class="principal_data">
                        <? if( ! empty($this_module->language->icon) ): ?>
                            <span class="fa <?= $this_module->language->icon ?>"></span>
                        <? endif; ?>
                        <?= empty($this_module->language->display_name) ? $module_name : $this_module->language->display_name ?>
                    </p>
                    <p>
                        <?
                        echo "{$current_module->language->per_module_fields->version} {$this_module->version} ";
                        $info_xml = ROOTPATH . "/{$this_module->name}/module_info.xml";
                        $last_update = date("Y-m-d H:i:s", filemtime($info_xml));
                        echo replace_escaped_vars(
                            $current_module->language->per_module_fields->uploaded, '{$time}',
                            time_elapsed_string($last_update)
                        );
                        ?>
                    </p>
                    <div class="actions">
                        <? if( ! $this_module->installed ): ?>
                            <? if($this_module->working_flags->install != "true"): ?>
                                <span class="action pseudo_link disabled">
                                    <span class="fa fa-plus-circle"></span>
                                        <?= $current_module->language->per_module_fields->action_buttons->install ?>
                                </span>
                            <? else: ?>
                                <span class="action pseudo_link"  
                                      onclick="do_module('install', '<?=$this_module->name?>'); return false;">
                                    <span class="fa fa-plus-circle"></span>
                                    <?= $current_module->language->per_module_fields->action_buttons->install ?>
                                </span>
                            <? endif; ?>
                            <span class="action pseudo_link disabled">
                                <span class="fa fa-chevron-circle-up"></span>
                                <?= $current_module->language->per_module_fields->action_buttons->enable ?>
                            </span>
                        <? else: ?>
                            
                            <? if($this_module->working_flags->uninstall != "true"): ?>
                                <span class="action pseudo_link disabled">
                                    <span class="fa fa-minus-circle"></span>
                                        <?= $current_module->language->per_module_fields->action_buttons->uninstall ?>
                                </span>
                            <? else: ?>
                                <span class="action pseudo_link" 
                                      onclick="do_module('uninstall', '<?=$this_module->name?>')">
                                    <span class="fa fa-minus-circle"></span>
                                        <?= $current_module->language->per_module_fields->action_buttons->uninstall ?>
                                </span>
                            <? endif; ?>
                            
                            <? if( ! $this_module->enabled ): ?>
        
                                <? if($this_module->working_flags->enable != "true"): ?>
                                    <span class="action pseudo_link disabled">
                                        <span class="fa fa-chevron-circle-up"></span>
                                            <?= $current_module->language->per_module_fields->action_buttons->enable ?>
                                    </span>
                                <? else: ?>
                                    <span class="action pseudo_link"
                                          onclick="do_module('enable', '<?=$this_module->name?>')">
                                        <span class="fa fa-chevron-circle-up"></span>
                                            <?= $current_module->language->per_module_fields->action_buttons->enable ?>
                                    </span>
                                <? endif; ?>
                                
                            <? else: ?>
                                <? if($this_module->working_flags->disable != "true"): ?>
                                    <span class="action pseudo_link disabled">
                                        <span class="fa fa-chevron-circle-down"></span>
                                            <?= $current_module->language->per_module_fields->action_buttons->disable ?>
                                    </span>
                                <? else: ?>
                                    <span class="action pseudo_link"
                                          onclick="do_module('disable', '<?=$this_module->name?>')">
                                        <span class="fa fa-chevron-circle-down"></span>
                                            <?= $current_module->language->per_module_fields->action_buttons->disable ?>
                                    </span>
                                <? endif; ?>
                            <? endif; ?>
                        <? endif; ?>
                    </div>
                </td>
                
                <? $info_handler = "{$current_module->name}_extended_info_{$this_module->name}";
                   $infos_per_handler[$info_handler] = 0; ?>
                <td width="70%">
                    <p style="font-size: 1.2em;">
                        <?= $this_module->language->description ?>
                        <span class="info_section_trigger pseudo_link" data-trigger-for="<?= $info_handler ?>"
                              onclick="toggle_info_section('<?= $info_handler ?>')"><span
                              class="fa fa-info-circle" title="<?= $language->toggle_info ?>"></span></span>
                    </p>
                    
                    <p>
                        <?= $current_module->language->per_module_fields->group ?>
                        <span class="framed_content">
                            <?= $this_module->group ?>
                        </span>
                        
                        <?= $current_module->language->per_module_fields->admin_only->caption ?>
                        <span class="framed_content">
                            <?= $this_module->admin_only == "true"
                                ? $current_module->language->per_module_fields->admin_only->generic
                                : $current_module->language->per_module_fields->admin_only->only_admins ?>
                        </span>
                        
                        <?= $current_module->language->per_module_fields->working_flags->caption ?>
                        <span class="framed_content">
                            <span class="fa fa-<?= $this_module->working_flags->install == "true" ? "check" : "warning" ?>"></span>
                            <?= $current_module->language->per_module_fields->working_flags->install->{$this_module->working_flags->install}?>
                        </span>
                        <span class="framed_content">
                            <span class="fa fa-<?= $this_module->working_flags->enable == "true" ? "check" : "times" ?>"></span>
                            <?= $current_module->language->per_module_fields->working_flags->enable->{$this_module->working_flags->enable}?>
                        </span>
                        <span class="framed_content">
                            <span class="fa fa-<?= $this_module->working_flags->disable == "true" ? "check" : "times" ?>"></span>
                            <?= $current_module->language->per_module_fields->working_flags->disable->{$this_module->working_flags->disable}?>
                        </span>
                        <span class="framed_content">
                            <span class="fa fa-<?= $this_module->working_flags->uninstall == "true" ? "check" : "times" ?>"></span>
                            <?= $current_module->language->per_module_fields->working_flags->uninstall->{$this_module->working_flags->uninstall}?>
                        </span>
                        
                        <? if( ! empty($this_module->menu_items->item) ): ?>
                            <?= $current_module->language->per_module_fields->menu_items->caption ?>
                            <? foreach($this_module->menu_items->item as $this_menu_item): ?>
                                <span class="framed_content">
                                    «<?= $this_module->language->admin->menu_items->{$this_menu_item->caption_language_var}  ?>»
                                </span>
                            <? endforeach; ?>
                        <? endif; ?>
                    </p>
                    
                    <div class="info_handler" id="<?= $info_handler ?>"
                         style="<? if($account->engine_prefs[$info_handler] != "visible") echo "display: none;"; ?>">
                        <? if( is_object($this_module->php_includes) ): ?>
                            <?= $current_module->language->per_module_fields->php_includes->caption ?>
                            <ul>
                                <? foreach($this_module->php_includes as $key => $val):
                                    $infos_per_handler[$info_handler]++; ?>
                                    <li>
                                        [<?= $key ?>] <?= $current_module->language->per_module_fields->php_includes->{$key}  ?>
                                    </li>
                                <? endforeach; ?>
                            </ul>
                        <? endif; ?>
                        
                        <? if( is_object($this_module->template_includes) ):  ?>
                            <?= $current_module->language->per_module_fields->template_includes->caption ?>
                            <ul>
                                <? foreach($this_module->template_includes as $key => $val):
                                    $infos_per_handler[$info_handler]++; ?>
                                    <li>
                                        [<?= $key ?>] <?= $current_module->language->per_module_fields->template_includes->{$key}  ?>
                                    </li>
                                <? endforeach; ?>
                            </ul>
                        <? endif; ?>
                        
                        <? if( is_array($this_module->extends_to) || is_object($this_module->extends_to) ): ?>
                            <?= $current_module->language->per_module_fields->extends_to->caption ?>
                            <ul>
                                <? foreach($this_module->extends_to as $extended_module_key => $extended_module_case_data):
                                    $infos_per_handler[$info_handler]++; ?>
                                    <li>
                                        <?= ucwords(str_replace("_", " ", $extended_module_key)) ?>:
                                        <ul>
                                            <? foreach($extended_module_case_data as $extended_module_case => $extended_module_include): ?>
                                                <li><?= ucwords(str_replace("_", " ", $extended_module_case)) ?>:
                                                    <?= ucwords(str_replace("_", " ", implode(", ", array_keys( (array) $extended_module_include )))) ?>.</li>
                                            <? endforeach; ?>
                                        </ul>
                                    </li>
                                <? endforeach; ?>
                            </ul>
                        <? endif; ?>
                        
                        <? if( ! empty($this_module->extension_areas_info) ):
                            $infos_per_handler[$info_handler]++; ?>
                            <?= $current_module->language->per_module_fields->extension_areas_info->caption ?>
                            <? $info = str_replace("\n", "</li><li>", trim($this_module->extension_areas_info) ); ?>
                            <ul>
                                <li><?= $info ?></li>
                            </ul>
                        <? endif; ?>
                    </div>
                </td>
                
            </tr>
        <? endforeach; ?>

        </tbody>
    </table>
</div>

<? if( count($infos_per_handler) > 0 ): ?>
    <script type="text/javascript">
        <? foreach($infos_per_handler as $info_handler => $echoes): if( $echoes > 0 ) continue; ?>
            $('.info_section_trigger[data-trigger-for="<?= $info_handler ?>"]').hide();
        <? endforeach; ?>
    </script>
<? endif; ?>