
function do_module(action, module_name)
{
    var url = 'maintenance.php?install_action=' + action
            + '&do_module_name=' + module_name
            + '&wasuuup=' + parseInt(Math.random() * 1000000000000000);
    
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
    var url = 'purge_cache.php?wasuuup=' + parseInt(Math.random() * 1000000000000000);
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
        'wasuuup':    parseInt(Math.random() * 1000000000000000)
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
    location.href = $_PHP_SELF + '?wasuuup=' + parseInt(Math.random() * 1000000000000000);
}
