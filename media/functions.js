
function filter_modules()
{
    var group = get_current_group_name();
    if( group != 'all_modules' ) toggle_modules('all_modules', false);
    
    var needle = $('#modules_filter').find('input[name="filter"]').val().trim().toLowerCase();
    $('#modules_listing').find('.module').each(function()
    {
        var $this    = $(this);
        var haystack = $(this).text().toLowerCase();
        
        if( haystack.indexOf(needle) >= 0 ) $this.show();
        else                                $this.hide();
    });
}

function clear_modules_filter()
{
    $('#modules_filter').find('input[name="filter"]').val('');
    filter_modules();
}

function toggle_modules(key, clear_filter_before)
{
    if( typeof clear_filter_before == 'undefined' ) clear_filter_before = true;
    
    if( clear_filter_before ) $('#modules_filter').find('input[name="filter"]').val('');
    
    var $tabs = $('#flag_tabs');
    $tabs.find('.tab').toggleClass('state_active', false);
    $tabs.find('.tab[data-for="' + key + '"]').toggleClass('state_active', true);
    $('#group_filter').find('option[value="' + key + '"]').prop('selected', true);
    
    $('#modules_listing').find('.module').each(function()
    {
        var $this = $(this);
        if( $this.hasClass(key) ) $this.show();
        else                      $this.hide();
    });
}

function get_current_group_name()
{
    var group  = $('#group_filter').find('option:selected').val();
    
    if( group == 'all_modules' )
        group = $('#flag_tabs').find('.tab.state_active').attr('data-for');
    
    return group;
}

function do_module(action, module_name)
{
    var url = 'maintenance.php?install_action=' + action
            + '&do_module_name=' + module_name
            + '&wasuuup=' + wasuuup();
    
    stop_notifications_getter();
    $.blockUI(blockUI_default_params);
    $.get(url, function(response)
    {
        if( response != 'OK' )
        {
            $.unblockUI();
            alert( response );
            start_notifications_getter();
            
            return;
        }
        
        reload_self();
    });
}

function purge_modules_cache()
{
    var url = sprintf('purge_cache.php');
    
    var params = {
        token:   $_CACHE_PURGING_TOKEN,
        lang:    '',
        wasuuup: wasuuup()
    };
    
    var languages_to_purge = $_ALL_LANGUAGES.length;
    var purged_languages   = 0;
    
    $.blockUI(blockUI_default_params);
    for(var i in $_ALL_LANGUAGES)
    {
        params.lang = $_ALL_LANGUAGES[i];
        
        $.get(url, params, function(response)
        {
            purged_languages++;
            
            if( response !== 'OK' )
            {
                throw_notification(response, 'error');
                $.unblockUI();
                
                return;
            }
            
            if( purged_languages >= languages_to_purge )
                reload_self();
        });
    }
}

function change_caching_status(new_status)
{
    var url = 'change_caching_status.php';
    var params = {
        'new_status': new_status,
        'wasuuup':    wasuuup()
    };
    $.blockUI(blockUI_default_params);
    $.get(url, params, function(response)
    {
        if( response != 'OK' )
        {
            $.unblockUI();
            alert( response );
            
            return;
        }
        
        reload_self();
    });
}

function reload_self()
{
    var group  = get_current_group_name();
    var filter = $('#modules_filter').find('input[name="filter"]').val();
    
    location.href = $_PHP_SELF
                  + '?filter=' + encodeURI(filter)
                  + '&group='  + encodeURI(group)
                  + '&wasuuup=' + wasuuup();
}

$(document).ready(function()
{
    var group = get_current_group_name();
    if( group != 'all_modules' ) toggle_modules(group);
    
    if( $('#modules_filter').find('input[name="filter"]').val() != '' )
        filter_modules();
});
