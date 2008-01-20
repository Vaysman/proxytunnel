<?php

include("_begin.php");

$path = "./news/";
$dh = @opendir($path);

while (false !== ($file=readdir($dh)))
{
     if (substr($file,0,1) != ".")
         $files[]=array(filemtime($path.$file),$file);  #2-D array
}
closedir($dh);

if ($files)
{
     rsort($files); #sorts by filemtime

     #done! Show the files sorted by modification date
     foreach ($files as $file)
     {
	echo "<h2 class=\"headline\">" . date('Y/m/d', $file[0] ) . " - " . $file[1] . "</h2>\n";
	include( "news/" . $file[1] );
     }
}

include("_end.php");
?>
