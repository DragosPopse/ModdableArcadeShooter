local spawn = { }

function spawn.wall(planeType, amount, firstPosition, angle, distanceBetween)
  local result = { }
  local rad = engine.toRadian(angle)
  for i = 1, amount do 
    local x = firstPosition[1] + distanceBetween * math.cos(rad) * i
    local y = firstPosition[2] + distanceBetween * math.sin(rad) * i
    
    result[i] = {planeType, x, y}
  end

  print('Final: ' .. #result .. '\n')
  return result
end



return spawn