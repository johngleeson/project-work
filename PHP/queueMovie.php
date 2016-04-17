
<!-- File that allows the user to queue a movie -->

<?php
session_start();
include("connectToDb.php");

$id = $_POST['movieID'];
$email = $_SESSION['email'];

# finds the username of the user matching the email session variable
$usernameQuery = mysql_query("SELECT username from customer WHERE EMAIL = '$email'");
$usernameResult = mysql_fetch_assoc($usernameQuery);
$username = implode($usernameResult);

# finds the title of the movie belonging to the ID that was posted to this page
$titleQuery = mysql_query("SELECT title FROM movies WHERE movie_id = '$id'");
$titleResult = mysql_fetch_assoc($titleQuery);
$title = implode($titleResult);

# updates the rentalhistory by putting the movie title into the 'wishlist' column
$updateRentalHistory = mysql_query("INSERT INTO rentalhistory VALUES ('', '$username', '','$title','') ");

header("refresh:0;url=myAccount.php");

mysql_close($connect);

?>
