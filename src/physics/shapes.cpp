#include "physics/shapes.h"

void Print_OBBShape(OBBShape s) {
    printf("\n");
    printf("center\t(%.2f, %.2f, %.2f, %.2f)\n",
        s.center.x, s.center.y, s.center.z, s.center.w);
    printf("axis x\t(%.2f, %.2f, %.2f, %.2f)\n",
        s.axis.x.x, s.axis.x.y, s.axis.x.z, s.axis.x.w);
    printf("axis y\t(%.2f, %.2f, %.2f, %.2f)\n",
        s.axis.y.x, s.axis.y.y, s.axis.y.z, s.axis.y.w);
    printf("axis z\t(%.2f, %.2f, %.2f, %.2f)\n",
        s.axis.z.x, s.axis.z.y, s.axis.z.z, s.axis.z.w);
    printf("half length x %.2f\n", s.half_length.x);
    printf("half length y %.2f\n", s.half_length.y);
    printf("half length z %.2f\n", s.half_length.z);
}

void Print_SphereShape(SphereShape s) {
    printf("\n");
    printf("center\t(%.2f, %.2f, %.2f, %.2f)\n",
        s.point.x, s.point.y, s.point.z, s.point.w);
    printf("radius\t%.2f\n", s.radius);
}

void Print_PlaneShape(PlaneShape s) {
    printf("\nPlaneShape\n");
    printf("point");
    PrintVector(s.point);
    printf("normal");
    PrintVector(s.normal);
    printf("limits\n");
    printf("[ %+0.2f ] [ %+0.2f ] [ %+0.2f ] [ %+0.2f ]\n",
        s.limits[0].x, s.limits[1].x, s.limits[2].x, s.limits[3].x);
    printf("[ %+0.2f ] [ %+0.2f ] [ %+0.2f ] [ %+0.2f ]\n",
        s.limits[0].y, s.limits[1].y, s.limits[2].y, s.limits[3].y);
    printf("[ %+0.2f ] [ %+0.2f ] [ %+0.2f ] [ %+0.2f ]\n",
        s.limits[0].z, s.limits[1].z, s.limits[2].z, s.limits[3].z);
    printf("[ %+0.2f ] [ %+0.2f ] [ %+0.2f ] [ %+0.2f ]\n",
        s.limits[0].w, s.limits[1].w, s.limits[2].w, s.limits[3].w);
}