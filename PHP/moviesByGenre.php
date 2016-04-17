<!-- page to display movies of a particular genre-->

<link href="style.css" rel="stylesheet" type="text/css">
<?php

# starts a session to use any session variables
session_start();

# includes any files that are necessary for this file to work
include("userHeader.php");
include("connectToDb.php");
include("functions.php");

# selects all of the movies of whatever genre was posted to this page from the browseMovies page and echoes them back as a HTML table
$query = "SELECT movie_id,title,release_date,genre, instock FROM MOVIES WHERE genre='" . $_POST['category'] . "'";
$result = mysql_query($query);
echo('<table><tr id="tableHeader"><td>Title</td><td>Year of Release</td><td>Genre</td><td>Actions</td></tr>');

while ($resultRow = mysql_fetch_array($result)) {

    # creates two HTML buttons that are actually tiny HTML forms. These buttons will appear if certain conditions are met
    $queueButton = '<form action="queueMovie.php" method="post"><button type="submit" value="' . $resultRow['movie_id'] . '" name="movieID">Queue!</button></form></td></tr>';
    $rentButton = '<form action="rentMovie.php" method="post"><button type="submit" value="' . $resultRow['movie_id'] . '" name="movieID">Rent!</button></form></td></tr>';

    # checkQueued function called to see if the displayed movies are already queued
    $queued = checkQueued($resultRow['title']);

    echo '<tr id="tableBody">
              <td>' . $resultRow['title'] . '</td>
              <td>' . $resultRow['release_date'] . '</td>
              <td>' . $resultRow['genre'] . '</td>
              <td>';


    # if the movies are not in stock and are already queued, user is informed that they are booked
    if (($resultRow['instock'] == 0) && ($queued == $resultRow['title'])) {
        $queueButton = '</td></tr>';
        echo 'Booked!';

        # if the movie is not in stock but not in a queue (i.e. if it is currently rented, user can queue the movie)
    } elseif (($resultRow['instock']) == 0 && ($queued != $resultRow['title'])) {
        echo $queueButton;

        # if move is in stock , rent button is displayed so the user can rent the movie
    } elseif ($resultRow['instock'] == 1) {
        echo $rentButton;
    }


}

echo '</table>';

mysql_close($connect);
?>


