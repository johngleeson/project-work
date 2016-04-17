<?php

# connection related properties that most php files will need access too

$server = "localhost";
$username = "root";
$password = "";
$database = "onlinedvd";

$connect = mysql_connect($server, $username, $password);
if (!$connect) {
    die('Could not connect: ' . mysql_error());
} else {
    mysql_select_db($database);
}

?>

