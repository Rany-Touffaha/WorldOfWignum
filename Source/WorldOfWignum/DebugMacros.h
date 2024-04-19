// World of Wignum by Rany Touffaha

#pragma once

#include "DrawDebugHelpers.h"

//Draw sphere debug macro for both persistent and non-persistent lines.
#define DRAW_SPHERE(Location) if (GetWorld()) (DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, true));
#define DRAW_SPHERE_COLOR(Location, Color) if (GetWorld()) (DrawDebugSphere(GetWorld(), Location, 10.f, 12, Color, false, 5.f));
#define DRAW_SPHERE_SINGLE_FRAME(Location) if (GetWorld()) (DrawDebugSphere(GetWorld(), Location, 25.f, 12, FColor::Red, false, -1.f));

//Draw line debug macro for both persistent and non-persistent lines.
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) (DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f));
#define DRAW_LINE_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld()) (DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f));

//Draw point debug macro for both persistent and non-persistent lines.
#define DRAW_POINT(Location) if (GetWorld()) (DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, true));
#define DRAW_POINT_SINGLE_FRAME(Location) if (GetWorld()) (DrawDebugPoint(GetWorld(), Location, 15.f, FColor::Red, false, -1.f));

//Draw vector debug macro for both persistent and non-persistent lines.
#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, true); \
	}
#define DRAW_VECTOR_SINGLE_FRAME(StartLocation, EndLocation) if (GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), EndLocation, 15.f, FColor::Red, false, -1.f); \
	}
