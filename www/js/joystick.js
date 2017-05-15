$(function () {
    setInterval(function () {
        var package = {
            speed: $('#speed').val(),
            direction: $('#direction').val()
        };
        // console.log(package);
        app.sendData(package);

    }, 10);

    $(".slider").on('touchend', function () {
        $(this).val('100');
        console.log('mouseup');
    })
});

