#include "DLFLCubicBezierConnect.hh"


namespace DLFL {
    CubicBezierCurve::CubicBezierCurve()
    {
        cntrl_pnts[0].set(0.0);
        cntrl_pnts[1].set(0.0);
        cntrl_pnts[2].set(0.0);
        cntrl_pnts[3].set(0.0);

        diff_cntrl_pnts[0].set(0.0);
        diff_cntrl_pnts[1].set(0.0);
        diff_cntrl_pnts[2].set(0.0);
    }

    CubicBezierCurve::CubicBezierCurve(const Vector3d (&cp)[4])
    {
        set(cp);
    }

    void CubicBezierCurve::set(const Vector3d (&cp)[4])
    {
        cntrl_pnts[0] = cp[0];
        cntrl_pnts[1] = cp[1];
        cntrl_pnts[2] = cp[2];
        cntrl_pnts[3] = cp[3];

        diff_cntrl_pnts[0] = cntrl_pnts[1] - cntrl_pnts[0];
        diff_cntrl_pnts[1] = cntrl_pnts[2] - cntrl_pnts[1];
        diff_cntrl_pnts[2] = cntrl_pnts[3] - cntrl_pnts[2];
    }

    void CubicBezierCurve::point_at(const double &t, Vector3d &point) const
    {
        double u = 1-t;
        double w0 = u*u*u;
        double w1 = 3*u*u*t;
        double w2 = 3*u*t*t;
        double w3 = t*t*t;
        //set x
        point[0] =
            w0 * cntrl_pnts[0][0] +
            w1 * cntrl_pnts[1][0] +
            w2 * cntrl_pnts[2][0] +
            w3 * cntrl_pnts[3][0] ;
        //set y
        point[1] =
            w0 * cntrl_pnts[0][1] +
            w1 * cntrl_pnts[1][1] +
            w2 * cntrl_pnts[2][1] +
            w3 * cntrl_pnts[3][1] ;
        //set z
        point[2] =
            w0 * cntrl_pnts[0][2] +
            w1 * cntrl_pnts[1][2] +
            w2 * cntrl_pnts[2][2] +
            w3 * cntrl_pnts[3][2] ;
    }

    void CubicBezierCurve::differentiate_at(const double &t, Vector3d &derivative) const
    {
        double u = 1-t;
        double w0 = 3*u*u;
        double w1 = 6*u*t;
        double w2 = 3*t*t;
        //set x
        derivative[0] =
                w0 * diff_cntrl_pnts[0][0] +
                w1 * diff_cntrl_pnts[1][0] +
                w2 * diff_cntrl_pnts[2][0] ;
        //set y
        derivative[1] =
                w0 * diff_cntrl_pnts[0][1] +
                w1 * diff_cntrl_pnts[1][1] +
                w2 * diff_cntrl_pnts[2][1] ;
        //set z
        derivative[2] =
                w0 * diff_cntrl_pnts[0][2] +
                w1 * diff_cntrl_pnts[1][2] +
                w2 * diff_cntrl_pnts[2][2] ;
        //normalize
        derivative /= sqrt(derivative.lengthsqr());
    }

    CubicBezierBuilder::CubicBezierBuilder()
    {
        is_valid = false;
        instantiated = false;
    }

    CubicBezierBuilder::CubicBezierBuilder(
            const Vector3d &start, const Vector3d &normal_s,
            const Vector3d &end, const Vector3d &normal_e,
            const double &rotation_angle)
    {
        set(start,normal_s, end, normal_e, rotation_angle);
    }

    void CubicBezierBuilder::set(
            const Vector3d &start, const Vector3d &normal_s,
            const Vector3d &end, const Vector3d &normal_e,
            const double &rotation_angle)
    {
        is_valid = false;
        instantiated = false;

        double angle_between = DLFL::getAngleBetween(normal_s, normal_e);
        std::cout<< "vector 1: " << STRING(normal_s[0]) << " " << STRING(normal_s[1]) << " " <<STRING(normal_s[2])<<'\n';
        std::cout<< "vector 2: " << STRING(normal_e[0]) << " " << STRING(normal_e[1]) << " " <<STRING(normal_e[2])<<'\n';
        std::cout<< "angle between: " << STRING(angle_between)<<'\n';
        if (angle_between == 0 || angle_between == M_PI) {
            return;
        }
        if (angle_between>M_PI) {
            angle = 2*M_PI - angle_between;
            point_at_start = end;
            normal_at_start = normal_e;
            point_at_end = start;
            normal_at_end = normal_s;
        }
        else
        {
            angle = angle_between;
            point_at_start = start;
            normal_at_start = normal_s;
            point_at_end = end;
            normal_at_end = normal_e;
        }
        std::cout << "angle: " << STRING(angle)<<'\n';
        rotation_axis = normal_at_start % normal_at_end;
        rotation_axis/=sqrt(rotation_axis.lengthsqr());
        std::cout << "rotation_axis: " << STRING(rotation_axis[0])
                << " " << STRING(rotation_axis[1]) << " " <<STRING(rotation_axis[2])<<'\n';
        instantiated = true;
        std::cout << "before rotation"<<'\n';
        rotate(rotation_angle);

    }

    bool CubicBezierBuilder::isValid() const
    {
        return is_valid && instantiated;
    }

    void CubicBezierBuilder::updateBezier(CubicBezierCurve &curve) const
    {
        curve.set(cntrl_pnts);
    }

    void CubicBezierBuilder::rotate(const double &rotation)
    {
        std::cout << "from rotation"<<'\n';
        if (!instantiated) {
            return;
        }
        double angle_test = cos(angle-rotation);
        if (angle_test == 1 || angle_test == -1) {
            std::cout << "rotation >= angle/2"<<'\n';
            this->is_valid = false;
            return;
        }
        double cosine = cos(rotation/2);
        double sine = sin(rotation/2);
        Vector3d v_start = cosine * normal_at_start + sine * (rotation_axis % normal_at_start);
        Vector3d v_end   = cosine * normal_at_end   - sine * (rotation_axis % normal_at_end);
        //calculate the control points
        Vector3d p = point_at_end - point_at_start, n = v_end - v_start;
        std::cout << "n: " << STRING(n[0]) <<" "<<STRING(n[1]) << " " << STRING(n[2])<<'\n' ;
        std::cout << "p: " << STRING(p[0]) <<" "<<STRING(p[1]) << " " << STRING(p[2])<<'\n' ;
        double a = n*n-1.0;
        std::cout << "a: " << STRING(a)<<'\n';
        double b = 2*(p*n);
        std::cout << "b: " << STRING(b)<<'\n';
        double c = p*p;
        std::cout << "c: " << STRING(c)<<'\n';
        double delta = b*b-4*a*c;
        std::cout << "delta: " << delta<<'\n';
        if (delta<0) {
            is_valid = false;
            return;
        }
        is_valid = true;
        double root = sqrt(delta);
        double t1 = (-b + root)/(2*a);
        std::cout << "t1: " << STRING(t1)<<'\n';
        double t2 = (-b - root)/(2*a);
        std::cout << "t2: " << STRING(t2)<<'\n';
        double length_param = max(t1,t2);
        std::cout << "length_param: " << STRING(length_param)<<'\n';
        if (length_param<=0) {
            is_valid = false;
            return;
        }
        cntrl_pnts[0] = point_at_start;
        cntrl_pnts[1] = point_at_start + length_param * v_start;
        cntrl_pnts[2] = point_at_end   + length_param * v_end;
        cntrl_pnts[3] = point_at_end;
    }


}
