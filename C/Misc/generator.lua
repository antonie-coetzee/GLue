local slt = require "/libs/slt2"

local gen = {}

function string.insert(value, insert, place)
  if place == nil then
    place = string.len(value) + 1  
  end

  return string.sub(value,1,place-1) .. tostring(insert) .. string.sub(value,place,string.len(value)) 
end

gen.processLine = function(procLine, line)
  local res = {}
  local patrn = "%s-(LUA.-API)%s+(.-%s+%*?)(%b())%s+(%b())"  
  procLine.output = line
  local st, en, api, returnType, func, funcArgs = string.find(line, patrn)
  if st ~= nil then
    res.funcName =  string.sub(func,2,-2) 
    res.funcPointer = returnType .. string.insert(func,"*",2) .. funcArgs
    res.typePointer = returnType .. "(*)" .. funcArgs
    procLine.output = api .." ".. res.funcPointer .. ';'
    return res
  else
    return nil    
  end 
end

gen.parseFiles = function(output, files)
  for i=1,#files do
    local path = files[i]
    local file = io.open(path,"r") 
    if  file ~= nil then
      local linkFile = io.open(path .. "_l","w+")
      for line in file:lines() do
        local procLine = {ouput = ""};
        local res = gen.processLine(procLine, line)
        if res ~= nil then
            table.insert(output,res)
        end
        linkFile:write(procLine.output .. "\n")
      end
      linkFile:close()
      file:close()
    end   
  end
  return output
end

gen.buildBindingfile = function(input)
  local sourceTmpl = slt.loadstring(
[[
// **** auto generated, do not modify **** //
#include <windows.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "luaconf.h"

// **** Function Pointers **** //#{for i=1,#model do}#
#{=model[i].funcPointer}#;#{end}#

// **** Loading Function **** //
void LinkLuaFunctions()
{
  HINSTANCE luaLib;
  luaLib = LoadLibrary(TEXT("lua52.dll")); 
  #{for i=1,#model do}#
  #{=model[i].funcName}# = (#{=model[i].typePointer}#)GetProcAddress(luaLib,"#{=model[i].funcName}#");#{end}#  
}
]])
  local output = slt.render(sourceTmpl,{model = input})
  return output
end

return gen