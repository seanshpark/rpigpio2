let net = require('net');
let os = require('os');
const fs = require('fs');

function exitHandler(options, exitCode) {
  if (options.exit) {
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

let client = new net.Socket();
let connected = false;

let temp_total = 0;
let temp_count = 0;
let avail_mem = 0;

client.connect(8181, SERVER_IP, function() {
	console.log('Connected');
  connected = true;
});

client.on('data', function(data) {
	console.log('Received: ' + data);
});

client.on('close', function() {
	console.log('Connection closed');
  connected = false;
});

function sendMeasure() {
  if (connected) {
    if (temp_count >= 1) {
      let temp_avg;
      temp_avg =  temp_total / temp_count;
      client.write("TEMP:" + temp_avg.toString());
      console.log("TEMP:", temp_avg.toString());
    }
    client.write("AMEM:" + avail_mem.toString());
    console.log("AMEM:", avail_mem.toString());
  }
}

function matchFilter(first, second) {
  if (first.length == 0 && second.length == 0)
    return true;

  if ((first.length > 1 && first[0] == '?') ||
      (first.length != 0 && second.length != 0 && first[0] == second[0]))
    return matchFilter(first.substring(1), second.substring(1));

  return false;
}

// Debian 12 running on 32bit ATOM
const path_temp = "/sys/devices/platform/coretemp.0/hwmon/hwmon0";
const filter_temp = "temp?_input"

function fetchLocal() {
  fs.readdir(path_temp, (err, files) => {
    if (err) {
      console.log("readdir error:", err);
      return;
    }
    temp_total = 0;
    temp_count = 0;
    files.forEach((fileitem) => {
      if (matchFilter(filter_temp, fileitem)) {
        item_path = path_temp + "/" + fileitem;
        fs.readFile(item_path, 'ascii', function(err, data) {
          if (err) {
            console.log("readFile err:", err);
            return;
          }
          let strcuts = data.split('\n');
          temp_total += parseInt(strcuts[0]);
          temp_count += 1;
        });
      }
    });
  });

  let memfree = Math.round(os.freemem() / 1024 / 1024);
  avail_mem = memfree.toString();
}

setInterval(fetchLocal, 500);
setInterval(sendMeasure, 500);
