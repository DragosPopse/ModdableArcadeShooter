local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.name = "Jungle Ride"
level.backgroundTexture = 'Jungle'
level.repeatBackground = false
level.scrollSpeed = 70
level.height = 5000

level.usedAirplanes = {
    'Eagle'
}

level.spawnpoints = {
    
}

level.usedTextures = {
    {'Eagle',  'Eagle.png'},
    {'Jungle', 'Jungle.png'}
}

level.player = {
    airplane = 'Eagle',
    spawnPoint = {1024 / 2, 4023}
}

return level