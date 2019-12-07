local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.title = "Overseer"
level.backgroundTexture = 'Island'
--level.scrollSpeed = 60
level.scrollSpeed = 120
level.scale = 3.125
--level.scale = 4
level.borderSize = 40
level.length = 300 * level.scale * 3
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
    {'Health', 'assets/textures/Health.png'},
    {'Ship', 'assets/textures/Ship.png'}
}

level.usedFonts = {
    {'Mont', 'assets/fonts/Montserrat-Regular.otf'},
    {'Pixel', 'assets/fonts/pcsenior.ttf'}
}

level.usedSounds = {
    {'Bullet', 'assets/audio/sfx/Bullet.wav'},
    {'Hit', 'assets/audio/sfx/OnDamage.wav'},
    {'Exp1', 'assets/audio/sfx/Exp1.wav'},
    {'Switch', 'assets/audio/sfx/Switch.wav'},
    {'Pickup', 'assets/audio/sfx/RocketPickup.wav'}
}

level.player = {
    airplane = 'Eagle'
}

level.usedAirplanes = {
    'Eagle'
}

level.usedPickups = {
    'SmallHealth',
    'HomingMissileAmmo'
}

level.spawnPoints = { 
    {'Eagle', 0, 1000},
    {'Eagle', 100, 1500}
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
        position = {-74 * level.scale, 615 * level.scale}
    },

    {
        texture = 'Ship',
        frames = 4,
        firstRect = {0, 0, 161 / 4, 67},
        frameDuration = 0.2,
        position = {-33 * level.scale, 287 * level.scale}
    }
}

level.texts = {
    {
        y = 400,
        text = 'Wave 1',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },
    {
        y = 1000,
        text = 'Wave 2',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },
}


return level