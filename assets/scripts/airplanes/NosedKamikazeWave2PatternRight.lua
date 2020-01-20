local Plane = dofile('assets/scripts/airplanes/NosedKamikazeFast.lua')

Plane.speed = 600

Plane.aiPattern = {
    {45, 200},
    {90, 100},
    {0, 20000}
}

Plane.drops = {
    {
        pickup = 'BigHealth',
        dropRate = 70
    }
}

return Plane