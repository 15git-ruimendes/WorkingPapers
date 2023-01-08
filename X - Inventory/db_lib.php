<?php
class MyDB extends SQLite3
{
    function __construct()
    {
        $this->open('Teste.db',SQLITE3_OPEN_READWRITE | SQLITE3_OPEN_CREATE);
    }
}

function get_name($id,$db)
{
    $query = "SELECT Name FROM Inventory WHERE Id = ";
    $query .= $id;

    $results = $db->query($query);
    
    $row = $results->fetchArray();

    return $row[0];
}

function get_quantity($id,$db)
{
    $query = "SELECT Quantity FROM Inventory WHERE Id = ";
    $query .= $id;

    $results = $db->query($query);
    
    $row = $results->fetchArray();

    return $row[0];
}

function get_url($id,$db)
{
    $query = "SELECT URL FROM Inventory WHERE Id = ";
    $query .= $id;

    $results = $db->query($query);
    
    $row = $results->fetchArray();

    return $row[0];
}

function get_location($id,$db)
{
    $query = "SELECT Location FROM Inventory WHERE Id = ";
    $query .= $id;

    $results = $db->query($query);
    
    $row = $results->fetchArray();

    return $row[0];
}

function get_status($id,$db)
{
    $query = "SELECT STATUS FROM Inventory WHERE Id = ";
    $query .= $id;

    $results = $db->query($query);
    
    $row = $results->fetchArray();

    return $row[0];
}

function update_quantity($id,$db,$qt)
{
    $query = "UPDATE Inventory SET Quantity  = ";
    $query .= $qt;
    $query .= " WHERE Id = ";
    $query .= $id;

    $results = $db->query($query);
    return;
}
