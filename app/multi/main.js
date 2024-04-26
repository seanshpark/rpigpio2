//
// Test application: LCD1602(HD44780) + PCF8574 + I2C + Blink LED
//

const RPiGpio2 = require(process.cwd() + '/build/Debug/RPiGpio2.js');

class TestApp {
  constructor() {
    this._rpigpio2 = new RPiGpio2();
  }

  init() {
    // 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
    // 0x27 is for green backlight LCD1602
    this._rpigpio2.init(1, 0x27);

    this._gpio = this._rpigpio2.gpio;
    this._DEF = this._rpigpio2.DEF;
    this._lcd1602 = this._rpigpio2.lcd1602;
    console.log("gpio", this._gpio);
    console.log("DEF", this._DEF);

    this._gpio.pin(23, this._DEF.OUT);  // set gpio23 as OUT
    this._gpio.pin(24, this._DEF.OUT);  // set gpio24 as OUT

    // app
    this._p23 = true;  // LED as ON
    this._p24 = false; // LED as OFF
    this._action = 0;
  }

  exit() {
    this._gpio.set(23, true);  // reset LED as ON
    this._gpio.set(24, true);  // reset LED as OFF
    this._rpigpio2.release();
  }

  lcd1602(msg) {
    this._lcd1602.puts(msg);
  }

  tick_blink() {
    this._gpio.set(23, this._p23);
    this._gpio.set(24, this._p24);
    this._p23 = !this._p23;
    this._p24 = !this._p24;
  }

  tick_lcd16() {
    switch (this._action) {
      case 0:
        this._lcd1602.clear();
        this._lcd1602.cursor(true);
        this._lcd1602.blink(true);
        this._lcd1602.puts('Hello');
        break;
  
      case 10:
        this._lcd1602.cursor(true);
        this._lcd1602.blink(false);
        this._lcd1602.move(1, 0);
        this._lcd1602.puts('Raspberry Pi');
        break;
  
      case 20:
        this._lcd1602.clear();
        this._lcd1602.cursor(false);
        this._lcd1602.blink(false);
        this._lcd1602.move(0, 0);
        this._lcd1602.puts('Hello World!!!');
        break;
  
      case 30:
        this._lcd1602.move(1, 0);
        this._lcd1602.cursor(false);
        this._lcd1602.blink(true);
        this._lcd1602.puts('^___^');
        break;
    }
  }

  tick_next() {
    this._action = this._action + 1;
    if (this._action > 40) {
      this._action = 0;
    }
  }
}


function main() {
  let app = new TestApp();
  let timer_id = null;

  function exit_handler(options, exitCode) {
    if (options.exit) {
      if (timer_id != null)
        clearInterval(timer_id);
      app.exit();
      process.exit(0);
    }
  }

  process.stdin.resume();
  process.on('SIGINT', exit_handler.bind(null, {exit: true}));   // ctrl+c
  process.on('exit', exit_handler.bind(null, {cleanup: true}));  // app closing

  app.init();
  app.lcd1602('Running...');

  timer_id = setInterval(() => {
    app.tick_blink();
    app.tick_lcd16();
    app.tick_next();
  }, 200);
}

main();
