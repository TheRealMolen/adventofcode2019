#include <iomanip>
#include <tmmintrin.h>
#include <vector>

#include "harness.h"
#include "util.h"


ostream& operator<<(ostream& os, const d12_pt& v)
{
    os << "<x=" << setw(2) << setfill(' ') << v.x << ", y=" << setw(2) << v.y << ", z=" << setw(2) << v.z << '>';
    return os;
};

struct d12_moon4
{
    __m128i px, py, pz;
    __m128i vx, vy, vz;

    d12_moon4(const vector<d12_pt>& i)
    {
        px = _mm_set_epi32(i[0].x, i[1].x, i[2].x, i[3].x);
        py = _mm_set_epi32(i[0].y, i[1].y, i[2].y, i[3].y);
        pz = _mm_set_epi32(i[0].z, i[1].z, i[2].z, i[3].z);
        vx = _mm_setzero_si128();
        vy = _mm_setzero_si128();
        vz = _mm_setzero_si128();
    }

    bool operator==(const d12_moon4& o) const
    {
        __m128i pxe = _mm_cmpeq_epi32(px, o.px);
        __m128i pye = _mm_cmpeq_epi32(py, o.py);
        __m128i pze = _mm_cmpeq_epi32(pz, o.pz);
        __m128i vxe = _mm_cmpeq_epi32(vx, o.vx);
        __m128i vye = _mm_cmpeq_epi32(vy, o.vy);
        __m128i vze = _mm_cmpeq_epi32(vz, o.vz);

        __m128i ceq = _mm_and_si128(_mm_and_si128(_mm_and_si128(pxe, pye), _mm_and_si128(vxe, vye)), _mm_and_si128(pze, vze));

        return _mm_test_all_ones(ceq) != 0;
    }

    __forceinline __m128i update_vel_cpt(__m128i c)
    {
        __m128i m1 = _mm_shuffle_epi32(c, 0x39);
        __m128i m2 = _mm_shuffle_epi32(c, 0x4e);
        __m128i m3 = _mm_shuffle_epi32(c, 0x93);

        __m128i d1 = _mm_sub_epi32(m1, c);
        __m128i d2 = _mm_sub_epi32(m2, c);
        __m128i d3 = _mm_sub_epi32(m3, c);

        __m128i one = _mm_set1_epi32(1);

        __m128i dv1 = _mm_sign_epi32(one, d1);
        __m128i dv2 = _mm_sign_epi32(one, d2);
        __m128i dv3 = _mm_sign_epi32(one, d3);

        return _mm_add_epi32(dv1, _mm_add_epi32(dv2, dv3));
    }

    void update()
    {
        vx = _mm_add_epi32(vx, update_vel_cpt(px));
        vy = _mm_add_epi32(vy, update_vel_cpt(py));
        vz = _mm_add_epi32(vz, update_vel_cpt(pz));

        px = _mm_add_epi32(px, vx);
        py = _mm_add_epi32(py, vy);
        pz = _mm_add_epi32(pz, vz);
    }

    int energy() const
    {
        __m128i pe = _mm_add_epi32(_mm_abs_epi32(px), _mm_add_epi32(_mm_abs_epi32(py), _mm_abs_epi32(pz)));
        __m128i ke = _mm_add_epi32(_mm_abs_epi32(vx), _mm_add_epi32(_mm_abs_epi32(vy), _mm_abs_epi32(vz)));
        __m128i te = _mm_mullo_epi32(pe, ke);
        __m128i hsum = _mm_hadd_epi32(te, te);
        __m128i sum = _mm_hadd_epi32(hsum, hsum);
        return sum.m128i_i32[0];
    }



    void updateX()
    {
        vx = _mm_add_epi32(vx, update_vel_cpt(px));
        px = _mm_add_epi32(px, vx);
    }
    bool eqX(const d12_moon4& o) const
    {
        __m128i peq = _mm_cmpeq_epi32(px, o.px);
        __m128i veq = _mm_cmpeq_epi32(vx, o.vx);
        __m128i ceq = _mm_and_si128(peq, veq);
        return _mm_test_all_ones(ceq) != 0;
    }

    void updateY()
    {
        vy = _mm_add_epi32(vy, update_vel_cpt(py));
        py = _mm_add_epi32(py, vy);
    }
    bool eqY(const d12_moon4& o) const
    {
        __m128i peq = _mm_cmpeq_epi32(py, o.py);
        __m128i veq = _mm_cmpeq_epi32(vy, o.vy);
        __m128i ceq = _mm_and_si128(peq, veq);
        return _mm_test_all_ones(ceq) != 0;
    }

    void updateZ()
    {
        vz = _mm_add_epi32(vz, update_vel_cpt(pz));
        pz = _mm_add_epi32(pz, vz);
    }
    bool eqZ(const d12_moon4& o) const
    {
        __m128i peq = _mm_cmpeq_epi32(pz, o.pz);
        __m128i veq = _mm_cmpeq_epi32(vz, o.vz);
        __m128i ceq = _mm_and_si128(peq, veq);
        return _mm_test_all_ones(ceq) != 0;
    }
};

ostream& operator<<(ostream& os, const d12_moon4& m)
{
    for (int i = 3; i >= 0; --i)
    {
        os << setfill(' ')
            << "   pos=<x=" << setw(2) << m.px.m128i_i32[i]
            << ", y=" << setw(2) << m.py.m128i_i32[i]
            << ", z=" << setw(2) << m.pz.m128i_i32[i]
            << ">, vel=<x=" << setw(2) << m.vx.m128i_i32[i]
            << ", y=" << setw(2) << m.vy.m128i_i32[i]
            << ", z=" << setw(2) << m.vz.m128i_i32[i] << ">" << endl;
    }
    return os;
}


int day12(const vector<d12_pt>& input, int steps, bool show)
{
    d12_moon4 moons(input);

    const int64_t chunk = 100000000;
    int64_t taken = 1;
    while ((steps - taken) > chunk)
    {
        for (int64_t tick = 1; tick <= chunk; ++tick)
        {
            moons.update();
        }
        taken += chunk;
        cout << "After " << taken << " steps:\n" << moons;
    }

    for (int64_t tick = taken; tick <= steps; ++tick)
    {
        moons.update();
    }

    if (show)
    {
        cout << "After " << steps << " steps, energy=" << moons.energy() << "; moons=\n" << moons;
    }

    return moons.energy();
}


// bruteforce (>100bn steps and counting)
int64_t day12_2(const vector<d12_pt>& input, bool show = false)
{
    d12_moon4 moons(input);
    vector<d12_moon4> zeros{ input };

    int64_t taken = 0;
    for (;;)
    {
        moons.update();
        taken++;

        if (moons.energy() == 0)
        {
            if (find(zeros.begin(), zeros.end(), moons) != zeros.end())
                break;

            zeros.push_back(moons);
            cout << "found zero after " << taken << " steps:\n" << moons << endl;
        }
    }

    if (show)
    {
        cout << "After " << taken << " steps, energy=" << moons.energy() << "; moons=\n" << moons << endl;
    }

    return taken;
}


int64_t day12_2b(const vector<d12_pt>& input, bool show = false)
{
    d12_moon4 moons(input);
    d12_moon4 original(input);

    // find loop length for x
    int64_t takenX = 0;
    for (;;)
    {
        moons.updateX();
        takenX++;

        if (moons.eqX(original))
        {
            if (show)
                cout << "found X loop after " << takenX << " steps" << endl;
            break;
        }
    }
    // find loop length for y
    int64_t takenY = 0;
    for (;;)
    {
        moons.updateY();
        takenY++;

        if (moons.eqY(original))
        {
            if (show)
                cout << "found Y loop after " << takenY << " steps" << endl;
            break;
        }
    }
    // find loop length for z
    int64_t takenZ = 0;
    for (;;)
    {
        moons.updateZ();
        takenZ++;

        if (moons.eqZ(original))
        {
            if (show)
                cout << "found Z loop after " << takenZ << " steps" << endl;
            break;
        }
    }

    return lcm(takenX, lcm(takenY, takenZ));
}


