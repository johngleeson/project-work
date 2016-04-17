<!-- This file, when accessed by a user displays up to 3 movies for every genre contained in the movie database-->

<link href="style.css" rel="stylesheet" type="text/css">

<?php

# Start a new session to allow access to any stored session variables
session_start();

# Various files are included so this php file can utilise any functionality contained within them
include("userHeader.php");
include("connectToDb.php");
include("functions.php");

# checks the database table for distinct genre values
$sql = "SELECT DISTINCT genre FROM movies";
$query = mysql_query($sql);

# loops through the query results and echos them as a table
while ($row = mysql_fetch_array($query)) {
    echo '<div class="movies">';

    echo '<h3>' . $row['genre'] . '</h3>';
    echo '<table name="' . $row['genre'] . '<tr id="tableHeader"><td>Title</td><td>Year of Release</td><td>Genre</td><td>Actions</td></tr>';

    # Selects and prints up to 3 movies for each genre in the database
    $result = mysql_query("SELECT movie_id,title,release_date,genre, instock FROM MOVIES WHERE genre= '" . $row['genre'] . "' ORDER BY release_date ASC LIMIT 3");
    $numRows = mysql_num_rows($result);
    while ($resultRow = mysql_fetch_array($result)) {

        # Defines a queue and rent button that will be displayed in the resulting table depending on whether or not certain conditions are met.
        $queueButton = '<form action="queueMovie.php" method="post"><button type="submit" value="' . $resultRow['movie_id'] . '" name="movieID">Queue!</button></form></td></tr>';
        $rentButton = '<form action="rentMovie.php" method="post"><button type="submit" value="' . $resultRow['movie_id'] . '" name="movieID">Rent!</button></form></td></tr>';

        # checks if a movie is already queued by a customer
        $queued = checkQueued($resultRow['title']);


        # echoes the results of the query to the screen as formatted HTML
        echo '<tr id="tableBody">
              <td>' . $resultRow['title'] . '</td>
              <td>' . $resultRow['release_date'] . '</td>
              <td>' . $resultRow['genre'] . '</td>
              <td>';

        # if the movie is not in stock and the movie is queued, informs the user that the movie is booked
        if (($resultRow['instock'] == 0) && ($queued == $resultRow['title'])) {
            $queueButton = '</td></tr>';
            echo 'Booked!';

            # if the movie is not in stock and not queued, allows the user to queue the movie by displaying a queue button
        } elseif (($resultRow['instock']) == 0 && ($queued != $resultRow['title'])) {
            echo $queueButton;

            # if the movie is in stock, allows the user to rent the movie by displaying a rent button
        } elseif ($resultRow['instock'] == 1) {
            echo $rentButton;
        }
    }

    echo '</table>';

    # if there are more than 3 rows returned, displays a 'more' button so the user can access the other movies of a particular genre.
    if ($numRows == 3) {
        echo '<form action="moviesByGenre.php" method="post"><label><input id="button" type="submit" value="More..."/><input name="category" type="hidden" value="', $row['genre'], '"/></label></form>';
    }

    echo '</div>';
}

mysql_close($connect);
?>


