const window = require('../c++/findWindow/build/Release/findWindow');

const findWindowHandle = (windowName) => window.findWindowHandle(windowName)
const setForegroundWindow = (handle) => window.setForegroundWindow(handle)

module.exports = {
    findWindowHandle,
    setForegroundWindow,
}