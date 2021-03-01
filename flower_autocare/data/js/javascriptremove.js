
$(document).on("click", "#weeds", () => {
    alert("You have pressed remove weeds button");
    $.ajax({
        type: "GET",
        url: "api/weeds",
        data: {},
        success: (data) => {
            console.log("he tenido una respuesta");
        }
    });
});

