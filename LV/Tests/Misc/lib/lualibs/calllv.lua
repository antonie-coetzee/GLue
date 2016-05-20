local httpd = require "xavante.httpd"
local glue = require 'glue'

return function(req, res)
    res.content = glue.invokeModule('Test','','Hallo!!!')
    return res
  end
