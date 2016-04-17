<?php
session_start();
include('userHeader.php');

?>

<link href="style.css" rel="stylesheet" type="text/css">

<form id="form1" name="form1" method="post" action="searchMoviesResults.php">
    <div id="searchForm">
        <p>Movie Name:
            <input type="text" name="movie" id="movie"/>
            <label> <input id="button" type="submit" name="search" id="search" value="Search"/></label>
        </p>
    </div>
</form>
