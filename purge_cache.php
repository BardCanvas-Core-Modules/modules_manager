<?php
/**
 * Modules manager cache purge
 * 
 * @package    HNG2
 * @subpackage modules_manager
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * @param string token (encrypted)
 * @param string lang             
 */

header("Content-Type: text/plain; charset=utf-8");
include "../config.php";

if( empty($_GET["token"]) ) die("Cache purging error: missing token.");
if( empty($_GET["lang"]) ) die("Cache purging error: missing language string.");

class tmp_crypt
{
    public function decrypt($text_encrypted, $key1, $key2, $key3)
    {
        return $this->_3le_keyED($this->_3le_decrypt($this->_3le_keyED(base64_decode($text_encrypted), $key3), $key2), $key1);
    }
    
    private function _3le_decrypt($txt, $key)
    {
        $txt = $this->_3le_keyED($txt, $key);
        $tmp = "";
        for( $i = 0; $i < strlen($txt); $i++ )
        {
            $md5 = substr($txt, $i, 1);
            $i++;
            $tmp .= (substr($txt, $i, 1) ^ $md5);
        }
        
        return $tmp;
    }
    
    private function _3le_keyED($txt, $encrypt_key)
    {
        $ctr = 0;
        $tmp = "";
        for( $i = 0; $i < strlen($txt); $i++ )
        {
            if( $ctr == strlen($encrypt_key) ) $ctr = 0;
            $tmp .= substr($txt, $i, 1) ^ substr($encrypt_key, $ctr, 1);
            $ctr++;
        }
        
        return $tmp;
    }
}
$tmp_crypt = new tmp_crypt();

$token  = $tmp_crypt->decrypt(
    stripslashes($_GET["token"]), WEBSITE_ID, ENCRYPTION_KEY, ROOTPATH
);

if( $token != WEBSITE_ID ) die("Cache purging error: invalid token received.");

$lang = stripslashes($_GET["lang"]);

foreach( glob("../data/cache/always_on-{$lang}*.dat") as $file )
    if( ! @unlink($file) ) die(sprintf(
        "Cannot remove '%s' - cache purge for %s aborted.",
        basename($file),
        $_GET["lang"]
    ));

foreach( glob("../data/cache/editable_prefs-{$lang}*.dat") as $file )
    if( ! @unlink($file) ) die(sprintf(
        "Cannot remove '%s' - cache purge for %s aborted.",
        basename($file),
        $_GET["lang"]
    ));

foreach( glob("../data/cache/modules-{$lang}*.dat") as $file )
    if( ! @unlink($file) ) die(sprintf(
        "Cannot remove '%s' - cache purge for %s aborted.",
        basename($file),
        $_GET["lang"]
    ));

include "../includes/bootstrap.inc";
$avoid_postinits = true;
include ROOTPATH . "/includes/modules_autoloader.inc";

echo "OK";
