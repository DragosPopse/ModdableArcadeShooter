local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.name = "Jungle Ride"
level.backgroundTexture = 'Island'
level.repeatBackground = false
level.scrollSpeed = 70
level.height = 5000

level.usedAirplanes = {
    'Eagle'
}

level.spawnPoints = { }

for i, v in ipairs(spawn.wall('Eagle', 5, {0, 2000}, 0, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('Eagle', 6, {100, 1100}, 45, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end
  

level.usedTextures = {
    {'Eagle',  'Eagle.png'},
    {'Island', 'Island.png'},
    {'Projectiles', 'Projectiles.png'},
    {'Explosions', 'Explosions.png'},
    {'Fragments', 'Fragments.png'}
}

level.usedFonts = {
    {'Mont', 'Montserrat-Regular.otf'}
}

level.player = {
    airplane = 'Eagle',
    spawnPoint = {400, 2700}
}

return level