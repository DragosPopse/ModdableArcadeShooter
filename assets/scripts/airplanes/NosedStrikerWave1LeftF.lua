local Plane = dofile('assets/scripts/airplanes/NosedStriker.lua')

Plane.aiPattern = {
    {-45, 80}
}

Plane.weapons = {
    {
        projectile = 'EnemyLollipop',
        ammo = 5
    }
}

return Plane