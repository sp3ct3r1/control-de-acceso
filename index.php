<?php
$servername = "localhost";
$username = "root";
$password = "";
$db_name = "prueba1";

// Crear conexión
$conex = new mysqli($servername, $username, $password, $db_name);

// Verificar la conexión
if ($conex->connect_error) {
    die("Conexion fallida: " . $conex->connect_error);
}

// Verificar si se recibió una ID a través de POST
if (isset($_POST['id'])) {
    // Escapar la ID para prevenir inyección SQL
    $id = $conex->real_escape_string($_POST['id']);

    // Consulta SQL para buscar la ID en la tabla 'test'
    $sql = "SELECT * FROM test WHERE id = '$id'";

    // Ejecutar la consulta
    $result = mysqli_query($conex, $sql);

    // Verificar si la consulta se ejecutó correctamente
    if ($result) {
        // Verificar si se encontró alguna fila con la ID proporcionada
        $row = mysqli_fetch_assoc($result);
        
        if ($row) {
            echo "ID encontrada";
            // Aquí podrías hacer algo con los datos obtenidos, si es necesario
        } else {
            echo "ID no encontrada";
        }
    } else {
        echo "Error al ejecutar la consulta: " . mysqli_error($conex);
    }
} else {
    echo "No se recibió ningún ID por POST.";
}

// Cerrar la conexión
$conex->close();
?>
