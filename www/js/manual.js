$(function () {
    setInterval(function () {
        var package = {
            speed: $('#speed').val(),
            direction: $('#direction').val(),
            control: 4
        };
        app.sendData(package);

    }, 40);

    $(".slider").on('touchend', function () {
        $(this).val('100');
        console.log('mouseup');
    })
});