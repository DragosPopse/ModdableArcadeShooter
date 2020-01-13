local spawn = { }

function spawn.wall(planeType, amount, firstPosition, angle, distanceBetween)
  local result = { }
  local rad = engine.toRadian(angle)
  for i = 1, amount do 
    local x = firstPosition[1] + distanceBetween * math.cos(rad) * (i-1)
    local y = firstPosition[2] + distanceBetween * math.sin(rad) * (i-1)
    
    result[i] = {planeType, x, y}
  end
  return result
end


function spawn.centeredWall(planeType, count, middlePoint, distanceBetween)
  local result = { }
  local firstPositionX 
  firstPositionX = middlePoint[1] - (count // 2) * distanceBetween
  if count % 2 == 0 then 
    firstPositionX = firstPositionX + distanceBetween / 2
  end
  for i = 1, count do
    local positionX = firstPositionX + (i - 1) * distanceBetween
    result[i] = {planeType, positionX, middlePoint[2]}
  end
  return result
end


function spawn.star(planeType, centerPosition) 
  local result = { }
  result[#result + 1] = {planeType, centerPosition[1], centerPosition[2] + 20}
  result[#result + 1] = {planeType, centerPosition[1] + 70, centerPosition[2]}
  result[#result + 1] = {planeType, centerPosition[1] - 70, centerPosition[2]}
  result[#result + 1] = {planeType, centerPosition[1] - 50, centerPosition[2] - 20}
  result[#result + 1] = {planeType, centerPosition[1] + 50, centerPosition[2] - 20}
  return result
end


return spawn