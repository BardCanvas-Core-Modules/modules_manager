<?php
/**
 * @var module[] $_all_modules
 */

use hng2_base\module;

$infos_per_handler = array();
foreach($_all_modules as $module_name => $this_module):
    
    $tr_state = "";
    if(     $this_module->group == "core" )                        $tr_state = "core";
    elseif( stristr($module_name, "templates/") !== false )        $tr_state = "template";
    elseif( ! $this_module->installed  )                           $tr_state = "new";
    elseif( $this_module->installed && ! $this_module->enabled )   $tr_state = "disabled";
    
    $filter_classes = "all_modules group_{$this_module->group} ";
    if( $this_module->installed ) $filter_classes .= " flag_installed";
    else                          $filter_classes .= " flag_not_installed";
    if( $this_module->enabled   ) $filter_classes .= " flag_enabled";
    else                          $filter_classes .= " flag_disabled";
    
    $config->globals["rendering_module"]                = $this_module;
    $config->globals["rendering_module_filter_classes"] = $filter_classes;
    ?>
    
    <tr class="module <?= $tr_state ?> <?= $filter_classes ?>" valign="top"
        data-module-group="<?= $this_module->group ?>" data-module-name="<?= $this_module->name ?>"
        data-package-version="<?= $this_module->version ?>">
        
        <td width="30%">
            <p class="principal_data">
                <? $current_module->load_extensions("index", "rendering_module_in_principal_data_before"); ?>
                <? if( ! empty($this_module->language->icon) ): ?>
                    <span class="fa <?= $this_module->language->icon ?>"></span>
                <? endif; ?>
                <?= empty($this_module->language->display_name) ? $module_name : $this_module->language->display_name ?>
                <? $current_module->load_extensions("index", "rendering_module_in_principal_data_after"); ?>
            </p>
            
            <p class="tag">
                <span class="inline-block nowrap pseudo_link clipboard-copy"
                      data-clipboard-text="<?= $this_module->group ?>/<?= $this_module->name ?>">
                    <i class="fa fa-tag fa-fw"></i>
                    <?= $this_module->group ?>/<?= $this_module->name ?>
                    <i class="fa fa-copy fa-fw"></i>
                </span>
            </p>
            
            <p class="local_version_info">
                <?
                echo "{$current_module->language->per_module_fields->version} ";
                echo "<span class='local_version_details'>";
                echo "{$this_module->version} ";
                echo replace_escaped_vars(
                    $current_module->language->per_module_fields->uploaded, '{$time}',
                    time_elapsed_string($last_update_dates[$module_name])
                );
                echo "</span>"
                ?>
                <? $current_module->load_extensions("index", "rendering_module_after_version_info"); ?>
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
                <? $current_module->load_extensions("index", "rendering_module_before_description"); ?>
                
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
                    <?= $this_module->admin_only != "true"
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
