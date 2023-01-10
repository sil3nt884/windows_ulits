const keyPress = require('../c++/keyPress/build/Release/keypress')


const keyDown = (keyCode) => keyPress.sendKeyDownEvent(keyCode)
const keyUp = (keyCode) => keyPress.sendKeyUpEvent(keyCode)

const keyToggle = (keyCode) => {
    keyDown(keyCode)
    keyUp(keyCode)
}

module.exports = {
    keyDown,
    keyUp,
    keyToggle
}