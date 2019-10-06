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

level.spawnPoints = {
    {'Eagle', 1024 / 2, 3700},
    {'Eagle', 1024 / 2, 3720},
    {'Eagle', 1024 / 2, 3600},
    {'Eagle', 1024 / 2, 3400}
}

level.usedTextures = {
    {'Eagle',  'Eagle.png'},
    {'Jungle', 'Jungle.png'},
    {'Projectiles', 'Projectiles.png'}
}

level.player = {
    airplane = 'Eagle',
    spawnPoint = {1024 / 2, 4023}
}

return level