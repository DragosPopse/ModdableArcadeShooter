local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.title = "Overseer"
level.backgroundTexture = 'Island'
level.repeatBackground = true
--level.scrollSpeed = 60
level.scrollSpeed = 120
level.scale = 3.125
level.scale = 4
level.borderSize = 40
level.length = 400 * level.scale * 3
--level.length = 1000
level.previewImage = 'assets/textures/levels/Level1.png'
level.soundtrack = 'assets/audio/music/Level1.wav'
level.menuSoundtrack = 'assets/audio/music/MainMenu.wav'
level.saveFile = 'assets/config/saves/Level1.json'
level.defaultFont = 'Pixel'


level.usedTextures = {
    {'Eagle',  'assets/textures/Eagle.png'},
    {'Island', 'assets/textures/Island.png'}, 
    {'Projectiles', 'assets/textures/Projectiles.png'},
    {'Explosions', 'assets/textures/Explosions.png'},
    {'Fragments', 'assets/textures/Fragments.png'},
    {'Vortex', 'assets/textures/Vortex.png'},
    {'Smoke', 'assets/textures/Smoke.png'},
    {'SmokeExplosion', 'assets/textures/SmokeExplosion.png'},
    {'Health', 'assets/textures/Health.png'}
}

level.usedFonts = {
    {'Mont', 'assets/fonts/Montserrat-Regular.otf'},
    {'Pixel', 'assets/fonts/pcsenior.ttf'}
}

level.usedSounds = {
    {'Bullet', 'assets/audio/sfx/Bullet.wav'},
    {'Hit', 'assets/audio/sfx/OnDamage.wav'},
    {'Exp1', 'assets/audio/sfx/Exp1.wav'},
    {'Switch', 'assets/audio/sfx/Switch.wav'}
}

level.player = {
    airplane = 'Eagle'
    --spawnPoint = {400, 400 * level.scale * 3 - 100}
}

level.usedAirplanes = {
    'Eagle'
}

level.usedPickups = {
    'SmallHealth',
    'HomingMissileAmmo'
}

level.spawnPoints = { 
    {'Eagle', 0, 600}
}
 
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
        y = 600,
        text = 'Wave 1 {MoveDown}',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    }
}


return level