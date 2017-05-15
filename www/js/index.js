var app = {
    macAddress: "00:06:66:72:87:52",  // get your mac address from bluetoothSerial.list
    chars: "",

    /*
     Application constructor
     */
    initialize: function () {
        this.bindEvents();
    },
    /*
     bind any events that are required on startup to listeners:
     */
    bindEvents: function () {
        document.addEventListener('deviceready', this.onDeviceReady, false);
        connectButton.addEventListener('touchend', app.manageConnection, false);
    },

    /*
     this runs when the device is ready for user interaction:
     */
    onDeviceReady: function () {
        // check to see if Bluetooth is turned on.
        // this function is called only
        //if isEnabled(), below, returns success:
        var listPorts = function () {

        };

        // if isEnabled returns failure, this function is called:
        var notEnabled = function () {
            app.display("Bluetooth is not enabled.")
        };

        // check if Bluetooth is on:
        bluetoothSerial.isEnabled(
            listPorts,
            notEnabled
        );
    },
    /*
     Connects if not connected, and disconnects if connected:
     */
    manageConnection: function () {

        // connect() will get called only if isConnected() (below)
        // returns failure. In other words, if not connected, then connect:
        var connect = function () {
            macAddress = prompt("Enter a mac address", "00:06:66:72:87:52");
            // if not connected, do this:
            // clear the screen and display an attempt to connect
            app.clear();
            app.display("Attempting to connect. " +
                "Make sure the serial port is open on the target device.");
            // attempt to connect:
            bluetoothSerial.connect(
                app.macAddress,  // device to connect to
                app.openPort,    // start listening if you succeed
                app.showError    // show the error if you fail
            );
        };

        // disconnect() will get called only if isConnected() (below)
        // returns success  In other words, if  connected, then disconnect:
        var disconnect = function () {
            app.display("attempting to disconnect");
            // if connected, do this:
            bluetoothSerial.disconnect(
                app.closePort,     // stop listening to the port
                app.showError      // show the error if you fail
            );
        };

        // here's the real action of the manageConnection function:
        bluetoothSerial.isConnected(disconnect, connect);
    },
    sendData: function (package) {
        var failure = function () {
            alert("Failed writing data to Bluetooth peripheral");
        };

        var success = function () {
            app.clear();
            app.display("Sent data successfully");
        };

        bluetoothSerial.write(JSON.stringify(package), success, failure);
    },
    /*
     subscribes to a Bluetooth serial listener for newline
     and changes the button:
     */
    openPort: function () {
        // if you get a good Bluetooth serial connection:
        app.display("Connected to: " + app.macAddress);
        // change the button's name:
        connectButton.innerHTML = "Disconnect";
        // set up a listener to listen for newlines
        // and display any new data that's come in since
        // the last newline:
        bluetoothSerial.subscribe('\n', function (data) {
            app.clear();
            app.display(data);
        });
    },

    /*
     unsubscribes from any Bluetooth serial listener and changes the button:
     */
    closePort: function () {
        // if you get a good Bluetooth serial connection:
        app.display("Disconnected from: " + app.macAddress);
        // change the button's name:
        connectButton.innerHTML = "Connect";
        // unsubscribe from listening:
        bluetoothSerial.unsubscribe(
            function (data) {
                app.display(data);
            },
            app.showError
        );
    },
    /*
     appends @error to the message div:
     */
    showError: function (error) {
        app.display(error);
    },

    /*
     appends @message to the message div:
     */
    display: function (message) {
        var display = document.getElementById("message"), // the message div
            lineBreak = document.createElement("br"),     // a line break
            label = document.createTextNode(message);     // create the label

        display.appendChild(lineBreak);          // add a line break
        display.appendChild(label);              // add the message node
    },
    /*
     clears the message div:
     */
    clear: function () {
        var display = document.getElementById("message");
        display.innerHTML = "";
    }
};      // end of app

