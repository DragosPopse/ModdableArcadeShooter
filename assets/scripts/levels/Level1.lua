local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.title = "Overseer"
level.backgroundTexture = 'Island'
level.repeatBackground = true
level.scrollSpeed = 60
level.scale = 3.125
level.scale = 4
level.borderSize = 40
level.length = 400 * level.scale * 3
--level.length = 200
level.previewImage = 'assets/textures/levels/Level1.png'
level.soundtrack = 'assets/audio/music/Level1.wav'
level.saveFile = 'assets/config/saves/Level1.json'
level.defaultFont = 'Pixel'


level.usedTextures = {
    {'Eagle',  'Eagle.png'},
    {'Island', 'Island.png'}, 
    {'Projectiles', 'Projectiles.png'},
    {'Explosions', 'Explosions.png'},
    {'Fragments', 'Fragments.png'},
    {'Vortex', 'Vortex.png'},
    {'Smoke', 'Smoke.png'},
    {'SmokeExplosion', 'SmokeExplosion.png'},
    {'Health', 'Health.png'}
}

level.usedFonts = {
    {'Mont', 'Montserrat-Regular.otf'},
    {'Pixel', 'pcsenior.ttf'}
}

level.usedSounds = {
    {'Bullet', 'Bullet.wav'},
    {'Hit', 'OnDamage.wav'},
    {'Exp1', 'Exp1.wav'}
}

level.player = {
    airplane = 'Eagle',
    --spawnPoint = {400, 400 * level.scale * 3 - 100}
}

level.usedAirplanes = {
    'Eagle'
}

level.usedPickups = {
    'SmallHealth',
    'HomingMissileAmmo'
}

level.spawnPoints = { }
 
for i, v in ipairs(spawn.wall('Eagle', 5, {0, 3500}, 0, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('Eagle', 6, {100, 2000 + 500}, 45, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

level.animations = {
    {
        texture = 'Vortex',
        frames = 8,
        firstRect = {0, 0, 90, 90},
        frameDuration = 0.1,
        position = {78 * 3.125, 702 * level.scale}
    }
}

level.texts = {
    {
        y = 4000,
        text = 'Wave 1',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    }
}


return level