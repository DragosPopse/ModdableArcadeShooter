local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.title = 'Christmas (WIP)'
level.backgroundTexture = 'Map'
level.repeatBackground = true
--level.scrollSpeed = 60
level.scrollSpeed = 100
level.scale = 850 / 258
--level.scale = 4
level.borderSize = 40
level.length = 16000
--level.length = 1000
level.previewImage = 'assets/textures/levels/Level1.png'
level.soundtrack = 'assets/audio/music/Level2.wav'
level.menuSoundtrack = 'assets/audio/music/MainMenu.wav'
level.saveFile = 'assets/config/saves/Level2.json'
level.defaultFont = 'Pixel'


level.usedTextures = {
    {'Airplanes',  'assets/textures/ChristmasAirplanes.png'},
    {'Map', 'assets/textures/ChristmasMap.png'}, 
    {'Projectiles', 'assets/textures/ChristmasProjectiles.png'},
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
    airplane = 'Sled'
}

level.usedAirplanes = {
    'Sled'
}

level.usedPickups = {
    'SmallHealth',
    'HomingMissileAmmo'
}


 -- SPAWNS --

level.spawnPoints = { 
    
}


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
        y = 0,
        text = 'Christmas',
        timePerLetter = 0.2,
        timeBeforeDestroy = 1,
        charSize = 40,
        font = 'Pixel',
        color = {0, 255, 0}
    },
    {
        y = 700,
        text = 'Wave 1',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel',
        color = {0, 255, 0}
    },
    {
        y = 3200,
        text = 'Wave 2',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel',
        color = {0, 255, 0}
    },
    {
        y = 6400,
        text = 'Wave 3',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel',
        color = {0, 255, 0}
    },
    {
        y = 9600,
        text = 'Wave 4',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel',
        color = {0, 255, 0}
    },
    {
        y = 12000,
        text = 'Wave 5',
        timePerLetter = 0.2,
        timeBeforeDestroy = 2,
        charSize = 40,
        font = 'Pixel',
        color = {0, 255, 0}
    }
}


return level