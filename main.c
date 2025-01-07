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

//--------------------------------------------

int main(int argc, char const *argv[])
{
    FILE *chua_data = fopen("time_evolution_data.dat", "w");
    FILE *lyap_data = fopen("lyapunov.dat", "w");

    double h = 1e-3;

    // Circuit parameters
    /* double alpha = 15.6;
    double beta = 25; // varing from 25 to 51 # 35 doppio periodo # 33.8 quadruplo peiodo
    double gamma = 0.005;
    double m0 = -1.1428;
    double m1 = -0.71428; */

    double alpha = 8.4;
    double beta = 12.; // varing from 25 to 51 # 35 doppio periodo # 33.8 quadruplo peiodo
    double m0 = -0.121;
    double m1 = -1.09;
    double gamma = 0.005;

    int N_lyap_step = 10;
    int N_cutoff = 0;
    int max_N_Propagations = 20000;
    for (size_t N_propagations = 20000; N_propagations <= max_N_Propagations; N_propagations += 100)
    {
        Vect v = {0, (Vec3){6, 6, 0.0001}};
        double par[5] = {alpha, beta, m0, m1, gamma};

        Vec3 lyapunov_exp = {0, 0, 0};

        // Perturbed initial position along each axis
        double delta = 1;
        Vect vx = {0, add(v.q, (Vec3){delta, 0, 0})};
        Vect vy = {0, add(v.q, (Vec3){0, delta, 0})};
        Vect vz = {0, add(v.q, (Vec3){0, 0, delta})};

        for (size_t i = 0; i < N_propagations; i++)
        {

            for (size_t j = 0; j < N_lyap_step; j++)
            {
                if (max_N_Propagations == N_propagations)
                    fprintf(chua_data, "%10.5E %10.5E %10.5E %10.5E\n", v.t, v.q.x, v.q.y, v.q.z);

                rk4(&v, h, F, par);
                rk4(&vx, h, F, par);
                rk4(&vy, h, F, par);
                rk4(&vz, h, F, par);
            }

            // Change frame of reference on solution v(t)
            vx.q = sub(vx.q, v.q);
            vy.q = sub(vy.q, v.q);
            vz.q = sub(vz.q, v.q);

            // First Vector
            double vx_mod = product(vx.q, vx.q);

            // Second Vector
            double fourier_coeff = product(vx.q, vy.q) / vx_mod;
            vy.q = sub(vy.q, mult(vx.q, fourier_coeff));
            double vy_mod = product(vy.q, vy.q);

            // Third Vector
            double fourier_coeff_x = product(vx.q, vz.q) / vx_mod;
            double fourier_coeff_y = product(vy.q, vz.q) / vy_mod;
            vz.q = sub(vz.q, mult(vx.q, fourier_coeff_x));
            vz.q = sub(vz.q, mult(vy.q, fourier_coeff_y));

            double vz_mod = product(vz.q, vz.q);

            // Exponents evaluation

            double a = log(sqrtl(vx_mod) / delta);
            double b = log(sqrtl(vy_mod) / delta);
            double c = log(sqrtl(vz_mod) / delta);

            lyapunov_exp.x += a;
            lyapunov_exp.y += b;
            lyapunov_exp.z += c;

            // Test Gram-Smith orthogonalization
            // double test1 = product(vz.q, vx.q);
            // double test2 = product(vz.q, vy.q);
            // double test3 = product(vx.q, vy.q);

            vx.q = mult(vx.q, sqrtl(delta / vx_mod));
            vy.q = mult(vy.q, sqrtl(delta / vy_mod));
            vz.q = mult(vz.q, sqrtl(delta / vz_mod));

            // printf("%lf;%lf;%lf\n", product(vx.q, vx.q), product(vy.q, vy.q), product(vz.q, vz.q));

            vx.q = add(v.q, vx.q);
            vy.q = add(v.q, vy.q);
            vz.q = add(v.q, vz.q);

            /* Vect vx = {0, add(v.q, (Vec3){delta, 0, 0})};
            Vect vy = {0, add(v.q, (Vec3){0, delta, 0})};
            Vect vz = {0, add(v.q, (Vec3){0, 0, delta})}; */
        }
        double T = (N_propagations - N_cutoff) * N_lyap_step * h;

        fprintf(lyap_data, "%lf %lf %lf %lf\n", T, lyapunov_exp.x / T, lyapunov_exp.y / T, lyapunov_exp.z / T);
        printf("%lf %lf %lf %lf\n", T, lyapunov_exp.x / T, lyapunov_exp.y / T, lyapunov_exp.z / T);
    }
    return 0;
}
