local Plane = dofile('assets/scripts/airplanes/NosedKamikaze.lua')

Plane.speed = 600

Plane.aiPattern = {
    {-45, 200},
    {-90, 100},
    {0, 20000}
}

return Plane