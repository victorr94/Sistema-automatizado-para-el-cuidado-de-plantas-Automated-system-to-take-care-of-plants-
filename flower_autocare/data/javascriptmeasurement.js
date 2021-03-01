
$(document).on("click", "#measurement", () => {
    alert("You have pressed measurement button");
    $.ajax({
        type: "GET",
        url: "api/measurement",
        data: {},
        success: (data) => {
            console.log("I have obtained the data");
        }
    });
});
setInterval(() => {
    $.ajax({
        type: "GET",
        url: "api/datos",
        data: {},
        success: (data) => {
            let datos = JSON.parse(data);
            console.dir(datos);
            $('#temperatura').text(datos.temperature);
            $('#humedad-aire').text(datos.air_humidity);
            $('#soil-humidity').text(datos.soilhumidity);

        }
    });
}, 5000);

