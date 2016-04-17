<?php

# This file is where the info from the registration page gets posted to

include("connectToDb.php");

# if - else construct to validate form contents before the data is entered into the database
if ($_POST['password'] != $_POST['retype-password']) {
    echo '<script type="text/javascript">alert("The two entered passwords don`t match!")</script>';
    header("refresh:0;url=registerForm.php");

} else if (strlen($_POST['username']) > 45) {
    echo '<script type="text/javascript">alert("Your username is too long!")</script>';
    header("refresh:0;url=registerForm.php");
} else if (strlen($_POST['username']) < 1) {
    echo '<script type="text/javascript">alert("Your username is too short!")</script>';
    header("refresh:0;url=registerForm.php");
} else if (strlen($_POST['password']) > 256) {
    echo '<script type="text/javascript">alert("Password is too long!")</script>';
    header("refresh:0;url=registerForm.php");
} else if (strlen($_POST['password']) < 1) {
    echo '<script type="text/javascript">alert("Password is too short!")</script>';
    header("refresh:0;url=registerForm.php");

} else {

    # various checks to see if the info posted by the user already exists on the database.
    # if the info doesn't already exist in the database, ok. Otherwise, gives warnings to the user
    $username = filter_var($_POST['username'], FILTER_SANITIZE_STRING);
    $passHash = hash('sha512', (filter_var($_POST['password'], FILTER_SANITIZE_STRING)));
    $email = filter_var($_POST['email'], FILTER_SANITIZE_EMAIL);


    $check_username = mysql_query("SELECT * FROM customer WHERE username='$username'");
    $check_username_result = mysql_fetch_array($check_username);

    $check_email = mysql_query("SELECT * FROM customer WHERE email='$email'");
    $check_email_result = mysql_fetch_array($check_email);

    if (!$check_username_result == NULL) {
        echo '<script type="text/javascript">alert("Username already registered!")</script>';
        header("refresh:0;url=registerForm.php");

    } elseif (!$check_email_result == NULL) {
        echo '<script type="text/javascript">alert("Email already registered!")</script>';
        header("refresh:0;url=registerForm.php");

    } else if ($check_username_result == NULL && $check_email_result == NULL) {


        mysql_query("INSERT INTO customer( email, password, username) VALUES
											('" . $email . "',
											'" . $passHash . "',
											'" . $username . " ')") or die(mysql_error());

        echo '<script type="text/javascript">alert("Successfully registered")</script>';
        header("refresh:0;url=index.php");

    }

}
mysql_close($connect);
?>