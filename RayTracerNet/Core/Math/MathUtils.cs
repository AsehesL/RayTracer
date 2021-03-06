using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public static class MathUtils
    {
        public const double InvPi = 1.0 / Math.PI;

        public static double Clamp(double value, double min, double max)
        {
            value = value < min ? min : value;
            value = value > max ? max : value;
            return value;
        }

        public static float ClampF(float value, float min, float max)
        {
            value = value < min ? min : value;
            value = value > max ? max : value;
            return value;
        }

        public static double Saturate(double value)
        {
            value = value < 0.0 ? 0.0 : value;
            value = value > 1.0 ? 1.0 : value;
            return value;
        }

        public static float SaturateF(float value)
        {
            if (float.IsNaN(value))
                value = 0.0f;
            value = value < 0.0f ? 0.0f : value;
            value = value > 1.0f ? 1.0f : value;
            return value;
        }
    }
}
