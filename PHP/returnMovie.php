<?php

# Allows a user to return a movie. Effectively just changes the movie status in the 'rentalhistory' database table

session_start();
include("connectToDb.php");

$title = $_POST['movieTitle'];
$email = $_SESSION['email'];
$usernameQuery = mysql_query("SELECT username from customer WHERE EMAIL = '$email'");
$usernameResult = mysql_fetch_assoc($usernameQuery);
$username = implode($usernameResult);

$updateRentalHistory = mysql_query("INSERT INTO rentalhistory VALUES ('', '$username', '','','$title') ");

$setMovieToInStock = mysql_query("UPDATE movies SET instock = 1 WHERE title = $'$title'");

$deleteFromCurrent = mysql_query("DELETE FROM rentalhistory WHERE current = '$title' AND username = '$username'");

header("refresh:0;url=myAccount.php");
mysql_close($connect);

?>
