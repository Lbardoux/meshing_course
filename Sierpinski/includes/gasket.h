#ifndef GASKET_H
#define GASKET_H

#include "Mesh.hpp"

typedef enum {
    MESH,
    CURVE,
    TRIANGULATION,
	CONSTRAINTS
} WhatIs_e;

struct DrawConfiguration final
{
	WhatIs_e type;
	bool triangles;
	bool centers;
	bool curve;
	bool circles;
	bool cells;
	bool voronois;
	bool points;
	DrawConfiguration(void)
	{
		type = MESH;
		triangles = true;
		centers = false;
		curve = true;
		circles = false;
		cells = false;
		voronois = false;
		points = false;
	}
};

class Gasket
{
    public:
        Gasket();
        Mesh mesh;
        void draw();
        
		DrawConfiguration config;
};

#endif
