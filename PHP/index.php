<!-- Page contains a simple login page and login/register buttons-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Movieland DVD Rental Service</title>
</head>
<link href="style.css" rel="stylesheet" type="text/css">

<body>

<div id="container">
    <div id="header">
        <h3>Welcome to Movieland</h3>
    </div>
    <div>
        <table>
            <form id="form" action="checkLogin.php" method="POST">
                <tr>
                    <td>Login:</td>
                </tr>
                <tr>
                    <td><label for="email">Email:</label></td>
                    <td><input type="emsil" name="email" id="email"/></td>
                </tr>
                <tr>
                    <td><label for="password">Password:</label></td>
                    <td><input type="password" name="password" id="password"/></td>
        </table>

        <button type="submit" name="login">Login</button>
        </form>

        <form method="post" action="registerForm.php">
            <input type="submit" value="Register">

        </form>

    </div>
</div>

</body>
</html>
