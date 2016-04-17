<?php

session_start();
//Connect to DB
include("connectToDb.php");

//assign posted user input from index to variables, strings are sanitized to prevent SQL injection
$email = filter_var($_POST['email'], FILTER_SANITIZE_EMAIL);
$password = filter_var($_POST['password'], FILTER_SANITIZE_STRING);
//calculate the hash of the input password and store in a variable
$passHash = hash('sha512', $password);

//check to see if email exists in DB and store in variable
$check_email = mysql_query("SELECT * FROM customer WHERE EMAIL = '$email'");
$check_email_result = mysql_fetch_array($check_email);

$checkPassword = mysql_query("SELECT PASSWORD FROM customer WHERE PASSWORD = '$passHash' AND EMAIL = '$email'");
$checkPassword = mysql_fetch_array($checkPassword);

//if there is no input
if (($email == "") || ($password == "")) {
    //inform the user they did not input anything
    echo '<script type="text/javascript">alert("Invalid input, please enter your email and password!")</script>';
    //return to to index.php after a 2 second sleep
    header("refresh:0;url=index.php");
} //if there is input
else {

    //if nothing returned from SQL call, no entry for the that email
    if ($check_email_result == NULL) {
        //inform the user they have used an invalid email address
        echo '<script type="text/javascript">alert("Invalid Email Address!")</script>';
        //return to to index.php after a 2 second sleep
        header("refresh:0;url=index.php");
    }

    //if the username address exists in the DB
    if ($check_email_result != NULL) {
        //SQL query to get the password for a selected email address
        $query = mysql_query("SELECT * FROM customer WHERE PASSWORD = '$passHash' AND EMAIL = '$email'");
        $result = mysql_fetch_array($query) or die(mysql_error());
        $result = $result['PASSWORD'];
        //if the passwords are not equal
        if (($passHash !== $result)) {
            //inform the user of invalid password
            echo '<script type="text/javascript">alert("Invalid Password!")</script>';
            //return to to index.php after a 2 second sleep
            header("refresh:0;url=index.php");
        } else
            //if passwords match
            if ($passHash == $result) {
                //Assign users email address, to a new session variable called email
                $_SESSION['email'] = $email;

                //inform the user they have logged in successfully
                echo '<script type="text/javascript">alert("Login Successful")</script>';
                //return to to browseMovies.php after a 2 second sleep
                header("refresh:0;url=browseMovies.php");
            } else {
                //inform the user of invalid password
                echo '<script type="text/javascript">alert("Invalid Password!")</script>';
                //return to to index.php after a 2 second sleep
                header("refresh:0;url=index.php");
            }
    }
}

//close connection to DB
mysql_close($connect);
?>

