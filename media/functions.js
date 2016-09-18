
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
    var url = 'purge_cache.php?wasuuup=' + wasuuup();
    $.blockUI(blockUI_default_params);
    $.get(url, function(response)
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
    location.href = $_PHP_SELF + '?wasuuup=' + wasuuup();
}
