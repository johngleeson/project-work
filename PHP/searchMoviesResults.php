
<!--Similar to any of the other pages that shows results from the database in table form.
    Also allows the user to rent and queue movies.-->

<link href="style.css" rel="stylesheet" type="text/css">

<?php
session_start();
include('userHeader.php');
include("connectToDb.php");
include("functions.php");

$query = "SELECT movie_id,title,release_date,genre, instock FROM movies WHERE INSTR(UPPER(title),UPPER('" . $_POST['movie'] . "')) > 0";
$result = mysql_query($query);
$numRows = mysql_num_rows($result);
echo('<table><tr id="tableHeader"><td align="left">Title</td><td>Date of Release</td><td>Genre</td><td>Actions</td></tr>');

while ($resultRow = mysql_fetch_array($result)) {

    $queueButton = '<form action="queueMovie.php" method="post"><button type="submit" value="' . $resultRow['movie_id'] . '" name="movieID">Queue!</button></form></td></tr>';
    $rentButton = '<form action="rentMovie.php" method="post"><button type="submit" value="' . $resultRow['movie_id'] . '" name="movieID">Rent!</button></form></td></tr>';

    $queued = checkQueued($resultRow['title']);


    echo '<tr id="tableBody">
              <td>' . $resultRow['title'] . '</td>
              <td>' . $resultRow['release_date'] . '</td>
              <td>' . $resultRow['genre'] . '</td>
              <td>';

    if (($resultRow['instock'] == 0) && ($queued == $resultRow['title'])) {
        $queueButton = '</td></tr>';
        echo 'Booked!';

    } elseif (($resultRow['instock']) == 0 && ($queued != $resultRow['title'])) {
        echo $queueButton;
    } elseif ($resultRow['instock'] == 1) {
        echo $rentButton;
    }


}

if (empty ($numRows)) {
    echo '<tr><td>No Match Found!</td></tr>';
}

echo '</table>';

mysql_close($connect);
?>

