var radius = 100;

var rightNipple = {
    zone: document.getElementById("rightZone"),
    mode: 'static',
    position: {
      left: '75%',
      top: '65%'
    },
    size: radius*2,
    color: 'black'
};
var leftNipple = {
    zone: document.getElementById("leftZone"),
    mode: 'static',
    position: {
      left: '25%',
      top: '65%'
    },
    size: radius*2,
    color: 'black'
};


var leftJoystick;
var leftPosition;
var rightJoystick;
var rightPosition;

leftJoystick = nipplejs.create(leftNipple);
leftJoystick.on('start end', function(evt, data) {
  leftPosition.distance = 0;
}).on('move', function(evt, data) {
  leftPosition = data;
});

rightJoystick = nipplejs.create(rightNipple);
rightJoystick.on('start end', function(evt, data) {
  rightPosition.distance = 0;
}).on('move', function(evt, data) {
  rightPosition = data;
});
$(function(){
  setInterval(function(){
    app.sendData(leftPosition, rightPosition);
  }, 100);
});
