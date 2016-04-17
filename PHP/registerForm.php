<!-- HTML registration page-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Movieland DVD Rental Service</title>
</head>
<link href="style.css" rel="stylesheet" type="text/css">

<body>
<div id="header">
    <h3>Welcome to Movieland</h3>
</div>
<form id="form" name="form" method="post" action="registerProcessing.php">

    <table>
        <tr>
            <td>Username:</td>
            <td><input name="username" type="text" id="username"/></td>
        </tr>

        <tr>
            <td>Email:</td>
            <td>
                <input name="email" type="email" id="email"/></td>
        </tr>

        <tr>
            <td>Password:</td>
            <td>
                <input name="password" type="password" id="password"/></td>
        </tr>

        <tr>
            <td>Confirm Password :</td>
            <td>
                <input name="retype-password" type="password" id="retype-password"/></td>
        </tr>
    </table
    <label>
        <input type="submit" name="submit" id="submit" value="Register"/>
    </label>
    <input type="reset" name="reset" id="reset" value="Reset"/>

</form>
</body>
</html>