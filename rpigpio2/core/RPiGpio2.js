const rpigpio2 = require(process.cwd() + '/build/Release/rpigpio2');

class RPiGpio2 {

  constructor() {
    this._gpio = new rpigpio2.GPIO();
    this._i2c = new rpigpio2.I2C();
    this._pcf8574 = new rpigpio2.PCF8574();
    this._lcd1602 = new rpigpio2.LCD1602();
  }

  // command -> lcd1602 -> pcf8574 -> i2c (-> gpio)

  init(i2cnum, port) {
    this._gpio.init();
    this._i2c.init(this._gpio, i2cnum, port);
    this._pcf8574.init(this._i2c);
    this._lcd1602.init(this._pcf8574);
  }

  release() {
    this._lcd1602.release();
    this._pcf8574.release();
    this._i2c.release();
    this._gpio.release();
  }

  get DEF() {
    return rpigpio2.DEF;
  }

  get gpio() {
    return this._gpio;
  }

  get lcd1602() {
    return this._lcd1602;
  }

};

module.exports = RPiGpio2;

console.log("!!! Hello class RPiGpio2 !!!");
