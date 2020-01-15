local Plane = dofile('assets/scripts/airplanes/NosedKamikaze.lua')

Plane.speed = 300

Plane.aiPattern = {
    {-45, 200},
    {180, 70},
    {-90, 100}
}

return Plane