local spawn = { }

function spawn.wall(planeType, amount, firstPosition, angle, distanceBetween)
  local result = { }
  local rad = toRadians(angle)
  
  for i = 0, amount - 1 do 
    local x = firstPosition[1] + distanceBetween * math.cos(rad) * i
    local y = firstPosition[2] + distanceBetween * math.sin(rad) * i
    
    result[i] = {
      type = planeType,
      position = {x, y}
    }
  end

  return result 
end



return spawn