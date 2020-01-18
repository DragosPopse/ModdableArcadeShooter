local level = { }
local spawn = dofile('assets/scripts/utility/Spawn.lua')

level.title = 'A Merry Retaliation'
level.backgroundTexture = 'Map'
level.repeatBackground = true
--level.scrollSpeed = 60
level.scrollSpeed = 100
level.scale = 850 / 285
--level.scale = 4
level.borderSize = 40
level.length = 15800
--level.length = 1000
level.previewImage = 'assets/textures/levels/Level2.png'
level.soundtrack = 'assets/audio/music/Level2.wav'
level.menuSoundtrack = 'assets/audio/music/Level2Menu.wav'
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
    {'Ship', 'assets/textures/Ship.png'},
    {'Snowflakes', 'assets/textures/Snowflakes.png'},
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

level.usedPickups = { }

level.player = {
    airplane = 'Sled'
}

level.usedAirplanes = {
    'Sled',
    'NosedStriker',
    'NosedKamikaze',
    'NosedStrikerWave1Left',
    'NosedStrikerWave1Right',
    'NosedStrikerWave1LeftF',
    'NosedStrikerWave1RightF',
    'NosedKamikazeFast',
    'NosedKamikazeWave2Right',
    'NosedKamikazeWave2Left',
    'NosedStrikerWave2F',
    'NosedOverlord',
    'NosedStrikerF',
    'NosedKamikazeWave4',
    'NosedKamikazeWave4Left',
    'NosedKamikazeWave4Left0P',
    'NosedKamikazeWave4Right0P',
    'NosedKamikazeWave4Right',
    'NosedKamikazeWave4PatternLeft',
    'NosedKamikazeWave4PatternRight',
    'NosedKamikazeWave2PatternLeft',
    'NosedKamikazeWave2PatternRight',
    'NosedOverlordStar'
}

function spawn.smallTree(baseX, baseY)
    for i, v in ipairs(spawn.centeredWall('NosedStriker', 4, {baseX, baseY}, 90)) do
        level.spawnPoints[#level.spawnPoints + 1] = v 
    end
    
    for i, v in ipairs(spawn.centeredWall('NosedStriker', 3, {baseX, baseY + 30}, 90)) do
        level.spawnPoints[#level.spawnPoints + 1] = v 
    end
    
    for i, v in ipairs(spawn.centeredWall('NosedStriker', 2, {baseX, baseY + 60}, 90)) do
        level.spawnPoints[#level.spawnPoints + 1] = v 
    end
    
    for i, v in ipairs(spawn.centeredWall('NosedKamikaze', 1, {baseX, baseY + 90}, 90)) do
        level.spawnPoints[#level.spawnPoints + 1] = v 
    end
end

 -- SPAWNS --

level.spawnPoints = { 
    
}

-- Wave 1 --

-- level.spawnPoints[#level.spawnPoints + 1] = {'NosedStriker', -100, 1690}
-- level.spawnPoints[#level.spawnPoints + 1] = {'NosedStriker', 100, 1690}
for i, v in ipairs(spawn.star('NosedKamikaze', {0, 1690})) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.star('NosedKamikaze', {-300, 2100})) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.star('NosedKamikaze', {300, 2100})) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 3, {0, 2490}, 120)) do
    if i == 1 then
        v[1] = 'NosedStrikerWave1Left'
    elseif i == 3 then
        v[1] = 'NosedStrikerWave1Right'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 3, {-300, 2700}, 70)) do
    if i == 3 then
        v[1] = 'NosedStrikerWave1LeftF'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 3, {300, 2700}, 70)) do
    if i == 1 then
        v[1] = 'NosedStrikerWave1RightF'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 2900}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedStrikerWave1LeftF', -350, 3000}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedStrikerWave1RightF', 350, 3000}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 3500}


-- Wave 2 --

--level.spawnPoints[#level.spawnPoints + 1] = {'NosedStriker', 0, 4100}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2Left', -350, 4100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2Right', 350, 4100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 4200}

for i, v in ipairs(spawn.centeredWall('NosedStrikerWave2F', 2, {0, 4400}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 2, {0, 4420}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlord', -350, 4490}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlord', 350, 4520}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 4800}

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 2, {0, 4900}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 3, {0, 5100}, 200)) do
    if i == 1 then
        v[1] = 'NosedStrikerWave1LeftF'
    elseif i == 3 then
        v[1] = 'NosedStrikerWave1RightF'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedOverlord', 2, {0, 5200}, 400)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 2, {0, 5300}, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 5400}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right0P', 300, 5500}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 5600}


for i, v in ipairs(spawn.centeredWall('NosedStriker', 2, {0, 5700}, 400)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStrikerWave1Left', 2, {-300, 5800}, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStrikerWave1Right', 2, {300, 5800}, 100)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -400, 6100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 400, 6100}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -400, 6300}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 400, 6300}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -400, 6500}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 400, 6500}

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 5, {0, 6600}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 4, {0, 6700}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end




-- Wave 3 --
--level.spawnPoints[#level.spawnPoints + 1] = {'NosedStriker', 0, 6400 + 900}
for i, v in ipairs(spawn.centeredWall('NosedStriker', 7, {0, 7300}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 6, {0, 7300 + 50 * 1}, 120)) do
    if i == 2 then
        v[1] = 'NosedKamikaze'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 5, {0, 7300 + 50 * 2}, 120)) do
    if i == 4 then
        v[1] = 'NosedKamikaze'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 4, {0, 7300 + 50 * 3}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 3, {0, 7300 + 50 * 4}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 2, {0, 7300 + 50 * 5}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedKamikaze', 1, {0, 7300 + 50 * 6}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.centeredWall('NosedStriker', 8, {0, 7700}, 90)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlord', 0, 7800}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2Left', -200, 7900}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2Right', 200, 7900}

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 4, {0, 8050}, 120)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedStrikerWave1LeftF', -400, 8200}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedStrikerWave1RightF', 400, 8200}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedStrikerF', 0, 8250}

spawn.smallTree(240, 8350)
spawn.smallTree(-240, 8450)
spawn.smallTree(240, 8550)
spawn.smallTree(-240, 8650)
spawn.smallTree(240, 8750)
spawn.smallTree(-240, 8850)
spawn.smallTree(240, 8950)
spawn.smallTree(-240, 9050)

level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlord', 0, 9300}

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 4, {-240, 9350}, 70)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 4, {240, 9350}, 70)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeFast', 5, {0, 9750}, 70)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

-- Wave 4 --
--level.spawnPoints[#level.spawnPoints + 1] = {'NosedStriker', 0, 10700}
for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 4, {0, 10500}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {0, 10700}, 190)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 3, {0, 10900}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {0, 11300}, 190)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 2, {0, 11380}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 3, {0, 11500}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 6, {0, 11600}, 170)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 3, {0, 11700}, 200)) do
    if i == 1 then
        v[1] = 'NosedKamikazeWave4PatternRight'
    elseif i == 3 then
        v[1] = 'NosedKamikazeWave4PatternLeft'
    end
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 11800}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right0P', 300, 11850}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 11900}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right0P', 300, 11950}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 12000}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right0P', 300, 12050}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 12100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right0P', 300, 12150}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 12200}


-- Wave 5 --

level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlord', 0, 12900}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -400, 13100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 400, 13100}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedStrikerF', 0, 13300}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left', -300, 13300}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right', 300, 13300}

for i, v in ipairs(spawn.centeredWall('NosedStrikerF', 4, {0, 13500}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 3, {0, 13600}, 200)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.star('NosedKamikazeWave4', {0, 13800})) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.star('NosedKamikaze', {-250, 13900})) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

for i, v in ipairs(spawn.star('NosedKamikaze', {250, 13900})) do
    level.spawnPoints[#level.spawnPoints + 1] = v
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlordStar', -250, 13900}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedOverlordStar', 250, 13900}


spawn.smallTree(240, 14100)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right', 300, 14190}

spawn.smallTree(-240, 14200)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left', -300, 14290}

spawn.smallTree(240, 14300)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right', 300, 14390}

spawn.smallTree(-240, 14400)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left', -300, 14490}

spawn.smallTree(240, 14500)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right', 300, 14590}

spawn.smallTree(-240, 14600)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left', -300, 14690}
    
spawn.smallTree(240, 14700)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right', 300, 14790}

spawn.smallTree(-240, 14800)
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left', -300, 14890}


for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {0, 15000}, 90)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {100, 15100}, 90)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {-100, 15200}, 90)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {100, 15300}, 90)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 5, {-100, 15400}, 90)) do
    level.spawnPoints[#level.spawnPoints + 1] = v 
end

for i, v in ipairs(spawn.centeredWall('NosedKamikazeWave4', 3, {0, 15600}, 200)) do
    if i == 1 then
        v[1] = 'NosedKamikazeWave4PatternRight'
    elseif i == 3 then
        v[1] = 'NosedKamikazeWave4PatternLeft'
    end
    --level.spawnPoints[#level.spawnPoints + 1] = v 
end

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Right0P', 300, 15700}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave4Left0P', -300, 15800}


level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -300, 15900}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 300, 15900}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 15900}

level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -300, 16100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 300, 16100}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 16100}


level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternLeft', -300, 16300}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeWave2PatternRight', 300, 16300}
level.spawnPoints[#level.spawnPoints + 1] = {'NosedKamikazeFast', 0, 16300}



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
        text = 'A Merry Retaliation',
        timePerLetter = 0.1,
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