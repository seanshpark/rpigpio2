//
// Test application: LCD1602(HD44780) + PCF8574 + I2C
//

const RpiLcd1602 = require(process.cwd() + '/build/Release/RpiLcd1602.js');
const rpilcd1602 = new RpiLcd1602();
rpilcd1602.init(1, 0x27);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
// 0x27 is for green backlight LCD1602

function exitHandler(options, exitCode) {
  if (options.exit) {
    rpilcd1602.release();
    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

const lcd1602 = rpilcd1602.lcd1602;
lcd1602.puts('Running...');

var action = 0;
function timerInterval() {
  console.log('lcd1602/main.js action=', action);

  switch (action) {
    case 0:
      lcd1602.clear();
      lcd1602.cursor(true);
      lcd1602.blink(true);
      lcd1602.puts('Hello');
      action = action + 1;
      break;

    case 1:
      lcd1602.cursor(true);
      lcd1602.blink(false);
      lcd1602.move(1, 0);
      lcd1602.puts('Raspberry Pi');
      action = action + 1;
      break;

    case 2:
      lcd1602.clear();
      lcd1602.cursor(false);
      lcd1602.blink(false);
      lcd1602.move(0, 0);
      lcd1602.puts('Hello World!!!');
      action = action + 1;
      break;

    case 3:
      lcd1602.move(1, 0);
      lcd1602.cursor(false);
      lcd1602.blink(true);
      lcd1602.puts('^___^');
      action = action + 1;
      break;

    case 4:
      action = 0;
      break;
  }
}

setInterval(timerInterval, 2000);
