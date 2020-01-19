local Plane = dofile('assets/scripts/airplanes/NosedKamikaze.lua')

Plane.drops = {
    {
        pickup = 'GiftAmmo',
        dropRate = 100
    }
}

return Plane