//
// Test application: blink LED at gpio23(pin16), gpio24(pin18)
//

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');
const DEF = rpigpio2.DEF;
console.log('DEF.IN = ', DEF.IN);
console.log('DEF.OUT = ', DEF.OUT);
console.log('DEF.ALT0 = ', DEF.ALT0);

const gpiobj = new rpigpio2.GPIO();
gpiobj.init();

function exitHandler(options, exitCode) {
  if (options.exit) {
    gpiobj.set(23, true);  // reset LED as ON
    gpiobj.set(24, true);  // reset LED as OFF
    gpiobj.release();

    process.exit(exitCode);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

console.log('press ctrl+c to quit.')

gpiobj.pin(23, DEF.OUT);  // set gpio23 as OUT
gpiobj.pin(24, DEF.OUT);  // set gpio24 as OUT
p23 = true;               // LED as ON
p24 = false;              // LED as OFF

function timerInterval() {
  gpiobj.set(23, p23);
  gpiobj.set(24, p24);

  p23 = !p23;
  p24 = !p24;
}

setInterval(timerInterval, 500);
