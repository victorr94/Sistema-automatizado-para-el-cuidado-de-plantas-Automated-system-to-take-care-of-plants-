$(document).on("click", "#watering", () => {
    alert("You have pressed watering button");
    $.ajax({
        type: "GET",
        url: "api/watering",
        data: {},
        success: (data) => {
            console.log("he tenido una respuesta");
        }
    });
});

