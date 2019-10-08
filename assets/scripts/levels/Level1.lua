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

level.spawnPoints = {
    {'Eagle', 1024 / 2 + 10, 3700},
    {'Eagle', 1024 / 2 - 20, 3600},
    {'Eagle', 1024 / 2 - 30, 3400},
    {'Eagle', 1024 / 2, 3000},
    {'Eagle', 1024 / 2, 2900},
    {'Eagle', 1024 / 2, 2700},
    {'Eagle', 1024 / 2, 2400},
    {'Eagle', 1024 / 2, 2000},
    {'Eagle', 1024 / 2, 1900},
    {'Eagle', 1024 / 2, 1900},
    {'Eagle', 1024 / 2, 1900},
    {'Eagle', 1024 / 2, 1900}
}

level.usedTextures = {
    {'Eagle',  'Eagle.png'},
    {'Island', 'Island.png'},
    {'Projectiles', 'Projectiles.png'}
}

level.player = {
    airplane = 'Eagle',
    spawnPoint = {400, 2700}
}

return level