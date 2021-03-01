

$(document).on("click", "#forward", () => {
    alert("You have pressed forward button");
    $.ajax({
        type: "POST",
        url: "api/forward",
        data: {},
        success: (data) => {
            console.log("Going forward");
        }
    });
});


$(document).on("click", "#left", () => {
    alert("You have pressed left button");
    $.ajax({
        type: "POST",
        url: "api/left",
        data: {},
        success: (data) => {
            console.log("Going left");
        }
    });
});

$(document).on("click", "#right", () => {
    alert("You have pressed right button");
    $.ajax({
        type: "POST",
        url: "api/right",
        data: {},
        success: (data) => {
            console.log("Going right");
        }
    });
});

$(document).on("click", "#backward", () => {
    alert("You have pressed backward button");
    $.ajax({
        type: "POST",
        url: "api/backward",
        data: {},
        success: (data) => {
            console.log("Going backward");
        }
    });
});


$(document).on("click", "#UP", () => {
    alert("You have pressed UP button");
    $.ajax({
        type: "POST",
        url: "api/UP",
        data: {},
        success: (data) => {
            console.log("Going UP");
        }
    });
});

$(document).on("click", "#down", () => {
    alert("You have pressed DOWN button");
    $.ajax({
        type: "POST",
        url: "api/down",
        data: {},
        success: (data) => {
            console.log("Going DOWN");
        }
    });
});

$(document).on("click", "#LED", () => {

    $.ajax({
        type: "POST",
        url: "api/LED",
        data: {},
        success: (data) => {
            console.log("LED panel switched ON");
        }
    });
});

$(document).on("click", "#FAN", () => {

    $.ajax({
        type: "POST",
        url: "api/FAN",
        data: {},
        success: (data) => {
            console.log("FAN switched ON");
        }
    });
});