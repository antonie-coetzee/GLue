glue = require 'glue'
os.exit = function() end
output = ''

print = function(arg)
  output = output .. tostring(arg)
end
--require('mobdebug').start()

local cocreate
local function coro()
  if cocreate then return end -- only set once
  cocreate = cocreate or coroutine.create
  coroutine.create = function(f, ...)
    return cocreate(function(...)
      return f(...)
    end, ...)
  end
end
coro()

local xavante = require "xavante"

local simplerules = {
  {
    match = ".",
    with = require 'calllv'
  }
} 

xavante.HTTP{
    server = {host = "localhost", port = 8081},
    
    defaultHost = {
    	rules = simplerules
    },
}
xavante.start(function() 
      --glue.invokeModule('Test','')
    end, 1)
return output