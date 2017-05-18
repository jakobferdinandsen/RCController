$(function () {
    $(".control").on('click', function () {
        var package = {
            speed: 100,
            direction: 100,
            control: parseInt($(this).attr('data-signal'))
        };

        app.sendData(package);
    });

    $("#back").on('click', function () {
        window.history.back();
    })
});