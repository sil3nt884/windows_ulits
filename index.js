const screenshot = require('./src/screenshot')
const keypress = require('./src/keyPress')
const mouse = require('./src/mouse')

module.exports  = ()=> ({
    screenshot,
    keypress,
    mouse,
})

