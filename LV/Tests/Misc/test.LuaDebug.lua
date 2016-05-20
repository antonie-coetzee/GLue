glue = require 'glue'
middleclass = require 'middleclass'
output = ''

print = function(arg)
  output = output .. arg
end

print(glue.invokeModule('Test','NumberReturnNumber',5))

return output