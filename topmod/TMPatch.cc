#include "TMPatch.hh"
#include <QDebug>

#define MATRIX 1

static Matrix4_4 WEIGHTING_FACTORS_MODIFIED[4][4] =
{
    /*Row 0*/
    {
        //0,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,1
        {
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },
    /*Row 1*/
    {
        //1,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,1
        {
            {2.0,   0.0,    0.0,    0.0},
            {0.0,   2.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,2
        {
            {2.0,   1.0,    1.0,    0.0},
            {0.0,   3.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,3
        {
            {2.0,   2.0,    2.0,    2.0},
            {0.0,   4.0,    4.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },

    /*Row 2*/
    {
        //2,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {2.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,1
        {
            {2.0,   0.0,    0.0,    0.0},
            {1.0,   3.0,    0.0,    0.0},
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,2
        {
            {2.0,   1.0,    1.0,    0.0},
            {1.0,   5.0,    2.0,    0.0},
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,3
        {
            {2.0,   2.0,    2.0,    2.0},
            {1.0,   7.0,    7.0,    1.0},
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },
    /*Row 3*/
    {
        //3,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {3.0,   0.0,    0.0,    0.0},
            {3.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0}
        },
        //3,1
        {
            {2.0,   0.0,    0.0,    0.0},
            {2.0,   4.0,    0.0,    0.0},
            {2.0,   4.0,    0.0,    0.0},
            {2.0,   0.0,    0.0,    0.0}
        },
        //3,2
        {
            {2.0,   1.0,    1.0,    0.0},
            {2.0,   7.0,    3.0,    0.0},
            {2.0,   7.0,    3.0,    0.0},
            {2.0,   1.0,    1.0,    0.0}
        },
        //3,3
        {
            {2.0,   2.0,    2.0,   2.0},
            {2.0,   10.0,    10.0,    2.0},
            {2.0,   10.0,    10.0,   2.0},
            {2.0,   2.0,    2.0,    2.0}
        }
    }
};


//WEIGHTING_FACTORS_MIX
static Matrix4_4 WEIGHTING_FACTORS[4][4] =
{
    /*Row 0*/
    {
        //0,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,1
        {
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },
    /*Row 1*/
    {
        //1,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,1
        {
            {2.0,   0.0,    0.0,    0.0},
            {0.0,   2.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,2
        {
            {2.0,   1.0,    1.0,    0.0},
            {0.0,   3.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,3
        {
            {2.0,   2.0,    2.0,    2.0},
            {0.0,   4.0,    4.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },

    /*Row 2*/
    {
        //2,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {2.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,1
        {
            {2.0,   0.0,    0.0,    0.0},
            {1.0,   3.0,    0.0,    0.0},
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {2.0,   4.0,    2.0,    0.0},
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {2.0,   6.0,    6.0,    2.0},
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },
    /*Row 3*/
    {
        //3,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {3.0,   0.0,    0.0,    0.0},
            {3.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0}
        },
        //3,1
        {
            {2.0,   0.0,    0.0,    0.0},
            {2.0,   4.0,    0.0,    0.0},
            {2.0,   4.0,    0.0,    0.0},
            {2.0,   0.0,    0.0,    0.0}
        },
        //3,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {3.0,   6.0,    3.0,    0.0},
            {3.0,   6.0,    3.0,    0.0},
            {1.0,   2.0,    1.0,    0.0}
        },
        //3,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {3.0,   9.0,    9.0,    3.0},
            {3.0,   9.0,    9.0,    3.0},
            {1.0,   3.0,    3.0,    1.0}
         }
    }
};

static Matrix4_4 WEIGHTING_FACTORS_DECASTELJAU[4][4] =
{
    /*Row 0*/
    {
        //0,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,1
        {
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //0,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },
    /*Row 1*/
    {
        //1,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,1
        {
            {1.0,   1.0,    0.0,    0.0},
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //1,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },

    /*Row 2*/
    {
        //2,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {2.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,1
        {
            {1.0,   1.0,    0.0,    0.0},
            {2.0,   2.0,    0.0,    0.0},
            {1.0,   1.0,    0.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {2.0,   4.0,    2.0,    0.0},
            {1.0,   2.0,    1.0,    0.0},
            {0.0,   0.0,    0.0,    0.0}
        },
        //2,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {2.0,   6.0,    6.0,    2.0},
            {1.0,   3.0,    3.0,    1.0},
            {0.0,   0.0,    0.0,    0.0}
        }
    },
    /*Row 3*/
    {
        //3,0
        {
            {1.0,   0.0,    0.0,    0.0},
            {3.0,   0.0,    0.0,    0.0},
            {3.0,   0.0,    0.0,    0.0},
            {1.0,   0.0,    0.0,    0.0}
        },
        //3,1
        {
            {1.0,   1.0,    0.0,    0.0},
            {3.0,   3.0,    0.0,    0.0},
            {3.0,   3.0,    0.0,    0.0},
            {1.0,   1.0,    0.0,    0.0}
        },
        //3,2
        {
            {1.0,   2.0,    1.0,    0.0},
            {3.0,   6.0,    3.0,    0.0},
            {3.0,   6.0,    3.0,    0.0},
            {1.0,   2.0,    1.0,    0.0}
        },
        //3,3
        {
            {1.0,   3.0,    3.0,    1.0},
            {3.0,   9.0,   9.0,   3.0},
            {3.0,   9.0,   9.0,   3.0},
            {1.0,   3.0,    3.0,   1.0}
        }
    }
};

IndexTransform TMPatch::FORWARD = IndexTransform(true);
IndexTransform TMPatch::BACKWARD = IndexTransform(false);
GLint TMPatch::PatchGridDimension = 6;
int TMPatch::PatchNestingFactor = 0;
vector<GLint> TMPatch::GridDimensions = vector<GLint>(1,6);

void TMPatch::UpdatePatchGridDimensions(){
    TMPatch::GridDimensions.clear();
    for (int i = 0; i <= TMPatch::PatchNestingFactor; ++i) {
        int n = 1<<(TMPatch::PatchNestingFactor-i);
        GLint d = (GLint)(n*TMPatch::PatchGridDimension);
        TMPatch::GridDimensions.push_back(d);
    }
}

void TMPatch::SetPatchNestingFactor(int maxLevel){
    TMPatch::PatchNestingFactor = maxLevel;
    TMPatch::UpdatePatchGridDimensions();
}

int TMPatch::GetPatchNestingFactor(void){
    return TMPatch::PatchNestingFactor;
}


bool TMPatch::LocalPatchNesting = false;
void TMPatch::SetLocalPatchNesting(bool value){
    TMPatch::LocalPatchNesting = value;
}

bool TMPatch::GetLocalPatchNesting(){
    return TMPatch::LocalPatchNesting;
}

bool TMPatch::ShowControlMesh = false;
bool TMPatch::GetShowControlMesh() {
    return ShowControlMesh;
}

void TMPatch::SetShowControlMesh(bool value) {
    ShowControlMesh = value;
}

void TMPatch::SetPatchGridDimension(int size){
    GLint n = (GLint)size;
    if (n == TMPatch::PatchGridDimension) {
        return;
    }
    if (n>1) {
        TMPatch::PatchGridDimension = n;
    }else{
        TMPatch::PatchGridDimension = 1;
    }
    //reset the vector of grid Dimentions
    TMPatch::UpdatePatchGridDimensions();
}
int TMPatch::GetPatchGridDimension(void){
    return (int)(TMPatch::PatchGridDimension);
}

Point3d TMPatch::point_at(const FourUValues &u_values, const FourUValues &v_values)const{
    double x = 0.0, y = 0.0, z = 0.0;
    for (int i = 0; i < patchsize; ++i) {
        double w1 = BEZIER_COEFFICIENTS[i] * u_values[i];
        for (int j = 0; j < patchsize; ++j) {
            double w = w1 * BEZIER_COEFFICIENTS[j]*v_values[j];
            x+=ctrlpts[j][i][0]*w;
            y+=ctrlpts[j][i][1]*w;
            z+=ctrlpts[j][i][2]*w;
        }
    }
    return Point3d(x,y,z);
}

Point3d TMPatch::point_at(const double &u, const double &v)const{
    double u_values[4]=
    {
        u*u*u,
        u*u*(1-u),
        u*(1-u)*(1-u),
        (1-u)*(1-u)*(1-u),
    };
    double v_values[4]=
    {
        v*v*v,
        v*v*(1-v),
        v*(1-v)*(1-v),
        (1-v)*(1-v)*(1-v),
    };
    double x = 0.0, y = 0.0, z = 0.0;
    for (int i = 0; i < patchsize; ++i) {
        double w1 = BEZIER_COEFFICIENTS[i] * u_values[i];
        for (int j = 0; j < patchsize; ++j) {
            double w = w1 * BEZIER_COEFFICIENTS[j]*v_values[j];
            x+=ctrlpts[j][i][0]*w;
            y+=ctrlpts[j][i][1]*w;
            z+=ctrlpts[j][i][2]*w;
        }
    }
    return Point3d(x,y,z);
}

Vector3d TMPatch::normal_at(const double &u, const double &v) const
{
    double u_values[4]=
    {                                                                         //derivatives
        u*u*u,              //u^3         3u^2                                  3*u*u
        u*u*(1-u),          //u^2-u^3     2u-3u^2                               u*(2-3*u)
        u*(1-u)*(1-u),      //u(1-u)^2    (1-u)^2-2(1-u)*u    (1-u)*(1-u-2*u)   (1-u)*(1-3*u)
        (1-u)*(1-u)*(1-u),  //(1-u)^3     -3(1-u)^2                             -3*(1-u)*(1-u)
    };
    double u_values_derevative[4]=
    {
        3*u*u,
        u*(2-3*u),
        (1-u)*(1-3*u),
        -3*(1-u)*(1-u),
    };
    double v_values[4]=
    {
        v*v*v,
        v*v*(1-v),
        v*(1-v)*(1-v),
        (1-v)*(1-v)*(1-v),
    };
    double v_values_derevative[4]=
    {
        3*v*v,
        v*(2-3*v),
        (1-v)*(1-3*v),
        -3*(1-v)*(1-v),
    };
    double x = 0.0, y = 0.0, z = 0.0;
    for (int i = 0; i < patchsize; ++i) {
        double w1 = BEZIER_COEFFICIENTS[i] * u_values_derevative[i];
        for (int j = 0; j < patchsize; ++j) {
            double w = w1 * BEZIER_COEFFICIENTS[j]*v_values[j];
            x+=ctrlpts[j][i][0]*w;
            y+=ctrlpts[j][i][1]*w;
            z+=ctrlpts[j][i][2]*w;
        }
    }
    Vector3d du(x,y,z);
    x = 0.0; y = 0.0; z = 0.0;
    for (int i = 0; i < patchsize; ++i) {
        double w1 = BEZIER_COEFFICIENTS[i] * u_values[i];
        for (int j = 0; j < patchsize; ++j) {
            double w = w1 * BEZIER_COEFFICIENTS[j]*v_values_derevative[j];
            x+=ctrlpts[j][i][0]*w;
            y+=ctrlpts[j][i][1]*w;
            z+=ctrlpts[j][i][2]*w;
        }
    }
    Vector3d dv(x,y,z);
    Vector3d normal=du%dv;
    double lengthSquared=normal.lengthsqr();
    if(lengthSquared != 0.0){
        double length = sqrt(lengthSquared);
        normal/=length;
    }
    return normal;
}



void GetGrid(const Vector3dGrid &inputGrid,
             const IndexTransform &X,
             const IndexTransform &Y,
             const bool &normalize,
             Vector3dGrid &outputGrid){

#if MATRIX
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            GetNextLevelPoint(inputGrid,WEIGHTING_FACTORS[i][j],X,Y,normalize,outputGrid[X.Get(i)][Y.Get(j)]);
        }
    }
#else
    /*START*/
        //First row
        outputGrid[X.Get(0)][Y.Get(0)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)];
        outputGrid[X.Get(0)][Y.Get(1)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 1.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)];
        outputGrid[X.Get(0)][Y.Get(1)] /= 2.0;

        outputGrid[X.Get(0)][Y.Get(2)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 2.0 * inputGrid[X.Get(0)][Y.Get(1)] + 1.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)];
        outputGrid[X.Get(0)][Y.Get(2)] /= 4;

        outputGrid[X.Get(0)][Y.Get(3)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 3.0 * inputGrid[X.Get(0)][Y.Get(1)] + 3.0 * inputGrid[X.Get(0)][Y.Get(2)] + 1.0 * inputGrid[X.Get(0)][Y.Get(3)];
        outputGrid[X.Get(0)][Y.Get(3)] /= 8.0;

        //Second Row
        outputGrid[X.Get(1)][Y.Get(0)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(1)][Y.Get(0)] + 0.0 * inputGrid[X.Get(1)][Y.Get(1)] + 0.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)];
        outputGrid[X.Get(1)][Y.Get(0)] /= 2.0;

        outputGrid[X.Get(1)][Y.Get(1)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          0.0 * inputGrid[X.Get(1)][Y.Get(0)] + 1.0 * inputGrid[X.Get(1)][Y.Get(1)] + 0.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)];
        outputGrid[X.Get(1)][Y.Get(1)] /= 2.0;
        //this is modified
        outputGrid[X.Get(1)][Y.Get(2)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 1.0 * inputGrid[X.Get(0)][Y.Get(1)] + 1.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          0.0 * inputGrid[X.Get(1)][Y.Get(0)] + 3.0 * inputGrid[X.Get(1)][Y.Get(1)] + 1.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)];
        outputGrid[X.Get(1)][Y.Get(2)] /= 8.0;

        outputGrid[X.Get(1)][Y.Get(3)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 2.0 * inputGrid[X.Get(0)][Y.Get(1)] + 2.0 * inputGrid[X.Get(0)][Y.Get(2)] + 2.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          0.0 * inputGrid[X.Get(1)][Y.Get(0)] + 4.0 * inputGrid[X.Get(1)][Y.Get(1)] + 4.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)];
        outputGrid[X.Get(1)][Y.Get(3)] /= 16.0;

        //third row
        outputGrid[X.Get(2)][Y.Get(0)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(1)][Y.Get(0)] + 0.0 * inputGrid[X.Get(1)][Y.Get(1)] + 0.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(2)][Y.Get(0)] + 0.0 * inputGrid[X.Get(2)][Y.Get(1)] + 0.0 * inputGrid[X.Get(2)][Y.Get(2)] + 0.0 * inputGrid[X.Get(2)][Y.Get(3)];
        outputGrid[X.Get(2)][Y.Get(0)] /= 4.0;
        //this is modified
        outputGrid[X.Get(2)][Y.Get(1)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(1)][Y.Get(0)] + 3.0 * inputGrid[X.Get(1)][Y.Get(1)] + 0.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(2)][Y.Get(0)] + 1.0 * inputGrid[X.Get(2)][Y.Get(1)] + 0.0 * inputGrid[X.Get(2)][Y.Get(2)] + 0.0 * inputGrid[X.Get(2)][Y.Get(3)];
        outputGrid[X.Get(2)][Y.Get(1)] /= 8.0;

        outputGrid[X.Get(2)][Y.Get(2)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 1.0 * inputGrid[X.Get(0)][Y.Get(1)] + 1.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(1)][Y.Get(0)] + 5.0 * inputGrid[X.Get(1)][Y.Get(1)] + 2.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(2)][Y.Get(0)] + 2.0 * inputGrid[X.Get(2)][Y.Get(1)] + 1.0 * inputGrid[X.Get(2)][Y.Get(2)] + 0.0 * inputGrid[X.Get(2)][Y.Get(3)];
        outputGrid[X.Get(2)][Y.Get(2)] /= 16.0;

        outputGrid[X.Get(2)][Y.Get(3)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 2.0 * inputGrid[X.Get(0)][Y.Get(1)] + 2.0 * inputGrid[X.Get(0)][Y.Get(2)] + 2.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(1)][Y.Get(0)] + 7.0 * inputGrid[X.Get(1)][Y.Get(1)] + 7.0 * inputGrid[X.Get(1)][Y.Get(2)] + 1.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(2)][Y.Get(0)] + 3.0 * inputGrid[X.Get(2)][Y.Get(1)] + 3.0 * inputGrid[X.Get(2)][Y.Get(2)] + 1.0 * inputGrid[X.Get(2)][Y.Get(3)];
        outputGrid[X.Get(2)][Y.Get(3)] /= 32.0;

        //fourth row
        outputGrid[X.Get(3)][Y.Get(0)] =
          1.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          3.0 * inputGrid[X.Get(1)][Y.Get(0)] + 0.0 * inputGrid[X.Get(1)][Y.Get(1)] + 0.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          3.0 * inputGrid[X.Get(2)][Y.Get(0)] + 0.0 * inputGrid[X.Get(2)][Y.Get(1)] + 0.0 * inputGrid[X.Get(2)][Y.Get(2)] + 0.0 * inputGrid[X.Get(2)][Y.Get(3)] +
          1.0 * inputGrid[X.Get(3)][Y.Get(0)] + 0.0 * inputGrid[X.Get(3)][Y.Get(1)] + 0.0 * inputGrid[X.Get(3)][Y.Get(2)] + 0.0 * inputGrid[X.Get(3)][Y.Get(3)];
        outputGrid[X.Get(3)][Y.Get(0)] /= 8.0;

        outputGrid[X.Get(3)][Y.Get(1)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 0.0 * inputGrid[X.Get(0)][Y.Get(1)] + 0.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(1)][Y.Get(0)] + 4.0 * inputGrid[X.Get(1)][Y.Get(1)] + 0.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(2)][Y.Get(0)] + 4.0 * inputGrid[X.Get(2)][Y.Get(1)] + 0.0 * inputGrid[X.Get(2)][Y.Get(2)] + 0.0 * inputGrid[X.Get(2)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(3)][Y.Get(0)] + 0.0 * inputGrid[X.Get(3)][Y.Get(1)] + 0.0 * inputGrid[X.Get(3)][Y.Get(2)] + 0.0 * inputGrid[X.Get(3)][Y.Get(3)];
        outputGrid[X.Get(3)][Y.Get(1)] /= 16.0;

        outputGrid[X.Get(3)][Y.Get(2)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 1.0 * inputGrid[X.Get(0)][Y.Get(1)] + 1.0 * inputGrid[X.Get(0)][Y.Get(2)] + 0.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(1)][Y.Get(0)] + 7.0 * inputGrid[X.Get(1)][Y.Get(1)] + 3.0 * inputGrid[X.Get(1)][Y.Get(2)] + 0.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(2)][Y.Get(0)] + 7.0 * inputGrid[X.Get(2)][Y.Get(1)] + 3.0 * inputGrid[X.Get(2)][Y.Get(2)] + 0.0 * inputGrid[X.Get(2)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(3)][Y.Get(0)] + 1.0 * inputGrid[X.Get(3)][Y.Get(1)] + 1.0 * inputGrid[X.Get(3)][Y.Get(2)] + 0.0 * inputGrid[X.Get(3)][Y.Get(3)];
        outputGrid[X.Get(3)][Y.Get(2)] /= 32.0;

        outputGrid[X.Get(3)][Y.Get(3)] =
          2.0 * inputGrid[X.Get(0)][Y.Get(0)] + 2.0 *  inputGrid[X.Get(0)][Y.Get(1)] + 2.0 *  inputGrid[X.Get(0)][Y.Get(2)] + 2.0 * inputGrid[X.Get(0)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(1)][Y.Get(0)] + 10.0 * inputGrid[X.Get(1)][Y.Get(1)] + 10.0 * inputGrid[X.Get(1)][Y.Get(2)] + 2.0 * inputGrid[X.Get(1)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(2)][Y.Get(0)] + 10.0 * inputGrid[X.Get(2)][Y.Get(1)] + 10.0 * inputGrid[X.Get(2)][Y.Get(2)] + 2.0 * inputGrid[X.Get(2)][Y.Get(3)] +
          2.0 * inputGrid[X.Get(3)][Y.Get(0)] + 2.0 *  inputGrid[X.Get(3)][Y.Get(1)] + 2.0 *  inputGrid[X.Get(3)][Y.Get(2)] + 2.0 * inputGrid[X.Get(3)][Y.Get(3)];
        outputGrid[X.Get(3)][Y.Get(3)] /= 64.0;
        /*END*/
        if (normalize) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    double length = sqrt(outputGrid[i][j].lengthsqr());
                    if(length != 0) outputGrid[i][j]/=length;
                }
            }
        }
#endif
}


void GetNextLevelPoint(const Vector3dGrid &inputGrid, const Matrix4x4 &weights, const IndexTransform &X, const IndexTransform &Y, const bool &normalize, Vector3d &out)
{
    out.reset();
    double sum = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            sum += weights[i][j];
            out+=weights[X.Get(i)][Y.Get(j)]*inputGrid[i][j];
        }
    }
    if (normalize) {
        double length_squared = out.lengthsqr();
        if(length_squared != 0.0){
            double length = sqrt(length_squared);
            out/=length;
        }
    }else{
        out/=sum;
    }
}

