
<!-- File that displays account info to the user-->

<link href="style.css" rel="stylesheet" type="text/css">


<?php

# starts a session so the user can access any created user variables
session_start();

# includes any files that are required by this file
include("userHeader.php");
include("connectToDb.php");

# assigns the email session variable to a new 'email' variable
$email = $_SESSION['email'];

# sql query to find out the username of the current user
$usernameQuery = mysql_query("SELECT username FROM customer WHERE email = '$email'");
$usernameResult = mysql_fetch_assoc($usernameQuery);
$username = $usernameResult['username'];

# query to check what (if any) movies the user is currently renting
$currentRentals = mysql_query("SELECT current FROM rentalhistory WHERE username = '$username'");
$numCurrentRentals = mysql_num_rows($currentRentals);

# HTML table echoed to screen displaying any movies that the current user has rented
echo '<h3>Movies Currently Rented:</h3>';
echo('<table>');

while ($resultRow = mysql_fetch_array($currentRentals)) {

    # creates a HTML 'return' button that will appear in the table as needed
    $returnButton = '<td><form action="returnMovie.php" method="post"><button type="submit" value="' . $resultRow['current'] . '" name="movieTitle">Return!</button></form></td></tr>';

    # if statement to either display the movie along with a return button or display a message informing the user there are no movies currently rented
    if ($resultRow['current'] == null) {
        $numCurrentRentals--;
    } else echo '<tr id="tableBody">
              <td>' . $resultRow['current'] . '</td>' . $returnButton . '</tr>';

    if (empty ($numCurrentRentals)) {
        echo '<tr><td>You have no movies currently rented!</td></tr>';
    }
}
echo '</table>';

# query to check what (if any) movies the user has currently queued
$queuedMovies = mysql_query("SELECT wishlist FROM rentalhistory WHERE username = '$username'");
$numQueuedMovies = mysql_num_rows($queuedMovies);

# HTML table echoed to screen displaying any movies that the current user has queued
echo '<h3>Movies Currently Queued:</h3>';
echo('<table>');

while ($resultRow = mysql_fetch_array($queuedMovies)) {

    # if statement to either display the currently queued movies or a message informing the user that there are no currently queued movies
    if ($resultRow['wishlist'] == null) {
        $numQueuedMovies--;
    } else
        echo '<tr id="tableBody">
              <td>' . $resultRow['wishlist'] . '</td></tr>';


    if (empty ($numQueuedMovies)) {
        echo '<tr><td>You have not queued any movies!</td></tr>';
    }
}
echo '</table>';

# query to check what (if any) movies the user has previously returned
$returnedMovies = mysql_query("SELECT returned FROM rentalhistory WHERE username = '$username'");
$numReturnedMovies = mysql_num_rows($returnedMovies);

echo '<h3>Previously Returned Movies:</h3>';
echo('<table>');

while ($resultRow = mysql_fetch_array($returnedMovies)) {

    if ($resultRow['returned'] == null) {
        $numReturnedMovies--;
    } else
        echo '<tr id="tableBody">
              <td>' . $resultRow['returned'] . '</td></tr>';

    if (empty ($numReturnedMovies)) {
        echo '<tr><td>You have not returned any movies!</td></tr>';
    }

}
echo '</table>';

mysql_close($connect);
?>