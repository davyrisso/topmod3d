#ifndef DLFLCUBICBEZIERCONNECT_HH
#define DLFLCUBICBEZIERCONNECT_HH

#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
//#include <QDebug>
//#include <QString>
#include <string>

#define STRING(x) to_string(x)

namespace DLFL {

    class CubicBezierCurve
    {
        public:
            CubicBezierCurve(void);
            CubicBezierCurve(const Vector3d (&cp)[4]);
            void set(const Vector3d (&cp)[4]);
            void point_at(const double &t, Vector3d &point) const;
            void differentiate_at(const double &t, Vector3d &derivative) const;
        protected:
            Vector3d cntrl_pnts[4], diff_cntrl_pnts[3];

    };

    class CubicBezierBuilder
    {
        public:
            CubicBezierBuilder(void);
            CubicBezierBuilder(const Vector3d &start, const Vector3d &normal_s,
                               const Vector3d &end, const Vector3d & normal_e,
                               const double &rotation_angle);
            void set(const Vector3d &start, const Vector3d &normal_s,
                     const Vector3d &end, const Vector3d & normal_e,
                     const double &rotation_angle);
            bool isValid() const;
            void updateBezier(CubicBezierCurve &curve) const;
            void rotate(const double &rotation);

        protected:
            bool is_valid, instantiated;
            Vector3d point_at_start, point_at_end, normal_at_start, normal_at_end, rotation_axis;
            Vector3d cntrl_pnts[4];
            double angle;

    };


}

#endif // DLFLCUBICBEZIERCONNECT_HH
