#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------

struct Vec3
{
    double x, y, z;
};

typedef struct Vec3 Vec3;

//--------------------------------------------

struct Vect
{
    double t;
    Vec3 q;
};

typedef struct Vect Vect;

//--------------------------------------------

Vec3 add(Vec3 a, Vec3 b)
{
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 sub(Vec3 a, Vec3 b)
{
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

double product(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 mult(Vec3 a, double h)
{
    return (Vec3){a.x * h, a.y * h, a.z * h};
}

int rk4(Vect *v, double h, Vec3 (*F)(Vect, double *), double *args)
{
    Vec3 k1, k2, k3, k4;

    k1 = F(*v, args);
    k1 = mult(k1, h);

    Vec3 tempq = F((Vect){v->t + h / 2, (Vec3){v->q.x + k1.x / 2, v->q.y + k1.y / 2, v->q.z + k1.z / 2}}, args);
    k2.x = h * tempq.x;
    k2.y = h * tempq.y;
    k2.z = h * tempq.z;

    tempq = F((Vect){v->t + h / 2, (Vec3){v->q.x + k2.x / 2, v->q.y + k2.y / 2, v->q.z + k2.z / 2}}, args);
    k3.x = h * tempq.x;
    k3.y = h * tempq.y;
    k3.z = h * tempq.z;

    tempq = F((Vect){v->t + h / 2, (Vec3){v->q.x + k3.x, v->q.y + k3.y, v->q.z + k3.z}}, args);
    k4.x = h * tempq.x;
    k4.y = h * tempq.y;
    k4.z = h * tempq.z;

    v->t += h;
    v->q.x += (k1.x + 2 * (k2.x + k3.x) + k4.x) / 6;
    v->q.y += (k1.y + 2 * (k2.y + k3.y) + k4.y) / 6;
    v->q.z += (k1.z + 2 * (k2.z + k3.z) + k4.z) / 6;

    return 1;
}

//--------------------------------------------

Vec3 F(Vect v, double *par)
{
    double alpha = par[0];
    double beta = par[1];
    double m0 = par[2];
    double m1 = par[3];
    double gamma = par[4];

    double non_linear_component = m1 * v.q.x + 0.5 * (m0 - m1) * (fabs(v.q.x + 1) - fabs(v.q.x - 1));

    Vec3 dqdt;

    dqdt.x = alpha * (v.q.y - v.q.x - non_linear_component);
    dqdt.y = v.q.x - v.q.y + v.q.z;
    dqdt.z = -beta * v.q.y - gamma * v.q.z;

    return dqdt;
}

int convergence(Vec3 init_condition, int N_time_steps, double time_step, double trashold_up, double trashold_down)
{

    Vect v = {0, init_condition};

    double alpha = 8.4;
    double beta = 12.; // varing from 25 to 51 # 35 doppio periodo # 33.8 quadruplo peiodo
    double m0 = -0.121;
    double m1 = -1.09;
    double gamma = 0.005;

    double par[5] = {alpha, beta, m0, m1, gamma};

    for (size_t i = 0; i < N_time_steps; i++)
    {
        rk4(&v, time_step, F, par);
        // Se la traiettoria diverge oltre un limite fissato la dò per divergente
        if (product(v.q, v.q) > trashold_up)
        {
            return 1;
        }

        if (product(v.q, v.q) < trashold_down)
        {
            return 2;
        }
    }

    return 0;
}

//--------------------------------------------

int main(int argc, char const *argv[])
{
    FILE *convergenza_file = fopen("convergenza.dat", "w");

    double h = 1e-2;

    int N_time_steps = 100000;

    double max_x = 10;
    double min_x = -10;
    double max_y = 10;
    double min_y = -10;
    double max_z = 0.0001;
    double min_z = 0.0001;
    double delta = (max_x - min_x) / 500;

    for (double x = min_x; x <= max_x; x += delta)
    {
        printf("Calcolo piano x=%lf\n", x);
        for (double y = min_y; y <= max_y; y += delta)
        {
            for (double z = min_z; z <= max_z; z += delta)
            {
                Vec3 init_condition = {x, y, z};
                int tmp = convergence(init_condition, N_time_steps, h, 1e8, 1e-8);
                fprintf(convergenza_file, "%f %f %f %d\n", x, y, z, tmp);
            }
        }
    }

    return 0;
}
