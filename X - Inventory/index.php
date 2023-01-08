<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta http-equiv="X-UA-Compatible" content="IE=edge" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<link rel="stylesheet" href="style.css" />
		<title>NT - Inventory</title>
		<?php
    	require_once "db_lib.php";
    	$db = new MyDB();
	    ?>
	</head>
	<script>
		function Increment() {
			let qty = document.getElementById("num");
			qty.innerHTML = Number(qty.innerHTML) + 1;
			document.cookie = "wqty = " + qty.innerHTML;
		}
		function Decrement() {
			let qty = document.getElementById("num");
			qty.innerHTML = Number(qty.innerHTML) - 1;
			document.cookie = "wqty = " + qty.innerHTML;
		}
	</script>
	<body>
		<div class="container">
			<div class="Header"><h1 class="Tl">NT - Inventory</h1></div>
			<div class="Data-Tables">
				<table class="DataTable">
					<thead>
						<tr>
							<th>Data Points</th>
							<th>Values</th>
						</tr>
					</thead>
					<tbody>
						<tr>
							<td>Part Id</td>
							<td><?php echo $_GET['id']?></td>
						</tr>
						<tr>
							<td>Part Name</td>
							<td><?php echo get_name($_GET['id'],$db);?></td>
						</tr>
						<tr>
							<td>Part URL</td>
							<td><?php echo get_url($_GET['id'],$db);?></td>
						</tr>
						<tr>
							<td>Part Quantity</td>
							<td><?php if (isset($_POST['sub']))
                                {
                                    update_quantity($_GET['id'],$db,$_COOKIE['wqty']);
                                    $string = "<p id = \"num\">";
                                    $string .= get_quantity($_GET['id'],$db);
                                    $string .= "</p> Updated Quantity";
                                    echo $string;
                                }
                                else 
                                {
                                    $string = "<p id = \"num\">";
                                    $string .= get_quantity($_GET['id'],$db);
                                    $string .= "</p>";
                                    echo $string;
                                }
                                ?></td>
						</tr>
						<tr>
							<td>Part Location</td>
							<td><?php echo get_location($_GET['id'],$db);?></td>
						</tr>
						<tr>
							<td>Inventory Status</td>
							<td><?php echo get_status($_GET['id'],$db);?></td>
						</tr>
					</tbody>
				</table>
			</div>
			<div class="Increase">
				<a onclick="Increment()" class="myButton">Increase Quantity</a>
			</div>
			<div class="Decrease">
				<a onclick="Decrement()" class="myButton">Decrease Quantity</a>
			</div>
			<div class="Submit">
				<form action="" method="post">
					<input type="submit" name="sub" class="myButton" />
				</form>
			</div>
		</div>
	</body>
</html>
