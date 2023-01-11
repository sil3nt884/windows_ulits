const mouse = require('../c++/getMousePos/build/Release/getMousePos')


const getMousePosition = () => mouse.getMousePosition()

module.exports = {
      getMousePosition
}