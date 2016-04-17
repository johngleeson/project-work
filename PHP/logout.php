<html>
<title>
    Logout
</title>

<body>
<?php

# starts a session to use any session variables
session_start();

# unsets the email session variable
unset($_SESSION['email']);

# destroys the current session and redirects back to the login page
session_destroy();
header("location: index.php");
?>
</body>
</html>
