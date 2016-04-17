<?php

# Allows a user to rent a movie. Effectively just changes the movie status in the 'rentalhistory' database table

session_start();
include("connectToDb.php");

$id = $_POST['movieID'];
$email = $_SESSION['email'];
$usernameQuery = mysql_query("SELECT username from customer WHERE EMAIL = '$email'");
$usernameResult = mysql_fetch_assoc($usernameQuery);
$username = implode($usernameResult);

$titleQuery = mysql_query("SELECT title FROM movies WHERE movie_id = '$id'");
$titleResult = mysql_fetch_assoc($titleQuery);
$title = implode($titleResult);

$setRented = mysql_query("UPDATE movies SET instock = 0 WHERE movie_id = $id");
$updateRentalHistory = mysql_query("INSERT INTO rentalhistory VALUES ('', '$username', '$title','','') ");

header("refresh:0;url=myAccount.php");
mysql_close($connect);

?>
