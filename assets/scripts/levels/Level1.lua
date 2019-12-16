local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.title = "Overseer"
level.backgroundTexture = 'Island'
level.repeatBackground = true
--level.scrollSpeed = 60
level.scrollSpeed = 120
level.scale = 3.125
--level.scale = 4
level.borderSize = 40
level.length = 16000
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
    'Eagle',
    'EnemyEagle0F',
    'EnemyEagleFBase'
}

level.usedPickups = {
    'SmallHealth',
    'HomingMissileAmmo'
}


 -- SPAWNS --

level.spawnPoints = { 
  
}
 
-- WAVE 1 --

for i, v in ipairs(spawn.wall('EnemyEagle0F', 4, {-350, 1500}, 15, 235)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 3, {-250, 1800}, 0, 250)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 5, {-350, 2000}, 0, 175)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 4, {0, 2100}, 90, 150)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 4, {-200, 2100}, 105, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 4, {200, 2100}, 75, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 4, {-350, 2500}, 40, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 4, {350, 2500}, 140, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {0, 2800}, 90, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {100, 2850}, 90, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {-100, 2850}, 90, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {200, 2900}, 90, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {-200, 2900}, 90, 50)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {-350, 3100}, 0, 250)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {100, 3100}, 0, 250)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 3, {-300, 3300}, 0, 300)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {-350, 3500}, 0, 250)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 2, {100, 3500}, 0, 250)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.wall('EnemyEagle0F', 3, {-300, 3550}, 0, 300)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

-- WAVE 2 --

for i, v in ipairs(spawn.wall('EnemyEagleFBase', 3, {-300, 3620}, 0, 300)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

-- WAVE 3 --

-- WAVE 4 --

-- WAVE 5 --

-- //SPAWNS --

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
        text = 'Overseer',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },

    {
        y = 720,
        text = 'Wave 1',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },

    {
        y = 3520,
        text = 'Wave 2',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },

    {
        y = 6160,
        text = 'Wave 3',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },

    {
        y = 8780,
        text = 'Wave 4',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    },

    {
        y = 11420,
        text = 'Wave 5',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel'
    }
}


return level