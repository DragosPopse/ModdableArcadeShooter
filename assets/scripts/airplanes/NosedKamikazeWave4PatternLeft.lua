local Plane = dofile('assets/scripts/airplanes/NosedKamikazeWave4.lua')

Plane.aiPattern = {
    {-45, 100},
    {0, 100},
    {45, 100},
    {0, 100}
}

return Plane