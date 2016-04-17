<?php

# only one function here as I figured out how to use functions far too late in the project

include('connectToDb.php');


# function to check if a movie is already queued. Accepts a movie title and returns movie title also
function checkQueued($title)
{

    $checkQueuedQuery = mysql_query("SELECT wishlist FROM  rentalhistory WHERE wishlist = '$title'");
    $queuedQueryResult = mysql_fetch_assoc($checkQueuedQuery);
    $queuedQueryResult = $queuedQueryResult['wishlist'];

    return $queuedQueryResult;
}


?>