#include "physics/collisions.h"

glm::vec4 origin = glm::vec4(0.0,0.0,0.0,1.0);
glm::vec4 null_vec = glm::vec4(0.0,0.0,0.0,0.0);

void Print_Contact(Contact c)
{
    printf("\n");
    printf("contact point");
    PrintVector(c.contact_point);
    printf("normal");
    PrintVector(c.normal);
    printf("min move");
    PrintVector(c.min_move);
}

bool Collide_OBB_Sphere(Contact result[2], OBBShape obb, SphereShape sphere)
{
    glm::mat4 obb_base = Matrix(
        obb.axis.x.x, obb.axis.y.x, obb.axis.z.x, obb.center.x,
        obb.axis.x.y, obb.axis.y.y, obb.axis.z.y, obb.center.y,
        obb.axis.x.z, obb.axis.y.z, obb.axis.z.z, obb.center.z,
        0.0f,      0.0f,      0.0f,      1.0f
    );

    float hx = obb.half_length.x,
        hy = obb.half_length.y,
        hz = obb.half_length.z;

    SphereShape sphere_b;
    sphere_b.point = glm::inverse(obb_base) * sphere.point;
    sphere_b.radius = sphere.radius;

    glm::vec4 sphere_vec_b = normalize(origin - sphere_b.point);

    glm::vec4 sphere_point_b = sphere_b.point + sphere_vec_b * sphere_b.radius;

    if (sphere_point_b.x >= -hx && sphere_point_b.x <= hx &&
            sphere_point_b.y >= -hy && sphere_point_b.y <= hy &&
            sphere_point_b.z >= -hz && sphere_point_b.z <= hz) {
        //colidiu
        glm::vec4 v_b = sphere_b.point - origin;

        glm::vec4 ns[6] = {
            glm::vec4( hx / v_b.x, 0.0f, 0.0f, 0.0f),
            glm::vec4(-hx / v_b.x, 0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f,  hy / v_b.y, 0.0f, 0.0f),
            glm::vec4(0.0f, -hy / v_b.y, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f,  hz / v_b.z, 0.0f),
            glm::vec4(0.0f, 0.0f, -hz / v_b.z, 0.0f),
        };

        glm::vec4 K_b;
        glm::vec4 n_b;
        for (const auto &n0 : ns)
        {
            float t = n0.x + n0.y + n0.z;
            if (t > 0.0f && t < 1.0f) {
                K_b = origin + t * v_b;
                n_b = normalize(n0);
                break;
            }
        }
        glm::vec4 min_move_b = sphere_point_b - K_b;

        glm::vec4 cube_contact_point = obb_base * K_b;
        glm::vec4 cube_contact_normal = normalize(obb_base * n_b);
        glm::vec4 min_move = obb_base * min_move_b;

        glm::vec4 sphere_contact_point = obb_base * sphere_point_b;
        glm::vec4 sphere_contact_normal = normalize(obb_base * sphere_vec_b);

        Contact cube_contact;
        cube_contact.contact_point = cube_contact_point;
        cube_contact.normal = cube_contact_normal;
        cube_contact.min_move = min_move;

        Contact sphere_contact;
        sphere_contact.contact_point = sphere_contact_point;
        sphere_contact.normal = sphere_contact_normal;
        sphere_contact.min_move = -min_move;

        result[0] = cube_contact;
        result[1] = sphere_contact;
        return true;
    } else {
        return false;
    }
}

bool Collide_OBB_Plane(Contact result[2], OBBShape obb, PlaneShape plane)
{
    glm::mat4 obb_base = Matrix(
        obb.axis.x.x, obb.axis.y.x, obb.axis.z.x, obb.center.x,
        obb.axis.x.y, obb.axis.y.y, obb.axis.z.y, obb.center.y,
        obb.axis.x.z, obb.axis.y.z, obb.axis.z.z, obb.center.z,
        0.0f,      0.0f,      0.0f,      1.0f
    );

    float hx = obb.half_length.x,
        hy = obb.half_length.y,
        hz = obb.half_length.z;

    glm::vec4 plane_point_vec = origin - plane.point;
    glm::vec4 obb_center_vec = origin - obb.center;

    float plane_d = -dotproduct(plane.normal, plane_point_vec);

    float r = std::abs(hx * dotproduct(plane.normal, obb.axis.x))
        + std::abs(hy * dotproduct(plane.normal, obb.axis.y))
        + std::abs(hz * dotproduct(plane.normal, obb.axis.z));

    float i = std::abs(dotproduct(plane.normal, obb_center_vec) + plane_d);
    if (i < r) {
        //colidiu
        glm::vec4 plane_point_b = glm::inverse(obb_base) * plane.point;
        glm::vec4 plane_normal_b = normalize(glm::inverse(obb_base) * plane.normal);

        glm::vec4 diagonals[4] = {
            glm::vec4( hx, hy, hz, 0.0),
            glm::vec4(-hx, hy, hz, 0.0),
            glm::vec4( hx,-hy, hz, 0.0),
            glm::vec4( hx, hy,-hz, 0.0)
        };

        float max_cos = -std::numeric_limits<float>::infinity();
        float direction = 1.0;
        glm::vec4 closest_diagonal_b;
        for (const auto &diagonal_b : diagonals)
        {
            float cos = dotproduct(normalize(diagonal_b), plane_normal_b);

            if (std::abs(cos) > max_cos)
            {
                max_cos = std::abs(cos);
                if (cos < 0.0)
                    direction = 1;
                else
                    direction = -1;
                closest_diagonal_b = diagonal_b;
            }
        }

        glm::vec4 cube_contact_point_b = origin + closest_diagonal_b*direction;
        glm::vec4 plane_contact_point_b = cube_contact_point_b
            + Ray_Cast_Plane(cube_contact_point_b, closest_diagonal_b, plane_point_b, plane_normal_b) * closest_diagonal_b;
        glm::vec4 min_move_b = plane_contact_point_b - cube_contact_point_b;

        glm::vec4 cube_contact_point = obb_base * cube_contact_point_b;
        glm::vec4 plane_contact_point = obb_base * plane_contact_point_b;
        glm::vec4 min_move = obb_base * min_move_b;

        Contact cube_contact;
        cube_contact.contact_point = cube_contact_point;
        cube_contact.normal = normalize(closest_diagonal_b);
        cube_contact.min_move = min_move;

        Contact plane_contact;
        plane_contact.contact_point = plane_contact_point;
        plane_contact.normal = plane.normal;
        plane_contact.min_move = -min_move;

        result[0] = cube_contact;
        result[1] = plane_contact;

        return true;
    }
    return false;
}

bool Collide_OBB_OBB(Contact result[2], OBBShape obb1, OBBShape obb2)
{
    glm::vec4 mtd = null_vec;
    float min_move = std::numeric_limits<float>::infinity();

    glm::vec4 face_normals[3] = {
        glm::vec4( 1.0, 0.0f, 0.0f, 0.0f),
        glm::vec4( 0.0f, 1.0, 0.0f, 0.0f),
        glm::vec4( 0.0f, 0.0f, 1.0, 0.0f)
    };

    glm::vec4 vertices[8] = {
        glm::vec4(-1.0,-1.0f,-1.0f, 0.0f),
        glm::vec4( 1.0,-1.0f,-1.0f, 0.0f),
        glm::vec4(-1.0, 1.0f,-1.0f, 0.0f),
        glm::vec4( 1.0, 1.0f,-1.0f, 0.0f),
        glm::vec4(-1.0,-1.0f, 1.0f, 0.0f),
        glm::vec4( 1.0,-1.0f, 1.0f, 0.0f),
        glm::vec4(-1.0, 1.0f, 1.0f, 0.0f),
        glm::vec4( 1.0, 1.0f, 1.0f, 0.0f)
    };

    glm::mat4 base1 = Matrix(
        obb1.axis.x.x, obb1.axis.y.x, obb1.axis.z.x, obb1.center.x,
        obb1.axis.x.y, obb1.axis.y.y, obb1.axis.z.y, obb1.center.y,
        obb1.axis.x.z, obb1.axis.y.z, obb1.axis.z.z, obb1.center.z,
        0.0f,      0.0f,      0.0f,      1.0f
    );

    OBBShape obb2_b1;
    obb2_b1.center = glm::inverse(base1) * obb2.center;
    obb2_b1.axis.x = glm::inverse(base1) * obb2.axis.x;
    obb2_b1.axis.y = glm::inverse(base1) * obb2.axis.y;
    obb2_b1.axis.z = glm::inverse(base1) * obb2.axis.z;

    printf("obb x obb\n");
    for (const auto &face_normal : face_normals)
    {
        float max_proj_2 = -std::numeric_limits<float>::infinity();
        float min_proj_2 =  std::numeric_limits<float>::infinity();

        for (const auto &obb2_vertex : vertices)
        {
            glm::vec4 vertex = obb2_b1.center
                + obb2_b1.axis.x * obb2.half_length.x * obb2_vertex.x
                + obb2_b1.axis.y * obb2.half_length.y * obb2_vertex.y
                + obb2_b1.axis.z * obb2.half_length.z * obb2_vertex.z;

            glm::vec4 vec_to_vertex = vertex - origin;
            vec_to_vertex.w = 0.0;
            float proj_v = dotproduct(vec_to_vertex, face_normal) / dotproduct(face_normal, face_normal);

            max_proj_2 = std::max(max_proj_2, proj_v);
            min_proj_2 = std::min(min_proj_2, proj_v);
        }

        glm::vec4 vec_half_length = obb2.half_length * face_normal;
        float max_proj_1 = vec_half_length.x + vec_half_length.y + vec_half_length.z;
        float min_proj_1 = -max_proj_1;

        if ((min_proj_2 > max_proj_1 && min_proj_2 > min_proj_1)
                || (max_proj_2 < min_proj_1 && max_proj_2 < max_proj_1))
            return false;

        float move;

        float move1 = max_proj_1 - min_proj_2;
        float move2 = max_proj_2 - min_proj_1;
        if (std::abs(move1) < std::abs(move2))
        {
            move = move1;
            if (move < min_move)
            {
                min_move = move;
                mtd = -(face_normal * move);
            }
        }
        else
        {
            move = move2;
            if (move < min_move)
            {
                min_move = move;
                mtd = face_normal * move;
            }
        }
    }

    glm::mat4 base2 = Matrix(
        obb2.axis.x.x, obb2.axis.y.x, obb2.axis.z.x, obb2.center.x,
        obb2.axis.x.y, obb2.axis.y.y, obb2.axis.z.y, obb2.center.y,
        obb2.axis.x.z, obb2.axis.y.z, obb2.axis.z.z, obb2.center.z,
        0.0f,      0.0f,      0.0f,      1.0f
    );

    OBBShape obb1_b2;
    obb1_b2.center = glm::inverse(base2) * obb1.center;
    obb1_b2.axis.x = glm::inverse(base2) * obb1.axis.x;
    obb1_b2.axis.y = glm::inverse(base2) * obb1.axis.y;
    obb1_b2.axis.z = glm::inverse(base2) * obb1.axis.z;

    for (const auto &face_normal : face_normals)
    {
        float max_proj_2 = -std::numeric_limits<float>::infinity();
        float min_proj_2 =  std::numeric_limits<float>::infinity();

        for (const auto &obb1_vertex : vertices)
        {
            glm::vec4 vertex = obb1_b2.center
                + obb1_b2.axis.x * obb1.half_length.x * obb1_vertex.x
                + obb1_b2.axis.y * obb1.half_length.y * obb1_vertex.y
                + obb1_b2.axis.z * obb1.half_length.z * obb1_vertex.z;

            glm::vec4 vec_to_vertex = vertex - origin;
            vec_to_vertex.w = 0.0;
            float proj_v = dotproduct(vec_to_vertex, face_normal) / dotproduct(face_normal, face_normal);

            max_proj_2 = std::max(max_proj_2, proj_v);
            min_proj_2 = std::min(min_proj_2, proj_v);
        }

        glm::vec4 vec_half_length = obb2.half_length * face_normal;
        float max_proj_1 = vec_half_length.x + vec_half_length.y + vec_half_length.z;
        float min_proj_1 = -max_proj_1;

        if ((min_proj_2 > max_proj_1 && min_proj_2 > min_proj_1)
                || (max_proj_2 < min_proj_1 && max_proj_2 < max_proj_1))
            return false;

        float move;

        float move1 = max_proj_1 - min_proj_2;
        float move2 = max_proj_2 - min_proj_1;
        if (std::abs(move1) < std::abs(move2))
        {
            move = move1;
            if (move < min_move)
            {
                min_move = move;
                mtd = -(face_normal * move);
            }
        }
        else
        {
            move = move2;
            if (move < min_move)
            {
                min_move = move;
                mtd = face_normal * move;
            }
        }
    }

    glm::vec4 cross_product_normals[9] = {
        crossproduct(obb1.axis.x, obb2.axis.x),
        crossproduct(obb1.axis.x, obb2.axis.y),
        crossproduct(obb1.axis.x, obb2.axis.z),
        crossproduct(obb1.axis.y, obb2.axis.x),
        crossproduct(obb1.axis.y, obb2.axis.y),
        crossproduct(obb1.axis.y, obb2.axis.z),
        crossproduct(obb1.axis.z, obb2.axis.x),
        crossproduct(obb1.axis.z, obb2.axis.y),
        crossproduct(obb1.axis.z, obb2.axis.z)
    };

    for (const auto &cross_vector : cross_product_normals)
    {
        if (cross_vector.x == 0 && cross_vector.y == 0 && cross_vector.z == 0)
            continue;

        float max_proj_1 = -std::numeric_limits<float>::infinity();
        float min_proj_1 =  std::numeric_limits<float>::infinity();
        for (const auto &obb1_vertex : vertices)
        {
            glm::vec4 vertex = obb1.center
                + obb1.axis.x * obb1.half_length.x * obb1_vertex.x
                + obb1.axis.y * obb1.half_length.y * obb1_vertex.y
                + obb1.axis.z * obb1.half_length.z * obb1_vertex.z;

            glm::vec4 vec_to_vertex = vertex - origin;
            vec_to_vertex.w = 0.0;
            float proj_v = dotproduct(vec_to_vertex, cross_vector) / dotproduct(cross_vector, cross_vector);

            max_proj_1 = std::max(max_proj_1, proj_v);
            min_proj_1 = std::min(min_proj_1, proj_v);
        }

        float max_proj_2 = -std::numeric_limits<float>::infinity();
        float min_proj_2 =  std::numeric_limits<float>::infinity();
        for (const auto &obb2_vertex : vertices)
        {
            glm::vec4 vertex = obb2.center
                + obb2.axis.x * obb2.half_length.x * obb2_vertex.x
                + obb2.axis.y * obb2.half_length.y * obb2_vertex.y
                + obb2.axis.z * obb2.half_length.z * obb2_vertex.z;

            glm::vec4 vec_to_vertex = vertex - origin;
            vec_to_vertex.w = 0.0;
            float proj_v  = dotproduct(vec_to_vertex, cross_vector) / dotproduct(cross_vector, cross_vector);

            max_proj_2 = std::max(max_proj_2, proj_v);
            min_proj_2 = std::min(min_proj_2, proj_v);
        }

        if ((min_proj_2 > max_proj_1 && min_proj_2 > min_proj_1)
                || (max_proj_2 < min_proj_1 && max_proj_2 < max_proj_1))
            return false;

        float move;

        float move1 = max_proj_1 - min_proj_2;
        float move2 = max_proj_2 - min_proj_1;
        if (std::abs(move1) < std::abs(move2))
        {
            move = move1;
            if (move < min_move)
            {
                min_move = move;
                mtd = -(cross_vector * move);
            }
        }
        else
        {
            move = move2;
            if (move < min_move)
            {
                min_move = move;
                mtd = cross_vector * move;
            }
        }
    }

    Contact obb1_contact;
    obb1_contact.contact_point = null_vec;
    obb1_contact.normal = null_vec;
    obb1_contact.min_move = mtd;

    Contact obb2_contact;
    obb2_contact.contact_point = null_vec;
    obb2_contact.normal = null_vec;
    obb2_contact.min_move = -mtd;

    result[0] = obb1_contact;
    result[1] = obb2_contact;

    return true;
}

float Ray_Cast_Plane(glm::vec4 ray_origin, glm::vec4 ray_vector, glm::vec4 plane_origin, glm::vec4 plane_normal)
{
    glm::vec4 s0 = ray_origin; // s0 sphere origin
    glm::vec4 v = ray_vector;
    glm::vec4 p0 = plane_origin;
    glm::vec4 n = plane_normal / norm(plane_normal);
    s0.w = 0.0f;
    p0.w = 0.0f;
    if (dotproduct(n, v) == 0.0f) {
        return std::numeric_limits<float>::infinity();
    }
    float t = (dotproduct(n, p0) - dotproduct(n, s0)) / dotproduct(n, v);
    return t;
}
