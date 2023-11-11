<?php
if(isset($_POST["Submit1"]))
{
setcookie("name",$_POST["name"], time() + 3600, "/", "", 0);
setcookie("age", $_POST["age"], time() + 3600, "/", "", 0);
setcookie("city", $_POST["city"], time() + 3600, "/", "", 0);
echo "Cookies Created !!";
}
if(isset($_POST["Submit3"]))
{
setcookie("name","", time() - 3600, "/", "", 0);
setcookie("age", "", time() - 3600, "/", "", 0);
setcookie("city", "", time() - 3600, "/", "", 0);
} 
?>
<html>
<head>
<title>PHP Cookies Example</title>
</head>
<body>

<FORM method="POST" action="cookie.php">
Enter Name : <input type="text" name="name"> <br/>
Enter Age : <input type="text" name="age"> <br/>
Enter City : <input type="text" name="city"> <br/>
<br/>
<input type="submit" name="Submit1" value="Create Cookie">
<input type="submit" name="Submit2" value="Retrive Cookie">
<input type="submit" name="Submit3" value="Delete Cookie">
</FORM>
<?php
if(isset($_POST['Submit2']))
{
if(isset($_COOKIE["name"]))
{
echo "Name = ". $_COOKIE["name"]."<br/>";
echo "Age = ". $_COOKIE["age"]."<br/>";
echo "City = ". $_COOKIE["city"]."<br/>";
}
else
{
echo "Cookies deleted !!";
}
}
?>

</body>
</html>
